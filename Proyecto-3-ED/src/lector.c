/**
 * @file lector.c
 * @author Benjamin Hernandez
 * @brief Implementación de la lectura de mazmorra.txt en 2 pasadas y revelación de secretos
 * @date 2026-11-07
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lector.h"
#define MAX_LINEA 512

/**
 * @brief Comprueba si un carácter corresponde a un espacio en blanco o salto de línea.
 * @param c Carácter a evaluar.
 * @return 1 si es espacio, tabulación o salto de línea; 0 en caso contrario.
 */
int es_espacio(char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/**
 * @brief Elimina espacios en blanco y saltos de línea al inicio y final de una cadena.
 *
 * @param str Cadena de texto a limpiar in-place.
 */
void recortar_espacios(char *str){
    char *inicio = str;
    while(*inicio != '\0' && es_espacio(*inicio)){
        inicio++;
    }
    if(inicio != str){
        memmove(str, inicio, strlen(inicio) + 1);
    }
    size_t len = strlen(str);
    while(len > 0 && es_espacio(str[len - 1])){
        str[--len] = '\0';
    }
}

/**
 * @brief Valida y convierte una cadena de texto en un entero positivo (> 0).
 *
 * @param str Cadena a convertir.
 * @param valor Puntero donde se almacenará el número convertido si es válido.
 * @return 1 si es un entero positivo válido, 0 en caso contrario.
 */
int es_entero_positivo(const char *str, int *valor){
    if(str == NULL || *str == '\0') return 0;
    char *fin = NULL;
    long v = strtol(str, &fin, 10);
    if(*fin != '\0' || v <= 0 || v > 1000000) return 0;
    *valor = (int)v;
    return 1;
}

/**
 * @brief Verifica si el tipo de habitación corresponde a uno de los valores permitidos.
 *
 * @param tipo Cadena con el tipo de habitación.
 * @return 1 si el tipo es válido, 0 si no lo es.
 */
int tipo_valido(const char *tipo){
    return strcmp(tipo, "INICIO") == 0 || strcmp(tipo, "NORMAL") == 0 || strcmp(tipo, "COMBATE") == 0 || strcmp(tipo, "TESORO") == 0 || strcmp(tipo, "JEFE") == 0 ||  strcmp(tipo, "VACIA") == 0 || strcmp(tipo, "SECRETA") == 0;
}

/**
 * @brief Parsea una lista de IDs vecinos separados por comas o '-' si no hay vecinos.
 *
 * @param campo Cadena con el campo de vecinas.
 * @param cantidad Puntero donde se guarda el total de IDs válidos extraídos.
 * @param num_linea Número de línea actual del archivo (para reporte de errores).
 * @param id_hab ID de la habitación actual (para reporte de errores).
 * @return Arreglo dinámico de enteros con los IDs, o NULL si no hay vecinos válidos.
 */
int *parsear_vecinas(const char *campo, int *cantidad, int num_linea, int id_hab){
    *cantidad = 0;
    if(strcmp(campo, "-") == 0 || *campo == '\0'){
        return NULL;
    }
    char copia[MAX_LINEA];
    strncpy(copia, campo, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';
    int cap = 8;
    int *ids = malloc(sizeof(int) * cap);
    if(ids == NULL){
        return NULL;
    }

    char *token = strtok(copia, ",");
    while(token != NULL){
        recortar_espacios(token);
        int id_v;
        if(!es_entero_positivo(token, &id_v)){
            printf("Error en linea %d (habitacion %d): ID de vecina invalido '%s'.\n",
                   num_linea, id_hab, token);
        }else{
            if(*cantidad >= cap){
                cap *= 2;
                int *tmp = realloc(ids, sizeof(int) * cap);
                if(tmp == NULL){
                    free(ids);
                    *cantidad = 0;
                    return NULL;
                }
                ids = tmp;
            }
            ids[(*cantidad)++] = id_v;
        }
        token = strtok(NULL, ",");
    }

    if(*cantidad == 0){
        free(ids);
        return NULL;
    }
    return ids;
}

/**
 * @brief Divide una línea en subcadenas usando '|' como delimitador, preservando campos vacíos.
 *
 * @param linea Línea leída del archivo (se modifica reemplazando '|' por '\0').
 * @param campos Arreglo de punteros donde se guardará el inicio de cada campo.
 * @param max_campos Capacidad máxima del arreglo campos.
 * @return Cantidad total de campos detectados en la línea.
 */
int dividir_campos(char *linea, char *campos[], int max_campos){
    int count = 0;
    char *p = linea;
    campos[count++] = p;
    while(*p != '\0'){
        if(*p == '|'){
            *p = '\0';
            if(count < max_campos){
                campos[count++] = p + 1;
            }else{
                count++;
            }
        }
        p++;
    }
    return count;
}

/**
 * @brief Comprueba si un ID ya existe dentro de la lista de habitaciones secretas pendientes.
 *
 * @param secretas Lista de habitaciones secretas pendientes.
 * @param id ID a buscar.
 * @return 1 si ya está registrado, 0 en caso contrario.
 */
int existe_id_en_secretas(const ListaSecretas *secretas, int id){
    const NodoSecreta *act = secretas->cabeza;
    while(act != NULL){
        if(act->habitacion->id == id){
            return 1;
        }
        act = act->siguiente;
    }
    return 0;
}

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
 * @param vecinas_ids 
 * @param num_vecinas 
 */
void agregar_secreta_pendiente(ListaSecretas *lista, Habitacion *h, int *vecinas_ids, int num_vecinas){
    if(lista == NULL || h == NULL){
        free(vecinas_ids);
        return;
    }
    NodoSecreta *nuevo = malloc(sizeof(NodoSecreta));
    if(nuevo == NULL){
        free(vecinas_ids);
        return;
    }
    nuevo->habitacion = h;
    nuevo->vecinas_ids = vecinas_ids;
    nuevo->num_vecinas = num_vecinas;
    nuevo->siguiente = NULL;
    if(lista->cabeza == NULL){
        lista->cabeza = nuevo;
    }else{
        NodoSecreta *act = lista->cabeza;
        while(act->siguiente != NULL){
            act = act->siguiente;
        }
        act->siguiente = nuevo;
    }
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
        free(temp->vecinas_ids);
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
 * @param id_inicio 
 * @return int 
 */
int cargar_mazmorra(const char *ruta_archivo, NodoArbol **raiz_arbol, grafo_t *grafo, ListaSecretas *secretas, int *id_inicio){
    if(ruta_archivo == NULL || raiz_arbol == NULL || grafo == NULL || secretas == NULL || id_inicio == NULL){
        return 0;
    }
    FILE *archivo = fopen(ruta_archivo, "r");
    if(archivo == NULL){
        printf("Error: No se pudo abrir el archivo '%s'.\n", ruta_archivo);
        return 0;
    }
    char linea[MAX_LINEA];
    int num_linea = 0;
    int count_inicio = 0;
    *id_inicio = -1;

    RegistroPasillos *pendientes_pasillos = NULL;
    int cant_reg = 0;
    int cap_reg = 0;
    while(fgets(linea, sizeof(linea), archivo) != NULL){
        num_linea++;
        recortar_espacios(linea);
        if(linea[0] == '\0' || linea[0] == '#'){
            continue;
        }

        char *campos[10];
        int n_campos = dividir_campos(linea, campos, 10);
        if(n_campos != 8){
            printf("Error en linea %d: se esperaban 8 campos separados por '|', se encontraron %d.\n",
                   num_linea, n_campos);
            continue;
        }

        for(int i = 0; i < 8; i++){
            recortar_espacios(campos[i]);
        }

        int id;
        if(!es_entero_positivo(campos[0], &id)){
            printf("Error en linea %d: ID no numerico o invalido ('%s').\n", num_linea, campos[0]);
            continue;
        }

        if(buscar_nodo(*raiz_arbol, id) != NULL || existe_id_en_secretas(secretas, id)){
            printf("Error en linea %d: ID duplicado (%d). Se conserva la primera aparicion.\n", num_linea, id);
            continue;
        }

        if(!tipo_valido(campos[3])){
            printf("Error en linea %d: Tipo de habitacion invalido ('%s').\n", num_linea, campos[3]);
            continue;
        }

        char c_oculta = campos[6][0];
        int es_secreta = (c_oculta == 'S' || c_oculta == 's');
        int es_visible = (c_oculta == 'N' || c_oculta == 'n');
        if(strlen(campos[6]) != 1 || (!es_secreta && !es_visible)){
            printf("Error en linea %d: Valor invalido en campo oculta ('%s'). Debe ser S o N.\n",
                   num_linea, campos[6]);
            continue;
        }

        Habitacion *hab = crear_habitacion(id, campos[1], campos[2], campos[3]);
        if(hab == NULL){
            printf("Error en linea %d: Memoria insuficiente.\n", num_linea);
            fclose(archivo);
            free(pendientes_pasillos);
            return 0;
        }

        if(strcmp(campos[4], "-") != 0 && campos[4][0] != '\0'){
            Objeto *obj = malloc(sizeof(Objeto));
            if(obj != NULL && objeto_inicializar(obj, id * 100 + 1, campos[4], TIPO_OTRO, 10) == 0){
                agregar_objeto(hab, obj);
            }else{
                free(obj);
            }
        }

        if(strcmp(campos[5], "-") != 0 && campos[5][0] != '\0'){
            Enemigo *ene = malloc(sizeof(Enemigo));
            if(ene != NULL){
                *ene = crear_enemigo_tipo1(id * 100 + 2, campos[5], 30, 8, 30);
                agregar_enemigo(hab, ene);
            }
        }

        int num_vecinas = 0;
        int *vecinas = parsear_vecinas(campos[7], &num_vecinas, num_linea, id);

        if(es_secreta){
            agregar_secreta_pendiente(secretas, hab, vecinas, num_vecinas);
        }else{
            if(strcmp(campos[3], "INICIO") == 0){
                count_inicio++;
                *id_inicio = id;
            }
            *raiz_arbol = insertar_nodo(*raiz_arbol, hab);
            grafo_agregar_vertice(grafo, id);

            if(cant_reg >= cap_reg){
                cap_reg = (cap_reg == 0) ? 16 : cap_reg * 2;
                RegistroPasillos *tmp = realloc(pendientes_pasillos, sizeof(RegistroPasillos) * cap_reg);
                if(tmp != NULL) pendientes_pasillos = tmp;
            }
            if(pendientes_pasillos != NULL){
                pendientes_pasillos[cant_reg].id_origen = id;
                pendientes_pasillos[cant_reg].num_linea = num_linea;
                pendientes_pasillos[cant_reg].vecinas = vecinas;
                pendientes_pasillos[cant_reg].num_vecinas = num_vecinas;
                cant_reg++;
            }else{
                free(vecinas);
            }
        }
    }

    fclose(archivo);

    // PASADA 2: Conectar los pasillos entre habitaciones visibles existentes
    for(int i = 0; i < cant_reg; i++){
        int u = pendientes_pasillos[i].id_origen;
        int linea_reg = pendientes_pasillos[i].num_linea;
        for(int j = 0; j < pendientes_pasillos[i].num_vecinas; j++){
            int v = pendientes_pasillos[i].vecinas[j];
            if(!grafo_existe_vertice(grafo, v)){
                printf("Error en linea %d: Vecina inexistente o secreta (%d) ignorada en habitacion %d.\n",
                       linea_reg, v, u);
            }else{
                grafo_agregar_pasillo(grafo, u, v);
            }
        }
        free(pendientes_pasillos[i].vecinas);
    }
    free(pendientes_pasillos);

    if(count_inicio != 1){
        printf("Error fatal: Debe existir exactamente 1 habitacion de tipo INICIO (encontradas: %d).\n", count_inicio);
        return 0;
    }

    return 1;
}

/**
 * @brief 
 * 
 * @param secretas 
 * @param id_actual 
 * @param raiz_arbol 
 * @param grafo 
 * @return int 
 */
int revelar_pasadizos_secretos(ListaSecretas *secretas, int id_actual, NodoArbol **raiz_arbol, grafo_t *grafo){
    if(secretas == NULL || raiz_arbol == NULL || grafo == NULL){
        return 0;
    }
    int descubiertas = 0;
    NodoSecreta *act = secretas->cabeza;
    NodoSecreta *prev = NULL;
    while(act != NULL){
        int conecta = 0;
        for(int i = 0; i < act->num_vecinas; i++){
            if(act->vecinas_ids[i] == id_actual){
                conecta = 1;
                break;
            }
        }

        if(conecta){
            NodoSecreta *revelada = act;
            act = act->siguiente;
            if(prev == NULL){
                secretas->cabeza = act;
            }else{
                prev->siguiente = act;
            }
            secretas->cantidad--;

            int id_sec = revelada->habitacion->id;
            if(descubiertas == 0){
                printf("Golpeas los muros... ¡un ladrillo cede!\n");
            }
            printf("¡Descubriste %s (ID %d)!\n", revelada->habitacion->nombre, id_sec);

            *raiz_arbol = insertar_nodo(*raiz_arbol, revelada->habitacion);
            grafo_agregar_vertice(grafo, id_sec);

            for(int i = 0; i < revelada->num_vecinas; i++){
                int v = revelada->vecinas_ids[i];
                if(grafo_existe_vertice(grafo, v)){
                    if(grafo_agregar_pasillo(grafo, id_sec, v) == GRAFO_OK){
                        printf("Nuevo pasillo: %d <-> %d\n", v, id_sec);
                    }
                }
            }

            free(revelada->vecinas_ids);
            free(revelada);
            descubiertas++;
        }else{
            prev = act;
            act = act->siguiente;
        }
    }

    if(descubiertas == 0){
        printf("Revisas las paredes cuidadosamente, pero no encuentras pasadizos secretos aquí.\n");
    }
    return descubiertas;
}