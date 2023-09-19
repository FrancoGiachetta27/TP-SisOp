#include <stdlib.h>
#include <stdio.h>
#include "consola.h"
#include <readline/readline.h>
#include <commons/string.h>
#include <planificadores/planificador.h>

void finalizar_proceso(){}
void detener_planificacion(){}
void reiniciar_planificacion(){}
void cambiar_grado_multiprogramacion(){}

void consola_interactiva(){
	char *linea;
	char **parametros;
	printf("[1]-INICIAR_PROCESO [PATH] [SIZE] [PRIORIDAD]\n");
	printf("[2]-FINALIZAR_PROCESO [PID]\n");
	printf("[3]-DETENER_PLANIFICACION \n");
	printf("[4]-INICIAR_PLANIFICACION \n");
	printf("[5]-MULTIPROGRAMACION [VALOR]\n");
	printf("[6]-PROCESO_ESTADO \n");
	linea = readline(">");
		while (strcmp(linea,"\0")!=0) {
			parametros = string_split(linea, " ");
			if(strcmp(parametros[0],"INICIAR_PROCESO")==0){
				//prueba_agregar_proceso_a_NEW();
				crear_proceso(parametros[1],parametros[2],parametros[3]); //esto se va a cambiar por la instruccion de crear proceso o lo q hagamos
				printf("%s\n", parametros[2]);

			}
			else if(strcmp(parametros[0],"FINALIZAR_PROCESO")==0){
				finalizar_proceso();
			}
			else if(strcmp(parametros[0],"DETENER_PLANIFICACION")==0){
				detener_planificacion();
			}
			else if(strcmp(parametros[0],"INICIAR_PLANIFICACION")==0){
				reiniciar_planificacion();
			}
			else if(strcmp(parametros[0],"MULTIPROGRAMACION")==0){
				cambiar_grado_multiprogramacion();
			}
			else if(strcmp(parametros[0],"PROCESO_ESTADO")==0){
				listar_procesos_por_estado();
				prueba_elementos_cola_bloqueados();
			}
			else {
				printf("\nComando no reconocido, intente de nuevo corroborando espacios entre parametros y su sintaxis\n");
			}
			free(linea);
			linea = readline(">");
			//I.cambiar la condicion de los ifs por un strcmp y el array de parametros para evitar problemas con los espacios luego del split.
			//II en vez de un encadenado de ifs, probar con un enum y un switch

		}
		free(linea);
}

