# Organización y buenas prácticas del grupo

## Integrantes

- Pablo Serón
- Daniela Soto
- Carlos Cofre

## Organización del trabajo

Cada integrante es responsable principal de una estructura de datos (lista enlazada, cola o pila) y de los módulos que la usan. Así cada uno:

Implementa una estructura propia desde cero (sin bibliotecas que la resuelvan).
La integra en funcionalidades reales del juego, no solo la crea y la muestra.
Puede explicarla y defenderla en la presentación y en el informe.

## Distribución inicial de tareas

### Pablo Serón - Lista enlazada (Inventario)
- Estructura Inventario (Lista)
- Módulo Objeto
- Inventario
- Usar/equipar
- Guardar/Cargar

### Daniela Soto - Cola (Enemigos y combate)
- Estructura de cola
- Módulo Enemigo
- Combate
- Aventura



### Carlos Cofre - Pila (Historial y deshacer)
- Estructura de pila
- Módulo Acción/Historial
- Historial
- Deshacer
- Personaje
- Interfaz

## Tareas compartidas

- Makefile
- README del proyecto
- Pruebas de cada módulo
- Informe (secciones de su estructura)
- Declaración de IA y contribuciones
- Presentación


## Uso de Git y GitHub

Para trabajar de forma ordenada:

- Cada integrante realizará commits relacionados con las funcionalidades que esté desarrollando.
- Los commits deberán tener mensajes descriptivos.
- Se evitarán commits con mensajes poco claros como cambios, cosas o prueba.
- Antes de comenzar a trabajar, se deberá actualizar el repositorio local.
- Antes de integrar cambios importantes, se revisará que el programa compile correctamente.
- Los integrantes deberán evitar modificar simultáneamente las mismas partes del código cuando sea posible.

## Buenas prácticas de programación

Durante el desarrollo se seguirán las siguientes reglas:

- Se usará la nomenclatura snake_case para el nombramiento de variables y funciones.
- Para las macros se usará la nomenclatura de SCREAMING_SNAKE_CASE
- Los nombres de la etiqueta struct deben de comenzar con guion bajo, mientras que el nombre definido mediante typedef debe de comenzar con mayúscula y sin un guión bajo.
- Evitar el uso innecesario de variables globales.
- Comentar las partes del código que puedan resultar difíciles de comprender.
- Documentar las funciones indicando su propósito, parámetros y valor de retorno.
- Identificación del autor en cada archivo y documentación de errores posibles

## Organización de archivos

Se intentará mantener una estructura clara en el repositorio, utilizando carpetas especificas para código fuente, archivos de cabecera. archivos de compilados y ejecutables.

## Revisión del código

Antes de incorporar una funcionalidad al proyecto principal:

1. Se verificará que compile correctamente.
2. Se probará la funcionalidad implementada.
3. Se revisará que no afecte funcionalidades existentes.
4. Otro integrante podrá revisar el código antes de integrarlo.

## Comunicación del grupo

La comunicación del equipo se realizará mediante el medio acordado por los integrantes via whatsapp.

Se informará al grupo cuando:

- Se comience una tarea importante.
- Se termine una funcionalidad.
- Se encuentre un error que pueda afectar a otros integrantes.
- Se realicen cambios importantes en la estructura del proyecto.

## Resolución de conflictos

Si dos integrantes realizan cambios incompatibles, el grupo revisará ambas versiones y decidirá conjuntamente cuál solución mantener.

No se eliminarán cambios realizados por otro integrante sin informar previamente al grupo.

## Responsabilidad del grupo

Todos los integrantes deberán participar en:

- Desarrollo del código.
- Escritura del informe.
- Preparación de la presentación.
- Revisión general del proyecto.
- Declaración de IA y contribuciones.
- Presentación posterior a la entrega del Proyecto.

Cada integrante deberá comprender las funcionalidades principales del sistema y ser capaz de explicar su implementación.
