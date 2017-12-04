/*
 ============================================================================
 Name        : fixed_point_template.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */
/* intension of code is to familiarize fixed point operation
// and to keep everyone on same page.*/



#include <stdio.h>
#include <stdlib.h>

int main(void)

{


	/* Task 1, Play with the fractional bits and check quantisation error
	 * Task 2, Multiply 2 fixed numbers and identify overflows, underflows possible */

	float fl_value=1.234,fl_n_value=-1.234;
	short int fi_value,fi_n_value,fi_mult;
	int frac_bits=7;
	// considering 16bit operation
	// integer part needs only one bit, assigning 2bit to represent sign
	// rest of 14 bits are for fractional
	fi_value= (short int)(fl_value*(1 <<frac_bits));
	fi_n_value= (short int)(fl_n_value*(1 <<frac_bits));
	printf("\n fixed value =%d, quantisation error =%f",fi_value,fl_value-(float)fi_value/(1<<frac_bits));
	printf("\n fixed -ve value =%hd, quantisation error =%f",fi_n_value,fl_n_value-(fi_n_value/(float)(1<<frac_bits)));

	// multiply

	fi_mult=fi_value*fi_n_value;
	printf("\n fixed value  =%d,actual multiply = %f, fi_multiply=%f",fi_mult,fl_value*fl_n_value,(fi_mult/(float)(1<<frac_bits)));
   // Check why is this happening, So what do you conclude?

	printf("\n fixed value  =%d,actual multiply = %f, fi_multiply=%f",fi_mult,fl_value*fl_n_value,(fi_mult/(float)(1<<frac_bits*2)));
  // what do you infer from this?

}
