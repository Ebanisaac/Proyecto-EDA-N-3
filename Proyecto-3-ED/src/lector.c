/**
 * @file lector.c
 * @author Benjamin Hernandez
 * @brief Implementación de la lectura de mazmorra.txt en 2 pasadas
 * @date 2026-11-07
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lector.h"

#define MAX_LINEA 512

/**
 * @brief 
 * 
 * @return ListaSecretas* 
 */
ListaSecretas* crear_lista_secretas(){
    ListaSecretas *lista = malloc(sizeof(ListaSecretas));
    if(lista == NULL){
        return NULL;
    }
    lista->cabeza = NULL;
    lista->cantidad = 0;
    return lista;
}

/**
 * @brief 
 * 
 * @param lista 
 * @param h 
 */
void agregar_secreta_pendiente(ListaSecretas *lista, Habitacion *h){
    if(lista == NULL || h == NULL){
        return;
    }
    NodoSecreta *nuevo = malloc(sizeof(NodoSecreta));
    if(nuevo == NULL){
        return;
    }
    nuevo->habitacion = h;
    nuevo->siguiente = lista->cabeza;
    lista->cabeza = nuevo;
    lista->cantidad++;
}

/**
 * @brief 
 * 
 * @param lista 
 */
void liberar_lista_secretas(ListaSecretas *lista){
    if(lista == NULL){
        return;
    }
    NodoSecreta *actual = lista->cabeza;
    while(actual != NULL){
        NodoSecreta *temp = actual;
        actual = actual->siguiente;
        liberar_habitacion(temp->habitacion);
        free(temp);
    }
    free(lista);
}

/**
 * @brief 
 * 
 * @param ruta_archivo 
 * @param raiz_arbol 
 * @param grafo 
 * @param secretas 
 * @return int 
 */
int cargar_mazmorra(const char *ruta_archivo, NodoArbol **raiz_arbol, grafo_t *grafo, ListaSecretas *secretas){
    FILE *archivo = fopen(ruta_archivo, "r");
    if(archivo == NULL){
        printf("Error: No se pudo abrir el archivo %s\n", ruta_archivo);
        return 0;
    }
    char linea[MAX_LINEA];
    int num_linea = 0;
    int en_seccion_pasillos = 0;

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        num_linea++;
        if(linea[0] == '\n' || linea[0] == '\r' || linea[0] == '#'){
            continue;
        }
        if(strncmp(linea, "PASILLOS", 8) == 0 || strncmp(linea, "CONEXIONES", 10) == 0){
            en_seccion_pasillos = 1;
            break;
        }

        if(!en_seccion_pasillos){
            int id;
            char nombre[MAX_NOMBRE], desc[MAX_DESCRIPCION], tipo[MAX_TIPO];
            int leidos = sscanf(linea, "%d;%59[^;];%199[^;];%29[^\n\r]", &id, nombre, desc, tipo);
            if(leidos < 4){
                printf("Error de formato en linea %d: '%s'\n", num_linea, linea);
                fclose(archivo);
                return 0;
            }
            Habitacion *hab = crear_habitacion(id, nombre, desc, tipo);
            if(hab == NULL){
                printf("Error en linea %d: Memoria insuficiente.\n", num_linea);
                fclose(archivo);
                return 0;
            }
            if(strcmp(tipo, "secreta") == 0 || strcmp(tipo, "SECRETA") == 0){
                agregar_secreta_pendiente(secretas, hab);
            }else{
                *raiz_arbol = insertar_nodo(*raiz_arbol, hab);
                grafo_agregar_vertice(grafo, id);
            }
        }
    }

    rewind(archivo);
    num_linea = 0;
    en_seccion_pasillos = 0;
    while(fgets(linea, sizeof(linea), archivo) != NULL){
        num_linea++;
        if(linea[0] == '\n' || linea[0] == '\r' || linea[0] == '#'){
            continue;
        }

        if(strncmp(linea, "PASILLOS", 8) == 0 || strncmp(linea, "CONEXIONES", 10) == 0){
            en_seccion_pasillos = 1;
            continue;
        }

        if(en_seccion_pasillos){
            int id_a, id_b;
            if(sscanf(linea, "%d;%d", &id_a, &id_b) == 2){
                // Solo agregar pasillos si ambas habitaciones existen en el grafo actual
                if(grafo_existe_vertice(grafo, id_a) && grafo_existe_vertice(grafo, id_b)){
                    grafo_agregar_pasillo(grafo, id_a, id_b);
                }
            }else{
                printf("Advertencia en linea %d: Pasillo con formato invalido.\n", num_linea);
            }
        }
    }
    fclose(archivo);
    printf("Carga exitosa de la mazmorra.\n");
    return 1;
}