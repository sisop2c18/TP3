#include "parametros.h"
#include "lista.h"

/*#################################################
#             Sistemas Operativos                 #
#       Trabajo Práctico 3 - Ejericio 3           #
#       Nombre del Script: Ejercicio3.c           #
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

FILE* fmov;
FILE* ffac;
int w;
int r;
t_datoL datoL;
t_listaL listaF;
t_lista listaE;

void clean_stdin(void){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void escribir(pid_t pid){
	signal(SIGINT, salirEscribir);

	char src[8];
	//char fin[8];
	//strcpy(fin,"eexit12");
	mkfifo("./fifo1",0666);

	//int w = open("./fifo1", O_WRONLY);
	w = open("./fifo1", O_WRONLY);

	printf("PID QUE LEE: %d\n", pid);
	printf("Ingrese informacion:\n");

   	do{
   		fgets(src,8,stdin);
   		clean_stdin();
   		while(src[0] != 'E' && src[0] != 'S' && src[0] != 'e' && src[0] != 's'){
   			printf("Re ingrese, el primer caracter debe ser 'e' o 's'\n");
   			fgets(src,8,stdin);
   			clean_stdin();
   		}
   		write(w,src,sizeof(src));
   	//}while(strcmp(src,fin) != 0);
	}while(1);

	close(w);

	unlink("./fifo1");

	return;
}

void procesar(double precio){
	signal(SIGINT, salirLeer);

	char pat[7];
	char src[8];
	//char fin[8];
	char fecha[64];
	char fileMov[64];
	char fileFac[64];
	char imprimir[100];
	t_dato dato;
	t_lista listaS;
	time_t t;
	struct tm tm;
	struct tm *tm1;
	double segs;

	//int r = open("./fifo1", O_RDONLY);
	r = open("./fifo1", O_RDONLY);

	//strcpy(fin,"EEXIT12");
	strcpy(fileMov,"Movimientos_");
	strcpy(fileFac,"Facturacion_");

	t = time(NULL);
	tm1 = localtime(&t);  
	strftime(fecha, sizeof(fecha), "%Y%m%d", tm1); // la formatea YYYYMMDD

	strcat(fileMov,fecha);
	strcat(fileFac,fecha);
	strcat(fileMov,".log");
	strcat(fileFac,".txt");

	crearLista(&listaE);
	crearLista(&listaS);
	crearListaL(&listaF);

	fmov= fopen(fileMov, "a");

	if( fmov == NULL){
		printf("ERROR, no se pudo abrir/crear el archivo.\n");
        exit(1);
	}

	ffac= fopen(fileFac, "a");

	if( ffac == NULL){
		printf("ERROR, no se pudo abrir/crear el archivo.\n");
        exit(1);
	}

	do{
		read(r,src,sizeof(src));

		for (int i = 0; i < strlen(src); i++){
	   		src[i] = toupper((unsigned char) src[i]);
		}
		//if(strcmp(src,fin) != 0){
			strncpy(pat, src + 1, strlen(src) - 1 );			

			t = time(NULL);
			tm = *localtime(&t);

			dato.tipo = src[0];

			strcpy(dato.patente,pat);
			dato.hora = tm.tm_hour;
			dato.min = tm.tm_min;
			dato.seg = tm.tm_sec;

			if(src[0] == 'E'){
				if(!exist(&listaE,&dato,cmp)){
					printf("Entrada %s %d:%d:%d\n", dato.patente, dato.hora, dato.min, dato.seg);
					sprintf(imprimir,"Entrada %s %d:%d:%d\n", dato.patente, dato.hora, dato.min, dato.seg);
					fputs(imprimir,fmov);
					insertarOrdenado(&listaE,&dato,cmp);
				}else{
					// msj error existe
					printf("%s ya se encuentra en el garage.\n", dato.patente);
				}
			}else{
				if((size(&listaE) >= 0) && (exist(&listaE,&dato,cmp)) && (!exist(&listaS,&dato,cmp))){
					sprintf(imprimir,"Salida %s %d:%d:%d\n", dato.patente, dato.hora, dato.min, dato.seg);
					fputs(imprimir,fmov);
					insertarOrdenado(&listaS,&dato,cmp);
					segs = buscarHora(&listaE,&dato,cmp);
					if((((dato.hora*3600) + (dato.min*60) + dato.seg - segs)/15) < 1){
						segs = precio/4;
					}else{
						segs = (((dato.hora*3600) + (dato.min*60) + dato.seg - segs)/15) * (precio/4);
					}
					printf("%s - $%.2f\n", dato.patente, segs);
					strcpy(datoL.patente,dato.patente);
					datoL.precio = segs;
					insertarOrdenadoL(&listaF,&datoL,cmpL);
					//eliminar de las 2 listas
					eliminarDato(&listaE,&dato,cmp);
					eliminarDato(&listaS,&dato,cmp);
				}else{
					// msj error existe
					printf("%s no ingreso al garage.\n", dato.patente);
				}
			}
		//}	
	//}while(strcmp(src,fin) != 0);
	}while(1);
	/*
	mostrarLista(&listaE);
	printf("-----------------\n");
	mostrarLista(&listaS);
	*/	
	if(size(&listaE) > 0){
		while(listaE){
			strcpy(datoL.patente,listaE->dato.patente);
			datoL.precio = 0;
			insertarOrdenadoL(&listaF,&datoL,cmpL);
			listaE = listaE->sig;
		}
	}
	imprimirListaL(&listaF,ffac);
	fclose(fmov);
	fclose(ffac);
	close(r);

	return;
}

