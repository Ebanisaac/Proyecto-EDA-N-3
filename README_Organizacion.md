# Organización y buenas prácticas del grupo

## Organización del trabajo

El trabajo se distribuirá entre los integrantes según los distintos módulos y estructuras de datos que componen el proyecto. Cada integrante tendrá responsabilidades principales, pero todos deberán colaborar en la integración de las funcionalidades y comprender el funcionamiento general del programa.

La distribución de tareas permitirá avanzar de manera organizada, reducir conflictos al trabajar sobre los mismos archivos y facilitar la revisión del código.

Para que el aporte sea equitativo, cada integrante se encarga de **al menos una estructura de datos o algoritmo central** y de **varias opciones del menú** que dependen de ella. Así, todos implementan funcionalidades solicitadas en el enunciado y trabajan con contenidos nuevos del curso.

## Distribución de tareas

### Pablo Seron: grafo, habitaciones secretas y quemar habitaciones

- Implementación del grafo no dirigido que representa el mapa (lista de adyacencia).
- Operaciones del grafo: agregar y eliminar habitaciones (vértices) y pasillos (aristas), consulta de vecinas.
- Lista de habitaciones secretas pendientes y lógica de "Buscar pasadizos secretos" (opción 8): revelar, insertar en el grafo y en el ABB/AVL, y crear pasillos hacia vecinas existentes.
- Lógica de "Quemar una habitación" (opción 9): validación de reglas, eliminación en el grafo y en el ABB/AVL, y aviso de pasillos destruidos.
- Opción "Mostrar habitaciones vecinas" (opción 2).

### Eban Delgado: algoritmos de recorrido y desplazamiento del personaje

- Implementación del recorrido en anchura (BFS): ruta más corta y detección de destino no alcanzable.
- Implementación del recorrido en profundidad (DFS): habitaciones alcanzables.
- Cálculo de habitaciones a k pasillos.
- Opciones de menú: moverse a una habitación vecina (3), buscar ruta (4), explorar alcanzables (5), mostrar habitaciones a k pasillos (7) y mostrar habitaciones visitadas (10).
- Registro del personaje (habitación actual y visitadas) y encuentros con enemigos (combatir o escapar).

### Benjamin Hernandez: habitaciones, ABB/AVL y carga del mapa

- Definición de la estructura de habitación (ID, nombre, descripción, tipo, tesoro y enemigo).
- Implementación del ABB/AVL ordenado por ID: inserción, búsqueda, eliminación (hoja, un hijo y dos hijos) y recorridos inorden, preorden y postorden.
- Lectura y validación de `mazmorra.txt`: construcción del grafo en dos pasadas, detección de errores con número de línea y manejo de habitaciones secretas pendientes.
- Opciones de menú: consultar habitación (1), mostrar todas las habitaciones con recorrido inorden (6) y salir (11).
- Bucle principal del programa (`main`) y menú.

La distribución podrá ajustarse durante el desarrollo según las necesidades del proyecto y los avances de cada integrante. Este reparto reemplaza a la distribución inicial, y quedó registrado con un commit propio en el repositorio.

## Tareas compartidas

- Elaboración y actualización del `Makefile`.
- Documentación del proyecto en el README.
- Pruebas de los distintos módulos.
- Integración de las estructuras y los algoritmos: cada integrante revisa e integra al menos un módulo de otro compañero.
- Preparación del informe y descripción de las estructuras implementadas (cada integrante redacta la parte de su módulo).
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
