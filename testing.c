#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glist.h"
#include "bstree.h"
#include <locale.h>
#include "tablahash.h"

#define B 121139

FuncionIgualdad* igual(void* a, void* b)
{
  return a == b;
}

unsigned int RSHash(char* str)
{ 	
	unsigned int b = 378551;
 	unsigned int a = 63689;
 	unsigned int hash = 0;
	unsigned int i = 0;
 	for(i = 0; *str; str++, i++)
 	{ 
 		hash = hash * a + (*str);
 		a = a * b;
 	}
 return (hash%B);
}


void creacion_abb(char* nombreArchivo, TablaHash* th)
{
	int cant = 0;
	char* palabra = NULL;
	FILE *fp = fopen(nombreArchivo, "r");
	size_t j = 0;
	size_t final;
	

	while(!(feof(fp)))
	{		
		final = getline(&palabra, &j, fp);
		palabra[final-1] = '\0';
		
		final--;
		
		cant +=tablahash_insertar(th, palabra,palabra, igual);
	}
	free(palabra);
	fclose(fp);
	printf("%d\n",cant );
	
}

int main(int argc, char const *argv[])
{
	TablaHash* th = tablahash_crear(121129, RSHash, igual);
	creacion_abb("listado-general.txt",th);


	return 0;
}