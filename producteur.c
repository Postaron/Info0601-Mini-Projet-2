
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "ncurses.h"

/**
 * argv[] = X(nombre entier impair), N(nombre max de consommateurs), nom_tube_tâches, nom_tube_résultat
 * @return
 */
int main(int argc, char *argv[]) {
	unsigned int X = 0, N = 0;
	char *tubeTache = NULL, *tubeResultat = NULL;
	WINDOW *fenetre = NULL, *connexions = NULL, *messages = NULL;
	if (argc != 5) {
		fprintf(stderr, "Erreur, nombre d'argument invalide\n");
		exit(EXIT_FAILURE);
	}
	tubeTache = argv[3], tubeResultat = argv[4];
	if (argv[1][0] == '-' && argv[2][0] == '-') {
		fprintf(stderr, "Erreur, les arguments doivent etre positifs.\n");
		exit(EXIT_FAILURE);
	}
	X = (unsigned) atoi(argv[1]), N = (unsigned) atoi(argv[2]);
	/*
	 * Vérification des arguments
	 */
	if ((X % 2) == 0) {
		fprintf(stderr, "Erreur, le nombre entier n'est pas impair.\n");
		exit(EXIT_FAILURE);
	}
	ncurses_initialiser(), ncurses_couleurs(), ncurses_souris();
	/* Vérification taille moitié terminal qui peut accueillir le nombre de consommateur max */
	if (((LINES - 3) * (COLS / 2)) < N) {
		ncurses_stopper();
		fprintf(stderr, "Erreur, terminal trop petit.\n");
		exit(EXIT_FAILURE);
	}
	if ((fenetre = newwin(0, 0, 0, 0)) == NULL) {
		ncurses_stopper();
		fprintf(stderr, "Erreur, creation fenetre principale.\n");
		exit(EXIT_FAILURE);
	}
	if (printw("F1 pour l'aide (si fini), F2 pour quitter.\n") == ERR) {
		ncurses_stopper();
		fprintf(stderr, "Erreur affichage message.\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}