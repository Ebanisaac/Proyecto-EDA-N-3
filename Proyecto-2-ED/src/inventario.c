/*
 * Archivo: inventario.c
 * Autor: Pablo Serón
 * Descripción: 
 * 
 * Implementación de las funciones del modulo inventario
 * (lista enlazada simple con nodo centinela).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objeto.h"
#include "inventario.h"

/**
 * @brief Crea el nodo centinela y deja el inventario vacio, listo para usar.
 *
 * @param inventario Inventario a inicializar.
 *
 * @return INVENTARIO_OK si se inicializó, INVENTARIO_ERROR_PARAMETRO si el
 *         puntero es NULL, o INVENTARIO_ERROR_MEMORIA si malloc falla (en
 *         ese caso el inventario queda sin inicializar).
 */
ResultadoInventario inventario_inicializar(Inventario *inventario)
{
    NodoInventario *centinela;

    // Evitamos escribir en un puntero nulo (segmentation fault)
    if (inventario == NULL)
    {
        return INVENTARIO_ERROR_PARAMETRO;
    }

    //Pedimos espacio a la memoria
    centinela = malloc(sizeof(NodoInventario));
    if (centinela == NULL)
    {
        // Si no queda/hay memoria dejamos el inventario "sin inicializar"
        //Las demas funciones detectan cabeza == NULL y no se caen.
        inventario->cabeza = NULL;
        inventario->cantidad = 0;
        return INVENTARIO_ERROR_MEMORIA;
    }

    //Nota: El centinela no guarda un objeto real pero igual dejamos sus campos
    //en un estado conocido (id 0 es invalido, porque los IDs reales son > 0)

    centinela->objeto.id = 0;
    centinela->objeto.nombre[0] = '\0';
    centinela->objeto.tipo = TIPO_OTRO;
    centinela->objeto.valor = 0;

    // Lista vacia: el centinela no apunta a ningún objeto real
    // Ej:  cabeza -> centinela -> NULL
    centinela->siguiente = NULL;

    inventario->cabeza = centinela;
    inventario->cantidad = 0;
    return INVENTARIO_OK;
}

/**
 * @brief Agrega una copia del objeto al final del inventario.
 *
 * @param inventario Inventario destino (ya inicializado).
 * @param objeto Objeto a copiar dentro de la lista.
 *
 * @return INVENTARIO_OK si se agrego, INVENTARIO_ERROR_PARAMETRO si algun
 *         puntero es NULL o el inventario no esta inicializado,
 *         INVENTARIO_ERROR_ID_DUPLICADO si ya existe un objeto con ese ID,
 *         o INVENTARIO_ERROR_MEMORIA si el malloc falla (en cualquiera de
 *         los errores el inventario queda sin cambios).
 */
ResultadoInventario inventario_agregar(Inventario *inventario, const Objeto *objeto)
{
    NodoInventario *nuevo;
    NodoInventario *ultimo;

    // Validamos los punteros
    if (inventario == NULL || inventario->cabeza == NULL || objeto == NULL)
    {
        return INVENTARIO_ERROR_PARAMETRO;
    }

    // Segun lo pedido los ID deben ser unicos 
    //si ya existe un objeto con este ID este no se
    // agregara (se revisamos ANTES del malloc para no pedir memoria que
    // luego habría que liberar).
    if (inventario_buscar_por_id(inventario, objeto->id) != NULL)
    {
        return INVENTARIO_ERROR_ID_DUPLICADO;
    }

    // Pedimos espacio para el nodo ANTES de tocar la lista. Si malloc falla
    // retornamos error correspondiente
    nuevo = malloc(sizeof(NodoInventario));
    if (nuevo == NULL)
    {
        return INVENTARIO_ERROR_MEMORIA;
    }

    // Copiamos el contenido del objeto dentro del nodo
    nuevo->objeto = *objeto;

    // El nuevo nodo será el último, por eso no apunta a nada
    nuevo->siguiente = NULL;

    // Partimos desde el centinela y avanzamos hasta el último nodo
    ultimo = inventario->cabeza;
    while (ultimo->siguiente != NULL)
    {
        ultimo = ultimo->siguiente;
    }

    // Enganchamos el nuevo nodo al final
    ultimo->siguiente = nuevo;

    // Solo aumentamos la cantidad si todo salió bien
    inventario->cantidad++;
    return INVENTARIO_OK;
}

