#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * This program counts the number of ON bits in a character array.  It is 
 * optimized through the use of register integer variables, which make
 * it about 14% faster on a one million element array.  It times 
 * the action with getrusage() on different sized arrays.
 *
 * Here is how I reckoned the optimization speed gain for a one million 
 * element array:
 *
 *    Time on my machine w/o register variables:  about 270,000 microseconds
 *    Time on my machine with " ":  about 230,000 microseconds
 * 
 *    The difference is about 40,000 microseconds.  
 * 
 *    40,000 is what percentage of 270,000?
 * 
 *              40,000 = 270,000(x / 100) 
 *                   4 = 27(x / 100)
 *              4 / 27 = x / 100
 *                   x = 400 / 27 
 *                   x = 14 = percentage speed gain
 */

/*
 * Three includes and two defines as specified in FreeBSD getrusage(2) 
 * man page.  
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#define  RUSAGE_SELF       0
#define  RUSAGE_CHILDREN  -1

/*
 * Define the number of elements our three arrays will feature. 
 */

#define NUM_ELEMENTS_0 10        /* Ten          */
#define NUM_ELEMENTS_1 1000      /* One thousand */
#define NUM_ELEMENTS_2 1000000   /* One million  */

/* timed_count is the structure that's filled in by count(). */

struct timed_count {
    unsigned int num_bits_on;  /* Number of ON bits in array. */
    long run_time;             /* Microseconds this took to calculate. */
};

/* Prototype count() */

void count(char *array_ptr, unsigned int len, 
    struct timed_count *timed_count_ptr); 

int main(int argc, char *argv[]) {
    char array_0[NUM_ELEMENTS_0];  /* A char array to sum ON bits in. */
    char array_1[NUM_ELEMENTS_1];  /* A larger one. */
    char array_2[NUM_ELEMENTS_2];  /* Yet larger. */

    struct timed_count timed_count = {0, 0};  /* Filled in by count() */

    /* 
     * Initialize our arrays to zeros, and then set some arbitary bits 
     * in each. 
     */

    memset(array_0, 0, sizeof(array_0));
    memset(array_1, 0, sizeof(array_1));
    memset(array_2, 0, sizeof(array_2));
    array_0[3] = 8;
    array_0[8] = 2;
    array_1[330] = 7; 
    array_1[820] = 19;
    array_2[172946] = 59;
    array_2[294600] = 80;

    /* 
     * Do timed counts of the ON bits in the three arrays, and report how 
     * long the individual counts took. 
     */

    printf("\n==> Number of array elements:  %d\n\n", NUM_ELEMENTS_0);
    count(array_0, NUM_ELEMENTS_0, &timed_count);
    printf("  Number of bits on:   %d\n", timed_count.num_bits_on);
    printf("  Time to count them:  %d microseconds\n\n", timed_count.run_time);

    printf("==> Number of array elements:  %d\n\n", NUM_ELEMENTS_1);
    count(array_1, NUM_ELEMENTS_1, &timed_count);
    printf("  Number of bits on:   %d\n", timed_count.num_bits_on);
    printf("  Time to count them:  %d microseconds\n\n", timed_count.run_time);
    
    printf("==> Number of array elements:  %d\n\n", NUM_ELEMENTS_2);
    count(array_2, NUM_ELEMENTS_2, &timed_count);
    printf("  Number of bits on:   %d\n", timed_count.num_bits_on);
    printf("  Time to count them:  %d microseconds\n\n", timed_count.run_time);

    return(0);
}

/***************************************************************************
 * count -- Count the number of ON bits in the array pointed to by the 
 *          first argument.  The use of `register' variables in this 
 *          functions makes it about 14% faster than without.  (See the 
 *          top of this file for my work.)
 * 
 * Parameters:
 * 
 *     register char *array_ptr  -- A pointer to the array to count 
 *                                  the ON bits in. 
 * 
 *     unsigned int len  -- The number of elements in the array. 
 * 
 *     struct timed_count *timed_count_ptr -- A pointer to the structure to
 *                                            fill in with the results. 
 *     
 ***************************************************************************/

void 
count(register char *array_ptr, unsigned int len, 
    struct timed_count *timed_count_ptr) {

    unsigned int element_num = 0;   /* Number of elements read. */
    register char shift_num;        /* Number of places shifted. */
    register unsigned char mask;    /* Mask byte to see if bits are ON. */

    /* These three definitions are for timing purposes. */

    struct rusage rusage;           /* Filled in by getrusage(). */
    long total_time_pre;            /* Total time before main work. */
    long total_time_post;           /* Total time after main work. */

    /* 
     * Set the number of ON bits back to zero to prepare for the next
     * invocation of the count() guts.
     */

    (*timed_count_ptr).num_bits_on = 0;  

    /* 
     * Get system and user mode times with getrsuage(), then add them 
     * together to get a CPU usage metric before the main work of this 
     * function happens.  The rusage and timeval structures are explained
     * in the man pages:  gettimeofday(2) and getrusage(2) (FreeBSD 4.2).
     */

    getrusage(RUSAGE_SELF, &rusage);
    total_time_pre = rusage.ru_stime.tv_usec + rusage.ru_utime.tv_usec;

    while (element_num != len) {    /* While there are more elements. */

        /* 
         * While we're not out of bits in the current element, if the 
         * current bit is on, increment the structure element num_bits_on.
         */

        for (shift_num = 0; shift_num < 8; ++shift_num) { 
            mask = (0x80 >> shift_num);  /* Shift the mask byte. */
            if ((*array_ptr & mask) != 0) {
                ++(*timed_count_ptr).num_bits_on;
            }
        }
        ++element_num;  /* We've completed another element (byte), so */
        ++array_ptr;    /* seek to the next element.                  */
    }    

    /* 
     * Get times from the microseconds clock again, calculate how long 
     * we took to run, put that value back into the timed_count.run_time 
     * structure element pointed to by timed_count_ptr, and Bob's your
     * uncle.  
     */

    getrusage(RUSAGE_SELF, &rusage);
    total_time_post = rusage.ru_stime.tv_usec + rusage.ru_utime.tv_usec;
    (*timed_count_ptr).run_time = total_time_post - total_time_pre;
}
