#include "hash.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash* tabla = malloc(sizeof(TablaHash));
  tabla->hash = hash;
  tabla->capacidad = capacidad;
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidad);
  tabla->numElems = 0;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->tabla[idx].clave = NULL;
    tabla->tabla[idx].dato = NULL;
    tabla->tabla[idx].next = NULL;
  }
  return tabla;
}



/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, void* clave, void* dato, FuncionCopia copia) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(clave);
  idx = idx % tabla->capacidad;
  void* copiaClave = copia(clave);
  void* copiaDato = copia(dato);
  // Si el lugar estaba vacío, incrementamos el número de elementos.
  if (tabla->tabla[idx].clave ==  NULL){
  	// Almacenamos los datos ingresados.
  	tabla->tabla[idx].clave = copiaClave;
  	tabla->tabla[idx].dato = copiaDato;
  	tabla->numElems++;
  }
  else
  { 
    CasillaHash* temp = &tabla->tabla[idx];  
    while(temp->next != NULL)
    {

    	temp = temp->next;
    }
    
    temp->next = malloc(sizeof(CasillaHash));
    temp = temp->next;
    temp->clave = copiaClave;
    temp->dato = copiaDato;
    temp->next = NULL;
  }
  if ((tabla->numElems / tabla->capacidad) > 0.7)
  {
    tabla->tabla = realloc(tabla->tabla, sizeof(CasillaHash)*(tabla->capacidad + (tabla->capacidad / 5)));
    tabla->capacidad = (tabla->capacidad + (tabla->capacidad / 5));
  }
    
  
}

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
int tablahash_buscar(TablaHash* tabla, void* clave, FuncionIgualdad igual) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(clave);
  idx = idx % tabla->capacidad;
  int flag = 0;
  // Si el lugar esta vacío, retornamos un puntero nulo.
  if (!(igual(tabla->tabla[idx].clave, clave)))
  {
  	CasillaHash* temp = &tabla->tabla[idx];   
    while(temp->next != NULL)
    {
    	temp = temp->next;
    	if (igual(temp->clave, clave))
    	{
    		flag = 1;
    	}
    }
     return flag;
  }
   
    flag = 1;
  return flag;
}


/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla) {
  free(tabla->tabla);
  free(tabla);
}
