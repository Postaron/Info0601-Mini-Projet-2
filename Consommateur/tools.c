#include "tools.h"
#include "handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error(int error) {
	if (error == -1) {
		perror("Erreur : ");
		exit(EXIT_FAILURE);
	}
}

void error_fd_opened(int error_here) {
	if (error_here == -1) {
		error(close(tubeTache));
		error(close(tubeResultat));
		perror("Erreur : ");
		exit(EXIT_FAILURE);
	}
}
