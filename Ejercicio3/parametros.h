#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

#include <libgen.h>
#include <errno.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ES_NUMERO(x) ((x > 47 && x < 58)) ? 1 : 0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 	clean_stdin(void);
void 	procesar(double precio);
void 	escribir(pid_t pid);
void 	salirEscribir();
void 	salirLeer();