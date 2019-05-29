#ifndef __COLA_H__
#define __COLA_H__ 


#define MAX_SIZE 500
typedef struct 
{
	int size;
	void* array[MAX_SIZE];
	int primero;
	int ultimo;
	int cant;

}Cola;

Cola* cola_crear(int size);

int cola_vacia(Cola* c);

int cola_llena(Cola* c);

int cola_cant_elementos(Cola* c);

void cola_agregar(Cola* c, void* dato);

void* cola_pop(Cola* c);

#endif