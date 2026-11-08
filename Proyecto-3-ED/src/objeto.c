/*
 * Archivo: objeto.c
 * Autor: Pablo Serón
 * Descripción: Implementación de las funciones del módulo Objeto.
 */

#include <stdio.h>
#include "objeto.h"

/**
 * @brief Rellena un objeto con los datos entregados.
 * 
 * @param objeto Objeto destino (no debe ser NULL).
 * @param id Identificador único (> 0).
 * @param nombre Nombre no vacío; si es muy largo se trunca.
 * @param tipo Categoría del objeto.
 * @param valor Efecto numérico del objeto.
 * @return 0 si todo fue correcto, -1 si algún parámetro es inválido.
 */
int objeto_inicializar(Objeto *objeto, int id, const char *nombre, TipoObjeto tipo, int valor)
{
    //validacion de: puntero nulo, nombre nulo, nombre vacío o id negativo.
    if (objeto == NULL || nombre == NULL || nombre[0] == '\0' || id <= 0)
    {
        return -1;
    }

    if((int)tipo < TIPO_ARMA || (int)tipo > TIPO_OTRO)
    {
        return -1;
    }

    if(tipo == TIPO_POCION && valor <= 0)
    {
        return -1;
    }

    objeto->id = id;
    snprintf(objeto->nombre, LARGO_NOMBRE_OBJETO, "%s", nombre);
    objeto->tipo = tipo;
    objeto->valor = valor;
    return 0;
}

/**
 * @brief Convierte un tipo de objeto a texto.
 * @param tipo Tipo de objeto a convertir.
 * @return Cadena estática ("arma", "armadura", "poción", "otro") o
 *         "desconocido" si el tipo no es válido. No debe liberarse.
 */
const char *objeto_tipo_a_texto(TipoObjeto tipo)
{
    switch (tipo) {
        case TIPO_ARMA:     return "arma";
        case TIPO_ARMADURA: return "armadura";
        case TIPO_POCION:   return "poción";
        case TIPO_OTRO:     return "otro";
        default:            return "desconocido";
    }
}

/**
 * @brief Imprime un objeto por terminal con el formato: [id] nombre (tipo, valor).
 * 
 * @param objeto Objeto a imprimir. Si es NULL, no hace nada.
 */
void objeto_imprimir(const Objeto *objeto)
{
    if (objeto == NULL)
    {
        return;
    }
    printf(" [%d] %s (%s, %d)\n", objeto->id, objeto->nombre, objeto_tipo_a_texto(objeto->tipo), objeto->valor);
}