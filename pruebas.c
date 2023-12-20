#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"
#include <string.h>

/**
 * Recibe dos void pointer, y los compara como si fueran enteros.
 * Devuelve 0 si son iguales, > 0 si elemento1 > elemento2, y < 0
 * si elemento1 < elemento2.
*/
int comparador(void *elemento1, void *elemento2)
{
	return *(int *)elemento1 - *(int *)elemento2;
}

/**
 * Prueba si se puede crear un arbol, y si intentar crear uno pasando un
 * comparador NULL devuelve NULL.
*/
void prueba_crear_abb()
{
	struct abb *abb = abb_crear(comparador);
	pa2m_afirmar(!abb->nodo_raiz && abb->comparador == comparador &&
			     abb->tamanio == 0,
		     "Se puede crear un nuevo abb.");
	pa2m_afirmar(!abb_crear(NULL),
		     "No pasa nada si intento crear abb con comparador NULL.");
	abb_destruir(abb);
}

/**
 * Prueba si se pueden insertar elementos en un arbol.
*/
void prueba_insertar()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 5, num3 = 2;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		abb->tamanio == 4 && *(int *)abb->nodo_raiz->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 6 &&
			*(int *)abb->nodo_raiz->derecha->izquierda->elemento ==
				5 &&
			*(int *)abb->nodo_raiz->izquierda->elemento == 2,
		"Se puede insertar.");
	abb_destruir_todo(abb, free);
}

/**
 * Prueba si se puede quitar un nodo hoja (sin hijos) de un arbol.
*/
void prueba_quitar_nodo_hoja()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 5, num3 = 2, num_quitar = 5;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(*(int *)abb_quitar(abb, &num_quitar) == 5 &&
			     !(abb->nodo_raiz->derecha->izquierda),
		     "Se puede quitar un nodo hoja.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar un nodo con un hijo de un arbol.
*/
void prueba_quitar_nodo_con_un_hijo()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 5, num3 = 2, num_quitar = 6;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(*(int *)abb_quitar(abb, &num_quitar) == 6 &&
			     *(int *)abb->nodo_raiz->derecha->elemento == 5,
		     "Se puede quitar un nodo con un hijo.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar un nodo con dos hijos de un arbol, cuyo
 * predecesor inorden es su hijo izquierdo (ya que este no tiene hijo derecho).
*/
void prueba_quitar_nodo_con_dos_hijos_caso1()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 5, num3 = 9, num_quitar = 7;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		*(int *)abb_quitar(abb, &num_quitar) == 7 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 5,
		"Se puede quitar un nodo con dos hijos con pred. inorden a la izq.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar un nodo con dos hijos de un arbol, cuyo
 * predecesor inorden no es su hijo izquierdo, ya que este tiene hijo derecho.
*/
void prueba_quitar_nodo_con_dos_hijos_caso2()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 5, num3 = 6, num_quitar = 7;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		*(int *)abb_quitar(abb, &num_quitar) == 7 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 5,
		"Se puede quitar un nodo con dos hijos con predecesor inorden 'lejos'.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar una raíz sin hijos de un árbol (tamaño 1).
*/
void prueba_quitar_raiz_sin_hijos()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num_quitar = 4;
	abb = abb_insertar(abb, &raiz);
	pa2m_afirmar(*(int *)abb_quitar(abb, &num_quitar) == 4 &&
			     !(abb->nodo_raiz),
		     "Se puede quitar raíz sin hijos.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar una raíz con un hijo de un árbol.
*/
void prueba_quitar_raiz_con_un_hijo()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 2, num2 = 3, num3 = 1, num_quitar = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(*(int *)abb_quitar(abb, &num_quitar) == 4 &&
			     *(int *)abb->nodo_raiz->elemento == 2,
		     "Se puede quitar raíz con un hijo.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar la raíz con dos hijos de un arbol, cuyo
 * predecesor inorden es su hijo izquierdo (ya que este no tiene hijo derecho).
*/
void prueba_quitar_raiz_con_dos_hijos_caso1()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 2, num_quitar = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	pa2m_afirmar(
		*(int *)abb_quitar(abb, &num_quitar) == 4 &&
			*(int *)abb->nodo_raiz->elemento == 2,
		"Se puede quitar raíz con dos hijos con pred. a la derecha.");
	abb_destruir(abb);
}

/**
 * Prueba si se puede quitar la raíz con dos hijos de un arbol, cuyo
 * predecesor inorden no es su hijo izquierdo, ya que este tiene hijo derecho.
*/
void prueba_quitar_raiz_con_dos_hijos_caso2()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 2, num3 = 3, num_quitar = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		*(int *)abb_quitar(abb, &num_quitar) == 4 &&
			*(int *)abb->nodo_raiz->elemento == 3,
		"Se puede quitar raíz con dos hijos con pred. 'más lejos'.");
	abb_destruir(abb);
}

/**
 * Prueba si al intentar quitar un elemento que no está en el árbol, se
 * devuelve NULL.
*/
void prueba_quitar_elemento_no_encontrado()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 2, num3 = 5, num_quitar = 3;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		!abb_quitar(abb, &num_quitar),
		"No pasa nada si no se encuentra el elemento que quiero quitar.");
	abb_destruir(abb);
}

