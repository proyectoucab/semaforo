#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFF_LEN 256

int main(){
	
	int i=0,giros=0;
	char girosText[5];
	char filename[BUFF_LEN]="prueba.txt";
	char _buffer[BUFF_LEN];
	char tokens[BUFF_LEN];
	char * pch;
	
	
	FILE *fp = fopen(filename,"rt");
	
	if(!fp){
		printf("Error de apertura de archivo");
		return -1;
		
	}
	
	giros=atoi(fgets(girosText,BUFF_LEN,fp)); // guardo el numero de giros del semaforo
	_buffer[BUFF_LEN] = fgets(_buffer,BUFF_LEN,fp);	// Guardo en una cadena las Letras Asociadas a Calles
	
	
	
    char subtokens[BUFF_LEN]; // subcadenas
    int cont=0;
    int cant_letras=0;
    char dos_letras[2];
    
    char *token = strtok(_buffer," ");
    if(token != NULL){
        while(token != NULL){
        	i++;
        	cont++;
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            printf("Token: %s\n", token);
            strcat(dos_letras,token); // asigno el valor de cada letra, tendría dos letras
            
            printf("%s",dos_letras);
            
                     
           
            
			 token = strtok(NULL, " ");
			 
			 
        }
        
    }
    
    printf("%s",subtokens);

	
	
	/*while(!feof(fp)){
		
		fgets(_buffer,BUFF_LEN,fp);
		printf("%s",_buffer);
	} */
	
		
	return 0;
	
	}

