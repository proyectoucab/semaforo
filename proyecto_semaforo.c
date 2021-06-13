/*
 *
 * Programa hecho para recibir giros posibles entre calles y 
 * retornar la menor cantidad de grupos posibles en los que 
 * pueden dar sin colicionar
 *
 * autores:
 * Jesus David Rodriguez
 * Wilter Diaz
 * Wilfredo Garcia Bonilla
 *
 * UCAB Guayana
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 100
#define MAX_LINEAS 500


//Estructura  que permitira
//almacenar los datos de los giros
//que puedan hacerse al mismo tiempo
typedef struct fase {
	
	
	//Para almacenar los giros de cada fase
	char **principales, **adicionales;
	
	//n_principales y n_adicionales guardar la cantidad de espacio que hay reservado para cada uno
	//principales_usados y adicionales_usados guardan la cantidad de espacio que ha usado
	//cada uno
	int n_principales, principales_usados, n_adicionales, adicionales_usados;
	
	
	struct fase *sig;
} Fase;

//split: separa una cadena basado en su separador y devuelve
//apuntadores a apuntadores de caracteres con las cadenas separadas
void split(char *token, char *separador, int longitud, char **resultado) {
	register int i = 0;
	
	//MIentras la cadena exista
	while(token != NULL) {
					
		//Se asigna memoria en el resultado
		resultado[i] = (char *)malloc(sizeof(char) * longitud);
					
		//Se copia el token separado en el resultado
		strcpy(resultado[i], token);
        i++;       
		token = strtok(NULL, separador);
			 
	}
}

//nueva_fase: crea un nodo para almacenar nueva
//fase de giros que se pueden dar al mismo tiempo
//recibe longitud de las cadenas de caracteres
Fase *nueva_fase(int longitud){
	
	Fase *nuevo = NULL;
	
	//Si falla asignacion de memoria se reporta
	if((nuevo=(Fase *)malloc(sizeof(Fase)))==NULL){
		fprintf(stderr, "nueva_fase: error en malloc\n");
		exit(1);
	}
	
	//Se reserva espacio para los giros principales
	nuevo->principales = (char **)malloc(sizeof(char *) * longitud);
	nuevo->principales[0] = (char *)malloc(sizeof(char) * longitud);
	
	//Se reserva espacio para giros adicionales
	nuevo->adicionales = (char **)malloc(sizeof(char *) * longitud);
	nuevo->adicionales[0] = (char *)malloc(sizeof(char) * longitud);
	
	//Se reserva espacio para la cantidad de giros principales
	//usados y para los que hay memoria disponible
	nuevo->n_principales = 1;
	nuevo->principales_usados = 0;
	
	//Se reserva espacio para la cantidad de giros adicionales
	//usados y para los que hay memoria disponible
	nuevo->n_adicionales = 1;
	nuevo->adicionales_usados = 0;
	
	//Por ser nuevo se apunta al vacio
	nuevo->sig=NULL;
	
	return nuevo;
}


//swap: intercambia 2 numeros de un arreglo
void swap(int *arreglo, int a,int b){
	
	int t;
	
	t=arreglo[b];
	arreglo[b]=arreglo[a];
	arreglo[a]=t;
}

//swap_str: intercambia 2 cadenas de un arreglo de caracteres
//recibe el arreglo y los indices a intercambiar
void swap_str(char **lista_cadenas, int i, int j) {
	
	char *auxiliar;
	
	auxiliar= lista_cadenas[j];
	lista_cadenas[j] = lista_cadenas[i];
	lista_cadenas[i] = auxiliar;
}  

//particion: funcion para particionar los arregos a ordenar con el algoritmo de Quicksort
int particion(int *arreglo, int inicio, int fin, char **cadenas, int **p_giros) {
	
	int pivote, inferior=inicio+1, superior=fin;
	pivote=arreglo[inicio];
	
	do{
		while(arreglo[inferior]>=pivote && inferior<=superior)
			inferior++;
		
		while(arreglo[superior]<pivote && inferior<=superior)
			superior--;
		
		if(inferior <= superior){
			swap(arreglo,inferior,superior);
			swap_str(cadenas, inferior, superior);
			
			inferior++;
			superior--;
		}
		
	}while(inferior<=superior);
	
	swap(arreglo,inicio,superior);
	swap_str(cadenas, inicio, superior);

	return superior;
}

//quicksort: algoritmo de ordenamiento recursivo, de mayo a menor, recibe 
//un arreglo de enteros, un arreglo de cadenas y el inicio y fin para las llamadas recursivas
//el arreglo de cadenas cambiar sus posiciones en funcion de como lo haga el arreglo de enteros
//pues esto se hace para ordenar las calles por numero de colisiones con otros giros que cada una tiene
void quicksort(int *arreglo,  int inicio, int fin, char **cadenas, int **p_giros) {
	int p;
	
	if(inicio<fin){
		p=particion(arreglo, inicio, fin, cadenas, p_giros);
		quicksort(arreglo, inicio, p-1, cadenas, p_giros);
		quicksort(arreglo, p+1, fin, cadenas, p_giros);
	}
}


//insertar: Inserta en una fase un giro, recibe la
//cadena, recibe n (si n == 0 se inserta giro principal, si n == 1 se inserta giro adicional)
//y recibe la longitud de la cadena
void insertar(Fase *fasep, char *giro, int n, int longitud) {
	
	//Asigno espacio para copiar la palabra
	char *palabra = (char *)malloc(sizeof(char) * longitud);
	strcpy(palabra, giro);
	
	//n == 0, se inserta giro principal
	if (n == 0) {
		
		//Si ya no hay espacio para agregar otro nuevo giro
		//se reserva dinamicamente
		if (fasep->n_principales == fasep->principales_usados) {
			
			register int i;
			
			//Se aumenta al doble el espacio disponible
			fasep->n_principales*=2;
			
			//Se cambia el tamaño de la estructura de datos en tiempo de ejecucion
			fasep->principales = (char **)realloc(fasep->principales, fasep->n_principales * sizeof(char *));
			
			//Se asigna nuevo espacio hasta donde permita el tamaño de la estructura
			for (i = fasep->principales_usados; i < fasep->n_principales; i++) {
				fasep->principales[i] = (char *)malloc(sizeof(char) * longitud);
			}
			
			//Se copia en la estructura la palabra
			strcpy(fasep->principales[fasep->principales_usados], palabra);
			
			//Se aumenta en 1 la cantidad de giros usados
			fasep->principales_usados++;
			
			//Si hay espacio se copia la palabra en la estructura
			//Se aumenta en 1 la cantida de giros usados
		} else {
			strcpy(fasep->principales[fasep->principales_usados], palabra);
			fasep->principales_usados++;
		}
	  //n == 1, insertar giro adicional
	} if (n == 1) {
		//Si ya no hay espacio para agregar otro nuevo giro
		//se reserva dinamicamente
		if (fasep->n_adicionales == fasep->adicionales_usados) {
			register int i;
			
			//Se aumenta al doble el espacio disponible
			fasep->n_adicionales*=2;
			
			//Se cambia el tamaño de la estructura de datos en tiempo de ejecucion
			fasep->adicionales = (char **)realloc(fasep->adicionales, fasep->n_adicionales * sizeof(char *));
			
			//Se asigna nuevo espacio hasta donde permita el tamaño de la estructura
			for (i = fasep->adicionales_usados; i < fasep->n_adicionales; i++) {
				fasep->adicionales[i] = (char *)malloc(sizeof(char) * longitud);
			}
			
			//Se copia la palabra en la estructura
			strcpy(fasep->adicionales[fasep->adicionales_usados], palabra);
			
			//Se aumenta en 1 la cantidad de adicionales usadoa
			fasep->adicionales_usados++;
			
			//Si hay espacio se copia la palabra en la estructura
			//Se aumenta en 1 la cantida de giros usados
		} else {
			strcpy(fasep->adicionales[fasep->adicionales_usados], palabra);
			fasep->adicionales_usados++;
		}
	}
	
}

//discriminador: esta funcion ayuda a discriminar los casos base
//retorna 1 si encuentra al menos un giro compatible
//retorna 0 si todos los giros son incompatibles

int discriminador(int **colisiones, int tamano){

	register int i, j;
    
	for (i = 0; i < tamano; i++) {
		
        for (j = 0; j < tamano; j++) {
			
            if (colisiones[i][j]!=1) {
                return 1;
			}
		}
	}
    return 0;

}

//busqueda: busca giro en los giros principales de una fase, si lo encuentra
//retorna 1, sino retorna 0, recibe la fase y el nombre del giro
int busqueda(Fase *fasep, char *giro){
	
	
	register int i;
		
	for (i = 0; i < fasep->principales_usados; i++) {
		if (strcmp(giro, fasep->principales[i]) == 0) {
			return 1;
		}
	}
		

	
	return 0;
}


//retorna_indice: busca en un arreglo el indice que corresponde
//al giro dado, recibe el nombre del giro, el arreglo y el tamaño
int retorna_indice(char *cadena, char **arreglo, int tamano) {
	static int indice;
	
	for (indice = 0; indice < tamano; indice++) {
		if (strcmp(cadena, arreglo[indice]) == 0) {
			return indice;
		}
	}
	
	return -1;
}

//busca_choque: busca en una fase dada si en los giros principales esta un giro dado,
//recibe matriz para verificar colisiones, arreglo con los nombres de giros disponibles
//y el tamaño tanto de la matriz como del arreglor
int busca_choque(Fase *fasep,char *cruce, int **matriz, char **arreglo, int tamano) {
	register int i;
	
	//Si encuentra colision con alguno de los giros retorna 1
	for (i = 0; i < fasep->principales_usados; i++) {
		if (matriz[retorna_indice(fasep->principales[i], arreglo, tamano)][retorna_indice(cruce, arreglo, tamano)] == 1) {
			return 1;
		}
	}
	
	return 0;
}

//cargar_archivo: Le pide al usuario un nombre de archivo
//para retornar el puntero a ese archivo
//si no encuentra el archivo le permite salir
//o volverlo a intentar, su parametro es el modo de lectura
FILE *cargar_archivo(char *modo) {
	
	int opcion;
	
	char nombre_archi[100];
	
	FILE *archivop = NULL;
	while (1) {
		printf("Introduce el nombre del archivo:\n");
		scanf("%s", nombre_archi);
			
		printf("\n");
		archivop = fopen(nombre_archi, modo);
		if (archivop != NULL) {
			printf("archivo cargado con exito\n\n");	
			break;
			
		}
		
		printf("\nNombre invalido.\n\n");
			
		//Menu de opciones por que el
		//puntero al archivo es NULL
		printf("Opciones:\n");
		printf("1. Intentar poner un nombre otra vez\n");
		printf("2. Salir\n");
		printf("Presiona un numero:\n");
		scanf("%d", &opcion);
		
		printf("\n");
		
		//Si se quiere volver a intentar
		//introducir nombre de archivo
		if (opcion == 1) 
			continue;

		//Si se quiere volver al menu
		if (opcion == 2) 
			break;
			
		//No se presiono numero valido
		printf("Opcion invalida.\n\n");			
			
	}
	
	
	return archivop;
	
}

//borrar_lista: funcion para liberar la memoria de la lista enlazada
void borrar_lista(Fase **fase) {
    
	
    Fase *actual = *fase;
    Fase *sig;
	
	register int i;
 
    while (actual != NULL) {
        sig = actual->sig;
 	   
 	   //Se libera arreglo dinamico que tenia
 	   //los giros principales
 	   for (i = 0; i < actual->n_principales; i++) {
 		   free(actual->principales[i]);
 	   }
	   
	   //Se libera arreglo dinamico que tenia
	   //los giros adicionales
	   for (i = 0; i < actual->n_adicionales; i++) {
		   free(actual->adicionales[i]);
	   }
	   
       free(actual);
       actual = sig;
    }
    

    *fase = NULL;
}

//guarda_datos: guarda los datos de un archivo dado en una matriz con la misma
//longitud por cadena que la longitud por linea del archivo
//recibe el archivo, la longitud del buffer y la matriz
//retorna el numero de colisiones
int guarda_datos(FILE *archivo, int buff_len, char matriz[][MAX_LINEAS]) {
	
	static int i = 0;
		
	int indice = buff_len - 1;
	//Mientras el archivo exista va llenando el arreglo
	while (fgets(matriz[i], buff_len, archivo)) {
				
		if ((matriz[i][indice]) == '\n') {
			matriz[i][indice] = '\0';
					
		}
		i++;
	}
	
	return i;
}

//lenar_colisiones: representa en una estructura de datos del tipo int ** las colisiones que pueden darse entre los distintos giros, llenando con el numero 1 los espacios donde
//coincidan los indices de las calles que puedan colisionar, recibe una matriz con los strings que representan las colisiones, una lista con los nombres de los giros, la cantidad de
//colisiones que hay, la longitud de los nomres de los giros, el numero de giros, recibe la estructura de datos giros que es donde se llenaran las colisiones posibles y 
//la estructura num_colisiones que tenga dentro el numero de posibles colisiones que tiene cada giro
void llenar_colisiones(char matriz_entrada[][MAX_LINEAS], char **lista_calles, int cant_colisiones, int longitud, int num_giros, int **colisiones, int *num_colisiones) {
	
	register int i;
	
	for (i = 0; i < cant_colisiones; i++) {
		char **giritos = (char **)malloc(sizeof(char *) * longitud);
		giritos[0] = (char *)malloc(sizeof(char) * longitud);
		giritos[1] = (char *)malloc(sizeof(char) * longitud);
		char *token = (char *)malloc(sizeof(char) * longitud);
		
		token = strtok(matriz_entrada[i], " - ");
		
		
		
		split(token, " - ", longitud, giritos);

				
		if ((giritos[1][strlen(giritos[1])-1]) == '\n') {
				    giritos[1][strlen(giritos[1]) - 1] = '\0';
		}
		
		register int j;

		for (j = 0; j < num_giros; j++) {
			
			if (strcmp(lista_calles[j], giritos[0]) == 0) {
			
    			register int l;
	 					
    			for (l = 0; l < num_giros; l++) {
			
    				if (strcmp(lista_calles[l], giritos[1]) == 0) {
					
	    	    		colisiones[j][l] = colisiones[l][j] = 1;
     	    			num_colisiones[l]++;
	        			if (l != j) {
			    			num_colisiones[j]++;
	    				}
		 						
		    		}
		    	}
			}
				
			if (strcmp(lista_calles[j], giritos[1]) == 0) {
			
				register int l;
						
				for (l = 0; l < num_giros; l++) {
				
					if (strcmp(lista_calles[l], giritos[0]) == 0) {

						colisiones[j][l] = colisiones[l][j] = 1;
								
						num_colisiones[l]++;
					
						if (l != j){
							num_colisiones[j]++;
						}
								
					}
				}
		
				
		    }
		}
				

      
    }
	
 
}

//adicionales: Verifica en una lista de fases cuales son los giros
//adicionales de cada nodo, recibe los apuntadores auxiliar y auxiliar2, la estructura de datos con las colisiones
//una estructura de datos con los giros, una estructura de datos con el numero de colisiones y un entero la longitud de cada cadena
void adicionales(Fase *fases, Fase *auxiliar, Fase *auxiliar2, int **colisiones, char **lista_calles, int num_giros, int longitud) {
	
	register int i;
	
	//Mientras verifiquemos auxiliar2
	while(auxiliar2 != NULL) {
		
		//Se apunta auxiliar al inicio de la lista
		auxiliar = fases;
		
		//Mientras verifiquemos auxiliar
		while(auxiliar != NULL) {
			
			//Si los nodos son distintos
	    	if (auxiliar != auxiliar2) {
				
				//Si ademas no hay colisiones entre ellos se insertan como giros adicionales
		    	for (i = 0; i < auxiliar->principales_usados; i++) {
	    			if (busca_choque(auxiliar2, auxiliar->principales[i], colisiones, lista_calles, num_giros) == 0) { 
	    				insertar(auxiliar2, auxiliar->principales[i], 1, longitud);
		     		}
							
			    }					
					
	        }
		    //Se avanza auxiliar
		    auxiliar = auxiliar->sig;
	   }
	   //Se avanza auxiliar2
	   auxiliar2 = auxiliar2->sig;
	}

}



int main(void) {
	
	register int i;
	
	int opcion, num_giros = 0, **giros, longi, *num_colisiones;
	
	char ch, linea_archivo[BUFFER], **calles, matriz_entrada[BUFFER][MAX_LINEAS], **copia_calles;

	FILE *archivo_entrada = NULL, *archivo_salida = NULL;
	
	Fase *conjuntos;
	
	printf("Bienvenido :)\n\n");
	
	//Mientras usuario no ordene salir
	while (opcion != 4) {

		printf("Opciones.\n");
		printf("1. Cargar archivo\n");
		printf("2. Visualizar datos del archivo de entrada\n");
		printf("3. Retornar conjuntos\n");
		printf("4. Salir\n");
		printf("Presiona un numero:\n");
		scanf("%d", &opcion);
		
		printf("\n");
		
		//opcion: cargar archivo
		if (opcion == 1) {
			system("clear");
			
			archivo_entrada = cargar_archivo("r");
			
		//opcion: leer datos	
		} else if (opcion == 2) {
			
			system("clear");
			//Si no hay archivo cargado se devuelve al menu
			if (archivo_entrada == NULL) {
				printf("No hay archivo cargado.\n\n");
				continue;
			}
			
			//si hay archivo imprime sus datos
			printf("Datos del archivo:\n\n");
			
			//Imprime archivo caracter por caracter
			while((ch = fgetc(archivo_entrada)) != EOF)
				printf("%c", ch);
			
			printf("\n\n");
			
			//Posiciona el archivo en el inicio
			rewind(archivo_entrada);
			
			

		//opcion: retornar conjuntos
		} if (opcion == 3) {
			
			system("clear");
			//Si no hay archivo se regresa al menu
			//Y se avisa que no hay archivo cargado
			if (archivo_entrada == NULL) {
				printf("No hay archivo cargado\n\n");
				continue;
			}
			
			//Se da la opcion de cargar salida a un archivo
			printf("Opciones.\n");
			printf("Si desea cargar datos de salida a un archivo presione 1.\n");
			printf("Si no lo desea presione cualquier otro numero.\n");
			scanf("%d", &opcion);
			
			//Si el usuario lo pide se busca un archivo
			//con el nombre que haya dado y se intenta abrir en modo de escritura
			if (opcion == 1) {
				archivo_salida = cargar_archivo("w");
				

			}
			
			//Esto evita que al salir del condicional se salga del programa
			//Si el usuario presiona el 4
			opcion = 0;
			
			//guardo el numero de giros del semaforo
			num_giros = atoi(fgets(linea_archivo,100,archivo_entrada)); 
			
			//Segunda linea del archivo
		    fgets(linea_archivo, BUFFER, archivo_entrada); 
			
			char *token = strtok(linea_archivo," ");
			
			//Longitud del nombre cada giro
			longi = strlen(token);
			
			//Asigno espacio dinamicamente para el numero de colisiones que tendra cada giro
			num_colisiones = (int *)malloc(sizeof(int) * num_giros);
			
			//Asigno espacio dinamicamente para los nombres de las calles
			calles = (char **)malloc(sizeof(char *) * num_giros);
			
			//Asigno espacio para cada calle de la estructura
			for (i = 0; i < num_giros; i++) {
				calles[i] = (char *)malloc(sizeof(char) * longi);
			}
			
			//Se retorna en calles la primera linea del archivo spliteado
			split(token, " ", longi, calles);
			
			//Se elimina salto de linea en la utlima cadena
			calles[num_giros -1][longi] = '\0';
			
			//Asigno espacio para estructura que guardara nuestro arreglo dianmico de calles
			copia_calles = (char **)malloc(sizeof(char *) * num_giros);
			
			//Copio las calles de calles en copia_calles  
			for (i = 0; i < num_giros; i++) {
				
				copia_calles[i] = (char *)malloc(sizeof(char) * longi);
				strcpy(copia_calles[i], calles[i]);
			}
			
			//Se asigna espacio para represenar las posibilidades
			//de giros entre las calles, cuando 2 colisiones se pondra 1
			giros = (int **)malloc(sizeof(int *) * num_giros);
			
			//Se guarda espacio para los apuntadores de nuestra estructura de datos de apuntadores
			//donde se rperesentaran las poosibles colisiones entre calles
			for (i = 0; i < num_giros; i++) {

				giros[i] = (int *)malloc(sizeof(int) * num_giros);
			}
			//Numero de colisiones
			int colisiones;
			
			//Guarda el numero de colisiones y las colisiones que hay en el archivo
			//las guarda en una matriz
			colisiones = guarda_datos(archivo_entrada, BUFFER, matriz_entrada);
			
			//Se llena nuestra estructura de datos que va a representar la incompatibilidad entre los giros
			llenar_colisiones(matriz_entrada, calles, colisiones, longi, num_giros, giros, num_colisiones);
			
			//Se ordenan los indices de las calles basado en la cantidad de
			//colisiones que tiene cada una, pues, se van a ir comparando en orden
			//descente
			//quicksort(int *arreglo,  int inicio, int fin, char **cadenas, int **p_giros)
			quicksort(num_colisiones, 0, num_giros - 1, calles, giros);
			
			//Cabecera de nuestra estructura de datos que representa las fases de salida
			conjuntos = nueva_fase(longi);
			
			//Creamos los punteros auxiliares para nuestra estructura de fases
			Fase *auxiliar;
			Fase *auxiliar2;
			
			//Si todos los giros son incompatibles se avisa por pantall
			if (discriminador(giros,num_giros) == 0) {
				
				if (archivo_salida) {
					fprintf(archivo_salida, "No hay giros disponibles\n");
				}
				
				printf("\nNo hay giros disponibles\n\n");
				
				continue;
			}
			
			//Por cada giro que hay
			for (i = 0; i < num_giros; i++) {
				
				//apuntamos el auxiliar a la primera fase
				auxiliar = conjuntos;
				
				//Mientras fase distinta de vacio
				while (auxiliar != NULL) {
					
					//Si el giro de indice i no colisiona con ningun giro principal de la fase que estamos verificando 
					if (busca_choque(auxiliar, copia_calles[retorna_indice(calles[i], copia_calles, num_giros)], giros, copia_calles, num_giros) == 0) {
						
						//Si ademas el giro no esta ya en la fase que estamos verificando
						//lo insertamos como giro principal
						if (busqueda(auxiliar, calles[i]) == 0) {
							insertar(auxiliar, calles[i], 0, longi);
						}
						break;
						//En caso de que el giro colisione con la fase que estamos verificando pasamos a la siguiente
						//Si la fase siguiente no existe la creamos e insertamos el giro de indice i
					} else {
						if (auxiliar->sig == NULL) {
							auxiliar->sig = nueva_fase(longi); 
							insertar(auxiliar->sig, calles[i], 0, longi);

						}
					}
					//Pasamos a la siguiente fase
					auxiliar = auxiliar->sig;
				}
			}
			
			//Apuntamos nuestros auxiliares a la cabecera otra vez
			auxiliar = conjuntos;
			auxiliar2 = conjuntos;


			//Buscamos giros que no colisiones con una fase pero que pertenezcan a otra
			//y si existen los insertamos como adicionales
			adicionales(conjuntos, auxiliar, auxiliar2, giros, copia_calles, num_giros, longi);

			int k;
			
			int j;
			i = 1;
			
			//Si hay archivo de salida se imprimen los datos de las fases por pantalla y se guardan
			//en el archivo
			if (archivo_salida != NULL) {
				for (auxiliar = conjuntos; auxiliar != NULL; auxiliar = auxiliar->sig) {
					printf("\nGiros principales(Fase numero %d):\n", i);
					fprintf(archivo_salida, "Giros principales(Fase numero %d):\n", i);
					for (j = 0; j < auxiliar->principales_usados; j++) {
						printf("%s ", auxiliar->principales[j]);
						fprintf(archivo_salida, "%s ", auxiliar->principales[j]);
					}
					printf("\nGiros adicionales:\n");
					fprintf(archivo_salida, "\nGiros adicionales:\n");
					for (k = 0; k < auxiliar->adicionales_usados; k++) {
						printf("%s ",  auxiliar->adicionales[k]);
						fprintf(archivo_salida, "%s ", auxiliar->adicionales[k]);
					}
					printf("\n\n");
					fprintf(archivo_salida, "\n\n");
					i++;
					
				}			
				
				fclose(archivo_salida);
				//Si no hay archivo solo se imprimen los datos por pantalla
			} else{
	    		for (auxiliar = conjuntos; auxiliar != NULL; auxiliar = auxiliar->sig) {
		    		printf("\nGiros principales(Fase numero %d):\n", i);
			    	for (j = 0; j < auxiliar->principales_usados; j++) {
				    	printf("%s ",auxiliar->principales[j]);
			    	}
			    	printf("\nGiros adicionales:\n");
				    for (k = 0; k < auxiliar->adicionales_usados; k++) {
					    printf("%s ", auxiliar->adicionales[k]);
			    	}
				    printf("\n\n");
					i++;
		    	}
			}
			
			//Puntero del archivo hasta el principio otra vez
			rewind(archivo_entrada);
			
			//Token que guarda la segunda linea == vacio
			token = NULL;
	
		}
		
		
	}
	
	//Si archivo esta abierto lo ceramos
	if (archivo_entrada != NULL) {
		fclose(archivo_entrada);
	}
	
	
	//Se libera memoria dinamica asignada a los punteros
	for (i = 0; i < num_giros; i++) {
		free(giros[i]);
	}
	
	
	for (i = 0; i < num_giros; i++) {
		free(copia_calles[i]);
	}
	
	
	
	for (i = 0; i < num_giros; i++) {
		free(calles[i]);
	}
	
	
	//Si se llegaron a usar estas estructuras de datos liberamos memoria
	if (num_giros > 0) {
		free(num_colisiones);
		borrar_lista(&conjuntos);
		free(giros);
		free(copia_calles);
		free(calles);
	}
	

	printf("Hasta luego. :)\n");
	
	return 0;
		

}
