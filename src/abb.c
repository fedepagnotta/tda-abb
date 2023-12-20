#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * Crea un arbol binario de búsqueda. Comparador es una función que se utiliza
 * para comparar dos elementos. La funcion de comparación no puede ser nula.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;
	struct abb *nuevo_abb = calloc(1, sizeof(struct abb));
	if (!nuevo_abb)
		return NULL;
	nuevo_abb->comparador = comparador;
	return nuevo_abb;
}

/**
 * Recibe un void pointer a un elemento, y crea un struct nodo_abb con
 * ese elemento.
 * Devuelve un puntero al nodo creado.
*/
struct nodo_abb *crear_nodo(void *elemento)
{
	struct nodo_abb *nuevo_nodo = calloc(1, sizeof(struct nodo_abb));
	if (!nuevo_nodo)
		return NULL;
	nuevo_nodo->elemento = elemento;
	return nuevo_nodo;
}

/**
 * Recibe un doble puntero a un struct nodo_abb, un void pointer a un elemento
 * y un abb_comparador, recorre recursivamente los hijos del nodo pasado por
 * parámetro e inserta un nuevo nodo con ese elemento de manera ordenada.
*/
void abb_insertar_recu(struct nodo_abb **nodo_actual, void *elemento,
		       abb_comparador comparador)
{
	if (!(*nodo_actual)) {
		*nodo_actual = crear_nodo(elemento);
		return;
	}
	if (comparador((*nodo_actual)->elemento, elemento) >= 0)
		abb_insertar_recu(&((*nodo_actual)->izquierda), elemento,
				  comparador);
	else
		abb_insertar_recu(&((*nodo_actual)->derecha), elemento,
				  comparador);
}

/**
 * Inserta un elemento en el arbol.
 * El arbol admite elementos con valores repetidos.
 *
 * Devuelve el arbol en caso de exito o NULL en caso de error.
 */
abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	abb_insertar_recu(&(arbol->nodo_raiz), elemento, arbol->comparador);
	arbol->tamanio++;
	return arbol;
}

/**
 * Recibe un puntero a un struct nodo_abb y devuelve cuantos hijos 
 * tiene ese nodo.
*/
int nodo_cantidad_hijos(struct nodo_abb *nodo)
{
	if (nodo->derecha && nodo->izquierda)
		return 2;
	if (nodo->derecha || nodo->izquierda)
		return 1;
	return 0;
}

/**
 * Recibe un struct nodo_abb y un entero (0 o 1).
 * Si el entero es 0, crea un puntero al hijo izquierdo del nodo pasado por
 * parámetro, o al derecho si es 1.
*/
struct nodo_abb *crear_nodo_a_quitar(struct nodo_abb *nodo_padre, int posicion)
{
	struct nodo_abb *nodo_a_quitar = NULL;
	if (posicion == 0)
		nodo_a_quitar = nodo_padre->izquierda;
	else
		nodo_a_quitar = nodo_padre->derecha;
	return nodo_a_quitar;
}

/**
 * Recibe dos punteros a struct nodo_abb, donde uno es el hijo del otro, y
 * a su vez no tiene ningún hijo, y un entero (0 o 1), que indica si este nodo
 * es el hijo izquierdo (0) o el derecho (1).
 * Libera el hijo y hace que el padre apunte a NULL en esa posición.
*/
void quitar_hijo_sin_hijos(struct nodo_abb *nodo_padre,
			   struct nodo_abb *nodo_a_quitar, int posicion)
{
	if (posicion == 0)
		nodo_padre->izquierda = NULL;
	else
		nodo_padre->derecha = NULL;
	free(nodo_a_quitar);
}

