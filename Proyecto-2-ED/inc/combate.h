/**
 * @file combate.h
 * @author your name (you@domain.com)
 * @brief funciones para el modulo de combate
 */
#ifndef COMBATE_H
#define COMBATE_H

#include "cola_enemigo.h"
#include "enemigo.h"
#include "inventario.h"
#include "objeto.h"
#include "personaje.h"
#include "uso_objeto.h"

/**
 * @brief muestra por consola el inventario
 * 
 * @param inventario 
 */
void ver_inventario(const Inventario* inventario);
/**
 * @brief ataque del jugador al enemigo y el uso de objetos en combate
 * 
 * @param enemigo que se encuentra frente en la cola
 * @param personaje 
 * @param cola para la mecanica de la bengala
 * @return int 1 derroto al enemigo, 0 no derroto al enemigo
 */
int ataque_jugador(Enemigo* enemigo, Personaje* personaje, ColaEnemigo cola);
/**
 * @brief ataque del enemigo actual al personaje
 * 
 * @param enemigo puntero al enemigo actual
 * @param personaje 
 */
void ataque_enemigo(Enemigo* enemigo, Personaje* personaje);
/**
 * @brief muestra el estado del combate, vida del personje y enemigo
 * 
 * @param personaje 
 * @param enemigo 
 * @param cola 
 */
void estado_combate(Personaje* personaje, Enemigo* enemigo, ColaEnemigo cola);
/**
 * @brief funcion principal del combate
 * 
 * @param personaje 
 * @param cola 
 * @param inventario 
 */
void combate(Personaje* personaje, ColaEnemigo cola, Inventario* inventario);

#endif