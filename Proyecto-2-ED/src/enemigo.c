/**
 * @file enemigo.c
 * @author Daniela Soto
 * @brief creacion de enemigos
 */
#include "enemigo.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief crea e inicializa al enemigo tipo 1 con los valores de entrada
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo1(int id, const char* nombre, int pv, int ataque, int vida_max){
    Enemigo  e;

    e.id = id;
    snprintf(e.nombre, sizeof(e.nombre), "%s", nombre);
    e.pv = pv;
    e.ataque = ataque;
    e.vida_max = vida_max;
    
    return e;
}

/**
 * @brief crea e inicializa al enemigo tipo 2 con los valores de entrada
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo2(int id, const char* nombre, int pv, int ataque, int vida_max){
    Enemigo  e;

    e.id = id;
    snprintf(e.nombre, sizeof(e.nombre), "%s", nombre);
    e.pv = pv;
    e.ataque = ataque;
    e.vida_max = vida_max;
    
    return e;
}

/**
 * @brief crea e inicializa al enemigo tipo 1 con los valores de entrada
 * 
 * @param id 
 * @param nombre 
 * @param pv 
 * @param ataque 
 * @param vida_max 
 * @return Enemigo 
 */
Enemigo crear_enemigo_tipo3(int id, const char* nombre, int pv, int ataque, int vida_max){
    Enemigo  e;

    e.id = id;
    snprintf(e.nombre, sizeof(e.nombre), "%s", nombre);
    e.pv = pv;
    e.ataque = ataque;
    e.vida_max = vida_max;
    
    return e;
}

/**
 * @brief muestra por consola la informacion del enemigo
 * 
 * @param e 
 */
void mostrar_enemigo(Enemigo e){
    printf("Enemigo: \n");
    printf("ID: %d | Nombre: %s | Pv: %d/%d | Ataque: %d | \n", e.id, e.nombre, e.pv, e.vida_max, e.ataque);
    return;
}