#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define RUSAGE_SELF 0
#define RUSAGE_CHILDREN -1




#define ARRAY_SIZE 1000

int main(int argc, char *argv[])
{

int getrusage_return;
struct rusage rusage;
long system_time;
long user_time;

unsigned int array[ARRAY_SIZE];
unsigned int index;
unsigned int sum = 0;

fprintf(stdout,"Summing an array of %d elements (unoptimized)...", ARRAY_SIZE);
fflush(stdout);

for (index = 0; index < ARRAY_SIZE; ++index){
	array[index] = 10;
	sum += array[index];
}

getrusage_return = getrusage(RUSAGE_SELF, &rusage);
if(getrusage_return == -1){
	fprintf(stderr,"%s: Error calling getrusage().\n",argv[0]);
	return 0;
}

system_time = rusage.ru_stime,tv_usec;
user_time = rusage.ru_utime.tv_usec;





printf("Hello");
return 0;
}


