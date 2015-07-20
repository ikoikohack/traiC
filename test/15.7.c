#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>



void *yocpy(void *dst, const void *src, size_t len){



	register unsigned char *dst_ptr = dst;
	register const unsigned char *src_ptr = src;
	size_t char_num;


	unsigned char *orig_dst_ptr = malloc(sizeof(*dst_ptr));
	if (orig_dst_ptr == NULL){
		fprintf(stderr, "OUt of memory. \n");
		exit(8);
	}

	for (char_num = 0; char_num < len; ++char_num){
		*dst_ptr = *orig_dst_ptr;
		++dst_ptr;
		++orig_dst_ptr;
	}


	dst_ptr = dst;
	for (char_num = 0; char_num<len; ++char_num){
		*dst_ptr = *src_ptr;
		++src_ptr;
		++dst_ptr;
	}


	orig_dst_ptr = orig_dst_ptr - sizeof(*orig_dst_ptr);
	return (orig_dst_ptr);
}

void *slow_memcpy(void *dst,const void *src,size_t num){

	unsigned char *dst_uchar = (unsigned char*) dst;
	unsigned char *src_uchar = (unsigned char*) src;
	int i;
	for ( i=0;i<num;i++){
		dst_uchar[i] = src_uchar[i];

	}


	return dst;

}


void *rapid_memcpy(void *dst, const void *src, size_t num){

	register uint32_t *dst_ptr;
	register uint32_t *src_ptr;

	for ( *dst_ptr = (uint32_t*)dst, *src_ptr = (uint32_t*)src;dst_ptr < &((uint32_t*)dst)[num/sizeof(uint32_t)];
		++dst_ptr,++src_ptr){
		*dst_ptr = * src_ptr;
	}

	register int i;
	for( i = 0; i < num & sizeof(uint32_t);i++){
		((unsigned char*)dst)[num -1 -i] = ((unsigned char*) src)[num -1 -i];
	}
	
	return dst;

}

int main(){
	int size = 1000000;
	struct timeval start, end;
	int* expected_ptr = (int*)malloc(size * sizeof(int));
	int* acctual_ptr  = (int*)malloc(size * sizeof(int));
	memset(expected_ptr,0xf0,size);
	printf("ok\n");
	gettimeofday(&start,NULL);
	slow_memcpy(expected_ptr,acctual_ptr,size);
	gettimeofday(&end,NULL);
	printf("%lf",(((end.tv_sec - start.tv_sec)+(end.tv_usec - start.tv_usec))) * 1.0E-6);
	


}
