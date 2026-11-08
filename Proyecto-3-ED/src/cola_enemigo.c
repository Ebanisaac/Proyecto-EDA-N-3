/**
 * @file cola_enemigo.c
 * @author Daniela Soto
 * @brief implementacion de cola_enemigo
 */
#include "cola_enemigo.h"

/**
 * @brief crea e inicializa una cola vacia 
 * 
 * @return ColaEnemigo. (puntero a la cola creada)
 */
ColaEnemigo cola_enemigo_create(){
    ColaEnemigo q = (ColaEnemigo)malloc(sizeof(struct _ColaEnemigo));
    if (q == NULL){
        printf("Error: falta de memoria (cola enemigo) \n");
        return NULL;
    }

    q->front = NULL;
    q->rear = NULL;
    q->tamanio = 0;

    return q;
}

/**
 * @brief vacia todos los elementos de la cola, liberando memoria
 * 
 * @param q (puntero a la cola que desea vaciar)
 * @return ColaEnemigo (la cola vacia)
 */
ColaEnemigo cola_enemigo_make_empty(ColaEnemigo q){
    if (q != NULL){
        NodoEnemigo* actual = q->front;
        while (actual != NULL){
            NodoEnemigo* siguiente_nodo = actual->siguiente;
            free(actual);
            actual = siguiente_nodo;
        }
        
        q->front = NULL;
        q->rear = NULL;
        q->tamanio = 0;
    }

    return q;
}

/**
 * @brief comprueba si la cola esta vacia
 * 
 * @param q (puntero de la cola)
 * @return int 1 si esta vacia, 0 si no lo esta
 */
int cola_enemigo_is_empty(ColaEnemigo q){
    if (q == NULL || q->front == NULL){
        return 1; //verdadero
    } else {
        return 0; //falso
    }
}

/**
 * @brief agrega un enemigo a la cola
 * 
 * @param e estructura de enemigo a agregar
 * @param q (puntero de la cola)
 * @return int 1 si la insercion se logro, 0 para no
 */
int cola_enemigo_enqueue(Enemigo e, ColaEnemigo q){
    if (q == NULL){
        printf("No existe la cola (Enemigo)\n");
        return 0;
    }

    NodoEnemigo* nuevo_nodo = (NodoEnemigo*)malloc(sizeof(NodoEnemigo));
    if (nuevo_nodo == NULL){
        printf("Error: falta de memoria (nuevo nodo enemigo) \n");
        return 0;
    }
    
    nuevo_nodo->enemigo = e;
    nuevo_nodo->siguiente = NULL;

    if (cola_enemigo_is_empty(q)){
        q->front = nuevo_nodo;
    }else{
        q->rear->siguiente = nuevo_nodo;
    }
    q->rear = nuevo_nodo;
    q->tamanio++;

    return 1;
}

/**
 * @brief quita y retorna al enemigo de la cola
 * 
 * @param q (puntero de la cola)
 * @return Enemigo 
 */
Enemigo cola_enemigo_dequeue(ColaEnemigo q){
    if (cola_enemigo_is_empty(q)){
        printf("La cola enemigo esta vacia \n");
        return (Enemigo){.id =-1, "Desconocido", .pv = 0, .ataque = 0, .vida_max = 0}; //devuelve enemigo de relleno con datos de error
    }
    
    NodoEnemigo* nodo_eliminar = q->front;
    Enemigo e = nodo_eliminar->enemigo;

    q->front = q->front->siguiente;

    if (q->front == NULL){
        q->rear = NULL;
    }
    
    free(nodo_eliminar);
    q->tamanio--;

    return e;
}

/**
 * @brief obtiene un puntero del enemigo que esta al frente de la cola que permite modificar sus datos
 * 
 * @param q (puntero de la cola)
 * @return Enemigo* puntero al enemigo
 */
Enemigo* cola_enemigo_front(ColaEnemigo q){
    if (cola_enemigo_is_empty(q)){
        printf("La cola de enemigo esta vacia (front) \n");
        return NULL;
    }
    return &(q->front->enemigo);
}

/**
 * @brief saber el tamanio actual de la cola
 * 
 * @param q (puntero de la cola)
 * @return int. cantidad de elementos en la cola
 */
int cola_enemigo_size(ColaEnemigo q){
    if (q == NULL){
        return 0;
    }

    return q->tamanio;
}

/**
 * @brief destruye la cola por completo y libera su memoria
 * 
 * @param q (puntero de la cola)
 */
void cola_enemigo_delete(ColaEnemigo q){
    if (q != NULL){
        cola_enemigo_make_empty(q);
        free(q);
    }
}