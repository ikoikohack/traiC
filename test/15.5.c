#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define RUSAGE_SELF 0
#define RUSAGE_CHILDREN -1
#define NUM_ELEMENTS_0 10
#define NUM_ELEMENTS_1 1000
#define NUM_ELEMENTS_2 1000000

struct timed_count {
	unsinged int num_bits_on;
	long run_time;
}


void count(char *array_ptr, unsinged int len,struct timed_count *timed_count_ptr);
int main(int argc, char *argv[]){
	char array_0[NUM_ELEMENTS_0];
	char array_1[NUM_ELEMENTS_1];
	char array_2[NUM_ELEMENTS_2];
	
	struct timed_count timed_count = {0, 0};

	memset(array_0, 0, sizeof(array_0);
	memset(array_1, 0, sizeof(array_1);
	memset(array_2, 0, sizeof(array_2);
	array_0[3] = 8;
	array_0[8] = 2;
	array_1[330] = 7;
	array_1[820] = 19;
	array_2[172946] = 59;
	array_2[294600] = 80;




	printf("\n==> Number of array elements: %d\n",NUM_ELEMENTS_0);
	count(array_0,NUM_ELEMTNTS_0, &timed_count);

}

void count(register char *array_ptr, unsigned int len, struct timed_count *timed_count_ptr){
	unsigned int element_num = 0;
	register char shift_num;
	register unsigned char mask;

	struct rusage rusage;
	long total_time_pre;
	long total_time_post;

	(*timed_count_ptr).num_bits_on = 0;
	getrusage(RUSAGE_SELF, &rusage);
	total_time_pre = rusage.ru_time.tv_usec + rusage.ru_time.tv_usec;

	while (element_num != len){

		for (shift_num = 0; shift_num < 8; ++shift_num){

			mask = (0x80 >> shift_num);
			if ((*array_ptr & mask) !=0){
				++(*timed_count_ptr).num_bits_on;
			}

		}
		++element_num;
		++array_ptr;

	}

	getrusage(RUSAGE_SELF, &rusage);
	total_time_post = rusage.ru_stime.tv_usec + rusage.ru_utime.tv_usec;
	(*timed_count_ptr).run_time = total_time_post - total_time_pre;

}

void slow_count(char *array_ptr, unsigned int lne, struct timed_count *timed_count_ptr){
	unsigned int element_num = 0;
	char shift_num;	
	while(element_num !=len){

		for(shift_num = 0; shift_num < 8; ++shift_num){

			mask = (0x80 >> shift_num);
			if((*array_ptr & mask) !=0){

				(*timed_count_ptr).num_bits_on +=1;
			}

		}



	}

}
