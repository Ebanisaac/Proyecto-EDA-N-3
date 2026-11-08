/*
 * Archivo: usoDe_objeto.h
 * Autor: Pablo Serón
 * Descripción: Uso y equipamiento de objetos del inventario sobre un
 *              personaje.
 */

#ifndef USO_OBJETO_H
#define USO_OBJETO_H

#include "inventario.h"
#include "personaje.h"

/**
 * @brief Codigos de resultado ante el uso de un objeto.
 */
typedef enum
{
    USO_OK = 0,
    USO_ERROR_PARAMETRO,      // puntero NULL
    USO_ERROR_NO_ENCONTRADO,  // el inventario no tiene un objeto con ese ID
    USO_ERROR_NO_USABLE,      // el tipo de objeto no se puede usar así
    USO_ERROR_VIDA_COMPLETA,   // la pocion no se consume: la vida ya esta al maximo
    USO_OK_ENEMIGO_DISTRAIDO  // se uso objeto "otro" (bengala), el enemigo no podra atacar el proximo turno

} ResultadoUso;

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
ResultadoUso usar_objeto(Inventario *inventario, int id, Personaje *personaje, int en_combate);

#endif /* USO_OBJETO_H */