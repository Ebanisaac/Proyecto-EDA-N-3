/*
 * Archivo: guardado.h
 * Autor: Pablo Serón
 * Descripción: Guardar y cargar partida en un archivo de texto
 *
 * Formato (los nombres no pueden tener espacios):
 *
 *   EJ:
 *   RPG_PARTIDA_V1
 */

#ifndef GUARDADO_H
#define GUARDADO_H

#include "inventario.h"
#include "personaje.h"

/**
 * @brief Codigos de resultado de guardar/cargar partida.
 */
typedef enum
{
    GUARDADO_OK = 0,
    GUARDADO_ERROR_PARAMETRO, // puntero NULL
    GUARDADO_ERROR_ARCHIVO,   // no se pudo abrir, escribir o leer el archivo
    GUARDADO_ERROR_FORMATO,   // el archivo existe pero sus datos son invalidos
    GUARDADO_ERROR_MEMORIA    // malloc devolvio NULL al reconstruir el inventario

} ResultadoGuardado;

/**
 * @brief Guarda el personaje y el inventario en un archivo de texto.
 *
 * @param ruta Ruta del archivo de guardado.
 * @param personaje Personaje a guardar.
 * @param inventario Inventario a guardar (ya inicializado; puede estar vacio).
 *
 * @return GUARDADO_OK, GUARDADO_ERROR_PARAMETRO si algun puntero es NULL, el
 *         inventario no esta inicializado o algun nombre esta vacio o tiene
 *         espacios (en ese caso no se escribe nada), o GUARDADO_ERROR_ARCHIVO
 *         si falla la escritura.
 */
ResultadoGuardado guardar_partida(const char *ruta, const Personaje *personaje,
                                  const Inventario *inventario);

/**
 * @brief Carga una partida desde un archivo de texto.
 *
 * @param ruta Ruta del archivo de guardado.
 * @param personaje Personaje donde se carga (se sobrescribe).
 * @param inventario Inventario donde se carga. (Este debe estar inicializado)
 *
 * @return GUARDADO_OK, GUARDADO_ERROR_PARAMETRO, GUARDADO_ERROR_ARCHIVO si no
 *         se puede abrir/leer, GUARDADO_ERROR_FORMATO si los datos son
 *         invalidos, o GUARDADO_ERROR_MEMORIA si malloc falla.
 */
ResultadoGuardado cargar_partida(const char *ruta, Personaje *personaje,
                                 Inventario *inventario);

#endif /* GUARDADO_H */