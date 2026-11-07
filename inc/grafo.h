/*
 * Archivo: grafo.h
 * Autor: Pablo Serón
 * Descripción:
 *
 * Mapa de la mazmorra implementado como grafo no dirigido con lista de
 * adyacencia. Cada vértice es una habitación (se identifica solo por su ID;
 * la información completa vive en el ABB/AVL) y cada pasillo es una arista
 * que se guarda en ambos sentidos.
 *
 * La lista de vértices usa nodo centinela, igual que el inventario.
 */

#ifndef GRAFO_H
#define GRAFO_H

/**
 * @brief Nodo de la lista de vecinos de un vértice.
 *
 * id_vecino: ID de la habitación conectada por el pasillo.
 * siguiente: siguiente vecino (NULL si es el último).
 */
typedef struct nodo_vecino
{
    int id_vecino;
    struct nodo_vecino *siguiente;

} nodo_vecino_t;

/**
 * @brief Vértice del grafo: una habitación y sus pasillos.
 *
 * id: ID de la habitación (> 0). El centinela usa id 0.
 * vecinos: lista de vecinos en orden de inserción (agregados al final).
 * cantidad_vecinos: número de pasillos de esta habitación.
 * siguiente: siguiente vértice de la lista (NULL si es el último).
 */
typedef struct vertice
{
    int id;
    nodo_vecino_t *vecinos;
    int cantidad_vecinos;
    struct vertice *siguiente;

} vertice_t;

/**
 * @brief Grafo: lista enlazada de vértices con nodo centinela.
 *
 * cabeza: puntero al nodo CENTINELA. No es una habitación real y siempre
 *         existe (aunque el grafo esté vacío).
 * cantidad: número de vértices reales (el centinela no cuenta).
 */
typedef struct grafo
{
    vertice_t *cabeza;
    int cantidad;

} grafo_t;

/**
 * @brief Códigos de resultado de las operaciones que pueden fallar.
 */
typedef enum
{
    GRAFO_OK = 0,
    GRAFO_ERROR_PARAMETRO,     // puntero NULL, grafo sin inicializar o pasillo de una habitación consigo misma
    GRAFO_ERROR_MEMORIA,       // malloc devolvió NULL
    GRAFO_ERROR_DUPLICADO,     // el vértice o el pasillo ya existe
    GRAFO_ERROR_NO_ENCONTRADO  // el vértice o el pasillo no existe

} resultado_grafo_t;

/**
 * @brief Crea el nodo centinela y deja el grafo vacío, listo para usar.
 *
 * @param grafo Grafo a inicializar.
 *
 * @return GRAFO_OK si se inicializó, GRAFO_ERROR_PARAMETRO si el puntero es
 *         NULL, o GRAFO_ERROR_MEMORIA si malloc falla (en ese caso el grafo
 *         queda sin inicializar).
 */
resultado_grafo_t grafo_inicializar(grafo_t *grafo);

/**
 * @brief Libera todos los vértices, sus listas de vecinos y el centinela.
 *
 * @param grafo Grafo a liberar. Si es NULL, no hace nada.
 */
void grafo_liberar(grafo_t *grafo);

/**
 * @brief Agrega una habitación (vértice) al final del grafo.
 *
 * @param grafo Grafo destino (ya inicializado).
 * @param id ID de la habitación (> 0).
 *
 * @return GRAFO_OK si se agregó, GRAFO_ERROR_PARAMETRO si el grafo es NULL,
 *         no está inicializado o id <= 0, GRAFO_ERROR_DUPLICADO si ya existe
 *         una habitación con ese ID, o GRAFO_ERROR_MEMORIA si malloc falla
 *         (en cualquier error el grafo queda sin cambios).
 */
resultado_grafo_t grafo_agregar_vertice(grafo_t *grafo, int id);

/**
 * @brief Elimina una habitación y todos sus pasillos (en ambos sentidos).
 *
 * @param grafo Grafo del que se elimina (ya inicializado).
 * @param id ID de la habitación a eliminar.
 *
 * @return GRAFO_OK si se eliminó, GRAFO_ERROR_PARAMETRO si el grafo es NULL
 *         o no está inicializado, o GRAFO_ERROR_NO_ENCONTRADO si no existe
 *         esa habitación (en ese caso el grafo queda sin cambios).
 */
resultado_grafo_t grafo_eliminar_vertice(grafo_t *grafo, int id);

/**
 * @brief Indica si existe una habitación con ese ID.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return 1 si existe, 0 si no existe o el grafo es NULL o no está inicializado.
 */
int grafo_existe_vertice(const grafo_t *grafo, int id);

/**
 * @brief Busca un vértice por su ID (solo lectura, útil para los recorridos).
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return Puntero al vértice dentro de la lista (no debe liberarse ni
 *         modificarse), o NULL si no existe, el grafo es NULL o no está
 *         inicializado. Deja de ser válido si se elimina ese vértice.
 */
const vertice_t *grafo_buscar_vertice(const grafo_t *grafo, int id);

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
resultado_grafo_t grafo_agregar_pasillo(grafo_t *grafo, int id_a, int id_b);

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
resultado_grafo_t grafo_eliminar_pasillo(grafo_t *grafo, int id_a, int id_b);

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
int grafo_son_vecinos(const grafo_t *grafo, int id_a, int id_b);

/**
 * @brief Obtiene la cantidad de vecinos (pasillos) de una habitación.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID de la habitación.
 *
 * @return Cantidad de vecinos (puede ser 0), o -1 si la habitación no
 *         existe, el grafo es NULL o no está inicializado.
 */
int grafo_cantidad_vecinos(const grafo_t *grafo, int id);

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
int *grafo_copiar_vecinos(const grafo_t *grafo, int id, int *cantidad);

#endif /* GRAFO_H */