/**
 * Recibe dos punteros a struct nodo_abb, donde uno es el hijo del otro, y
 * a su vez tiene un solo hijo, y un entero (0 o 1), que indica si este nodo es
 * el hijo izquierdo (0) o el derecho (1).
 * Hace que el padre apunte al unico hijo del nodo pasado por parámetro, y
 * libera este nodo.
*/
void quitar_hijo_con_un_hijo(struct nodo_abb *nodo_padre,
			     struct nodo_abb *nodo_a_quitar, int posicion)
{
	if (posicion == 0) {
		if (nodo_a_quitar->derecha)
			nodo_padre->izquierda = nodo_a_quitar->derecha;
		else
			nodo_padre->izquierda = nodo_a_quitar->izquierda;
	} else {
		if (nodo_a_quitar->derecha)
			nodo_padre->derecha = nodo_a_quitar->derecha;
		else
			nodo_padre->derecha = nodo_a_quitar->izquierda;
	}
	free(nodo_a_quitar);
}

/**
 * Recibe un puntero a struct nodo_abb, y recorre recursivamente hacia la
 * derecho, devolviendo un puntero al nodo cuyo hijo derecho no tiene hijo
 * derecho. Es decir que devuelve un puntero al nodo padre del nodo predecesor
 * inorden (el inmediatamente menor) al padre del nodo pasado por parámetro.
*/
struct nodo_abb *buscar_predecesor_inorden(struct nodo_abb *nodo_actual)
{
	if (!nodo_actual->derecha->derecha) {
		return nodo_actual;
	}
	return buscar_predecesor_inorden(nodo_actual->derecha);
}

/**
 * Recibe un puntero a struct nodo_abb, quita el predecesor inorden de ese nodo
 * (aquel que contiene el elemento inmediatamente menor al nodo pasado por
 * parámetro), y devuelve el elemento del predecesor.
*/
void *quitar_predecesor_a_derecha(struct nodo_abb *nodo_actual)
{
	struct nodo_abb *nodo_padre_del_predecesor =
		buscar_predecesor_inorden(nodo_actual->izquierda);
	struct nodo_abb *nodo_predecesor = nodo_padre_del_predecesor->derecha;
	void *elemento_predecesor = nodo_predecesor->elemento;
	if (nodo_cantidad_hijos(nodo_predecesor) == 0) {
		quitar_hijo_sin_hijos(nodo_padre_del_predecesor,
				      nodo_predecesor, 1);
	} else {
		quitar_hijo_con_un_hijo(nodo_padre_del_predecesor,
					nodo_predecesor, 1);
	}
	return elemento_predecesor;
}

/**
 * Recibe un puntero a struct nodo_abb, quita su hijo izquierdo, y devuelve el
 * elemento de ese hijo.
*/
void *quitar_hijo_izquierda(struct nodo_abb *nodo_actual)
{
	void *elemento_predecesor = nodo_actual->izquierda->elemento;
	if (nodo_cantidad_hijos(nodo_actual->izquierda) == 0) {
		quitar_hijo_sin_hijos(nodo_actual, nodo_actual->izquierda, 0);
	} else {
		quitar_hijo_con_un_hijo(nodo_actual, nodo_actual->izquierda, 0);
	}
	return elemento_predecesor;
}

/**
 * Recibe un puntero a struct nodo_abb que tiene dos hijos, quita el nodo 
 * predecesor inorden del pasado por parámetro (aquel que contiene el elemento
 * inmediatamente menor), y copia el elemento predecesor en el nodo pasado por
 * parámetro. Es decir que quita un nodo con dos hijos de un abb.
*/
void quitar_hijo_con_dos_hijos(struct nodo_abb *nodo_a_quitar)
{
	void *elemento_predecesor = NULL;
	if (nodo_a_quitar->izquierda->derecha) {
		elemento_predecesor =
			quitar_predecesor_a_derecha(nodo_a_quitar);
	} else {
		elemento_predecesor = quitar_hijo_izquierda(nodo_a_quitar);
	}
	nodo_a_quitar->elemento = elemento_predecesor;
}

