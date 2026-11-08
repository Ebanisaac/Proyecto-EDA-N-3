/**
 * @file aventura.h
 * @author Daniela Soto
 * @brief creacion de las zonas
 */
#ifndef AVENTURA_H
#define AVENTURA_H

#include "inventario.h"
#include "personaje.h"
#include "objeto.h"
#include "enemigo.h"
#include "combate.h"
#include "cola_enemigo.h"

/**
 * @brief inicializa y gestiona los eventos de la zona catacumba
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_catacumba(Inventario* inventario, Personaje* personaje);
/**
 * @brief inicializa y gestiona los eventos de la zona abismo
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_abismo(Inventario* inventario, Personaje* personaje);
/**
 * @brief inicializa y gestiona los eventos de la zona paramo
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_paramo(Inventario* inventario, Personaje* personaje);
/**
 * @brief imprime por consola para seleccionar que zona jugar
 * 
 * @param inventario 
 * @param personaje 
 */
void iniciar_zona(Inventario* inventario, Personaje* personaje);
/**
 * @brief funcion para recoger un objeto
 * 
 * @param inventario 
 * @param obj 
 * @param nombre_objeto 
 */
void recoger_objeto(Inventario* inventario, Objeto* obj, const char* nombre_objeto);

#endif