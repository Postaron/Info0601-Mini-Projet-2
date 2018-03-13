#define _POSIX_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "handlers.h"
#include "ncurses.h"

void handler_sigint(int signal) {
	if (signal == SIGINT) {
		ncurses_stopper();
		if (kill(0, SIGINT) == -1) {
			perror("Erreur kill : ");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
}