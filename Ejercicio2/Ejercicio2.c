#include "parametros.h"
#include "lista.h"

/*#################################################
#             Sistemas Operativos                 #
#       Trabajo Práctico 3 - Ejericio 2           #
#       Nombre del Script: Ejercicio2.c           #
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

t_lista lista;

int main(int argc, char *const argv[]){
  int a, j = 0, cantTotal = 0;
  char cwd[PATH_MAX];

  if(argc != 2){
    printf("Ingrese:\n\t./Ejercicio2 -h\n\t./Ejercicio2 -help\n\t./Ejercicio2 -?\n\tpara solicitar ayuda.\n");
    exit(1);
  } 
  if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
    printf("La sintaxis es ./Ejercicio2 [path]\nFormato: ./Ejercicio2 [string]\nEjemplo: ./Ejercicio2 ./Entrada\n");
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
    /* Directory exists. */
    closedir(dir);
  }else if (ENOENT == errno){
    /* Directory does not exist. */
    strcpy(cwd,argv[1]);
    strcat(cwd,"/");
    dir = opendir(cwd);
    if (dir){
    /* Directory exists. */
    closedir(dir);
    }else if (ENOENT == errno){
      printf("Directorio Inexistente.\n");
      exit(1);
    }
  }else{
    printf("Ha ocurrido un error.\n");
    exit(1);
  }

  crearLista(&lista);
  cargarArchivosEnLista(&lista,cwd);
  //mostrarLista(&lista);

  pthread_t hilos[size(&lista)];

  t_lista first = lista;
  
  while(lista){
    
    pthread_create(&hilos[j], NULL, analyze, (void*)&(lista->dato));
    cantTotal += sizeL(&(lista->dato.list));
    lista = lista->sig;
    j++;
  }
  
  lista = first;
  j = 0;

  while(lista){
    
    pthread_join(hilos[j], NULL);  

    lista = lista->sig;
    j++;
  }
  
  // Imprimo resultado por pantalla
  lista = first;
  printf("Cantidad Total de Archivos: %d\n", cantTotal);
  printf("*************************************************************************************************\n");
  while(lista){
    printf("Extension: %s - Cant Files: %ld - Tamaño: Max %ld - Min %ld -  Owner: Name %s - Cantidad Files %ld\n", lista->dato.ext, lista->dato.tamArch, lista->dato.tamMax, lista->dato.tamMin, lista->dato.owner, lista->dato.tamArch);
    printf("*************************************************************************************************\n");
    lista = lista->sig;
    j++;
  }
  return 0;
}

//*******************************************************************************************************************

void* analyze(void* d){
  t_dato* dato;
  dato = (t_dato*)d;

  // Buscar Mayor y Menor y Most Owner
  buscarMayorMenorOwner(dato);

  return(0);
}