/**
 * @brief Imprime por terminal todos los objetos del inventario.
 *        Si está vacío, muestra un mensaje indicándolo.
 *
 * @param inventario Inventario a mostrar. Si es NULL o no está
 *                   inicializado, se trata como vacío.
 */
void inventario_listar(const Inventario *inventario)
{
    NodoInventario *actual;

    //Si el inventario esta vacio (no existe), no está inicializado 
    //o si el centinela no apunta a ningún objeto real 
    if (inventario == NULL || inventario->cabeza == NULL ||
        inventario->cabeza->siguiente == NULL)
    {
        printf("El inventario está vacío.\n");
        return;
    }

    // Imprime "objeto" o "objetos" segun corresponda
    printf("Inventario (%d objeto%s):\n", inventario->cantidad,
           inventario->cantidad == 1 ? "" : "s");

    // Empezamos en cabeza->siguiente: saltamos el centinela, que no es un
    // objeto real y no debe imprimirse
    actual = inventario->cabeza->siguiente;
    while (actual != NULL)
    {
        objeto_imprimir(&actual->objeto);
        actual = actual->siguiente;
    }
}

/**
 * @brief Busca un objeto por su ID.
 *
 * @param inventario Inventario donde buscar.
 * @param id ID del objeto buscado.
 *
 * @return Puntero al objeto dentro de la lista  o NULL
 *         si no existe, el inventario está vacío, es NULL o no esta
 *         inicializado.
 */
Objeto *inventario_buscar_por_id(const Inventario *inventario, int id)
{
    NodoInventario *actual;

    if (inventario == NULL || inventario->cabeza == NULL)
    {
        return NULL;
    }

    // Saltamos el centinela: su id es 0 y no es un objeto real. Si el
    // inventario está vacío, cabeza->siguiente es NULL y el while no entra
    actual = inventario->cabeza->siguiente;
    while (actual != NULL)
    {
        if (actual->objeto.id == id)
        {
            // Devolvemos un puntero al objeto DENTRO del nodo (no una copia).
            // Deja de ser válido si ese objeto se elimina de la lista
            return &actual->objeto;
        }
        actual = actual->siguiente;
    }

    // Se recorrió toda la lista sin encontrar el id
    return NULL;
}

/**
 * @brief Busca un objeto por su nombre
 *
 * @param inventario Inventario donde buscar.
 * @param nombre Nombre exacto del objeto buscado.
 *
 * @return Puntero al objeto dentro de la lista
 */
Objeto *inventario_buscar_por_nombre(const Inventario *inventario, const char *nombre)
{
    NodoInventario *actual;

    // Validamos los dos punteros: el inventario y el texto buscado
    if (inventario == NULL || inventario->cabeza == NULL || nombre == NULL)
    {
        return NULL;
    }

    // Saltamos el centinela
    actual = inventario->cabeza->siguiente;
    while (actual != NULL)
    {
        // strcmp devuelve 0 cuando las dos cadenas son iguales
        if (strcmp(actual->objeto.nombre, nombre) == 0)
        {
            return &actual->objeto;
        }
        actual = actual->siguiente;
    }

    // Se recorrio toda la lista sin encontrar el nombre
    return NULL;
}

/**
 * @brief Elimina el nodo que está justo después de "anterior"
 * 
 * @param inventario Inventario al que pertenece la lista.
 * @param anterior Nodo anterior al que se elimina
 * @param eliminado Si no es NULL, recibe una copia del objeto eliminado.
 */
static void eliminar_siguiente_de(Inventario *inventario, NodoInventario *anterior, Objeto *eliminado)
{
    NodoInventario *a_eliminar;

    // Guardamos el nodo a eliminar ANTES de reconectar; de no ser asi despues de
    // saltarlo no habria forma de hacerle free
    a_eliminar = anterior->siguiente;

    // Copiamos el objeto ahora: despues del free ya no existe
    if (eliminado != NULL)
    {
        *eliminado = a_eliminar->objeto;
    }

    // Reconectamos: el anterior pasa a apuntar al siguiente del eliminado
    anterior->siguiente = a_eliminar->siguiente;

    // Liberamos la memoria del nodo (sin esto habría una fuga de memoria)
    free(a_eliminar);

    inventario->cantidad--;
}

