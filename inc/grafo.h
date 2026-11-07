/*
 * Archivo: grafo.h
 * Autor: Pablo Seron
 * Descripcion:
 *
 * Mapa de la mazmorra implementado como grafo no dirigido con lista de
 * adyacencia. Cada vertice es una habitacion (se identifica solo por su ID;
 * la información completa vive en el ABB/AVL) y cada pasillo es una arista
 * que se guarda en ambos sentidos.
 *
 * La lista de vertices usa nodo centinela, igual que el inventario.
 */

#ifndef GRAFO_H
#define GRAFO_H

/**
 * @brief Nodo de la lista de vecinos de un vertice.
 *
 * id_vecino: ID de la habitacion conectada por el pasillo.
 * siguiente: siguiente vecino (NULL si es el ultimo).
 */
typedef struct _NodoVecino
{
    int id_vecino;
    struct _NodoVecino *siguiente;

} NodoVecino;

/**
 * @brief Vertice del grafo: una habitación y sus pasillos.
 *
 * id: ID de la habitación (> 0). El centinela usa id 0.
 * vecinos: lista de vecinos en orden de insercion (agregados al final).
 * cantidad_vecinos: numero de pasillos de esta habitacion.
 * siguiente: siguiente vertice de la lista (NULL si es el ultimo).
 */
typedef struct _Vertice
{
    int id;
    NodoVecino *vecinos;
    int cantidad_vecinos;
    struct _Vertice *siguiente;

} Vertice;

/**
 * @brief Grafo: lista enlazada de vértices con nodo centinela.
 *
 * cabeza: puntero al nodo CENTINELA. No es una habitacion real y siempre
 *         existe (aunque el grafo este vacio).
 * cantidad: numero de vertices reales (el centinela no cuenta).
 */
typedef struct _Grafo
{
    Vertice *cabeza;
    int cantidad;

} Grafo;

/**
 * @brief Codigos de resultado de las operaciones que pueden fallar.
 */
typedef enum
{
    GRAFO_OK = 0,
    GRAFO_ERROR_PARAMETRO,     // puntero NULL, grafo sin inicializar o pasillo de una habitacion consigo misma
    GRAFO_ERROR_MEMORIA,       // malloc devolvio NULL
    GRAFO_ERROR_DUPLICADO,     // el vertice o el pasillo ya existe
    GRAFO_ERROR_NO_ENCONTRADO  // el vertice o el pasillo no existe

} ResultadoGrafo;

/**
 * @brief Crea el nodo centinela y deja el grafo vacio, listo para usar.
 *
 * @param grafo Grafo a inicializar.
 *
 * @return GRAFO_OK si se inicializo, GRAFO_ERROR_PARAMETRO si el puntero es
 *         NULL, o GRAFO_ERROR_MEMORIA si malloc falla (en ese caso el grafo
 *         queda sin inicializar).
 */
ResultadoGrafo grafo_inicializar(Grafo *grafo);

/**
 * @brief Libera todos los vertices, sus listas de vecinos y el centinela.
 *
 * @param grafo Grafo a liberar. Si es NULL, no hace nada.
 */
void grafo_liberar(Grafo *grafo);

/**
 * @brief Agrega una habitacion (vertice) al final del grafo.
 *
 * @param grafo Grafo destino (ya inicializado).
 * @param id ID de la habitación (> 0).
 *
 * @return GRAFO_OK si se agrego, GRAFO_ERROR_PARAMETRO si el grafo es NULL,
 *         no esta inicializado o id <= 0, GRAFO_ERROR_DUPLICADO si ya existe
 *         una habitación con ese ID, o GRAFO_ERROR_MEMORIA si malloc falla
 *         (en cualquier error el grafo queda sin cambios).
 */
ResultadoGrafo grafo_agregar_vertice(Grafo *grafo, int id);

/**
 * @brief Elimina una habitacion y todos sus pasillos (en ambos sentidos).
 *
 * @param grafo Grafo del que se elimina (ya inicializado).
 * @param id ID de la habitacion a eliminar.
 *
 * @return GRAFO_OK si se elimino, GRAFO_ERROR_PARAMETRO si el grafo es NULL
 *         o no esta inicializado, o GRAFO_ERROR_NO_ENCONTRADO si no existe
 *         esa habitacion (en ese caso el grafo queda sin cambios).
 */
