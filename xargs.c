#ifndef NARGS
#define NARGS 4
#endif
#include <stdio.h>

int
main(int argc, char *argv[])
{
	char* binario = argv[0];
	char* comando = argv[1]; //comando para ejecutar
	char* buffer[NARGS]; //buffer con capacidad para NARGS

	//int contador = 0; //contador para saber cuantos argumentos leí
	// Your code here
	//while contador < 4
	printf("este es el nombre del binario : %s\n",binario);
	printf("este es el nombre del comando : %s\n",comando);
	return 0;
}
