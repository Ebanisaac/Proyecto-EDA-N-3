/**
 * @file cola_enemigo.h
 * @author Daniela Soto
 * @brief Definicion de la estructura para la Cola de enemigos
 */
#ifndef COLA_ENEMIGO_H
#define COLA_ENEMIGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemigo.h"

/**
 * @brief estructura para un nodo dentro de la cola enemigos
 * 
 */
typedef struct _NodoEnemigo{
    Enemigo enemigo; //datos del enemigo almacenado en el nodo
    struct _NodoEnemigo* siguiente; //puntero al siguiente nodo de la cola
} NodoEnemigo;

typedef struct _ColaEnemigo* ColaEnemigo;

/**
 * @brief estructura de la cola enemigos
 * 
 */
struct _ColaEnemigo{
    NodoEnemigo* front; //primer elemento de la cola
    NodoEnemigo* rear; //ultimo elemento de la cola
    int tamanio; // tamanio de la cola actual
};

/**
 * @brief crea e inicializa una cola vacia 
 * 
 * @return ColaEnemigo. (puntero a la cola creada)
 */
ColaEnemigo cola_enemigo_create();
/**
 * @brief vacia todos los elementos de la cola, liberando memoria
 * 
 * @param q (puntero a la cola que desea vaciar)
 * @return ColaEnemigo (la cola vacia)
 */
ColaEnemigo cola_enemigo_make_empty(ColaEnemigo q);
/**
 * @brief comprueba si la cola esta vacia
 * 
 * @param q (puntero de la cola)
 * @return int 1 si esta vacia, 0 si no lo esta
 */
int cola_enemigo_is_empty(ColaEnemigo q);
/**
 * @brief agrega un enemigo a la cola
 * 
 * @param e estructura de enemigo a agregar
 * @param q (puntero de la cola)
 * @return int 1 si la insercion se logro, 0 para no
 */
int cola_enemigo_enqueue(Enemigo e, ColaEnemigo q);
/**
 * @brief quita y retorna al enemigo de la cola
 * 
 * @param q (puntero de la cola)
 * @return Enemigo 
 */
Enemigo cola_enemigo_dequeue(ColaEnemigo q);
/**
 * @brief obtiene un puntero del enemigo que esta al frente de la cola que permite modificar sus datos
 * 
 * @param q (puntero de la cola)
 * @return Enemigo* puntero al enemigo
 */
Enemigo* cola_enemigo_front(ColaEnemigo q);
/**
 * @brief saber el tamanio actual de la cola
 * 
 * @param q (puntero de la cola)
 * @return int. cantidad de elementos en la cola
 */
int cola_enemigo_size(ColaEnemigo q);
/**
 * @brief destruye la cola por completo y libera su memoria
 * 
 * @param q (puntero de la cola)
 */
void cola_enemigo_delete(ColaEnemigo q);

#endif