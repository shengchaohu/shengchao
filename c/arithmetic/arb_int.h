#include<stddef.h>

struct arb_int;
typedef struct arb_int arb_int;
typedef arb_int* arb_int_t;
int isnumstring(char* s);
char* reversecopy(char* s);
void arb_free (arb_int_t * i);
int arb_duplicate (arb_int_t * new, const arb_int_t original);
int arb_from_string (arb_int_t * i, const char * s);
int arb_from_int (arb_int_t * i, signed long long int source);
int arb_to_string (const arb_int_t i, char * buf, size_t max);
int arb_to_int (const arb_int_t i, long long int * out);
int arb_assign (arb_int_t x, const arb_int_t y);

void arb_zerocheck(arb_int_t u);
int arb_add (arb_int_t x, const arb_int_t y);
int arb_add_model(arb_int_t x, arb_int_t y);
int arb_subtract (arb_int_t x, const arb_int_t y);
int arb_substract_model(arb_int_t x, arb_int_t y);
arb_int_t arb_multiply_one(arb_int_t x, char v);
arb_int_t arb_multiply_byten(arb_int_t x,int times);
int arb_multiply (arb_int_t x, const arb_int_t y);
int arb_compare (const arb_int_t x, const arb_int_t y);
int arb_digit_count (const arb_int_t x);

