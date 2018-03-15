#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <ncurses.h>
#include "handlers.h"
#include "ncurses.h"
#include "constantes.h"

/**
 * À modifier pour permettre de finir le programme correctement.
 * @param signal
 */
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
			if (info->si_value.sival_int == CONNEXION) {
				int i;
				for (i = 0; i < N; ++i) {
					if (pidTabCo[i] == DECONNEXION)
						pidTabCo[i] = info->si_pid;
				}
				++nbrConnected;
				val.sival_int = CONNEXION;
				sigqueue(info->si_pid, SIGRTMIN + 1, val);
			} else if (info->si_value.sival_int == DECONNEXION) {
				int i;
				for (i = 0; i < N; ++i) {
					if (pidTabCo[i] == info->si_pid)
						pidTabCo[i] = DECONNEXION;
				}
				--nbrConnected;
			}
		} else {
			val.sival_int = DECONNEXION;
			sigqueue(info->si_pid, SIGRTMIN + 1, val);
		}
	}
}

void handler_connexion_int(int signo, siginfo_t* info, void* inutile) {
	if (signo == SIGINT && info->si_pid == getppid()) {
		int i;
		for (i = 0; i < N; ++i) {
			if (pidTabCo[i] == DECONNEXION) {
				union sigval val;
				val.sival_int = SHUTDOWN;
				sigqueue(pidTabCo[i], SIGRTMIN + 1, val);
				waitpid(pidTabCo[i], NULL, 0);
			}
		}
		N = 0;
	}
}

void handler_tache(int signo) {
	if (signo == SIGINT) {
		bool_tache = TRUE;
	}
}