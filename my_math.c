#include "my_math.h"

unsigned long int puissance(int number, int power) {
	int i;
	unsigned long int result = (unsigned) number;
	for (i = 2; i <= power; ++i) {
		result *= number;
	}
	return result;
}
