#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"



int main(int argc, char const *argv[])
{
	Cola* c = cola_crear(600);
	printf("%d, %d\n",cola_vacia(c), cola_llena(c) );
	cola_agregar(c, 5);
	cola_agregar(c,4);
	cola_agregar(c,3);
	cola_agregar(c,2);
	cola_agregar(c,1);
	printf("%d, %d\n",cola_cant_elementos(c), c->cant );
	for (int i = 0; i < c->cant; ++i)
	{
		printf("%d\n",c->array[i] );
	}
	cola_pop(c);
	cola_pop(c);
	printf("---------------<------\n");
	printf("primero: %d, ultimo: %d\n",c->primero, c->ultimo );
	for (int i = c->primero; i < c->ultimo+1; ++i)
	{
		printf("%d\n",c->array[i] );

	}


	return 0;
}