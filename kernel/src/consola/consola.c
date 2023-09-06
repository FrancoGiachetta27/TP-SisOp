#include <stdlib.h>
#include <stdio.h>
#include "consola.h"
#include <readline/readline.h>

void consola_interactiva(){
	char *linea;
	linea = readline(">");
		    while (strcmp(linea,"")!=0) {
		        if (!linea) {
		            break;
		        }
		        printf("%s\n", linea);
		        linea = readline(">");
		        free(linea);
		    }
}

