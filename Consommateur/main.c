#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include "constantes.h"
#include "tools.h"
#include "requete_reponse.h"
#include "primalite.h"
#include "handlers.h"

bool ok_code = false;
pid_t pid_connexion = 0;
int tubeTache = 0, tubeResultat = 0;


void handler_rtmin(int, siginfo_t*, void*);

/**
 * ligne d'argument : PID_connexion tube_tache tube_resultat
 * @return
 */
int main(int argc, char* argv[]) {
	union sigval val;
	struct sigaction actionRTMIN, actionRTMINPLUS, actionINT;
	reponse_t requete;
	unsigned long int tache;
	if (argc != 4) {
		fprintf(stderr, "Erreur, nombre d'argument insuffisant.\n");
		exit(EXIT_FAILURE);
	}
	pid_connexion = atoi(argv[1]);
	val.sival_int = CONNEXION;
	actionRTMIN.sa_flags = SA_SIGINFO;
	actionRTMINPLUS.sa_flags = SA_SIGINFO;
	actionRTMIN.sa_sigaction = handler_rtmin;
	actionRTMINPLUS.sa_sigaction = handler_rtmin_plus1;
	actionINT.sa_handler = handler_sigint;
	sigaction(SIGRTMIN, &actionRTMIN, NULL);
	sigaction(SIGRTMIN + 1, &actionRTMINPLUS, NULL);
	sigaction(SIGINT, &actionINT, NULL);
	sigqueue(pid_connexion, SIGRTMIN, val);
	pause();
	while (ok_code) {
		error(tubeTache = open(argv[2], O_RDONLY));
		error(tubeResultat = open(argv[3], O_WRONLY));
		error_fd_opened((int) read(tubeTache, &tache, sizeof(unsigned long int)));
		if (primalite(tache)) {
			requete.reponse_calcul = tache;
			error_fd_opened((int) write(tubeResultat, &requete, sizeof(reponse_t)));
		}
	}
	return EXIT_SUCCESS;
}

void handler_rtmin(int signo, siginfo_t* info, void* inutile) {
	if (signo == SIGRTMIN) {
		if (info->si_value.sival_int == CONNEXION) {
			ok_code = true;
		}
	}
}