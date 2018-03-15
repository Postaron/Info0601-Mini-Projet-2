#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>
#include "ncurses.h"
#include "tools_error.h"
#include "requete_reponse.h"
#include "handlers.h"
#include "constantes.h"

int X = 0, N = 0, nbrConnected = 0;

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

void gestionnaire_connexions(int tubeAno[2]) {
	struct sigaction actionConnexion, actionINT;
	actionConnexion.sa_flags = SA_SIGINFO;
	actionConnexion.sa_sigaction = handler_connexion;
	sigaction(SIGRTMIN, &actionConnexion, NULL);
	pause();
}

void gestionnaire_taches(int tubeAno[2], int N) {
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
}

/**
 * argv[] = X(nombre entier impair pour nbr chiffre pour nombre premier), N(nombre max de consommateurs),
 * 			nom_tube_tâches, nom_tube_résultat
 * @return
 */
int main(int argc, char* argv[]) {
	int tubeAno[2], carac, i;

	char* tubeTache = NULL, * tubeResultat = NULL;

	WINDOW* fenetre = NULL, * connexions = NULL, * messages = NULL;

	pid_t pid_gco = 0, pid_gtache = 0;

	struct sigaction action;

	reponse_t reponse = {0, 0, 0};

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
	action.sa_handler = handler_sigint;
	sigaction(SIGINT, &action, NULL);
	wmove(connexions, 1, 1);
	for (i = 0; i < N; ++i) {
		wattron(connexions, COLOR_PAIR(2));
		ncurses_error_err(wprintw(connexions, " "), "Erreur affichage des connexions.\n");
		wattroff(connexions, COLOR_PAIR(2));
		ncurses_error_err(wprintw(connexions, " "), "Erreur affichage des connexions.\n");
	}
	wmove(connexions, 1, 1);
	wrefresh(connexions);
	if ((pid_gco = fork()) == 0) {
		gestionnaire_connexions(tubeAno);
	}
	if ((pid_gtache = fork()) == 0) {
		gestionnaire_taches(tubeAno, N);
	}
	close(tubeAno[1]);
	while ((carac = getch()) != KEY_F(2)) {
		ncurses_error_errno((int) read(tubeAno[0], &reponse, sizeof(reponse_t)));
		if (pid_gco == reponse.pid_processus) {
			/**
			 * TODO
			 * NCURSE : problème suivant : un qui se connecte puis se déconnecte, incohérence des couleurs. Fais chier.
			 */
			switch (reponse.connexion) {
				case CONNEXION:
					wattron(connexions, COLOR_PAIR(3));
					ncurses_error_err(waddch(connexions, ' '), "Erreur changement couleur connexion.\n");
					wattroff(connexions, COLOR_PAIR(3));
					ncurses_error_err(waddch(connexions, ' '), "Erreur changement couleur connexion.\n");
					break;
				case DECONNEXION:
					break;
				default:
					ncurses_error_err(ERR, "Erreur, valeur de connexion invalide.\n");
					break;
			}
		} else if (pid_gtache == reponse.pid_processus) {

		} else
			ncurses_error_err(ERR, "Erreur lecture du tube : aucun PID correspondant.\n");
	}
	ncurses_stopper();
	delwin(messages);
	delwin(connexions);
	delwin(fenetre);
	return EXIT_SUCCESS;
}