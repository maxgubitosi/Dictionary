# dictionary: Trabajo práctico Nº3 de Algoritmos y estructuras de datos

## Introducción
El diccionario implementado en este trabajo práctico utiliza una estructura de datos basada en tablas de hashing para lograr un acceso eficiente a los elementos. Se emplea el método de encadenamiento (hashing abierto) para gestionar las colisiones, lo que garantiza un rendimiento estable incluso en situaciones de alta carga.

## Características Principales
* Tablas Hash Dinámicas: El tamaño del diccionario se ajusta automáticamente según la carga actual y los parámetros definidos.
* Rehashing Automático: Cuando la capacidad del diccionario alcanza un umbral predefinido, se realiza automáticamente un rehashing para mantener un factor de carga óptimo.
* Chaining para Resolución de Colisiones: Se utiliza el método de encadenamiento para manejar las colisiones, lo que garantiza un rendimiento predecible incluso en presencia de claves duplicadas.
* Operaciones Básicas: El diccionario soporta operaciones básicas como inserción, eliminación, búsqueda y recuperación de elementos.
  
### Funciones Principales
* dictionary_create: Crea un diccionario vacío con la capacidad inicial especificada.
* dictionary_put: Inserta un nuevo par clave-valor en el diccionario.
* dictionary_get: Recupera el valor asociado a una clave específica.
* dictionary_delete: Elimina un elemento del diccionario.
* dictionary_contains: Verifica si una clave está presente en el diccionario.
* dictionary_size: Devuelve el número de elementos en el diccionario.
* dictionary_destroy: Libera la memoria utilizada por el diccionario.

## Consideraciones de Implementación
* Parámetros Globales Configurables: Se definen parámetros globales como el tamaño inicial de la tabla y el factor de carga para ajustar el rendimiento del diccionario.
* Función de Hashing FNV-1a: Se implementa la función de hashing FNV-1a para calcular los índices de las claves en la tabla hash.
* Gestión de Memoria: Se realizan verificaciones de errores y liberaciones de memoria adecuadas para evitar fugas de memoria y comportamientos inesperados.

## Estructura del repositorio
### Dockerfile y Makefile: 
Se utilizan estas herramientas para crear un entorno de desarrollo reproducible, facilitando la configuración del entorno de desarrollo y garantizando que el código se ejecute facilmente y de la misma manera en diferentes sistemas.

### Archivos Principales
El archivo tp3.h define las estructuras de datos y las funciones públicas del diccionario, mientras que tp3.c contiene la implementación de estas funciones.

### Archivos de Pruebas

  * test_malloc.h y test_malloc.c: Estos archivos contienen funciones auxiliares para simular el comportamiento de malloc y free durante las pruebas. Esto permite verificar la gestión de memoria del diccionario y detectar posibles fugas de memoria.

  * tests.c: Este archivo contiene las pruebas unitarias específicas del diccionario. Aquí se definen casos de prueba para evaluar diferentes aspectos de la implementación, como el manejo de colisiones, el rehashing automático y la gestión de memoria.

