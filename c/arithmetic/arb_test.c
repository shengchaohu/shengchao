#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "arb_int.h"
#include <limits.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc <2){
		long long int *c=(long long int*)malloc(sizeof(long long int));
		arb_int_t a;arb_int_t b;
		arb_from_string(&a, "12345123451234512345999");
		arb_duplicate (&b, a);
		arb_from_int(&b, 233235123459992);
		arb_compare(b,a);
		arb_add(a,b);
		arb_subtract(a,b);
		arb_multiply(a,b);
		arb_assign(b, a);
		char output[100];
		arb_to_string(b, output, sizeof(output));
		arb_to_int(b,c);
		arb_free (&a);
		arb_free (&b);
		free(c);
		return EXIT_SUCCESS;
	}
	if (argc==4){
		arb_int_t a;arb_int_t b;char output[1200];
		if(arb_from_string(&a, argv[1])){
			printf("the first argument is not number!\n");
			return EXIT_SUCCESS;
		}
		if(arb_from_string(&b, argv[3])){
			printf("the second argument is not number!\n");
			return EXIT_SUCCESS;
		}
		if (strcmp(argv[2],"+")==0){
			arb_add(a,b);
			arb_to_string(a, output, sizeof(output));
			printf("%s\n",output);
		}
		if (strcmp(argv[2],"-")==0){
			arb_subtract(a,b);
			arb_to_string(a, output, sizeof(output));
			printf("%s\n",output);
		}
		if (strcmp(argv[2],"*")==0){
			arb_multiply(a,b);
			arb_to_string(a, output, sizeof(output));
			printf("%s\n",output);
		}
		return EXIT_SUCCESS;
	}
	else{
		printf("insufficiant input\n");
		return EXIT_SUCCESS;
	}
}

