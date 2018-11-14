#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MUTEX "/mutex"

/*
2 procesos hijos
3 procesos nietos
5 procesos bisnietos
2 procesos zombies, en cualquier nivel
3 procesos demonios, que deben quedar activos
*/

sem_t * mutex;

void mostrar(int pid, int gen, int ppid, char* parentesco){
	sem_wait(mutex);
	printf("Soy el proceso con PID %d y pertenezco a la generación Nº %d.\nPid: %d Pid padre: %d Parentesco: %s\n\n", pid, gen, pid, ppid, parentesco);
	sem_post(mutex);
}

int main(int argc, char *argv[]){

	char fin[32];

	mutex = sem_open(MUTEX, O_CREAT | O_EXCL, 0666, 1);

	pid_t pidh1, pidh2,
	      pidn1, pidn2, pidn3,
	      pidb1, pidb2, pidb3, pidb4, pidb5;

	int statush1, statush2, 
	    statusn1, statusn2, statusn3,
	    statusb1, statusb2, statusb3, statusb4, statusb5;

	if ( (pidh1=fork()) == 0 ){
		if( (pidn1=fork()) == 0){
			if( (pidb1=fork()) == 0){
				mostrar(getpid(), 4, getppid(), "BISNIETO");	
			}
			else{
				if( (pidb2=fork()) == 0){
					mostrar(getpid(), 4, getppid(), "BISNIETO");
				}
				else{
					mostrar(getpid(), 3, getppid(), "NIETO");				
				}			
			}
						
		}
		else{
			if( (pidn2=fork()) == 0){
				if( (pidb3=fork()) == 0){
					mostrar(getpid(), 4, getppid(), "BISNIETO");				
				}
				else{
					if( (pidb4=fork()) == 0){
						mostrar(getpid(), 4, getppid(), "BISNIETO");				
					}
					else{
						mostrar(getpid(), 3, getppid(), "NIETO");	
					}
				}	
			}
			else{
				mostrar(getpid(), 2, getppid(), "HIJO");
			}
		}
    	}
	else{
		if ( (pidh2=fork()) == 0 ){
            if( (pidn3=fork()) == 0){
				if( (pidb5=fork()) == 0){
					mostrar(getpid(), 4, getppid(), "BISNIETO");								
				}
				else{
					mostrar(getpid(), 3, getppid(), "NIETO");
				}
			}
			else{
				mostrar(getpid(), 2, getppid(), "HIJO");
			}
        }else{
            mostrar(getpid(), 1, getppid(), "PADRE");
        }
    }

	fgets(fin,32,stdin);

    sem_close(mutex);
	sem_unlink(MUTEX);
	return 0;
}
