#include "../get_next_line.h" // Assuming get_next_line.h is in the parent directory
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

void run_test(const char *test_name, const char *file_path, const char *expected_output[], int expected_line_count) {
    int fd;
    char *line;
    int i = 0;
    int pass = 1;
    char log_file_path[256];
    FILE *log_fp;

    sprintf(log_file_path, "logs/%s.log", test_name);
    log_fp = fopen(log_file_path, "w");
    if (!log_fp) {
        perror("Failed to open log file");
        return;
    }

    fprintf(log_fp, "Prueba: %s\n", test_name);
    fprintf(log_fp, "Archivo de entrada: %s\n", file_path);
    fprintf(log_fp, "--- Comportamiento Esperado ---\n");
    for (int k = 0; k < expected_line_count; k++) {
        fprintf(log_fp, "%s\n", expected_output[k] ? expected_output[k] : "(NULL)");
    }
    if (expected_line_count == 0) {
        fprintf(log_fp, "(No se esperan líneas)\n");
    }


    fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, RED "Error abriendo el archivo: %s\n" RESET, file_path);
        fprintf(log_fp, "--- Comportamiento Recibido ---\nError abriendo el archivo.\n");
        fprintf(log_fp, "--- Resultado ---\n%sFAILED%s (No se pudo abrir el archivo)%s\n", RED, YELLOW, RESET);
        fclose(log_fp);
        return;
    }

    fprintf(log_fp, "--- Comportamiento Recibido ---\n");
    printf(YELLOW "\nEjecutando prueba: %s (%s)\n" RESET, test_name, file_path);

    while ((line = get_next_line(fd)) != NULL) {
        printf("Recibido: '%s'\n", line);
        fprintf(log_fp, "%s\n", line);
        if (i < expected_line_count) {
            if (strcmp(line, expected_output[i]) != 0) {
                pass = 0;
                printf(RED "FAIL: Línea %d: Esperado '%s', Recibido '%s'\n" RESET, i + 1, expected_output[i], line);
            }
        } else {
            pass = 0; // Received more lines than expected
            printf(RED "FAIL: Se recibieron más líneas de las esperadas. Recibido: '%s'\n" RESET, line);
        }
        free(line);
        i++;
    }
    // Check if gnl returned a final NULL for the last line if it was not empty
    if (i > 0 && line == NULL){
         // This is expected if there was content and then EOF
         fprintf(log_fp, "(NULL) (EOF)\n");
    } else if (i == 0 && line == NULL && expected_line_count == 0) {
        // Correct for empty file: gnl should return NULL immediately
        fprintf(log_fp, "(NULL) (EOF)\n");
    } else if (line != NULL) { // Should be NULL at the end
        pass = 0;
        printf(RED "FAIL: get_next_line no retornó NULL al final.\n" RESET);
        fprintf(log_fp, "Error: get_next_line no retornó NULL al final.\n");
    }


    if (i < expected_line_count) {
        pass = 0;
        printf(RED "FAIL: Se esperaban %d líneas, pero se recibieron %d.\n" RESET, expected_line_count, i);
        fprintf(log_fp, "Error: Se esperaban %d líneas, pero se recibieron %d.\n", expected_line_count, i);
    }

    close(fd);

    fprintf(log_fp, "--- Resultado ---\n");
    if (pass) {
        printf(GREEN "PASS: %s\n" RESET, test_name);
        fprintf(log_fp, "%sPASSED%s\n", GREEN, RESET);
    } else {
        printf(RED "FAILED: %s\n" RESET, test_name);
        fprintf(log_fp, "%sFAILED%s\n", RED, RESET);
    }
    fclose(log_fp);
}

// Este archivo contendrá la lógica principal de una prueba.
// Se compilará por separado para cada caso de prueba específico,
// o un main() separado llamará a run_test() con diferentes parámetros.

// Ejemplo de cómo podría ser un main para una prueba específica:
/*
int main(void) {
    const char *expected[] = {"Primera linea", "Segunda linea"};
    run_test("nombre_de_la_prueba", "test_files/mi_archivo.txt", expected, 2);
    return 0;
}
*/

// Por ahora, dejaremos el main vacío o con un ejemplo simple.
// El script run_tests.sh se encargará de compilar esto con mains especificos o
// crear un main general que llame a run_test para cada caso.

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <nombre_prueba> <ruta_archivo_prueba> [lineas_esperadas...]\n", argv[0]);
        return 1;
    }
    const char *test_name = argv[1];
    const char *file_path = argv[2];
    // El resto de argv son las líneas esperadas
    // Esta es una forma simplificada; idealmente, las líneas esperadas se leerían de otra fuente
    // o se pasarían de una manera más estructurada.
    // Por simplicidad, el script run_tests.sh pasará strings directamente.

    int expected_line_count = argc - 3;
    const char **expected_lines = malloc(expected_line_count * sizeof(char *));
    if (!expected_lines && expected_line_count > 0) {
        perror("Malloc failed for expected_lines");
        return 1;
    }

    for (int i = 0; i < expected_line_count; i++) {
        expected_lines[i] = argv[i + 3];
    }

    // Para el caso de archivo vacío, donde no hay expected_lines en argv
    if (expected_line_count == 0 && argc == 3) {
         run_test(test_name, file_path, NULL, 0);
    } else {
        run_test(test_name, file_path, expected_lines, expected_line_count);
    }

    if (expected_lines) {
        free(expected_lines);
    }

    return 0;
}
