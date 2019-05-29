#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"




Cola* cola_crear(int size)
{
	Cola* c = malloc(sizeof(Cola));
	c->size = size;
	*(c->array) = malloc(sizeof(void*) * size);
	c->primero = 0;
	c->ultimo = -1;
	c->cant = 0;
}

int cola_vacia(Cola* c)
{
	return (c->cant > 0);
}

int cola_llena(Cola* c)
{
	return(c->cant == c->size);
}

int cola_cant_elementos(Cola* c)
{
	return c->cant;
}

void cola_agregar(Cola* c, void* dato)
{
	if (!(cola_llena(c)))
	{
		if (c->ultimo == (c->size - 1))
		{
			c->ultimo = -1;
		} 
		c->array[++c->ultimo] = dato;
		c->cant++;
	}
	else
	{
		*(c->array) = realloc(c->array, sizeof(void*) * c->size + 100);
		c->size += 100;
		if (c->ultimo == (c->size - 1))
		{
			c->ultimo = -1;
		} 
		c->array[++c->ultimo] = dato;
		c->cant++;
	}
}

void* cola_pop(Cola* c)
{
	void* dato = c->array[c->primero++];
	if(c->primero == c->size)
		c->primero = 0;
	c->cant--;
	return dato;
}