void salirEscribir(){
	printf("\nSaliendo Escribir...\n");

	//printf("Cerre Write...\n");
	close(w);
	//printf("Cerre fifo1...\n");
	unlink("./fifo1");

	//printf("GoodBye Escribir...\n");
	exit(EXIT_SUCCESS);
}

void salirLeer(){
	printf("\nSaliendo Leer...\n");

	if(size(&listaE) > 0){
		while(listaE){
			strcpy(datoL.patente,listaE->dato.patente);
			datoL.precio = 0;
			insertarOrdenadoL(&listaF,&datoL,cmpL);
			listaE = listaE->sig;
		}
	}
	imprimirListaL(&listaF,ffac);

	//printf("Cerre fmov...\n");
	fclose(fmov);
	//printf("Cerre ffac...\n");
	fclose(ffac);
	//printf("Cerre Read...\n");
	close(r);
	//printf("GoodBye Leer...\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *const argv[]){

	if(argc != 2){
		printf("Ingrese:\n\t./Ejercicio3 -h\n\t./Ejercicio3 -help\n\t./Ejercicio3 -?\n\tpara solicitar ayuda.\n");
		exit(1);
	} 
	if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
		printf("La sintaxis es ./Ejercicio3 [precio]\nFormato: ./Ejercicio3 [int]\nEjemplo: ./Ejercicio3 3\n");
		exit(1);
	}

	if(!ES_NUMERO(argv[1][0])){
		printf("Precio inadecuado. Debe ser un numero entero.\n");
		exit(1);
	}

	double precio = atoi(argv[1]);

	pid_t pid = fork();

	if(pid > 0){
		// el padre escribe 
		escribir(pid);
	}else{
		// el hijo lee
		procesar(precio);
	}
	

	/*char pat[7];
	char src[8];
	t_dato dato;
	int fifo;
	char fecha[64];
	char msj[1024];
	time_t t;
	struct tm tm;
	struct tm *tm1;

	crearLista(&lista);

	for (int i = 0; i < 1; i++){
   		printf( "Ingrese informacion :");

	   	fgets(src,8,stdin);
   		//printf("%ld - %ld\n", strlen(src), sizeof(src));
	   	for (int i = 0; i < strlen(src); i++){
	   		src[i] = toupper((unsigned char) src[i]);
	   	}
		strncpy(pat, src + 1, strlen(src) - 1 );
		
		clean_stdin();

		t = time(NULL);
		tm = *localtime(&t);

		dato.tipo = src[0];
		strcpy(dato.patente,pat);
		dato.hora = tm.tm_hour;
		dato.min = tm.tm_min;
		dato.seg = tm.tm_sec;
		insertarOrdenado(&lista,&dato,cmp);
   	}
	printf("**********************\n");
	mostrarLista(&lista);

	t = time(NULL);
	tm1 = localtime(&t);
	strftime(fecha, sizeof(fecha), "%Y%m%d", tm1); // la formatea YYYYMMDD

	int result = mkfifo("./fifo", 0666);
	
	printf("%d %d %d %d %d %d %d %d\n", EACCES, EDQUOT, EEXIST, ENAMETOOLONG, ENOENT, ENOSPC, ENOTDIR, EROFS);
	
	if (result == -1){
		printf("Error MKFIFO %d\n", errno);
		exit(1);
	}

	printf("a\n");

	fifo = open("./fifo", O_WRONLY);

	printf("b\n");

	if (!fifo){
		printf("Error OPEN MKFIFO\n");
		exit(1);
	}

	strcpy(msj, "holaaaaaaaaa");
	printf("1	Fecha: %s - Msj: %s\n", fecha, msj);
	write(fifo, msj, sizeof(msj)+1);

	//int buffer = read(fifo, msj, sizeof(msj));

	printf("Fecha: %s - Msj: %s\n", fecha, msj);

	close(fifo);
	*/
	/*
	int a, j = 0, cantTotal = 0;
	char cwd[PATH_MAX];

	if(argc != 2){
	printf("Ingrese:\n\t./Ejercicio3 -h\n\t./Ejercicio3 -help\n\t./Ejercicio3 -?\n\tpara solicitar ayuda.\n");
	exit(1);
	} 
	if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
	printf("La sintaxis es ./Ejercicio3 [path]\nFormato: ./Ejercicio3 [string]\nEjemplo: ./Ejercicio3 ./Entrada\n");
	exit(1);
	}

	// Obtengo el directorio de trabajo
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
	 perror("getcwd() error");
	 return 1;
	}

	// Concateno con el nombre del directorio ingresado
	strcat(cwd,"/");
	strcat(cwd,argv[1]);
	strcat(cwd,"/");

	// Compruebo si el directorio existe
	DIR* dir = opendir(cwd);
	if (dir){
	// Directory exists. 
	closedir(dir);
	}else if (ENOENT == errno){
	// Directory does not exist. 
	strcpy(cwd,argv[1]);
	strcat(cwd,"/");
	dir = opendir(cwd);
	if (dir){
	// Directory exists. 
		closedir(dir);
	}else if (ENOENT == errno){
	  printf("Directorio Inexistente.\n");
	  closedir(dir);
	  exit(1);
	}
	}else{
	printf("Ha ocurrido un error.\n");
	closedir(dir);
	exit(1);
	}

	*/
  return 0;
}
