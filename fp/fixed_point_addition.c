/*
 ============================================================================
 Name        : fixed_point_template.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */
// intension of code is to familiarize fixed point operation
// and to keep everyone on same page.
// most of the arithmetic operations are discussed

#include <stdio.h>
#include <stdlib.h>

int main(void)

{

	/* Task 1, Play with the fractional bits and check quantisation error
	 * Task 2, add 2 fixed numbers and identify overflows, underflows possible */

	float fl_value=1.23456,fl_n_value=-1.23456;
	short int fi_value,fi_n_value,fi_add;
	const int frac_bits=14;
	// considering 16bit operation
	// integer part needs only one bit, assigning 2bit to represent sign
	// rest of 14 bits are for fractional
	fi_value= (short int)(fl_value*(1 <<frac_bits));
	fi_n_value= (short int)(fl_n_value*(1 <<frac_bits));
	printf("\n fixed value =%hd, quantisation error =%f",fi_value,fl_value-(float)fi_value/(1<<frac_bits));
	printf("\n fixed value -ve =%hd, quantisation error =%f",fi_n_value,fl_n_value-(fi_n_value/(float)(1<<frac_bits)));

	// addition

	fi_add=fi_value-fi_n_value;
	printf("\n fixed value  =%hu,actual sum = %f, addtion=%f",fi_add,fl_value-fl_n_value,((unsigned short int)fi_add/(float)(1<<frac_bits)));
   // Check why is this happening, So what do you conclude?
   // Hint re run the code changing frac_bits and compare results now.
}