/**
 * Recibe un puntero a struct abb, un puntero a struct nodo_abb, un void
 * pointer a un elemento, y un entero (0 o 1).
 * Quita el hijo derecho (si el entero es 1) o el izquierdo (si es 0) del nodo
 * pasado por parámetro, reduce el tamaño del arbol, y devuelve el elemento.
*/
void *quitar_un_hijo(abb_t *arbol, struct nodo_abb *nodo_padre, void *elemento,
		     int posicion)
{
	struct nodo_abb *nodo_a_quitar =
		crear_nodo_a_quitar(nodo_padre, posicion);
	if (nodo_cantidad_hijos(nodo_a_quitar) == 0) {
		quitar_hijo_sin_hijos(nodo_padre, nodo_a_quitar, posicion);
	} else if (nodo_cantidad_hijos(nodo_a_quitar) == 1) {
		quitar_hijo_con_un_hijo(nodo_padre, nodo_a_quitar, posicion);
	} else {
		quitar_hijo_con_dos_hijos(nodo_a_quitar);
	}
	arbol->tamanio--;
	return elemento;
}

/**
 * Recibe un puntero a struct abb, un puntero a struct nodo_abb, un void
 * pointer a un elemento, y un abb_comparador. Recorre el arbol recursivamente
 * a partir del nodo pasado por parámetro hasta encontrar el nodo que contiene
 * el elemento recibido, lo quita del árbol y lo devuelve.
*/
void *abb_quitar_recu(abb_t *arbol, struct nodo_abb *nodo_actual,
		      void *elemento, abb_comparador comparador)
{
	if (!nodo_actual) {
		return NULL;
	} else if (nodo_actual->derecha &&
		   comparador(nodo_actual->derecha->elemento, elemento) == 0) {
		return quitar_un_hijo(arbol, nodo_actual, elemento, 1);
	} else if (nodo_actual->izquierda &&
		   comparador(nodo_actual->izquierda->elemento, elemento) ==
			   0) {
		return quitar_un_hijo(arbol, nodo_actual, elemento, 0);
	} else if (comparador(nodo_actual->elemento, elemento) > 0) {
		return abb_quitar_recu(arbol, nodo_actual->izquierda, elemento,
				       comparador);
	} else {
		return abb_quitar_recu(arbol, nodo_actual->derecha, elemento,
				       comparador);
	}
}

/**
 * Recibe un puntero a un struct abb que contiene un único elemento, y un
 * puntero a ese elemento. Quita el único elemento del árbol, reduce su tamaño
 * y devuelve el elemento.
*/
void *quitar_unico_elemento(abb_t *arbol, void *elemento)
{
	free(arbol->nodo_raiz);
	arbol->nodo_raiz = NULL;
	arbol->tamanio--;
	return elemento;
}

/**
 * Recibe un puntero a un struct abb cuya raíz tiene un solo hijo, y quita esa
 * raíz, haciendo que su único hijo sea la nueva raiz.
*/
void quitar_raiz_con_un_hijo(abb_t *arbol)
{
	struct nodo_abb *raiz_aux = crear_nodo(arbol->nodo_raiz->elemento);
	raiz_aux->derecha = arbol->nodo_raiz;
	quitar_hijo_con_un_hijo(raiz_aux, arbol->nodo_raiz, 1);
	arbol->nodo_raiz = raiz_aux->derecha;
	free(raiz_aux);
}

/**
 * Recibe un puntero a un struct abb, un void pointer a un elemento, y la
 * un entero (1 o 2), que indica la cantidad de hijos de la raíz del arbol, 
 * quita la raíz, reduce el tamaño del árbol y devuelve el elemento pasado
 * por parámetro (el de la raíz eliminada).
*/
void *quitar_raiz_con_hijos(abb_t *arbol, void *elemento, int cant_hijos)
{
	if (cant_hijos == 1) {
		quitar_raiz_con_un_hijo(arbol);
	} else {
		quitar_hijo_con_dos_hijos(arbol->nodo_raiz);
	}
	arbol->tamanio--;
	return elemento;
}

/**
 * Busca en el arbol un elemento igual al provisto (utilizando la funcion de
 * comparación) y si lo encuentra lo quita del arbol y lo devuelve.
 * Devuelve el elemento extraido del árbol o NULL en caso de error.
 */
