# RPG de Texto

Proyecto desarrollado en C para la asignatura de Estructura de Datos.

El programa simula un juego de rol por terminal en el que el personaje explora zonas, recoge objetos, administra su inventario y enfrenta enemigos en orden de aparición. Todo el sistema está construido sobre estructuras de datos implementadas por el grupo: una lista enlazada para el inventario, una pila para el historial de acciones y una cola para los enemigos de cada combate. La partida se puede guardar y cargar mediante archivos de texto.

---

## Integrantes

- Pablo Serón
- Carlos Cofré
- Daniela Soto

---

## Funcionalidades

El programa permite:

- Crear un personaje nuevo (con nombre, vida, ataque y defensa iniciales).
- Cargar una partida guardada desde un archivo de texto.
- Mostrar el estado actual del personaje (vida, ataque y defensa con sus bonos).
- Explorar tres zonas de dificultad creciente:
  - Catacumba (dificultad baja).
  - Abismo (dificultad media).
  - Páramo (dificultad alta).
- Encontrar objetos durante la exploración y decidir si recogerlos.
- Enfrentar grupos de enemigos en orden de aparición.
- Durante el combate:
  - Atacar al enemigo actual.
  - Ver el inventario.
  - Usar un objeto.
- Administrar el inventario:
  - Agregar objetos.
  - Listar todos los objetos.
  - Buscar un objeto por nombre.
  - Usar o equipar un objeto.
  - Descartar un objeto por ID.
- Registrar las acciones realizadas y mostrarlas desde la más reciente a la más antigua.
- Deshacer la última acción, únicamente si fue descartar un objeto.
- Guardar la partida en un archivo.
- Validar las entradas del usuario y mostrar mensajes claros ante operaciones inválidas.

---

## Menú del juego

```text
1. Explorar Zona
2. Ver Inventario
3. Usar / Equipar Objeto
4. Buscar Objeto por Nombre
5. Descartar Objeto
6. Ver Historial de Acciones
7. Deshacer última acción
8. Guardar Partida
9. Salir del Juego
```

Al iniciar, el juego ofrece crear un personaje nuevo o cargar una partida guardada.

---

## Estructuras de datos utilizadas

Todas las estructuras fueron implementadas por el grupo, sin utilizar bibliotecas que resuelvan la estructura completa.

### Inventario: lista enlazada simple con nodo centinela

Se eligió una lista enlazada porque el inventario crece y decrece constantemente, y los objetos se eliminan desde cualquier posición. El nodo centinela evita casos especiales al agregar o eliminar el primer elemento.

Operaciones implementadas:

- `inventario_inicializar`: crea el centinela y deja el inventario vacío.
- `inventario_agregar`: agrega al final y rechaza IDs duplicados.
- `inventario_listar`: muestra los objetos o indica que está vacío.
- `inventario_buscar_por_id` / `inventario_buscar_por_nombre`: búsqueda secuencial.
- `inventario_eliminar_por_id` / `inventario_eliminar_por_nombre`: eliminan un nodo y devuelven una copia del objeto eliminado.
- `inventario_liberar`: libera todos los nodos, incluido el centinela.

### Historial: pila (LIFO)

Se eligió una pila porque las acciones se consultan desde la más reciente a la más antigua, y solo se puede deshacer la última acción registrada. Está implementada con nodos enlazados y un puntero a la cima.

Operaciones implementadas:

- `historial_crear`: crea la pila vacía.
- `historial_registrar` (push): apila una acción con su descripción y, si corresponde, el objeto involucrado.
- `historial_desapilar` (pop): quita y devuelve la acción de la cima.
- `historial_cima` (top): consulta la última acción sin quitarla.
- `historial_esta_vacio`: indica si no hay acciones.
- `historial_mostrar`: recorre la pila desde la cima hacia abajo.
- `historial_liberar`: libera todos los nodos.

### Enemigos: cola (FIFO)

Se eligió una cola porque los enemigos deben enfrentarse en el mismo orden en que aparecen: el primero en llegar es el primero en combatir. Está implementada con nodos enlazados, punteros `front` y `rear` y un contador de tamaño.

Operaciones implementadas:

- `cola_enemigo_create`: crea la cola vacía.
- `cola_enemigo_enqueue`: agrega un enemigo al final.
- `cola_enemigo_dequeue`: quita y devuelve el enemigo del frente.
- `cola_enemigo_front`: obtiene un puntero al enemigo actual para modificar su vida durante el combate.
- `cola_enemigo_is_empty` / `cola_enemigo_size`: consultan el estado de la cola.
- `cola_enemigo_make_empty` / `cola_enemigo_delete`: vacían y destruyen la cola liberando su memoria.

---

## Sistema de juego

### Zonas

| Zona | Dificultad | Objetos | Enemigos |
|------|------------|---------|----------|
| Catacumba | Baja | Antiséptico (poción), Machete (arma) | Furtivo, Mastín |
| Abismo | Media | Impermeable (armadura), Adrenalina (poción) | Mastín, Vándalo |
| Páramo | Alta | Escopeta (arma), Antidisturbio (armadura), Bengala (otro) | Furtivo, Mastín, Vándalo |

