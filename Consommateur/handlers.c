#include "handlers.h"
#include "constantes.h"
#include "tools.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler_sigint(int signo) {
	if (signo == SIGINT) {
		union sigval val;
		val.sival_int = DECONNEXION;
		sigqueue(pid_connexion, SIGRTMIN + 1, val);
		error_fd_opened(close(tubeTache));
		error(close(tubeResultat));
		exit(EXIT_SUCCESS);
	}
}

void handler_rtmin_plus1(int signo, siginfo_t* info, void* rien) {
	if (signo == (SIGRTMIN + 1) || info->si_pid == pid_connexion) {
		error_fd_opened(close(tubeTache));
		error(close(tubeResultat));
		exit(EXIT_FAILURE);
	}
}