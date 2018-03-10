#ifndef MINI_PROJET_2_TOOLS_ERROR_H
#define MINI_PROJET_2_TOOLS_ERROR_H

void ncurses_error_err(int return_code, const char *message);

void ncurses_error_null(void *return_ptr, const char *message);

void ncurses_error_errno(int return_code);

#endif /*MINI_PROJET_2_TOOLS_ERROR_H*/
