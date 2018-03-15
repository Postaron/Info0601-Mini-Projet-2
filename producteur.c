#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ncurses.h>
#include "ncurses.h"
#include "tools_error.h"
#include "requete_reponse.h"
#include "handlers.h"
#include "constantes.h"
#include "my_math.h"

int X = 0, N = 0, nbrConnected = 0;
pid_t* pidTabCo = NULL;
bool bool_tache = FALSE;

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
	int oldNbrConnected = nbrConnected;
	struct sigaction actionConnexion, actionINT;
	reponse_t reponse;
	close(tubeAno[0]);
	actionConnexion.sa_flags = SA_SIGINFO, actionINT.sa_flags = SA_SIGINFO;
	actionConnexion.sa_sigaction = handler_connexion, actionINT.sa_sigaction = handler_connexion_int;
	sigaction(SIGINT, &actionINT, NULL);
	sigaction(SIGRTMIN, &actionConnexion, NULL);
	while (N > 0) { /* N > 0, pas de signal SIGINT reçu (voir handler). */
		pause();
		if (oldNbrConnected < nbrConnected && N > 0) {
			reponse.connexion = CONNEXION;
			reponse.pid_processus = getpid();
			reponse.reponse_calcul = 0;
			ncurses_error_errno((int) write(tubeAno[1], &reponse, sizeof(reponse_t)));
		}
	}
	close(tubeAno[1]);
	free(pidTabCo);
	exit(EXIT_SUCCESS);
}

void gestionnaire_taches(int tubeAno[2], char* ECRITURE, char* LECTURE) {
	int tubeTache, tubeResultat;
	unsigned long int tache = puissance(10, X), fin = puissance(10, X + 1);
	int attente = 0;
	reponse_t reponse;
	struct sigaction actionINT;
	actionINT.sa_handler = handler_tache;
	sigaction(SIGINT, &actionINT, NULL);
	ncurses_error_errno(close(tubeAno[0]));
	ncurses_error_errno(mkfifo(ECRITURE, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP));
	ncurses_error_errno(mkfifo(LECTURE, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP));
	ncurses_error_errno(tubeTache = open(ECRITURE, O_WRONLY));
	ncurses_error_errno(tubeResultat = open(LECTURE, O_RDONLY));
	while (tache < fin || bool_tache == FALSE) {
		while ((attente < 1000 && tache < fin) || bool_tache == FALSE) {
			ncurses_error_errno((int) write(tubeTache, &tache, sizeof(unsigned long int)));
			attente++;
			tache++;
		}
		while (attente > 500 || bool_tache == FALSE) {
			ncurses_error_errno((int) read(tubeResultat, &reponse, sizeof(reponse_t)));
			reponse.pid_processus = getpid();
			ncurses_error_errno((int) write(tubeAno[1], &reponse, sizeof(reponse_t)));
			attente--;
		}
	}
	/*ncurses_error_errno((int) read(tubeResultat, &resultat, sizeof(unsigned long int)));
	ncurses_error_errno((int) write(tubeAno[1], &resultat, sizeof(unsigned long int)));*/
	ncurses_error_errno(close(tubeAno[1]));
	ncurses_error_errno(close(tubeTache));
	ncurses_error_errno(close(tubeResultat));
	ncurses_error_errno(unlink(ECRITURE));
	ncurses_error_errno(unlink(LECTURE));
	exit(EXIT_SUCCESS);
}

/**
 * argv[] = X(nombre entier impair pour nbr chiffre pour nombre premier), N(nombre max de consommateurs),
 * 			nom_tube_tâches, nom_tube_résultat
 * @return
 */
int main(int argc, char* argv[]) {
	int tubeAno[2], carac, i;

	char* tubeTache = NULL, * tubeResultat = NULL, * string = NULL;

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
		ncurses_error_null((pidTabCo = (pid_t*) calloc((size_t) N, sizeof(pid_t))), "Erreur allocation tableau PID.\n");
		gestionnaire_connexions(tubeAno);
	}
	if ((pid_gtache = fork()) == 0) {
		gestionnaire_taches(tubeAno, tubeTache, tubeResultat);
	}
	close(tubeAno[1]);
	ncurses_error_null((string = (char*) malloc(((unsigned) X * sizeof(char)) + 1)),
					   "Erreur allocation chaine de caractère réponse.\n");
	while ((carac = getch()) != KEY_F(2)) {
		ncurses_error_errno((int) read(tubeAno[0], &reponse, sizeof(reponse_t)));
		if (pid_gco == reponse.pid_processus) {
			/**
			 * TODO
			 * NCURSE : problème suivant : un qui se connecte puis se déconnecte, incohérence des couleurs.
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
			sprintf(string, "%ld", reponse.reponse_calcul);
			ncurses_error_err(wprintw(messages, "%s\n", string), "Erreur affichaqe resultat dans fenetre.\n");
			ncurses_error_err(wrefresh(messages), "Erreur refresh fenetre message apres ecriture dedans.\n");
		} else
			ncurses_error_err(ERR, "Erreur lecture du tube : aucun PID correspondant.\n");
	}
	ncurses_stopper();
	free(string);
	delwin(messages);
	delwin(connexions);
	delwin(fenetre);
	return EXIT_SUCCESS;
}