void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_tamanio(arbol) == 0)
		return NULL;
	if (arbol->comparador(arbol->nodo_raiz->elemento, elemento) == 0) {
		if (nodo_cantidad_hijos(arbol->nodo_raiz) == 0)
			return quitar_unico_elemento(arbol, elemento);
		else
			return quitar_raiz_con_hijos(
				arbol, elemento,
				nodo_cantidad_hijos(arbol->nodo_raiz));
	}
	return abb_quitar_recu(arbol, arbol->nodo_raiz, elemento,
			       arbol->comparador);
}

/**
 * Recibe un struct nodo_abb, un void pointer a un elemento que se quiere 
 * buscar en el arbol y un abb_comparador.
 * Recorre recursivamente los hijos del nodo pasado por parámetro buscando
 * el elemento pasado, si lo encuentra lo devuelve, y si no devuelve NULL.
*/
void *abb_buscar_recu(struct nodo_abb *nodo_actual, void *elemento,
		      abb_comparador comparador)
{
	if (!nodo_actual)
		return NULL;
	else if (comparador(nodo_actual->elemento, elemento) == 0)
		return nodo_actual->elemento;
	else if (comparador(nodo_actual->elemento, elemento) < 0)
		return abb_buscar_recu(nodo_actual->derecha, elemento,
				       comparador);
	else
		return abb_buscar_recu(nodo_actual->izquierda, elemento,
				       comparador);
}

/**
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	return abb_buscar_recu(arbol->nodo_raiz, elemento, arbol->comparador);
}

/**
 * Determina si el árbol está vacío.
 *
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene al
 * menos un elemento.
 */
bool abb_vacio(abb_t *arbol)
{
	return !arbol || arbol->tamanio == 0;
}

/**
 * Devuelve la cantidad de elementos almacenados en el arbol o 0 si el arbol es
 * NULL.
 */
size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;
	return arbol->tamanio;
}

/**
 * Recibe un struct nodo_abb y un puntero a una funcion que recibe un void
 * pointer, la cual se invoca con cada elemento de los hijos del nodo pasado, 
 * y luego libera cada uno de los nodos.
*/
void abb_destruir_nodos(struct nodo_abb *nodo_actual,
			void (*destructor)(void *))
{
	if (!nodo_actual) {
		return;
	}
	abb_destruir_nodos(nodo_actual->izquierda, destructor);
	abb_destruir_nodos(nodo_actual->derecha, destructor);
	if (destructor && destructor != free) {
		/**
		 * Pongo destructor != free porque si no me tira invalid free
		 * en mis pruebas, ṕorque creo los elementos del abb en el
		 * stack.
		*/
		destructor(nodo_actual->elemento);
	}
	free(nodo_actual);
}

/**
 * Destruye el arbol liberando la memoria reservada por el mismo.
 */
void abb_destruir(struct abb *arbol)
{
	if (!arbol) {
		return;
	}
	abb_destruir_nodos(arbol->nodo_raiz, NULL);
	free(arbol);
}

/**
 * Destruye el arbol liberando la memoria reservada por el mismo.
 *
 * Adicionalmente invoca el destructor en cada uno de los elementos almacenados
 * en el arbol (si la funcion destructor no es NULL).
 */
void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}
	abb_destruir_nodos(arbol->nodo_raiz, destructor);
	free(arbol);
}

/**
 * Recorre los hijos del nodo pasado por parámetro de manera inorden e invoca
 * la funcion con el elemento de cada uno. El puntero aux se pasa como segundo
 * parámetro a la función. Si la función devuelve false, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve true se sigue
 * recorriendo mientras queden elementos.
*/
bool abb_recorrer_inorden(struct nodo_abb *nodo_actual,
			  bool (*funcion)(void *, void *), void *aux, size_t *i)
{
	if (!nodo_actual)
		return true;
	if (abb_recorrer_inorden(nodo_actual->izquierda, funcion, aux, i) ==
	    false)
		return false;
	(*i)++;
	if (funcion(nodo_actual->elemento, aux) == false)
		return false;
	return abb_recorrer_inorden(nodo_actual->derecha, funcion, aux, i);
}

