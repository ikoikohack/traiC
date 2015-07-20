#include <stdio.h>
#include <string.h>  /* For memcpy() definition. */
#include <memory.h>  /* For memset() definition. */
#include <stdlib.h>  /* For malloc() definition. */

/*
 * This program uses my own optimized version of the library function 
 * memcpy(), yocpy(), and compares the performance to the "real" 
 * memcpy().  
 */

/*
 * Five preprocessor directives as specified in the FreeBSD getrusage(2)
 * man page.  (Used to time this stuff.)
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h> 
#define RUSAGE_SELF       0
#define RUSAGE_CHILDREN  -1

#define ARRAY_SIZE 1000000  /* Ten million elements. */

/* Prototype my "fly" memcpy() look-alike, yocpy(). */

void *yocpy(void *dst, const void *src, size_t len); 

int main(int argc, char *argv[]) {
    struct rusage rusage;  /* Structure for getrusage() to fill in. */

    long sys_pre_memcpy;   /* Kernel-mode time before memcpy(). */
    long user_pre_memcpy;  /* User-mode time before memcpy(). */
    long sys_post_memcpy;  /* Kernel-mode time after memcpy(). */
    long user_post_memcpy; /* User-mode time after memcpy(). */
    long memcpy_time_sys;  /* Kernel-mode time used by memcpy(). */
    long memcpy_time_user; /* User-mode time used by memcpy(). */

    long sys_pre_yocpy;    /* Same metrics for my very own yocpy(). */
    long user_pre_yocpy; 
    long sys_post_yocpy;  
    long user_post_yocpy;
    long yocpy_time_sys;
    long yocpy_time_user;

    double yocpy_total_time;   /* The total time it took these to run. */
    double memcpy_total_time;  /* (User time + System time).           */

    /* 
     * The percent of time memcpy() took to do the work.  (As compared to 
     * yocpy() at 100%.  For instance:  memcpy() runs in 40% the time that
     * yocpy() takes.  This is used near the end of main().
     */

    float percent_faster;  

    /* src_array is copied to dest_array to time memcpy() and yocpy(). */

    unsigned char src_array[ARRAY_SIZE];
    unsigned char dest_array[ARRAY_SIZE];
    unsigned char *src_array_ptr = src_array;
    unsigned char *dest_array_ptr = dest_array;

    /* 
     * orig_dst_ptr is the pointer returned from yocpy() to the original
     * destination string before it was overwritten with the source 
     * string.  This is here to copy the behavior of memcpy(), although
     * there are side effects that I don't understand.  
     * 
     * In order to save a copy of the original dest. string in yocpy(), 
     * yocpy() malloc()'s enough memory to save the string.  It has to be 
     * explicitly free()'d.  memcpy() doesn't have any requirement 
     * like that, which must mean that memcpy() doesn't use malloc() 
     * to make space to keep a copy of the original destination string.
     * I'm not sure what memcpy() does instead.  Is this obvious or what?  
     */

    void *orig_dst_ptr;  /* To hold address returned by yocpy(). */

    /* Fill the source array with `250's.  (Arbitrary number.) */

    memset(src_array, 0xf0, sizeof(src_array));

	printf("%d %d\n",sizeof(src_array)),sizeof(*src_array_ptr);
    /* 
     * Get the kernel and user mode times right before we do our 
     * memcpy(), so that we can subract them from the post memcpy() 
     * times to see how long memcpy() took to run.
     */

    if ((getrusage(RUSAGE_SELF, &rusage)) == -1) {
        fprintf(stderr, "%s:  Error calling getrusage().\n", argv[0]);
        exit(8);
    } 
    sys_pre_memcpy = rusage.ru_stime.tv_usec;   /* Kernel-mode time. */
    user_pre_memcpy = rusage.ru_utime.tv_usec;  /* User-mode time. */

    /* Do our memcpy(). */

    memcpy(dest_array_ptr, src_array_ptr, sizeof(src_array_ptr));

    /*
     * Now, get kernel and user times with getrusage() again, find the 
     * difference between them and the original values, and we know 
     * how long memcpy() took to do the deed.
     */

    if ((getrusage(RUSAGE_SELF, &rusage)) == -1) {
        fprintf(stderr, "%s:  Error calling getrusage().\n", argv[0]);
        exit(8);
    } 
    sys_post_memcpy = rusage.ru_stime.tv_usec;   /* Kernel-mode time. */
    user_post_memcpy = rusage.ru_utime.tv_usec;  /* User-mode time. */

    /* 
     * Do our arithmetic to see how long memcpy() took to run and 
     * print some results.
     */

    memcpy_time_sys = sys_post_memcpy - sys_pre_memcpy;
    memcpy_time_user = user_post_memcpy - user_pre_memcpy;

    printf("\nmemcpy() times copying a %d element array:\n\n",
        ARRAY_SIZE);
    printf("%17s  %20d microseconds\n", "User-mode time:", 
        memcpy_time_user);
    printf("%17s  %20d microseconds\n", "Kernel-mode time:", 
        memcpy_time_sys);
    printf("%17s  %20d microseconds\n\n", "Real time:", 
        (memcpy_time_user + memcpy_time_sys));

    /* 
     * Now switch to timing my memcpy() look-alike, yocpy(). 
     *
     * First, get the pre-yocpy() CPU time utilization. 
     */

    if ((getrusage(RUSAGE_SELF, &rusage)) == -1) {
        fprintf(stderr, "%s:  Error calling getrusage().\n", argv[0]);
        exit(8);
    } 
    sys_pre_yocpy = rusage.ru_stime.tv_usec;   /* Kernel-mode time. */
    user_pre_yocpy = rusage.ru_utime.tv_usec;  /* User-mode time. */

    /* Do our yocpy(). */

    struct timeval start,end;
	gettimeofday(&start,NULL);



    orig_dst_ptr = yocpy(dest_array_ptr, src_array_ptr, 
        10000);
   
    /* 
     * This has to be here because yocpy() malloc()'s the space to store
     * a copy of the original destination string.  It needs to be free()'d
     * explicitly after being used for anything.  See above in this same
     * function where I declare orig_dst_ptr for more details.  
     */

    free(orig_dst_ptr); 


	gettimeofday(&end,NULL);


	printf("%lf\n",(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)*1.0E-6);



    /* Get the post-yocpy() times. */

    if ((getrusage(RUSAGE_SELF, &rusage)) == -1) {
        fprintf(stderr, "%s:  Error calling getrusage().\n", argv[0]);
        exit(8);
    } 
    sys_post_yocpy = rusage.ru_stime.tv_usec;   /* Kernel-mode time. */
    user_post_yocpy = rusage.ru_utime.tv_usec;  /* User-mode time. */

    /* Do our arithmetic to see how long yocpy() took to run. */

    yocpy_time_sys = sys_post_yocpy - sys_pre_yocpy;
    yocpy_time_user = user_post_yocpy - user_pre_yocpy;

    printf("yocpy() times copying a %d element array:\n\n",
        ARRAY_SIZE);
    printf("%17s  %20d microseconds\n", "User-mode time:", 
        yocpy_time_user);
    printf("%17s  %20d microseconds\n", "Kernel-mode time:", 
        yocpy_time_sys);
    printf("%17s  %20d microseconds\n\n", "Real time:", 
        (yocpy_time_user + yocpy_time_sys));

    /* 
     * Calculate and print how much faster memcpy() is than yocpy().
     */

    yocpy_total_time = (yocpy_time_user + yocpy_time_sys);
    memcpy_total_time = (memcpy_time_user + memcpy_time_sys);

    percent_faster = (memcpy_total_time / yocpy_total_time) * 100;
    printf("  ==> memcpy() runs in %.1f%% the time of yocpy()\n\n", 
        percent_faster);
    return(0);
}

