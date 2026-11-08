/*
 * Archivo: inventario.h
 * Autor: Pablo Serón
 * Descripción:
 * 
 * Inventario del personaje implementado como lista enlazada
 * simple con nodo centinela. Cada nodo guarda una copia del
 * Objeto.
 */

#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "objeto.h"

/**
 * @brief Nodo de la lista enlazada del inventario.
 *
 * objeto: copia del objeto almacenado.
 * siguiente: puntero al siguiente nodo (NULL si es el ultimo).
 */
typedef struct _NodoInventario
{
    Objeto objeto;
    struct _NodoInventario *siguiente;

} NodoInventario;

/**
 * @brief Inventario: lista enlazada simple con nodo centinela.
 *
 * cabeza: puntero al nodo CENTINELA. El centinela es un nodo "falso" que
 *         no guarda un objeto real este siempre existe (aunque el inventario
 *         esté vacío)
 * 
 * cantidad: número de objetos reales almacenados (el centinela no cuenta).
 */
typedef struct _Inventario
{
    NodoInventario *cabeza;
    int cantidad;

} Inventario;

/**
 * @brief Codigos de resultado de las operaciones que pueden fallar
 */
typedef enum
{
    INVENTARIO_OK = 0,
    INVENTARIO_ERROR_PARAMETRO,     // puntero NULL o inventario sin inicializar
    INVENTARIO_ERROR_MEMORIA,       // malloc devolvió NULL
    INVENTARIO_ERROR_ID_DUPLICADO,  // ya existe un objeto con ese ID
    INVENTARIO_ERROR_NO_ENCONTRADO  // el objeto buscado no existe

} ResultadoInventario;

/**
 * @brief Crea el nodo centinela y deja el inventario vacio, listo para usar.
 *
 * @param inventario Inventario a inicializar.
 *
 * @return INVENTARIO_OK si se inicializó, INVENTARIO_ERROR_PARAMETRO si el
 *         puntero es NULL, o INVENTARIO_ERROR_MEMORIA si malloc falla (en
 *         ese caso el inventario queda sin inicializar).
 */
ResultadoInventario inventario_inicializar(Inventario *inventario);

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
ResultadoInventario inventario_agregar(Inventario *inventario, const Objeto *objeto);

/**
 * @brief Imprime por terminal todos los objetos del inventario.
 *        Si está vacío, muestra un mensaje indicándolo.
 *
 * @param inventario Inventario a mostrar. Si es NULL o no está
 *                   inicializado, se trata como vacío.
 */
void inventario_listar(const Inventario *inventario);

/**
 * @brief Busca un objeto por su ID.
 *
 * @param inventario Inventario donde buscar.
 * @param id ID del objeto buscado.
 *
 * @return Puntero al objeto dentro de la lista (no debe liberarse), o NULL
 *         si no existe, el inventario está vacío, es NULL o no está
 *         inicializado.
 */
Objeto *inventario_buscar_por_id(const Inventario *inventario, int id);

/**
 * @brief Libera todos los nodos del inventario, incluido el centinela.
 *
 * @param inventario Inventario a liberar. Si es NULL, no hace nada.
 */
void inventario_liberar(Inventario *inventario);

/**
 * @brief Busca un objeto por su nombre
 *
 * @param inventario Inventario donde buscar.
 * @param nombre Nombre exacto del objeto buscado.
 *
 * @return Puntero al objeto dentro de la lista
 */
Objeto *inventario_buscar_por_nombre(const Inventario *inventario, const char *nombre);

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
ResultadoInventario inventario_eliminar_por_id(Inventario *inventario, int id, Objeto *eliminado);

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
ResultadoInventario inventario_eliminar_por_nombre(Inventario *inventario, const char *nombre, Objeto *eliminado);

#endif /* INVENTARIO_H */