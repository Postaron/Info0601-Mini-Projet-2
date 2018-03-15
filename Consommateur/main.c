#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/**
 * ligne d'argument : PID_connexion tube_tache tube_resultat
 * @return
 */
int main(int argc, char* argv[]) {
	pid_t pid_connexion = 0;
	int tubeTache = 0, tubeResultat = 0;
	if (argc != 4) {
		fprintf(stderr, "Erreur, nombre d'argument insuffisant.\n");
		exit(EXIT_FAILURE);
	}
	pid_connexion = atoi(argv[1]);
	return EXIT_SUCCESS;
}