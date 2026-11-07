# Organización y buenas prácticas del grupo

## Organización del trabajo

El trabajo se distribuirá entre los integrantes según los distintos módulos y estructuras de datos que componen el proyecto. Cada integrante tendrá responsabilidades principales, pero todos deberán colaborar en la integración de las funcionalidades y comprender el funcionamiento general del programa.

La distribución de tareas permitirá avanzar de manera organizada, reducir conflictos al trabajar sobre los mismos archivos y facilitar la revisión del código.

## Distribución inicial de tareas

### Estructuras de datos y salas (Pablo Seron)

- Implementación de las estructuras necesarias para representar las salas de la mazmorra.
- Organización de la información de cada sala.
- Administración de las conexiones entre salas.
- Operaciones de búsqueda, inserción y eliminación de salas.

### Algoritmos de recorrido (Eban Delgado)

- Implementación del recorrido en anchura (BFS).
- Implementación del recorrido en profundidad (DFS).
- Búsqueda de rutas entre salas.
- Identificación de salas alcanzables y tratamiento de las salas secretas.

### Archivos e integración del juego (Benjamin Hernandez)

- Lectura y validación de los datos de `mazmorra.txt`.
- Integración de las estructuras de datos y los algoritmos.
- Implementación de las opciones principales del juego.
- Manejo de errores y coordinación entre los distintos módulos.

La distribución podrá ajustarse durante el desarrollo según las necesidades del proyecto y los avances de cada integrante.

## Tareas compartidas

- Elaboración y actualización del `Makefile`.
- Documentación del proyecto en el README.
- Pruebas de los distintos módulos.
- Preparación del informe y descripción de las estructuras implementadas.
- Registro de las contribuciones de cada integrante.
- Declaración del uso de herramientas de inteligencia artificial, cuando corresponda.
- Preparación y realización de la presentación del proyecto.

## Uso de Git y GitHub

Para mantener un historial de cambios ordenado y facilitar el trabajo colaborativo, se seguirán las siguientes prácticas:

- Cada integrante realizará commits relacionados con las tareas que esté desarrollando.
- Los mensajes de los commits deberán describir claramente los cambios realizados.
- Antes de comenzar una tarea, se actualizará el repositorio local.
- Antes de integrar cambios importantes, se comprobará que el programa compile correctamente.
- Se evitará modificar simultáneamente los mismos archivos cuando sea posible.
- Si aparecen conflictos al combinar cambios, se revisarán las modificaciones antes de resolverlos.
- No se eliminarán cambios de otros integrantes sin comunicárselo previamente al grupo.

## Buenas prácticas de programación

Durante el desarrollo se mantendrán criterios comunes para facilitar la lectura y el mantenimiento del código:

- Se utilizará `snake_case` para nombrar variables y funciones.
- Las macros y constantes definidas mediante el preprocesador utilizarán `SCREAMING_SNAKE_CASE`.
- Los nombres de las etiquetas `struct` y los tipos definidos mediante `typedef` seguirán una convención común en todo el proyecto.
- Se evitará el uso innecesario de variables globales.
- Se comprobará el resultado de las operaciones que puedan fallar, especialmente las reservas de memoria y la apertura de archivos.
- Se liberará la memoria dinámica cuando ya no sea necesaria.
- Se agregarán comentarios cuando una operación o algoritmo requiera una explicación adicional.
- Las funciones deberán tener responsabilidades claras y nombres relacionados con su propósito.
- Se validarán los datos de entrada para evitar comportamientos inesperados.

## Organización de archivos

Los archivos del proyecto se mantendrán organizados en carpetas según su función, separando el código fuente, los archivos de cabecera, los datos de entrada y los archivos generados durante la compilación.

Se procurará mantener una estructura coherente para que todos los integrantes puedan localizar y modificar los módulos sin dificultad. Los archivos temporales y ejecutables generados durante la compilación no deberán confundirse con los archivos necesarios para el desarrollo.

## Revisión del código

Antes de incorporar una funcionalidad al proyecto principal, se realizarán las siguientes comprobaciones:

1. Verificar que el código compile sin errores.
2. Probar la funcionalidad implementada con diferentes casos.
3. Comprobar que los cambios no afecten las funciones existentes.
4. Revisar el manejo de errores y la liberación de memoria cuando corresponda.
5. Solicitar la revisión de otro integrante cuando se trate de cambios importantes.

## Comunicación del grupo

La comunicación se realizará mediante el grupo de WhatsApp acordado por los integrantes.

Se informará al resto del equipo cuando:

- Se comience una tarea importante.
- Se termine una funcionalidad.
- Se encuentre un error que pueda afectar otros módulos.
- Se modifique una estructura de datos o una interfaz utilizada por otros integrantes.
- Se realicen cambios importantes en la organización del proyecto.

## Resolución de conflictos

Cuando existan diferencias entre las implementaciones o se produzcan conflictos al integrar cambios, los integrantes revisarán las alternativas y acordarán una solución.

Las modificaciones que puedan afectar el trabajo de otros integrantes deberán comunicarse oportunamente. Se priorizará mantener la compatibilidad entre los módulos y evitar la pérdida de avances.