/*********************************************************************
 * yocpy  --  copy byte string
 * 
 * Parameters:
 * 
 *     dst  - destination string pointer
 *     src  - source string pointer
 *     len  - number of bytes to copy
 *
 * Returns:
 * 
 *     yocpy() returns a pointer to the original value of dst.  
 *********************************************************************/

void *yocpy(void *dst, const void *src, size_t len) {

    /* 
     * Convert the void pointer parameters to a type that can be 
     * dereferenced. 
     */

    register unsigned char *dst_ptr = dst;
    register const unsigned char *src_ptr = src;

    size_t char_num;  /* Used in loop to make sure we don't exceed len. */
	int count;
    /* 
     * Store the orignal value of the destination string, so it can 
     * be used for the return value.  The memory allocated to 
     * orig_dst_ptr will have to be free()'d in the calling 
     * environment.  Is that bad coding practice?  (As opposed to using
     * free() *inside* this function?)
     */

    unsigned char *orig_dst_ptr = malloc(sizeof(*dst_ptr));
    if (orig_dst_ptr == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(8);
    }
    for (char_num = 0; char_num < len; ++char_num) {
        *dst_ptr = *orig_dst_ptr;  /* Copy a char. */
        ++dst_ptr;                 /* Advance the pointers. */
        ++orig_dst_ptr;
	count++;
    }
 
	printf("%d",count);
    /* Copy `len' bytes from the source string to the destination. */

    dst_ptr = dst;  /* Reset the destination ptr to the base address. */
    for (char_num = 0; char_num < len; ++char_num) {
        *dst_ptr = *src_ptr;  /* Copy a char. */
        ++src_ptr;            /* Advance the pointers. */
        ++dst_ptr;
	count++;
    }
	printf("%d",count);
 
    /* 
     * Return the the ptr to the original destination value to its 
     * base address, and return it. 
     */
	printf("ok\n");
	printf("%d\n",(int)len);
    orig_dst_ptr = orig_dst_ptr - sizeof(*orig_dst_ptr); 
    return (orig_dst_ptr);
}

