#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
	NUM d;
	NUM x;
	NUM y;
} EE;

EE extended_euclid(NUM a, NUM b);
NUM modulo(NUM x, NUM N);
void decimal_to_binary(NUM op1, NUM aOp[]);
NUM get_d(NUM e, NUM phi);