/**
 * Prueba la función abb_buscar, tanto cuando el elemento buscado está en
 * el árbol como cuando no lo está.
*/
void prueba_buscar()
{
	struct abb *abb = abb_crear(comparador);
	int raiz = 4, num1 = 7, num2 = 2, num3 = 5, num_buscar1 = 7,
	    num_buscar2 = 3;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(*(int *)abb_buscar(abb, &num_buscar1) == 7,
		     "Puedo buscar un elemento que está en el abb.");
	pa2m_afirmar(!abb_buscar(abb, &num_buscar2),
		     "No se encontró un elemento que no estaba en el abb.");
	abb_destruir_todo(abb, free);
}

/**
 * Prueba la función abb_vacio.
*/
void prueba_vacio()
{
	struct abb *abb = abb_crear(comparador);
	pa2m_afirmar(abb_vacio(abb) == true,
		     "abb_vacio devuelve true si está vacío.");
	pa2m_afirmar(abb_vacio(NULL) == true,
		     "abb_vacio devuelve true si el abb es NULL.");
	int raiz = 2;
	abb = abb_insertar(abb, &raiz);
	pa2m_afirmar(abb_vacio(abb) == false,
		     "abb_vacio devuelve false si no está vacío.");
	abb_destruir(abb);
}

/**
 * Prueba la función abb_tamanio.
*/
void prueba_tamanio()
{
	struct abb *abb = abb_crear(comparador);
	pa2m_afirmar(abb_tamanio(NULL) == 0,
		     "abb_tamanio devuelve 0 si el abb es NULL.");
	int raiz = 2;
	abb = abb_insertar(abb, &raiz);
	pa2m_afirmar(abb_tamanio(abb) == 1,
		     "abb_tamanio devuelve el tamaño correcto.");
	abb_destruir(abb);
}

/**
 * Recibe dos void pointer que son tratados como int pointer.
 * Convierte el parámetro num en el contexto. 
 * Si ambos son iguales devuelve false, si no devuelve true.
*/
bool convertir_numero_en_otro(void *num, void *contexto)
{
	if (*(int *)num == *(int *)contexto)
		return false;
	*(int *)num = *(int *)contexto;
	return true;
}

/**
 * Prueba la función con_cada_elemento con recorrido inorden, cuando el
 * recorrido se corta porque la función pasada por parámetro devuelve false.
*/
void prueba_con_cada_elemento_inorden_con_corte()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 1, num1 = 4, num2 = 8, num3 = 3, contexto = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		abb_con_cada_elemento(abb, INORDEN, convertir_numero_en_otro,
				      &contexto) == 3 &&
			*(int *)abb->nodo_raiz->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->izquierda->elemento ==
				4 &&
			*(int *)abb->nodo_raiz->derecha->derecha->elemento == 8,
		"con_cada_elemento inorden corta si la función devuelve false.");
	abb_destruir(abb);
}

/**
 * Prueba la función con_cada_elemento con recorrido preorden, cuando el
 * recorrido se corta porque la función pasada por parámetro devuelve false.
*/
void prueba_con_cada_elemento_preorden_con_corte()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 1, num1 = 4, num2 = 8, num3 = 3, contexto = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		abb_con_cada_elemento(abb, PREORDEN, convertir_numero_en_otro,
				      &contexto) == 2 &&
			*(int *)abb->nodo_raiz->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->izquierda->elemento ==
				3 &&
			*(int *)abb->nodo_raiz->derecha->derecha->elemento == 8,
		"con_cada_elemento preorden corta si la función devuelve false.");
	abb_destruir(abb);
}

