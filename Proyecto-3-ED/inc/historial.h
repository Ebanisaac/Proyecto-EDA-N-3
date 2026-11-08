//Autor: Carlos Cofré
 

#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "objeto.h"

#define LARGO_DESC_ACCION 100

/**
 * @brief Tipos de acciones que se pueden registrar en el historial.
 */
typedef enum {
    ACCION_COMBATE,
    ACCION_EXPLORACION,
    ACCION_DESCARTAR_OBJETO,
    ACCION_OTRO,
    ACCION_VACIA,
    ACCION_ATACAR,
    ACCION_USAR_OBJETO
} TipoAccion;

/**
 * @brief Estructura que representa una acción realizada en el juego.
 */
typedef struct _accion {
    TipoAccion tipo;
    char descripcion[LARGO_DESC_ACCION];
    Objeto objeto_involucrado;
} Accion;

/**
 * @brief Nodo para la pila del historial.
 */
typedef struct _nodo_historial {
    Accion accion;
    struct _nodo_historial *siguiente;
} NodoHistorial;

/**
 * @brief Estructura principal de la pila de historial.
 */
typedef struct _pila_historial {
    NodoHistorial *cima;
} PilaHistorial;

/**
 * @brief Crea e inicializa una pila vacía para el historial.
 * 
 * @return Puntero a la nueva PilaHistorial o NULL si falla la asignación de memoria.
 */
PilaHistorial *historial_crear(void);

/**
 * @brief Registra una nueva acción en la cima de la pila (Operación Push).
 * 
 * @param historial Pila donde se guardará la acción.
 * @param tipo Categoría de la acción realizada.
 * @param descripcion Texto descriptivo del evento.
 * @param obj Puntero al objeto involucrado (NULL si no hay objeto involucrado).
 */
void historial_registrar(PilaHistorial *historial, TipoAccion tipo, const char *descripcion, const Objeto *obj);

/**
 * @brief Muestra todas las acciones desde la más reciente a la más antigua.
 * 
 * @param historial Pila del historial a listar.
 */
void historial_mostrar(const PilaHistorial *historial);

/**
 * @brief Remueve y devuelve la última acción realizada (Operación Pop).
 * 
 * @param historial Pila de donde se extrae la acción.
 * @return La estructura Accion extraída o una acción de tipo ACCION_VACIA si la pila está vacía.
 */
Accion historial_desapilar(PilaHistorial *historial);

/**
 * @brief Libera la memoria de todos los nodos de la pila y de la estructura principal.
 * 
 * @param historial Pila a liberar.
 */
void historial_liberar(PilaHistorial *historial);

/**
 * @brief Verifica si la pila del historial está vacía.
 * 
 * @param h Puntero constante a la pila del historial.
 * @return 1 si el historial está vacío o es nulo, 0 si contiene elementos.
 */
int historial_esta_vacio(const PilaHistorial *h);

/**
 * @brief Obtiene la acción en la cima del historial sin extraerla (Operación Peek).
 * 
 * @param h Puntero constante a la pila del historial.
 * @return Puntero constante a la estructura Accion en la cima, o NULL si está vacío.
 */
const Accion *historial_cima(const PilaHistorial *h);

#endif /* HISTORIAL_H */