/**
 * Recorre los hijos del nodo pasado por parámetro de manera preorden e invoca
 * la funcion con el elemento de cada uno. El puntero aux se pasa como segundo
 * parámetro a la función. Si la función devuelve false, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve true se sigue
 * recorriendo mientras queden elementos.
*/
bool abb_recorrer_preorden(struct nodo_abb *nodo_actual,
			   bool (*funcion)(void *, void *), void *aux,
			   size_t *i)
{
	if (!nodo_actual)
		return true;
	(*i)++;
	if (funcion(nodo_actual->elemento, aux) == false)
		return false;
	if (abb_recorrer_preorden(nodo_actual->izquierda, funcion, aux, i) ==
	    false)
		return false;
	return abb_recorrer_preorden(nodo_actual->derecha, funcion, aux, i);
}

/**
 * Recorre los hijos del nodo pasado por parámetro de manera postorden e invoca
 * la funcion con el elemento de cada uno. El puntero aux se pasa como segundo
 * parámetro a la función. Si la función devuelve false, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve true se sigue
 * recorriendo mientras queden elementos.
*/
bool abb_recorrer_postorden(struct nodo_abb *nodo_actual,
			    bool (*funcion)(void *, void *), void *aux,
			    size_t *i)
{
	if (!nodo_actual)
		return true;
	if (abb_recorrer_postorden(nodo_actual->izquierda, funcion, aux, i) ==
	    false)
		return false;
	if (abb_recorrer_postorden(nodo_actual->derecha, funcion, aux, i) ==
	    false)
		return false;
	(*i)++;
	return funcion(nodo_actual->elemento, aux);
}

/**
 * Recorre el arbol e invoca la funcion con cada elemento almacenado en el mismo
 * como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 * función. Si la función devuelve false, se finaliza el recorrido aun si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos.
 *
 * Recorrido especifica el tipo de recorrido a realizar.
 *
 * Devuelve la cantidad de veces que fue invocada la función.
*/
size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion)
		return 0;
	size_t contador = 0;
	if (recorrido == INORDEN)
		abb_recorrer_inorden(arbol->nodo_raiz, funcion, aux, &contador);
	else if (recorrido == PREORDEN)
		abb_recorrer_preorden(arbol->nodo_raiz, funcion, aux,
				      &contador);
	else if (recorrido == POSTORDEN)
		abb_recorrer_postorden(arbol->nodo_raiz, funcion, aux,
				       &contador);
	return contador;
}

/**
 * Estructura que almacena información sobre un array para iterarlo: el tamaño
 * del mismo, un puntero al array, y la posición actual de la iteración del
 * array (el índice).
*/
struct estado_array {
	size_t tamanio_maximo;
	void **array;
	int indice;
};

/**
 * Recibe un void pointer a un elemento y otro que debe ser a un struct
 * estado_array. Almacena el elemento en la posición del índice de estado_array
 * y aumenta el índice en uno. Si se pudo agregar el elemento devuelve true,
 * pero si el array está lleno (indice >= tamaño) devuelve false.
*/
bool agregar_elemento_al_array(void *elemento, void *estado_array)
{
	int indice = ((struct estado_array *)estado_array)->indice;
	int tamanio =
		(int)((struct estado_array *)estado_array)->tamanio_maximo;
	if (indice >= tamanio)
		return false;
	(((struct estado_array *)estado_array)->array)[indice] = elemento;
	(((struct estado_array *)estado_array)->indice)++;
	return true;
}

/**
 * Recorre el arbol según el recorrido especificado y va almacenando los
 * elementos en el array hasta completar el recorrido o quedarse sin espacio en
 * el array.
 *
 * El array tiene un tamaño maximo especificado por tamanio_array.
 *
 * Devuelve la cantidad de elementos que fueron almacenados exitosamente en el
 * array.
 */
size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!array)
		return 0;
	struct estado_array *estado_array =
		calloc(1, sizeof(struct estado_array));
	estado_array->tamanio_maximo = tamanio_array;
	estado_array->array = array;
	estado_array->indice = 0;
	abb_con_cada_elemento(arbol, recorrido, agregar_elemento_al_array,
			      estado_array);
	size_t resultado = (size_t)estado_array->indice;
	free(estado_array);
	return resultado;
}