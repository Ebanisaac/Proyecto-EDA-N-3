/*
 * Archivo: grafo.c
 * Autor: Pablo Seron
 * Descripcion:
 *
 * Implementacion del grafo no dirigido (lista de adyacencia) que representa
 * el mapa de la mazmorra. La lista de vertices usa nodo centinela, igual que
 * el inventario. Cada pasillo se guarda en ambos sentidos (A->B y B->A).
 */

#include <stdlib.h>
#include "grafo.h"


/**
 * @brief Comprueba que el grafo exista y tenga su centinela.
 *
 * @param grafo Grafo a revisar.
 *
 * @return 1 si esta listo para usarse, 0 si es NULL o no está inicializado.
 */
static int grafo_listo(const grafo_t *grafo)
{
    return grafo != NULL && grafo->cabeza != NULL;
}

/**
 * @brief Busca un vertice y lo devuelve modificable (uso interno).
 *
 * @param grafo Grafo ya inicializado.
 * @param id ID buscado.
 *
 * @return Puntero al vertice, o NULL si no existe.
 */
static vertice_t *buscar_vertice_interno(const grafo_t *grafo, int id)
{
    vertice_t *actual;

    // Saltamos el centinela: su id es 0 y no es una habitación real
    actual = grafo->cabeza->siguiente;
    while (actual != NULL)
    {
        if (actual->id == id)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

/**
 * @brief Indica si un vertice ya tiene a "id_vecino" en su lista de vecinos.
 *
 * @param vertice Vertice donde buscar (no debe ser NULL).
 * @param id_vecino ID del vecino buscado.
 *
 * @return 1 si está en la lista, 0 si no.
 */
static int tiene_vecino(const vertice_t *vertice, int id_vecino)
{
    const nodo_vecino_t *actual;

    actual = vertice->vecinos;
    while (actual != NULL)
    {
        if (actual->id_vecino == id_vecino)
        {
            return 1;
        }
        actual = actual->siguiente;
    }
    return 0;
}

/**
 * @brief Reserva un nodo de vecino ya inicializado (todavia sin enlazar).
 *
 * @param id_vecino ID del vecino que guardara el nodo.
 *
 * @return Puntero al nodo, o NULL si malloc falla.
 */
static nodo_vecino_t *crear_nodo_vecino(int id_vecino)
{
    nodo_vecino_t *nuevo;

    nuevo = malloc(sizeof(nodo_vecino_t));
    if (nuevo == NULL)
    {
        return NULL;
    }
    nuevo->id_vecino = id_vecino;
    nuevo->siguiente = NULL;
    return nuevo;
}

/**
 * @brief Enlaza un nodo de vecino AL FINAL de la lista de un vertice.
 *        Se agrega al final para conservar el orden de insercion, así los
 *        recorridos (BFS/DFS) dan siempre el mismo resultado.
 *
 * @param vertice Vertice destino (no debe ser NULL).
 * @param nodo Nodo ya reservado (no debe ser NULL).
 */
static void enlazar_vecino_al_final(vertice_t *vertice, nodo_vecino_t *nodo)
{
    nodo_vecino_t *ultimo;

    if (vertice->vecinos == NULL)
    {
        vertice->vecinos = nodo;
    }
    else
    {
        ultimo = vertice->vecinos;
        while (ultimo->siguiente != NULL)
        {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = nodo;
    }
    vertice->cantidad_vecinos++;
}

/**
 * @brief Quita a "id_vecino" de la lista de vecinos de un vertice y libera
 *        su nodo.
 *
 * @param vertice Vertice del que se quita (no debe ser NULL).
 * @param id_vecino ID del vecino a quitar.
 *
 * @return 1 si se quito, 0 si no estaba en la lista.
 */
static int quitar_vecino(vertice_t *vertice, int id_vecino)
{
    nodo_vecino_t *anterior;
    nodo_vecino_t *actual;

    anterior = NULL;
    actual = vertice->vecinos;
    while (actual != NULL && actual->id_vecino != id_vecino)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL)
    {
        return 0;
    }

    // Reconectamos ANTES de liberar el nodo
    if (anterior == NULL)
    {
        vertice->vecinos = actual->siguiente;
    }
    else
    {
        anterior->siguiente = actual->siguiente;
    }
    free(actual);
    vertice->cantidad_vecinos--;
    return 1;
}

/**
 * @brief Libera toda la lista de vecinos de un vertice.
 *
 * @param vertice Vertice cuya lista se libera (no debe ser NULL).
 */
static void liberar_vecinos(vertice_t *vertice)
{
    nodo_vecino_t *actual;
    nodo_vecino_t *siguiente;

    actual = vertice->vecinos;
    while (actual != NULL)
    {
        // Guardamos el siguiente ANTES del free
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    vertice->vecinos = NULL;
    vertice->cantidad_vecinos = 0;
}


/**
 * @brief Crea el nodo centinela y deja el grafo vacio, listo para usar.
 *
 * @param grafo Grafo a inicializar.
 *
 * @return GRAFO_OK si se inicializo, GRAFO_ERROR_PARAMETRO si el puntero es
 *         NULL, o GRAFO_ERROR_MEMORIA si malloc falla (en ese caso el grafo
 *         queda sin inicializar).
 */
resultado_grafo_t grafo_inicializar(grafo_t *grafo)
{
    vertice_t *centinela;

    if (grafo == NULL)
    {
        return GRAFO_ERROR_PARAMETRO;
    }

    centinela = malloc(sizeof(vertice_t));
    if (centinela == NULL)
    {
        grafo->cabeza = NULL;
        grafo->cantidad = 0;
        return GRAFO_ERROR_MEMORIA;
    }

    // El centinela no es una habitacion real: id 0 es invalido
    centinela->id = 0;
    centinela->vecinos = NULL;
    centinela->cantidad_vecinos = 0;
    centinela->siguiente = NULL;

    grafo->cabeza = centinela;
    grafo->cantidad = 0;
    return GRAFO_OK;
}

/**
 * @brief Libera todos los vertices, sus listas de vecinos y el centinela.
 *
 * @param grafo Grafo a liberar. Si es NULL, no hace nada.
 */
void grafo_liberar(grafo_t *grafo)
{
    vertice_t *actual;
    vertice_t *siguiente;

    if (grafo == NULL)
    {
        return;
    }

    // Recorremos desde el centinela para liberarlo tambien
    actual = grafo->cabeza;
    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        liberar_vecinos(actual);
        free(actual);
        actual = siguiente;
    }

    // Dejarlo "sin inicializar" evita un doble free si se llama dos veces
    grafo->cabeza = NULL;
    grafo->cantidad = 0;
}

/**
 * @brief Agrega una habitación (vertice) al final del grafo.
 *
 * @param grafo Grafo destino (ya inicializado).
 * @param id ID de la habitación (> 0).
 *
 * @return GRAFO_OK si se agrego, GRAFO_ERROR_PARAMETRO si el grafo es NULL,
 *         no esta inicializado o id <= 0, GRAFO_ERROR_DUPLICADO si ya existe
 *         una habitacion con ese ID, o GRAFO_ERROR_MEMORIA si malloc falla
 *         (en cualquier error el grafo queda sin cambios).
 */
resultado_grafo_t grafo_agregar_vertice(grafo_t *grafo, int id)
{
    vertice_t *nuevo;
    vertice_t *ultimo;

    if (!grafo_listo(grafo) || id <= 0)
    {
        return GRAFO_ERROR_PARAMETRO;
    }

    // Revisamos el duplicado ANTES del malloc
    if (buscar_vertice_interno(grafo, id) != NULL)
    {
        return GRAFO_ERROR_DUPLICADO;
    }

    nuevo = malloc(sizeof(vertice_t));
    if (nuevo == NULL)
    {
        return GRAFO_ERROR_MEMORIA;
    }
    nuevo->id = id;
    nuevo->vecinos = NULL;
    nuevo->cantidad_vecinos = 0;
    nuevo->siguiente = NULL;

    // Partimos desde el centinela hasta el último vertice
    ultimo = grafo->cabeza;
    while (ultimo->siguiente != NULL)
    {
        ultimo = ultimo->siguiente;
    }
    ultimo->siguiente = nuevo;

    grafo->cantidad++;
    return GRAFO_OK;
}

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
resultado_grafo_t grafo_eliminar_vertice(grafo_t *grafo, int id)
{
    vertice_t *anterior;
    vertice_t *eliminado;
    vertice_t *vecino;
    nodo_vecino_t *actual;

    if (!grafo_listo(grafo))
    {
        return GRAFO_ERROR_PARAMETRO;
    }

    // Nos paramos en el vertice ANTERIOR al que se elimina
    anterior = grafo->cabeza;
    while (anterior->siguiente != NULL && anterior->siguiente->id != id)
    {
        anterior = anterior->siguiente;
    }
    if (anterior->siguiente == NULL)
    {
        return GRAFO_ERROR_NO_ENCONTRADO;
    }
    eliminado = anterior->siguiente;

    // Quitamos el pasillo en el lado de cada vecino (B->A) antes de liberar
    actual = eliminado->vecinos;
    while (actual != NULL)
    {
        vecino = buscar_vertice_interno(grafo, actual->id_vecino);
        if (vecino != NULL)
        {
            quitar_vecino(vecino, id);
        }
        actual = actual->siguiente;
    }

    // Reconectamos la lista de vertices y liberamos
    anterior->siguiente = eliminado->siguiente;
    liberar_vecinos(eliminado);
    free(eliminado);
    grafo->cantidad--;
    return GRAFO_OK;
}

/**
 * @brief Indica si existe una habitacion con ese ID.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return 1 si existe, 0 si no existe o el grafo es NULL o no esta inicializado.
 */
int grafo_existe_vertice(const grafo_t *grafo, int id)
{
    return grafo_buscar_vertice(grafo, id) != NULL;
}

/**
 * @brief Busca un vertice por su ID (solo lectura, util para los recorridos).
 *
 * @param grafo Grafo donde buscar.
 * @param id ID buscado.
 *
 * @return Puntero al vertice dentro de la lista (no debe liberarse ni
 *         modificarse), o NULL si no existe, el grafo es NULL o no esta
 *         inicializado. Deja de ser valido si se elimina ese vertice.
 */
const vertice_t *grafo_buscar_vertice(const grafo_t *grafo, int id)
{
    if (!grafo_listo(grafo))
    {
        return NULL;
    }
    return buscar_vertice_interno(grafo, id);
}

/**
 * @brief Agrega un pasillo entre dos habitaciones (en ambos sentidos).
 *        Cada vecino se agrega al final de la lista de la otra habitacion.
 *
 * @param grafo Grafo destino (ya inicializado).
 * @param id_a ID de una habitacion.
 * @param id_b ID de la otra habitacion.
 *
 * @return GRAFO_OK si se agrego, GRAFO_ERROR_PARAMETRO si el grafo es NULL,
 *         no está inicializado o id_a == id_b, GRAFO_ERROR_NO_ENCONTRADO si
 *         alguna habitacion no existe, GRAFO_ERROR_DUPLICADO si el pasillo
 *         ya existe, o GRAFO_ERROR_MEMORIA si malloc falla (en cualquier
 *         error el grafo queda sin cambios).
 */
resultado_grafo_t grafo_agregar_pasillo(grafo_t *grafo, int id_a, int id_b)
{
    vertice_t *vertice_a;
    vertice_t *vertice_b;
    nodo_vecino_t *nodo_en_a;
    nodo_vecino_t *nodo_en_b;

    if (!grafo_listo(grafo) || id_a == id_b)
    {
        return GRAFO_ERROR_PARAMETRO;
    }

    vertice_a = buscar_vertice_interno(grafo, id_a);
    vertice_b = buscar_vertice_interno(grafo, id_b);
    if (vertice_a == NULL || vertice_b == NULL)
    {
        return GRAFO_ERROR_NO_ENCONTRADO;
    }

    // Un pasillo declarado en la linea de ambas habitaciones se agrega una vez
    if (tiene_vecino(vertice_a, id_b))
    {
        return GRAFO_ERROR_DUPLICADO;
    }

    // Reservamos los 2 nodos antes de tocar las listas: si el segundo
    // malloc falla, liberamos el primero y el grafo queda sin cambios
    nodo_en_a = crear_nodo_vecino(id_b);
    if (nodo_en_a == NULL)
    {
        return GRAFO_ERROR_MEMORIA;
    }
    nodo_en_b = crear_nodo_vecino(id_a);
    if (nodo_en_b == NULL)
    {
        free(nodo_en_a);
        return GRAFO_ERROR_MEMORIA;
    }

    enlazar_vecino_al_final(vertice_a, nodo_en_a);
    enlazar_vecino_al_final(vertice_b, nodo_en_b);
    return GRAFO_OK;
}

/**
 * @brief Elimina el pasillo entre dos habitaciones (en ambos sentidos).
 *
 * @param grafo Grafo del que se elimina (ya inicializado).
 * @param id_a ID de una habitacion.
 * @param id_b ID de la otra habitacion.
 *
 * @return GRAFO_OK si se elimino, GRAFO_ERROR_PARAMETRO si el grafo es NULL
 *         o no esta inicializado, o GRAFO_ERROR_NO_ENCONTRADO si alguna
 *         habitacion o el pasillo no existen (el grafo queda sin cambios).
 */
resultado_grafo_t grafo_eliminar_pasillo(grafo_t *grafo, int id_a, int id_b)
{
    vertice_t *vertice_a;
    vertice_t *vertice_b;

    if (!grafo_listo(grafo))
    {
        return GRAFO_ERROR_PARAMETRO;
    }

    vertice_a = buscar_vertice_interno(grafo, id_a);
    vertice_b = buscar_vertice_interno(grafo, id_b);
    if (vertice_a == NULL || vertice_b == NULL)
    {
        return GRAFO_ERROR_NO_ENCONTRADO;
    }

    // Revisamos que el pasillo exista ANTES de quitar nada
    if (!tiene_vecino(vertice_a, id_b))
    {
        return GRAFO_ERROR_NO_ENCONTRADO;
    }

    quitar_vecino(vertice_a, id_b);
    quitar_vecino(vertice_b, id_a);
    return GRAFO_OK;
}

/**
 * @brief Indica si dos habitaciones están conectadas directamente.
 *
 * @param grafo Grafo donde buscar.
 * @param id_a ID de una habitación.
 * @param id_b ID de la otra habitación.
 *
 * @return 1 si están conectadas, 0 si no lo estan, si alguna no existe, o
 *         si el grafo es NULL o no está inicializado.
 */
int grafo_son_vecinos(const grafo_t *grafo, int id_a, int id_b)
{
    const vertice_t *vertice_a;

    vertice_a = grafo_buscar_vertice(grafo, id_a);
    if (vertice_a == NULL || !grafo_existe_vertice(grafo, id_b))
    {
        return 0;
    }
    return tiene_vecino(vertice_a, id_b);
}


/**
 * @brief Obtiene la cantidad de vecinos (pasillos) de una habitación.
 *
 * @param grafo Grafo donde buscar.
 * @param id ID de la habitación.
 *
 * @return Cantidad de vecinos (puede ser 0), o -1 si la habitación no
 *         existe, el grafo es NULL o no está inicializado.
 */
int grafo_cantidad_vecinos(const grafo_t *grafo, int id)
{
    const vertice_t *vertice;

    vertice = grafo_buscar_vertice(grafo, id);
    if (vertice == NULL)
    {
        return -1;
    }
    return vertice->cantidad_vecinos;
}

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
int *grafo_copiar_vecinos(const grafo_t *grafo, int id, int *cantidad)
{
    const vertice_t *vertice;
    const nodo_vecino_t *actual;
    int *copia;
    int i;

    if (cantidad == NULL)
    {
        return NULL;
    }
    *cantidad = 0;

    vertice = grafo_buscar_vertice(grafo, id);
    if (vertice == NULL || vertice->cantidad_vecinos == 0)
    {
        return NULL;
    }

    copia = malloc(sizeof(int) * (size_t)vertice->cantidad_vecinos);
    if (copia == NULL)
    {
        return NULL;
    }

    i = 0;
    actual = vertice->vecinos;
    while (actual != NULL)
    {
        copia[i] = actual->id_vecino;
        i++;
        actual = actual->siguiente;
    }

    *cantidad = vertice->cantidad_vecinos;
    return copia;
}