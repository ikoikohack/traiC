#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float read_float(void *float_str_ptr);
void *write_float(float f);
void *add_float(void *float_str_ptr1, void *float_str_ptr2);



int main(int argc, char *argv[]){

	char float_str[10];
	char float_str2[10];
	strcpy(float_str, "+4.204E+2");
	strcpy(float_str, "-1.772E+2");
	printf("%s + %s = %s\n", float_str,float_str,add_float(float_str, float_str));

}


float read_float(void *float_str_ptr){
	const char *str_ptr = float_str_ptr;
	float fraction;
	int exponent = 0;
	
	sscanf(str_ptr,"%fE%d",&fraction,&exponent);
	if (exponent > 0) {
		while (exponent > 0){
			fraction = fraction /10;
			--exponent;
		}
	}else if (exponent < 0) {
		while (exponent < 0){
			fraction = fraction * 10;
			++exponent;
		}
	}
	return(fraction);
}


void *write_float(float f) {
	void *return_val;
	unsigned char *str_ptr;
	char exponent = 0;
	
	str_ptr = malloc(sizeof(char) * 10);
	if (str_ptr == NULL){
		fprintf(stderr, "Out of memory\n");
		exit(8);
	}

	while ((f >= 10) || (f <= -10)){
		f = f/10;
		++exponent;
	}
	while ((f < 1) && (f > -1)) {
		f = f * 10;
		--exponent;
	}
	sprintf(str_ptr,"%+1.3fE%+d",f,exponent);
	return_val = str_ptr;
	return(return_val);
}




void *add_float(void *float_str_ptr1, void *float_str_ptr2){
	float f1;
	float f2;
	float sum;
	f1 = read_float(float_str_ptr1);
	f2 = read_float(float_str_ptr2);
	sum = f1 + f2;
	return(write_float(sum));
}
