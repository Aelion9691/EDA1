#ifndef  __HASH_H__
#define __HASH_H__ 

/**
 * Tipo de las funciones hash a ser consideradas por las tablas hash.
 */
typedef unsigned (*FuncionHash)(void* clave);

typedef void* (*FuncionCopia)(void* dato);

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct _CasillaHash{
  void* clave;
  void* dato;
  struct _CasillaHash* next;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
typedef struct {
  CasillaHash* tabla;
  unsigned numElems;
  unsigned capacidad;
  FuncionHash hash;
} TablaHash;

typedef int (*FuncionIgualdad)(void *, void *);

TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash);

void tablahash_insertar(TablaHash* tabla, void* clave, void* dato, FuncionCopia copia);

int tablahash_buscar(TablaHash* tabla, void* clave, FuncionIgualdad igual);
#endif