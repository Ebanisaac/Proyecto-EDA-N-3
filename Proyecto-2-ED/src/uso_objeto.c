/*
 * Archivo: uso_objeto.c
 * Autor: Pablo Serón
 * Descripción: Implementación del uso y equipamiento de objetos.
 */

#include <stdio.h>
#include "uso_objeto.h"


/**
 * @brief Consume una pocion: la elimina del inventario y cura al personaje (no pasara su vida maxima)
 *
 * @param inventario Inventario donde esta la pocion.
 * @param personaje Personaje que se cura.
 * @param pocion Pocion a consumir (apunta dentro de la lista).
 *
 * @return USO_OK si se consumio la pocion
 *         USO_ERROR_VIDA_COMPLETA si la vida ya esta al máximo (la poción NO se consume)
 *         o USO_ERROR_NO_ENCONTRADO si la pocion no existe.
 */
static ResultadoUso consumir_pocion(Inventario *inventario, Personaje *personaje, const Objeto *pocion)
{
    Objeto consumida;
    int curacion;

    // Si la vida esta al maximo no gastamos la pocion
    if (personaje->puntos_vida >= personaje->vida_maxima)
    {
        return USO_ERROR_VIDA_COMPLETA;
    }

    // Sacamos la pocion del inventario PRIMERO y nos quedamos con una copia.
    // Después de esta llamada el nodo se libera, así que el puntero "pocion"
    // deja de ser valido: desde aqui solo usamos la copia "consumida"
    if (inventario_eliminar_por_id(inventario, pocion->id, &consumida) != INVENTARIO_OK)
    {
        return USO_ERROR_NO_ENCONTRADO;
    }

    // Curamos solo lo que falta para llegar a la vida maxima (así no nos pasamos)
    curacion = consumida.valor;
    if (curacion > personaje->vida_maxima - personaje->puntos_vida)
    {
        curacion = personaje->vida_maxima - personaje->puntos_vida;
    }
    personaje->puntos_vida += curacion;

    // Si se curo menos que el valor de la pocion, es porque se alcanzo el maximo
    printf("Usas %s: recuperas %d PV%s. %s: %d/%d PV.\n", consumida.nombre,
           curacion, curacion < consumida.valor ? " (máximo alcanzado)" : "",
           personaje->nombre, personaje->puntos_vida, personaje->vida_maxima);

    printf("%s se elimina del inventario.\n", consumida.nombre);
    return USO_OK;
}

/**
 * @brief Uso de la bengala sobre enemigos
 * 
 * @param inventario inventario donde se encuentra la bengala
 * @param objeto tipo de objeto
 * 
 * @return Enemigo distraido o bengala no encontrada 
 */
static ResultadoUso consumir_distraccion(Inventario *inventario, const Objeto *objeto)
{
    Objeto consumido;

    //Al igual que la pocion: Se saca el objeto y nos quedamos con la copia
    //porque despues del free el puntero "objeto" dejara de ser valido
    if(inventario_eliminar_por_id(inventario, objeto->id, &consumido) != INVENTARIO_OK)
    {
        return USO_ERROR_NO_ENCONTRADO;
    }

    printf("Usas %s: el enemigo se distrae y perdera su proximo ataque\n", consumido.nombre);
    return USO_OK_ENEMIGO_DISTRAIDO;
}

/**
 * @brief Uso o equipamiento de un objeto del inventario sobre el personaje.
 *        Arma: reemplaza el bono de ataque.
 *        Armadura: reemplaza el bono de defensa. 
 *        En ambos casos el objeto permanece en el inventario.
 *        Pocion: cura al personaje hasta su vida maxima y se elimina del
 *        inventario.
 *
 * @param inventario Inventario donde está el objeto.
 * @param id ID del objeto a usar.
 * @param personaje Personaje sobre el que se aplica el efecto.
 *
 * @return USO_OK si se aplico, USO_ERROR_PARAMETRO si algun puntero es NULL,
 *         USO_ERROR_NO_ENCONTRADO si no existe un objeto con ese ID,
 *         USO_ERROR_NO_USABLE si el tipo no se puede usar, o
 *         USO_ERROR_VIDA_COMPLETA si es una pocion y el personaje ya tiene
 *         la vida al maximo
 */
ResultadoUso usar_objeto(Inventario *inventario, int id, Personaje *personaje, int en_combate)
{
    Objeto *objeto;
    int ataque_antes;
    int defensa_antes;


    if (inventario == NULL || personaje == NULL)
    {
        return USO_ERROR_PARAMETRO;
    }

    // Buscamos el objeto con la funcion del inventario_buscar_por_id 
    //Nos devuelve un puntero de la lista
    objeto = inventario_buscar_por_id(inventario, id);
    if (objeto == NULL)
    {
        return USO_ERROR_NO_ENCONTRADO;
    }

    //Elegimos quq hacer segun la categoria del mismo objeto
    switch (objeto->tipo)
    {
        case TIPO_ARMA:
            // Guardamos el ataque total ANTES de cambiar el bono para
            // poder mostrar " el antes y despues"
            ataque_antes = personaje->ataque + personaje->bono_ataque;

            // Asignamos (=) no sumamos (+=); así equipar la misma arma dos
            // veces, o cambiar de arma nunca acumulara los bonos
            personaje->bono_ataque = objeto->valor;

            printf("Equipas %s. Ataque: %d -> %d.\n", objeto->nombre,
                   ataque_antes, personaje->ataque + personaje->bono_ataque);
            return USO_OK;

        case TIPO_ARMADURA:
            defensa_antes = personaje->defensa + personaje->bono_defensa;
            personaje->bono_defensa = objeto->valor;

            printf("Equipas %s. Defensa: %d -> %d.\n", objeto->nombre,
                   defensa_antes, personaje->defensa + personaje->bono_defensa);
            return USO_OK;
        
        case TIPO_POCION:
            return consumir_pocion(inventario, personaje, objeto);

        case TIPO_OTRO:
            //la bengala
            if(!en_combate) return USO_ERROR_NO_USABLE;
            return consumir_distraccion(inventario, objeto);
        default:
            // Otros tipos de objeto no se pueden usar sobre el personaje
            return USO_ERROR_NO_USABLE;
    }
}