/**
 * Prueba la función con_cada_elemento con recorrido postorden, cuando el
 * recorrido se corta porque la función pasada por parámetro devuelve false.
*/
void prueba_con_cada_elemento_postorden_con_corte()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 1, num1 = 4, num2 = 8, num3 = 3, contexto = 4;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	pa2m_afirmar(
		abb_con_cada_elemento(abb, POSTORDEN, convertir_numero_en_otro,
				      &contexto) == 3 &&
			*(int *)abb->nodo_raiz->elemento == 1 &&
			*(int *)abb->nodo_raiz->derecha->elemento == 4 &&
			*(int *)abb->nodo_raiz->derecha->izquierda->elemento ==
				4 &&
			*(int *)abb->nodo_raiz->derecha->derecha->elemento == 4,
		"con_cada_elemento postorden corta si la función devuelve false.");
	abb_destruir(abb);
}

/**
 * Recibe una lista de void pointers (que es tratada como una lista de
 * enteros) y su tamaño, y una lista de enteros. Valida si la primera lista
 * es igual a la segunda, devolviendo true o false según corresponda.
*/
bool validar_lista(void **lista_recibida, size_t tamanio_lista,
		   int *lista_esperada)
{
	int i = 0;
	while (i < tamanio_lista) {
		if (*(int *)lista_recibida[i] != lista_esperada[i])
			return false;
		i++;
	}
	return true;
}

/**
 * Prueba la función abb_recorrer con recorrido inorden.
*/
void prueba_recorrer_inorden()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 7, num3 = 5, num4 = 2, num5 = 3,
	    num6 = 1;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	abb = abb_insertar(abb, &num4);
	abb = abb_insertar(abb, &num5);
	abb = abb_insertar(abb, &num6);
	void *lista_inorden[7];
	int lista_esperada[7] = { 1, 2, 3, 4, 5, 6, 7 };
	abb_recorrer(abb, INORDEN, lista_inorden, 7);
	pa2m_afirmar(validar_lista(lista_inorden, 7, lista_esperada) == true,
		     "Se puede recorrer un arbol inorden.");
	abb_destruir(abb);
}

/**
 * Prueba la función abb_recorrer con recorrido preorden.
*/
void prueba_recorrer_preorden()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 7, num3 = 5, num4 = 2, num5 = 3,
	    num6 = 1;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	abb = abb_insertar(abb, &num4);
	abb = abb_insertar(abb, &num5);
	abb = abb_insertar(abb, &num6);
	void *lista_inorden[7];
	int lista_esperada[7] = { 4, 2, 1, 3, 6, 5, 7 };
	abb_recorrer(abb, PREORDEN, lista_inorden, 7);
	pa2m_afirmar(validar_lista(lista_inorden, 7, lista_esperada) == true,
		     "Se puede recorrer un arbol preorden.");
	abb_destruir(abb);
}

/**
 * Prueba la función abb_recorrer con recorrido postorden.
*/
void prueba_recorrer_postorden()
{
	abb_t *abb = abb_crear(comparador);
	int raiz = 4, num1 = 6, num2 = 7, num3 = 5, num4 = 2, num5 = 3,
	    num6 = 1;
	abb = abb_insertar(abb, &raiz);
	abb = abb_insertar(abb, &num1);
	abb = abb_insertar(abb, &num2);
	abb = abb_insertar(abb, &num3);
	abb = abb_insertar(abb, &num4);
	abb = abb_insertar(abb, &num5);
	abb = abb_insertar(abb, &num6);
	void *lista_inorden[7];
	int lista_esperada[7] = { 1, 3, 2, 5, 7, 6, 4 };
	abb_recorrer(abb, POSTORDEN, lista_inorden, 7);
	pa2m_afirmar(validar_lista(lista_inorden, 7, lista_esperada) == true,
		     "Se puede recorrer un arbol postorden.");
	abb_destruir(abb);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== ABB ========================");
	prueba_crear_abb();
	prueba_insertar();
	prueba_quitar_nodo_hoja();
	prueba_quitar_nodo_con_un_hijo();
	prueba_quitar_nodo_con_dos_hijos_caso1();
	prueba_quitar_nodo_con_dos_hijos_caso2();
	prueba_quitar_raiz_sin_hijos();
	prueba_quitar_raiz_con_un_hijo();
	prueba_quitar_raiz_con_dos_hijos_caso1();
	prueba_quitar_raiz_con_dos_hijos_caso2();
	prueba_quitar_elemento_no_encontrado();
	prueba_buscar();
	prueba_vacio();
	prueba_tamanio();
	prueba_con_cada_elemento_inorden_con_corte();
	prueba_con_cada_elemento_preorden_con_corte();
	prueba_con_cada_elemento_postorden_con_corte();
	prueba_recorrer_inorden();
	prueba_recorrer_preorden();
	prueba_recorrer_postorden();
	return pa2m_mostrar_reporte();
}
