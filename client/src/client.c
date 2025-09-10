#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger(); // retorna un t_log* 

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, " [En client.c] valor = %s",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	//enviar_mensaje(mensaje, socket cliente);
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);


	printf("Termina cliente\n");

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("../log.log", "CLIENTE", 1, LOG_LEVEL_INFO);
	if (nuevo_logger==NULL){
		perror("Error al crear el logger");
		abort();	
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL){
		perror("Error al crear el config");
		abort();

	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "")!=0){ // leido distinto a la cadena vacía
		log_info(logger, "--> %s", leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido! 
	char* leido;
	//Creamos el paquete
	t_paquete* paquete = crear_paquete();
	

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	// El resto, las vamos leyendo y empaquetando hasta recibir un string vacío
	while (strcmp(leido, "")!=0){ // leido distinto a la cadena vacía
		agregar_a_paquete(paquete, leido, strlen(leido));//void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio)
		free(leido);
		leido = readline("> ");
	}
	free(leido);
	//Aca envío el paquete
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
