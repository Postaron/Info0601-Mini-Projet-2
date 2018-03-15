#include "primalite.h"
#include <math.h>

bool primalite(unsigned long int nombre) {
	unsigned long int n = 0, i;
	if ((nombre % 2) == 0)
		return false;
	n = (unsigned long int) sqrt(nombre);
	for (i = 3; i <= n; i += 2)
		if (nombre % i == 0)
			return false;
	return true;
}
