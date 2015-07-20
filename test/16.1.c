
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * This program uses strings to represent floating-point numbers in the 
 * format used in chapter 16 of the text.  A typical string might look
 * like "+1.333E+2".  This includes functions to read, write, add, subtract
 * multiply, and divide floating-point numbers.
 */

/* Function prototypes.  See the definitions for more info. */

float read_float(void *float_str_ptr);
void *write_float(float f);
void *add_float(void *float_str_ptr1, void *float_str_ptr2);
void *sub_float(void *float_str_ptr1, void *float_str_ptr2);
void *mul_float(void *float_str_ptr1, void *float_str_ptr2);
void *div_float(void *float_str_ptr1, void *float_str_ptr2);

int main(int argc, char *argv[]) {
    char float_str[10];
    char float_str2[10];
    strcpy(float_str, "+4.204E+2");
    strcpy(float_str2, "-1.772E+2");

    /* Show off all the great new functions. */

    printf("%s     +     %s     =     %s\n", float_str, float_str,
        add_float(float_str, float_str));
    printf("%s     -     %s     =     %s\n", float_str, float_str2,
        sub_float(float_str, float_str2));
    printf("%s     *     %s     =     %s\n", float_str, float_str2, 
        mul_float(float_str, float_str2));
    printf("%s     /     %s     =     %s\n", float_str2, float_str2, 
        div_float(float_str2, float_str2));

    return(0);
}

/************************************************************************
 * read_float  --  Takes a pointer to a char string representation of a 
 *                 floating point number and returns a "real" floating
 *                 point number.  (Of the `float' type.)  For example:
 *                 the string "+1.333E+2" would result in a return 
 *                 float value of 133.3.
 * Parameters:
 * 
 *     void *float_str_ptr  --  A void pointer to a character string 
 *                              that represents a floating point number.
 *                              Such as "+1.333E+2". 
 * Returns:
 * 
 *     A (numerical) floating point number.
 ************************************************************************/
 
float read_float(void *float_str_ptr) {
    const char *str_ptr = float_str_ptr;  /* Convert from void pointer. */
    float fraction;   /* The fraction part of the number, like "2.472". */
    int exponent = 0; /* The exponent part of the number. */

    sscanf(str_ptr, "%fE%d", &fraction, &exponent);
    if (exponent > 0) {  /* Positive exponent. */
        while (exponent > 0) {
            fraction = fraction / 10;  /* Shift the decimal point left. */
            --exponent; 
        }
    }
    else if (exponent < 0) {  /* Negative exponent. */
        while (exponent < 0) {
            fraction = fraction * 10;  /* Shift the decimal point right. */
            ++exponent;
        }
    }
    return(fraction);
}

/************************************************************************
 * write_float  --  Converts a floating point number to a char string 
 *                  that represents it in scientific notation format. 
 *                 
 * Parameters:
 * 
 *     float f  --  A floating point number, such as 472.47268
 *  
 * Returns:
 * 
 *     void *   --  A void pointer to the char string representation of 
 *                  a float number, such as:  "+4.274E+2".
 ************************************************************************/

void *write_float(float f) {
    void *return_val;        /* The return value. */    
    unsigned char *str_ptr;  /* The return value while under construction. */
    char exponent = 0;       /* The scientific notation exponent. */

    /* Allocate enough memory to hold the string. */

    str_ptr = malloc(sizeof(char) * 10);
    if (str_ptr == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(8);
    }

    /* Shift the decimal place around to prepare scientific notation. */

    while ((f >= 10) || (f <= -10)) {
        f = f / 10;  /* Shift the decimal point to the left. */
        ++exponent;  /* Adjust the scientific notation exponent. */
    }
    while ((f < 1) && (f > -1)) {
        f = f * 10;  /* Shift the decimal point to the right. */
        --exponent;  /* Adjust the scientific notation exponent. */
    }
    sprintf(str_ptr, "%+1.3fE%+d", f, exponent);
    return_val = str_ptr;  /* Convert to void pointer. */
    return(return_val);    
}

