#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	pid_t pidh1, pidh2,
	      pidn1, pidn2, pidn3,
	      pidb1, pidb2, pidb3, pidb4, pidb5;

	int statush1, statush2, 
	    statusn1, statusn2, statusn3,
	    statusb1, statusb2, statusb3, statusb4, statusb5;

	if ( (pidh1=fork()) == 0 ){
		if( (pidn1=fork()) == 0){
			if( (pidb1=fork()) == 0){
				printf("Soy el proceso con PID %d y pertenezco a la generación Nº 4.\nPid: %d Pid padre: %d Parentesco: BISNIETO\n\n", 
				getpid(), getpid(), getppid() );
			}
			else{
				if( (pidb2=fork()) == 0){
				printf("Soy el proceso con PID %d y pertenezco a la generación Nº 4.\nPid: %d Pid padre: %d Parentesco: BISNIETO\n\n", 
				getpid(), getpid(), getppid() );
				}
				else{
					waitpid(pidb1, &statusb1, 0);
					waitpid(pidb2, &statusb2, 0);
					printf("Soy el proceso con PID %d y pertenezco a la generación Nº 3.\nPid: %d Pid padre: %d Parentesco: NIETO\n\n", 
					getpid(), getpid(), getppid() );
				
				}			
			}
						
		}
		else{
			if( (pidn2=fork()) == 0){
				if( (pidb3=fork()) == 0){
					printf("Soy el proceso con PID %d y pertenezco a la generación Nº 4.\nPid: %d Pid padre: %d Parentesco: BISNIETO\n\n", 
				getpid(), getpid(), getppid() );				
				}
				else{
					if( (pidb4=fork()) == 0){
						printf("Soy el proceso con PID %d y pertenezco a la generación Nº 4.\nPid: %d Pid padre: %d Parentesco: BISNIETO\n\n", 
						getpid(), getpid(), getppid() );				
					}
					else{
						waitpid(pidb3, &statusb3, 0);
						waitpid(pidb4, &statusb4, 0);
						printf("Soy el proceso con PID %d y pertenezco a la generación Nº 3.\nPid: %d Pid padre: %d Parentesco: NIETO\n\n", 
					getpid(), getpid(), getppid() );	
					}
				}	
			}
			else{
				waitpid(pidn1, &statusn1, 0);
				waitpid(pidn2, &statusn2, 0);
				printf("Soy el proceso con PID %d y pertenezco a la generación Nº 2.\nPid: %d Pid padre: %d Parentesco: HIJO\n\n", 
				getpid(), getpid(), getppid() );
			}
		}
    	}
	else{
		if ( (pidh2=fork()) == 0 ){
            		if( (pidn3=fork()) == 0){
				if( (pidb5=fork()) == 0){
					printf("Soy el proceso con PID %d y pertenezco a la generación Nº 4.\nPid: %d Pid padre: %d Parentesco: BISNIETO\n\n", 
					getpid(), getpid(), getppid() );								
				}
				else{
					waitpid(pidb5, &statusb5, 0);
					printf("Soy el proceso con PID %d y pertenezco a la generación Nº 3.\nPid: %d Pid padre: %d Parentesco: NIETO\n\n", 
					getpid(), getpid(), getppid() );
				}
			}
			else{
				waitpid(pidn3, &statusn3, 0);
				printf("Soy el proceso con PID %d y pertenezco a la generación Nº 2.\nPid: %d Pid padre: %d Parentesco: HIJO\n\n", 
				getpid(), getpid(), getppid() );
			}
        	}
        	else{
            		waitpid(pidh1, &statush1, 0);
            		waitpid(pidh2, &statush2, 0);
           		printf("Soy el proceso con PID %d y pertenezco a la generación Nº 1.\nPid: %d Pid padre: %d Parentesco: PADRE\n\n", 
			getpid(), getpid(), getppid() );
        	}
    	}
	return 0;
}
