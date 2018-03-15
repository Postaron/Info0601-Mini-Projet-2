#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include "handlers.h"
#include "ncurses.h"
#include "constantes.h"

void handler_sigint(int signal) {
	if (signal == SIGINT) {
		ncurses_stopper();
		if (kill(0, SIGINT) == -1) {
			perror("Erreur kill : ");
			exit(EXIT_FAILURE);
		}
		wait(NULL);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
}

void handler_connexion(int signo, siginfo_t* info, void* truc) {
	if (signo == SIGRTMIN) {
		union sigval val;
		if (nbrConnected < N) {
			++nbrConnected;
			val.sival_int = CONNEXION;
			sigqueue(info->si_pid, SIGRTMIN + 1, val);

		} else {
			val.sival_int = DECONNEXION;
			sigqueue(info->si_pid, SIGRTMIN + 1, val);
		}
	}
}