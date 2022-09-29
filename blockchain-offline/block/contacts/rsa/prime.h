#include <stdint.h>
#include<inttypes.h>
#define NUM int64_t

void writeArr(char *file, NUM *arr, NUM num);
NUM readArr(char *file, NUM **arr);//,  int num) {
bool isPrime(NUM n);
NUM *genPrimes(NUM min, NUM max);
NUM randomPrime();
