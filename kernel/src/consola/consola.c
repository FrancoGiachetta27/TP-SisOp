#include <consola/consola.h>

void crear_proceso(){}
void finalizar_proceso(){}
void detener_planificacion(){}
void reiniciar_planificacion(){}
void cambiar_grado_multiprogramacion(){}
void listar_procesos_por_estado(){}

void consola_interactiva(t_log* logger, t_conn* conn){
	printf("[1]-INICIAR_PROCESO [PATH] [SIZE] [PRIORIDAD]\n");
	printf("[2]-FINALIZAR_PROCESO [PID]\n");
	printf("[3]-DETENER_PLANIFICACION \n");
	printf("[4]-INICIAR_PLANIFICACION \n");
	printf("[5]-MULTIPROGRAMACION [VALOR]\n");
	printf("[6]-PROCESO_ESTADO \n");
	printf("[7]-ECHO [CASE] \n");
	char* linea = readline(">");
	while (!string_is_empty(linea)) {
		char **parametros = string_split(linea, " ");
		free(linea);
		if(strcmp(parametros[0],"INICIAR_PROCESO")==0){
			printf("%s\n", parametros[2]);
			crear_proceso(); //esto se va a cambiar por la instruccion de crear proceso o lo q hagamos
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
		}
		else if(strcmp(parametros[0],"ECHO")==0){
			int number = atoi(parametros[1]);
			if (number == 0) {
				log_error(logger, "Invalid CASE Input: %s", parametros[1]);
			} else {
				echo(number, conn, logger);
			}
		}
		else {
			printf("\nComando no reconocido, intente de nuevo corroborando espacios entre parametros y su sintaxis\n");
		}
		string_array_destroy(parametros);
		linea = readline(">");
		//I.cambiar la condicion de los ifs por un strcmp y el array de parametros para evitar problemas con los espacios luego del split.
		//II en vez de un encadenado de ifs, probar con un enum y un switch
	}
	rl_clear_history();
	free(linea);
}

