
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "ncurses.h"
#include "tools_error.h"

void demarrage(WINDOW **fenetre, WINDOW **connexions, WINDOW **messages) {
	ncurses_error_null((*fenetre = newwin(0, 0, 0, 0)), "Erreur, creation fenetre principale.\n");
	ncurses_error_err(printw("F1 pour l'aide (si fini), F2 pour quitter.\n"), "Erreur affichage message.\n");
	ncurses_error_null((*connexions = subwin(*fenetre, LINES / 2, COLS / 2, 2, 0)),
					   "Erreur creation fenetre connexion.\n");
	ncurses_error_null((*messages = subwin(*fenetre, LINES / 2, COLS / 2, 2, (COLS / 2) + 1)),
					   "Erreur creation fenetre messages.\n");
	ncurses_error_err(box(*connexions, 0, 0), "Erreur creation contour fenetre connexion.\n");
	ncurses_error_err(box(*messages, 0, 0), "Erreur creation contour fenetre message.\n");
	ncurses_error_err(wrefresh(*fenetre), "Erreur refresh fenetre principale.\n");
	ncurses_error_err(wrefresh(*connexions), "Erreur refresh fenetre connexion.\n");
	ncurses_error_err(wrefresh(*messages), "Erreur refresh fenetre message.\n");

}

/**
 * argv[] = X(nombre entier impair), N(nombre max de consommateurs), nom_tube_tâches, nom_tube_résultat
 * @return
 */
int main(int argc, char *argv[]) {
	int X = 0, N = 0, tubeAno[2];
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
	X = atoi(argv[1]), N = atoi(argv[2]);
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
	demarrage(&fenetre, &connexions, &messages);
	return EXIT_SUCCESS;
}