/**
 * @file enemigo.h
 * @author Daniela Soto
 * @brief estructura de enemigo y la creacion de los enemigos
 */
#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Estructura de enemigo
 * id: identificador
 * nombre[30]: nombre del enemigo
 * pv: puntos de vida del enemigo
 * ataque: ataque del enemigo
 * vida_max: vida maxima del enemigo
 * 
 */
typedef struct _Enemigo{
    int id;
    char nombre[30];
    int pv;
    int ataque;
    int vida_max;
} Enemigo;

/**
 * @brief crea e inicializa a enemigo tipo 1(furtivo)
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo1(int id, const char* nombre, int pv, int ataque, int vida_max);
/**
 * @brief crea e inicializa enemigo tipo 2 (mastin)
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo2(int id, const char* nombre, int pv, int ataque, int vida_max);
/**
 * @brief crea e inicializa enemigo tipo 3 (vandalo)
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo3(int id, const char* nombre, int pv, int ataque, int vida_max);
/**
 * @brief muestra por consola la informacion del enemigo
 * 
 * @param e 
 */
void mostrar_enemigo(Enemigo e);

#endif