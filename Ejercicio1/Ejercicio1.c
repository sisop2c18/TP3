#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

/*#################################################
#             Sistemas Operativos                 #
#       Trabajo Práctico 3 - Ejercicio 1          #
#       Nombre del Script: Ejercicio1.c           #
#                                                 #
#       Integrantes:                              #
#         Miller, Lucas            39353278       #
#         Ramos, Micaela           39266928       #
#         Sapaya, Nicolás Martín   38319489       #
#         Schafer, Federico        39336856       #
#         Secchi, Lucas            39267345       #
#                                                 #
#       Instancia de Entrega: Entrega             #
#                                                 #
###################################################*/

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
	      pidb1, pidb2, pidb3, pidb4, pidb5,
	      pidz1, pidz2,
	      pidd1, pidd2, pidd3;

	if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
	    printf("La sintaxis es ./Client [Materia]\nFormato: ./Client [string]\nEjemplo: ./Client Algebra\nPara ver los zombies: ps aux | awk '{ print $8 " " $2 }' | grep -w Z\nPara ver los demonios: ps aux | awk '{ print $8 " " $2 }' | grep -w Ss\n");
	    exit(1);
	}else if(argc >= 2){
	    printf("Error, cantidad de parametros inadecuados, para ayuda: ./Ejercicio1 -h\n");
	    exit(1);
	}

	if ( (pidh1=fork()) == 0 ){
		if( (pidn1=fork()) == 0){
			if( (pidb1=fork()) == 0){
				mostrar(getpid(), 4, getppid(), "BISNIETO");	
			}else{
				if( (pidb2=fork()) == 0){
					mostrar(getpid(), 4, getppid(), "BISNIETO");
				}else{
					mostrar(getpid(), 3, getppid(), "NIETO");				
				}			
			}			
		}else{
			if( (pidn2=fork()) == 0){
				if( (pidb3=fork()) == 0){
					mostrar(getpid(), 4, getppid(), "BISNIETO");				
				}else{
					if( (pidb4=fork()) == 0){
						mostrar(getpid(), 4, getppid(), "BISNIETO");				
					}else{
						mostrar(getpid(), 3, getppid(), "NIETO");	
					}
				}	
			}else{
				mostrar(getpid(), 2, getppid(), "HIJO");
			}
		}
   	}else{
		if ( (pidh2=fork()) == 0 ){
            if( (pidn3=fork()) == 0){
				if( (pidb5=fork()) == 0){
					if( (pidz1=fork()) == 0){
						mostrar(getpid(), 5, getppid(), "ZOMBIE");
						exit(0);
					}else{
						mostrar(getpid(), 4, getppid(), "BISNIETO");
					}								
				}else{
					if( (pidz2=fork()) == 0){
						mostrar(getpid(), 4, getppid(), "ZOMBIE");
						exit(0);
					}else{
						mostrar(getpid(), 3, getppid(), "NIETO");				
					}	
				}
			}else{
				if( (pidd1=fork()) == 0){
					if( (pidd2=fork()) == 0){
						mostrar(getpid(), 4, getppid(), "DEMONIO");
						setsid();				
					}else{
						if( (pidd3=fork()) == 0){
							mostrar(getpid(), 4, getppid(), "DEMONIO");
							setsid();				
						}else{
							mostrar(getpid(), 3, getppid(), "DEMONIO");
							setsid();	
						}
					}	
				}else{
					mostrar(getpid(), 2, getppid(), "HIJO");
				}
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
