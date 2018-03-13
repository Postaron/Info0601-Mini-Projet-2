#include "gestionnaires.h"

#include <unistd.h>

pid_t gestionnaire_taches(int* tubeAno) {
	pid_t fils;
	if ((fils = fork()) == 0) {

	} else
		return fils;
	return fils;
}

pid_t gestionnaire_connexion(int* tubeAno) {
	pid_t fils;
	if ((fils = fork()) == 0) {

	} else
		return fils;
	return fils;
}
