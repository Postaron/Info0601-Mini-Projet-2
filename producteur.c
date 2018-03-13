#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include "ncurses.h"
#include "tools_error.h"
#include "gestionnaires.h"

void demarrageNcurses(WINDOW** fenetre, WINDOW** connexions, WINDOW** messages) {
	ncurses_error_null((*fenetre = newwin(0, 0, 0, 0)), "Erreur, creation fenetre principale.\n");
	ncurses_error_err(printw("F1 pour l'aide (si fini), F2 pour quitter.\n"), "Erreur affichage message.\n");
	ncurses_error_null((*connexions = subwin(*fenetre, LINES - 3, COLS / 2, 2, 0)),
					   "Erreur creation fenetre connexion.\n");
	ncurses_error_null((*messages = subwin(*fenetre, LINES - 3, (COLS / 2) - 1, 2, (COLS / 2) + 1)),
					   "Erreur creation fenetre messages.\n");
	ncurses_error_err(box(*connexions, 0, 0), "Erreur creation contour fenetre connexion.\n");
	ncurses_error_err(box(*messages, 0, 0), "Erreur creation contour fenetre message.\n");
	ncurses_error_err(wrefresh(*fenetre), "Erreur refresh fenetre principale.\n");
	ncurses_error_err(wrefresh(*connexions), "Erreur refresh fenetre connexion.\n");
	ncurses_error_err(wrefresh(*messages), "Erreur refresh fenetre message.\n");

}

/**
 * argv[] = X(nombre entier impair pour nbr chiffre pour nombre premier), N(nombre max de consommateurs),
 * 			nom_tube_tâches, nom_tube_résultat
 * @return
 */
int main(int argc, char* argv[]) {
	int X = 0, N = 0, tubeAno[2], carac, requete = 0, i;
	char* tubeTache = NULL, * tubeResultat = NULL;
	WINDOW* fenetre = NULL, * connexions = NULL, * messages = NULL;
	pid_t pid_gco = 0, pid_gtache = 0;
	siginfo_t siginfo;
	if (argc != 5) {
		fprintf(stderr, "Erreur, nombre d'argument invalide\n");
		exit(EXIT_FAILURE);
	}
	tubeTache = argv[3], tubeResultat = argv[4];
	X = atoi(argv[1]), N = atoi(argv[2]);
	if (X < 0) {
		X = -X;
	}
	if (N < 0) {
		N = -N;
	}
	if (pipe(tubeAno) == -1) {
		perror("Erreur :");
		exit(EXIT_FAILURE);
	}
	ncurses_initialiser(), ncurses_couleurs(), ncurses_souris();
	/* Vérification taille moitié terminal qui peut accueillir le nombre de consommateur max */
	if (((LINES - 3) * (COLS / 2)) < N) {
		ncurses_stopper();
		fprintf(stderr, "Erreur, terminal trop petit.\n");
		exit(EXIT_FAILURE);
	}
	demarrageNcurses(&fenetre, &connexions, &messages);
	/**
	 * TODO
	 * Affichage des connexions (uniquement l'interface)
	 */
	wmove(connexions, 1, 1);
	for (i = 0; i < N; ++i) {
		wattron(connexions, COLOR_PAIR(2));
		ncurses_error_err(wprintw(connexions, " "), "Erreur affichage des connexions.\n");
	}
	wrefresh(connexions);
	/*pid_gco = gestionnaire_connexion(tubeAno);
	pid_gtache = gestionnaire_taches(tubeAno);
	*/while ((carac = getch()) != KEY_F(2)) {
		/*if (pid_gco == siginfo.si_pid) {

		} else if (pid_gtache == siginfo.si_pid) {

		}*/
	}
	ncurses_stopper();
	delwin(messages);
	delwin(connexions);
	delwin(fenetre);
	return EXIT_SUCCESS;
}