/**
 * @brief Elimina del inventario el objeto con el ID indicado.
 *
 * @param inventario Inventario del que se elimina (ya inicializado).
 * @param id ID del objeto a eliminar.
 * @param eliminado Si no es NULL, recibe una copia del objeto eliminado
 *                  (sirve para registrarlo en el historial y poder deshacer).
 *
 * @return INVENTARIO_OK si se eliminó, INVENTARIO_ERROR_PARAMETRO si el
 *         inventario es NULL o no está inicializado, o
 *         INVENTARIO_ERROR_NO_ENCONTRADO si no existe un objeto con ese ID
 *         (en ese caso el inventario queda sin cambios).
 */
ResultadoInventario inventario_eliminar_por_id(Inventario *inventario, int id, Objeto *eliminado)
{
    NodoInventario *anterior;

    if (inventario == NULL || inventario->cabeza == NULL)
    {
        return INVENTARIO_ERROR_PARAMETRO;
    }

    // Nos paramos en el nodo ANTERIOR al que buscamos
    anterior = inventario->cabeza;
    while (anterior->siguiente != NULL && anterior->siguiente->objeto.id != id)
    {
        anterior = anterior->siguiente;
    }

    // Si anterior->siguiente es NULL, recorrimos toda la lista sin
    // encontrar el id
    if (anterior->siguiente == NULL)
    {
        return INVENTARIO_ERROR_NO_ENCONTRADO;
    }

    eliminar_siguiente_de(inventario, anterior, eliminado);
    return INVENTARIO_OK;
}

/**
 * @brief Libera todos los nodos del inventario, incluido el centinela.
 *
 * @param inventario Inventario a liberar. Si es NULL, no hace nada.
 */
void inventario_liberar(Inventario *inventario)
{
    NodoInventario *actual;
    NodoInventario *siguiente;

    if (inventario == NULL)
    {
        return;
    }

    // Ahora el centinela también se pidió con malloc, así que también hay
    // que liberarlo. Recorremos desde cabeza (el centinela), no desde
    // cabeza->siguiente; si no, el centinela quedaría como fuga de memoria.
    actual = inventario->cabeza;
    while (actual != NULL)
    {
        // Guardamos el siguiente ANTES de hacer free: después de liberar un
        // nodo ya no se puede leer actual->siguiente (memoria inválida)
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    // Dejamos el inventario "sin inicializar". Así llamar a liberar dos veces
    // no hace doble free (el while no entra porque cabeza es NULL) y las demás
    // funciones no usan punteros que ya fueron liberados
    inventario->cabeza = NULL;
    inventario->cantidad = 0;
}

/**
 * @brief Elimina del inventario el primer objeto con el nombre indicado.
 *
 * @param inventario Inventario del que se elimina (ya inicializado).
 * @param nombre Nombre exacto del objeto a eliminar.
 * @param eliminado Si no es NULL, recibe una copia del objeto eliminado.
 *
 * @return INVENTARIO_OK si se elimino, INVENTARIO_ERROR_PARAMETRO si algun
 *         puntero es NULL o el inventario no está inicializado, o
 *         INVENTARIO_ERROR_NO_ENCONTRADO si no existe un objeto con ese
 *         nombre (en ese caso el inventario queda sin cambios).
 */
ResultadoInventario inventario_eliminar_por_nombre(Inventario *inventario, const char *nombre, Objeto *eliminado)
{
    NodoInventario *anterior;

    if (inventario == NULL || inventario->cabeza == NULL || nombre == NULL)
    {
        return INVENTARIO_ERROR_PARAMETRO;
    }

    //strcmp devuelve 0 cuando
    // los nombres son iguales, por eso seguimos avanzando mientras sea != 0
    anterior = inventario->cabeza;
    while (anterior->siguiente != NULL &&
           strcmp(anterior->siguiente->objeto.nombre, nombre) != 0)
    {
        anterior = anterior->siguiente;
    }

    if (anterior->siguiente == NULL)
    {
        return INVENTARIO_ERROR_NO_ENCONTRADO;
    }

    eliminar_siguiente_de(inventario, anterior, eliminado);
    return INVENTARIO_OK;
}