/************************************************************************
 * add_float  --  Add two floating point numbers derived from char string
 *                representations in scientific notation format. 
 *                 
 * Parameters:
 * 
 *     void *float_str_ptr1  --  A pointer to a string representing a 
 *                               floating-point number in scientific 
 *                               notation.   Example:  "+1.500E+0"
 *
 *     void *float_str_ptr2  --  Same as above.  
 *  
 * Returns:
 * 
 *     void *   --  A void pointer to the char string representation of 
 *                  the sum of the arguments; for example:  "+4.274E+2".
 ************************************************************************/

void *add_float(void *float_str_ptr1, void *float_str_ptr2) {
    float f1;   /* The first number to sum. */
    float f2;   /* The second number to sum. */
    float sum;  /* The sum of these two numbers. */

    f1 = read_float(float_str_ptr1);  /* Convert from strings to numbers. */
    f2 = read_float(float_str_ptr2);
    sum = f1 + f2;  /* Add 'em up. */

    return(write_float(sum));
}

/************************************************************************
 * sub_float  --  Subtrace two floating point numbers derived from 
 *                char string representations in scientific notation 
 *                format. 
 *                 
 * Parameters:
 * 
 *     void *float_str_ptr1  --  A pointer to a string representing a 
 *                               floating-point number in scientific 
 *                               notation.   Example:  "+1.500E+0"
 *                               This is the number to subtract *from*.
 *
 *     void *float_str_ptr2  --  This is the number to *subtract*.
 *  
 * Returns:
 * 
 *     void *   --  A void pointer to the char string representation of 
 *                  the difference of the arguments; for example:  
 *                  "+4.274E+2".
 ************************************************************************/

void *sub_float(void *float_str_ptr1, void *float_str_ptr2) {
    float f1;          /* The number to subtract _from_. */
    float f2;          /* The number to _subtract_. */
    float difference;  /* The difference of these two numbers. */

    f1 = read_float(float_str_ptr1);  /* Convert from strings to numbers. */
    f2 = read_float(float_str_ptr2);
    difference = f1 - f2;  /* Do the subtraction. */

    return(write_float(difference));
}

/************************************************************************
 * mul_float  --  Multiply together two floating point numbers derived 
 *                from char string representations in scientific notation 
 *                format. 
 *                 
 * Parameters:
 * 
 *     void *float_str_ptr1  --  A pointer to a string representing a 
 *                               floating-point number in scientific 
 *                               notation.   Example:  "+1.500E+0"
 *
 *     void *float_str_ptr2  --  Same as above for the other number.
 *  
 * Returns:
 * 
 *     void *   --  A void pointer to the char string representation of 
 *                  the product of the arguments; for example:  
 *                  "+4.274E+2".
 ************************************************************************/

void *mul_float(void *float_str_ptr1, void *float_str_ptr2) {
    float f1;          /* The first number. */
    float f2;          /* The second number. */
    float product;     /* The product of these two numbers. */

    f1 = read_float(float_str_ptr1);  /* Convert from strings to numbers. */
    f2 = read_float(float_str_ptr2);
    product = f1 * f2;                /* Do the multiplication. */

    return(write_float(product));
}

/************************************************************************
 * div_float  --  Divide the first argument by the second, where the 
 *                numbers are floating point derived from char string
 *                representations in scientific notation format.  
 *                 
 * Parameters:
 * 
 *     void *float_str_ptr1  --  A pointer to a string representing a 
 *                               floating-point number in scientific 
 *                               notation.   Example:  "+1.500E+0"
 *                               This is the dividend (aka number on 
 *                               top of the fraction).
 * 
 *     void *float_str_ptr2  --  Same as above for the divisor (aka
 *                               number on bottom of the fraction).
 *  
 * Returns:
 * 
 *     void *   --  A void pointer to the char string representation of 
 *                  the quotient of the arguments; for example:  
 *                  "+4.274E+2".
 ************************************************************************/

void *div_float(void *float_str_ptr1, void *float_str_ptr2) {
    float f1;          /* The first number. */
    float f2;          /* The second number. */
    float quotient;    /* The product of these two numbers. */

    f1 = read_float(float_str_ptr1);  /* Convert from strings to numbers. */
    f2 = read_float(float_str_ptr2);
    quotient = f1 / f2;               /* Do the division. */

    return(write_float(quotient));
}