ResultadoGrafo grafo_eliminar_vertice(Grafo *grafo, int id);

/**
 * @brief Indica si existe una habitación con ese ID.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return 1 si existe, 0 si no existe o el grafo es NULL o no está inicializado.
 */
int grafo_existe_vertice(const Grafo *grafo, int id);

/**
 * @brief Busca un vertice por su ID (solo lectura, útil para los recorridos).
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return Puntero al vertice dentro de la lista (no debe liberarse ni
 *         modificarse), o NULL si no existe, el grafo es NULL o no esta
 *         inicializado. Deja de ser valido si se elimina ese vertice.
 */
const Vertice *grafo_buscar_vertice(const Grafo *grafo, int id);

/**
 * @brief Agrega un pasillo entre dos habitaciones (en ambos sentidos).
 *        Cada vecino se agrega al final de la lista de la otra habitación.
 *
 * @param grafo Grafo destino (ya inicializado).
 * @param id_a ID de una habitación.
 * @param id_b ID de la otra habitación.
 *
 * @return GRAFO_OK si se agregó, GRAFO_ERROR_PARAMETRO si el grafo es NULL,
 *         no está inicializado o id_a == id_b, GRAFO_ERROR_NO_ENCONTRADO si
 *         alguna habitación no existe, GRAFO_ERROR_DUPLICADO si el pasillo
 *         ya existe, o GRAFO_ERROR_MEMORIA si malloc falla (en cualquier
 *         error el grafo queda sin cambios).
 */
ResultadoGrafo grafo_agregar_pasillo(Grafo *grafo, int id_a, int id_b);

/**
 * @brief Elimina el pasillo entre dos habitaciones (en ambos sentidos).
 *
 * @param grafo Grafo del que se elimina (ya inicializado).
 * @param id_a ID de una habitación.
 * @param id_b ID de la otra habitación.
 *
 * @return GRAFO_OK si se eliminó, GRAFO_ERROR_PARAMETRO si el grafo es NULL
 *         o no está inicializado, o GRAFO_ERROR_NO_ENCONTRADO si alguna
 *         habitación o el pasillo no existen (el grafo queda sin cambios).
 */
ResultadoGrafo grafo_eliminar_pasillo(Grafo *grafo, int id_a, int id_b);

/**
 * @brief Indica si dos habitaciones están conectadas directamente.
 *
 * @param grafo Grafo donde buscar.
 * @param id_a ID de una habitación.
 * @param id_b ID de la otra habitación.
 *
 * @return 1 si están conectadas, 0 si no lo están, si alguna no existe, o
 *         si el grafo es NULL o no está inicializado.
 */
int grafo_son_vecinos(const Grafo *grafo, int id_a, int id_b);

/**
 * @brief Obtiene la cantidad de vecinos (pasillos) de una habitación.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID de la habitación.
 *
 * @return Cantidad de vecinos (puede ser 0), o -1 si la habitación no
 *         existe, el grafo es NULL o no está inicializado.
 */
int grafo_cantidad_vecinos(const Grafo *grafo, int id);

/**
 * @brief Copia los IDs de los vecinos de una habitación en un arreglo nuevo.
 *        Sirve para mostrar las vecinas y para listar los pasillos destruidos
 *        al quemar (hay que copiarlos ANTES de eliminar el vértice).
 *
 * @param grafo Grafo donde buscar.
 * @param id ID de la habitación.
 * @param cantidad Recibe el tamaño del arreglo (0 si retorna NULL).
 *
 * @return Arreglo reservado con malloc en orden de inserción (el llamador
 *         debe hacer free), o NULL si la habitación no existe, no tiene
 *         vecinos, falla malloc, o el grafo o cantidad son NULL.
 */
int *grafo_copiar_vecinos(const Grafo *grafo, int id, int *cantidad);

#endif /* GRAFO_H */