En Catacumba y Abismo, el jugador puede decidir si sigue explorando después del primer combate.

### Objetos

- **Arma:** al equiparla, reemplaza el bono de ataque del personaje (los bonos no se acumulan).
- **Armadura:** al equiparla, reemplaza el bono de defensa.
- **Poción:** cura al personaje sin superar su vida máxima y se elimina del inventario. Si la vida ya está al máximo, no se consume.
- **Bengala (otro):** solo puede usarse en combate. El enemigo actual pierde su siguiente ataque y la bengala se consume.

### Combate

- El daño del jugador es `ataque + bono de ataque`.
- El daño recibido es `ataque del enemigo - (defensa + bono de defensa)`. Si el resultado es 0 o menos, la defensa bloquea el golpe.
- Después de atacar, equipar un arma o armadura, o usar una poción, el enemigo actual responde con su ataque.
- Cuando un enemigo llega a 0 PV se retira de la cola y se pasa al siguiente.
- El combate termina cuando la cola queda vacía o el personaje es derrotado.

### Historial y deshacer

El historial registra mensajes descriptivos de las acciones (exploraciones, objetos usados o descartados, partidas guardadas). Solo se puede deshacer la última acción si fue descartar un objeto: en ese caso el objeto se vuelve a agregar al inventario y la acción se quita de la pila.

---

## Guardado y carga de partida

La partida se guarda en un archivo de texto con el siguiente formato:

```text
RPG_PARTIDA_V1
PERSONAJE
nombre vida vida_maxima ataque bono_ataque defensa bono_defensa
INVENTARIO
cantidad
id tipo valor nombre
...
FIN
```

Medidas de robustez:

- Al guardar, se escribe primero en un archivo temporal (`ruta.tmp`) y solo al terminar sin errores se renombra. Así un fallo no corrompe un guardado anterior.
- Al cargar, todo se lee a variables temporales y solo se reemplaza el estado actual si el archivo completo es válido.
- Se validan las marcas del archivo, la cantidad de datos leídos, los rangos de los valores, los tipos de objeto y los IDs duplicados.
- Los nombres de personaje y objetos no pueden contener espacios.

---

## Manejo de casos límite

- **Estructura vacía:** listar un inventario vacío, desapilar un historial vacío, y hacer `dequeue` o `front` en una cola vacía muestran un mensaje y devuelven un valor de error sin romper el programa.
- **Un solo elemento:** al eliminar el único nodo de una lista o cola, los punteros (`siguiente`, `front`, `rear`, `cima`) quedan consistentes.
- **Memoria insuficiente:** cada `malloc` se verifica; ante un fallo se informa el error y la estructura queda sin cambios.
- **Eliminación de elementos:** se guarda una copia del objeto antes de liberar el nodo, lo que permite registrarlo en el historial y deshacer.
- **Liberación de memoria:** al salir del juego se liberan el inventario, el historial y el personaje; las colas de enemigos se destruyen al terminar cada zona. Liberar un inventario dos veces es seguro.
- **Entradas inválidas:** los números se leen de forma segura (se limpia el buffer y se vuelve a pedir el dato) y las opciones fuera de rango muestran un mensaje.

---

## Estructura del proyecto

```text
Proyecto-2-ED/
│
├── src/
│   ├── main.c
│   ├── interfaz.c
│   ├── personaje.c
│   ├── inventario.c
│   ├── objeto.c
│   ├── uso_objeto.c
│   ├── historial.c
│   ├── aventura.c
│   ├── combate.c
│   ├── enemigo.c
│   ├── cola_enemigo.c
│   └── guardado.c
│
├── inc/
│   ├── interfaz.h
│   ├── personaje.h
│   ├── inventario.h
│   ├── objeto.h
│   ├── uso_objeto.h
│   ├── historial.h
│   ├── aventura.h
│   ├── combate.h
│   ├── enemigo.h
│   ├── cola_enemigo.h
│   └── guardado.h
│
├── Makefile
└── README.md
```

### Módulos y responsables

| Módulo | Descripción | Autor |
|--------|-------------|-------|
| `personaje` | Crear personaje, cargar y mostrar estado | Carlos Cofré |
| `historial` | Pila de acciones | Carlos Cofré |
| `objeto` | Definición y validación de objetos | Pablo Serón |
| `inventario` | Lista enlazada con centinela | Pablo Serón |
| `uso_objeto` | Uso y equipamiento de objetos | Pablo Serón |
| `guardado` | Guardar y cargar partida | Pablo Serón |
| `enemigo` | Creación de enemigos | Daniela Soto |
| `cola_enemigo` | Cola de enemigos | Daniela Soto |
| `combate` | Lógica de combate | Daniela Soto |
| `aventura` | Zonas, objetos y encuentros | Daniela Soto |
| `interfaz` / `main` | Menú por terminal y bucle principal | COMPLETAR |

---

## Compilación y ejecución

El proyecto se compila con `gcc` (estándar C11) en un entorno Linux.

```bash
make          # compila y genera el ejecutable "juego"
make run      # compila y ejecuta el juego
make clean    # elimina los archivos objeto y el ejecutable
```

También se puede ejecutar directamente:

```bash
./juego
```
