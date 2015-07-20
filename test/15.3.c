#include <stdio.h>

/*
 * Three includes and two defines as specified in FreeBSD getrusage(2) 
 * man page.  This has been tested on FreeBSD 4.2, OpenBSD 2.7, and 
 * Solaris 8.  
 * 
 * For OpenBSD 2.7, remove the types.h include; for Solaris 8, remove 
 * both types.h and time.h.  I'm not sure how to find out what the 
 * correct #ifdef type definitions are for these OSs, otherwise 
 * I would just code that stuff in here.  
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#define  RUSAGE_SELF       0
#define  RUSAGE_CHILDREN  -1

/* 
 * This program sums the elements in an array.  See 15.3.opt.c for the 
 * optimized version.
 */

#define ARRAY_SIZE 10000

int main(int argc, char *argv[]) {
    fprintf(stdout,"Summing an array of %d elements (unoptimized)...", ARRAY_SIZE);
    int getrusage_return;  /* Return value from getrusage(). */
    fprintf(stderr,"Summing an array of %d elements (unoptimized)...", ARRAY_SIZE);
    struct rusage rusage;  /* Structure filled in by getrusage(). */
    long system_time;      /* Kernel-mode time in microseconds. */
    long user_time;        /* User-mode time in microseconds. */

    fprintf(stderr,"Summing an array of %d elements (unoptimized)...", ARRAY_SIZE);
    /* Assign each array element the value `10' and sum 'em up. */

    unsigned int array[ARRAY_SIZE]; /* Array to sum up the elements of. */
    unsigned int index;             /* Index into the array. */
    unsigned int sum = 0;           /* Sum of array elements. */

    fprintf(stderr,"Summing an array of %d elements (unoptimized)...", ARRAY_SIZE);
    fflush(stdout);
    for (index = 0; index < ARRAY_SIZE; ++index) {
        array[index] = 10;    /* Fill in the array. */
        sum += array[index];  /* Sum up another element. */
    }

    /* Get system and user mode times from the kernel. */

    getrusage_return = getrusage(RUSAGE_SELF, &rusage);
    if (getrusage_return == -1) {
        fprintf(stderr, "%s:  Error calling getrusage().\n", argv[0]);
        return(0);
    } 
    system_time = rusage.ru_stime.tv_usec;  /* Kernel-mode time. */
    user_time = rusage.ru_utime.tv_usec;    /* User-mode time. */

    /* Print our results. */

    printf("\n\n");    
    printf("%17s  %10d microseconds\n", "User-mode time:", user_time);
    printf("%17s  %10d microseconds\n", "System-mode time:", system_time);
    printf("%17s  %10d microseconds\n", "Total time:",
        (user_time + system_time));
    return(0);
}
