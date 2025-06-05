# Entorno de Pruebas para get_next_line

Este directorio contiene un conjunto de pruebas para verificar el correcto funcionamiento de la función `get_next_line` y su versión bonus.

## Estructura del Directorio

-   **`run_tests.sh`**: Script principal para compilar y ejecutar todas las pruebas.
-   **`test_main_mandatory.c`**: Programa C base para las pruebas de la parte obligatoria. Compara la salida de `get_next_line` con líneas esperadas.
-   **`test_main_bonus.c`**: Programa C base para las pruebas de la parte bonus. Lee de múltiples descriptores de archivo de forma intercalada.
-   **`test_invalid_fd.c`**: Programa C que prueba el comportamiento de `get_next_line` con descriptores de archivo inválidos.
-   **`test_files/`**: Contiene los archivos de texto utilizados como entrada para las pruebas.
    -   `empty_file.txt`: Archivo vacío.
    -   `single_line.txt`: Línea única sin salto de línea final.
    -   `single_line_with_nl.txt`: Línea única con salto de línea final.
    -   `multiple_lines.txt`: Múltiples líneas con saltos de línea.
    -   `multiple_nl.txt`: Múltiples saltos de línea consecutivos.
    -   `long_line.txt`: Una línea muy larga.
    -   `file_with_no_nl.txt`: Archivo cuya última línea no tiene salto de línea.
    -   `bonus_file1.txt`, `bonus_file2.txt`, `bonus_file3.txt`: Archivos para pruebas de múltiples FDs (bonus).
-   **`logs/`**: Contiene los archivos de log detallados para cada prueba ejecutada.
    -   Cada archivo de log (ej: `empty_file.log`) incluye:
        -   Nombre de la prueba y archivo de entrada.
        -   Comportamiento esperado.
        -   Comportamiento recibido de `get_next_line`.
        -   Resultado final (PASSED/FAILED o COMPLETED).

## Requisitos

-   Un compilador C (como `gcc`).
-   Los archivos de `get_next_line` (`get_next_line.c`, `get_next_line.h`, `get_next_line_utils.c` y las versiones `_bonus`) deben estar en el directorio padre (`../`).

## Ejecución de las Pruebas

1.  Asegúrate de que los archivos de `get_next_line` estén en el directorio padre.
2.  Navega a este directorio (`entorno_pruebas`) en tu terminal.
3.  Ejecuta el script: `./run_tests.sh`

El script realizará lo siguiente:
-   Limpiará los logs antiguos.
-   Compilará `get_next_line` con el programa de prueba para la parte obligatoria.
-   Ejecutará pruebas individuales para diferentes escenarios (archivo vacío, líneas simples/múltiples, línea larga, etc.).
-   Compilará y ejecutará una prueba específica para descriptores de archivo inválidos.
-   Compilará `get_next_line_bonus` con el programa de prueba para la parte bonus.
-   Ejecutará pruebas leyendo de múltiples archivos de forma intercalada.
-   Mostrará los resultados en la terminal usando colores para fácil identificación (Verde para PASSED, Rojo para FAILED).
-   Generará archivos de log detallados en la carpeta `logs/`.
-   Presentará un resumen final del total de pruebas pasadas/ejecutadas.
-   Limpiará los ejecutables de prueba generados.

## Interpretación de los Resultados

-   **Salida en Terminal**: Observa los mensajes de `PASSED` o `FAILED` para cada prueba. El resumen final te dará una visión general.
-   **Archivos de Log (`logs/`)**: Para cualquier prueba `FAILED` (o para inspeccionar el comportamiento detallado), revisa el archivo `.log` correspondiente.
    -   Para las pruebas obligatorias, compara la sección "Comportamiento Esperado" con "Comportamiento Recibido".
    -   Para las pruebas bonus, el log mostrará la secuencia de líneas leídas de cada descriptor de archivo. La verificación de la correcta intercalación es principalmente manual.
    -   Los logs también pueden contener los colores ANSI si se visualizan con `cat` o `less -R`.

## BUFFER_SIZE

Actualmente, las pruebas se compilan con un `BUFFER_SIZE=42` definido en el script `run_tests.sh`. Para probar con otros `BUFFER_SIZE`, puedes modificar esta variable directamente en el script antes de ejecutarlo.
