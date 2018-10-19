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

/*
	PARA OBTENER EL TIEMPO REAL

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("Now: %d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
*/
t_lista lista;

void clean_stdin(void){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int main(int argc, char *const argv[]){
	char pat[8];
	char src[9];
	//char *c;
	//int count = 0;
	t_dato dato;

	crearLista(&lista);

	for (int i = 0; i < 3; i++){
   		printf( "Ingrese informacion :");
/*
   		while(count < 9){
   			strcpy(c,(char*)((char)getchar()));
   			//src+=getchar();
   			strcat(src,c);
   		}*/
   		//scanf("%s",src);
	   	fgets(src,9,stdin);
   		printf("%ld - %ld\n", strlen(src), sizeof(src));
	   	for (int i = 0; i < strlen(src); i++){
	   		src[i] = toupper((unsigned char) src[i]);
	   	}
		strncpy(pat, src + 1, strlen(src) - 1 );
		strcpy(src,"");
		clean_stdin();
		//printf("%c\n", src[0]);
		//printf("%s\n", pat);

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		//printf("Now: %d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

		strcpy(dato.patente,pat);
		dato.hora = tm.tm_hour;
		dato.min = tm.tm_min;
		dato.seg = tm.tm_sec;
		insertarOrdenado(&lista,&dato,cmp);
		//count = 0;
   	}
	printf("**********************\n");
	mostrarLista(&lista);
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