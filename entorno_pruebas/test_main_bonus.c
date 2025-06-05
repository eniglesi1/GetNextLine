#include "../get_next_line_bonus.h" // Assuming get_next_line_bonus.h is in the parent directory
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For close()

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

const char *colors[] = {BLUE, MAGENTA, CYAN, YELLOW};
const int num_colors = sizeof(colors) / sizeof(colors[0]);

typedef struct {
    int fd;
    char *path;
    int lines_read;
    int closed;
} FileData;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nombre_prueba_bonus> <ruta_archivo1> <ruta_archivo2> ... <ruta_archivoN>\n", argv[0]);
        return 1;
    }

    const char *test_name = argv[1];
    int num_files = argc - 2;
    FileData *files = malloc(num_files * sizeof(FileData));
    char log_file_path[256];
    FILE *log_fp;

    if (!files) {
        perror("Failed to allocate memory for files");
        return 1;
    }

    sprintf(log_file_path, "logs/%s.log", test_name);
    log_fp = fopen(log_file_path, "w");
    if (!log_fp) {
        perror("Failed to open log file for bonus test");
        free(files);
        return 1;
    }

    fprintf(log_fp, "Prueba Bonus: %s\n", test_name);
    fprintf(log_fp, "Archivos de entrada:\n");
    for (int i = 0; i < num_files; i++) {
        files[i].path = argv[i + 2];
        files[i].fd = open(files[i].path, O_RDONLY);
        files[i].lines_read = 0;
        files[i].closed = 0;
        if (files[i].fd < 0) {
            fprintf(stderr, RED "Error abriendo el archivo: %s\n" RESET, files[i].path);
            fprintf(log_fp, "%s (Error al abrir)\n", files[i].path);
            // Close already opened files
            for (int j = 0; j < i; j++) {
                if (files[j].fd >= 0) close(files[j].fd);
            }
            free(files);
            fclose(log_fp);
            return 1;
        }
        fprintf(log_fp, "- %s (fd: %d)\n", files[i].path, files[i].fd);
    }

    printf(YELLOW "\nEjecutando prueba Bonus: %s\n" RESET, test_name);
    fprintf(log_fp, "--- Comportamiento Recibido (Lectura Intercalada) ---\n");

    int files_remaining = num_files;
    int current_file_idx = 0;
    char *line;

    while (files_remaining > 0) {
        if (files[current_file_idx].closed) {
            current_file_idx = (current_file_idx + 1) % num_files;
            continue;
        }

        line = get_next_line(files[current_file_idx].fd);
        const char* color = colors[current_file_idx % num_colors];

        if (line != NULL) {
            printf("%s[FD:%d %s] Recibido: '%s'%s\n", color, files[current_file_idx].fd, files[current_file_idx].path, line, RESET);
            fprintf(log_fp, "[FD:%d %s] %s\n", files[current_file_idx].fd, files[current_file_idx].path, line);
            files[current_file_idx].lines_read++;
            free(line);
        } else {
            // End of this file or error
            printf("%s[FD:%d %s] get_next_line retornó NULL (EOF o error).%s\n", color, files[current_file_idx].fd, files[current_file_idx].path, RESET);
            fprintf(log_fp, "[FD:%d %s] (NULL) (EOF o error)\n", files[current_file_idx].fd, files[current_file_idx].path);
            close(files[current_file_idx].fd);
            files[current_file_idx].closed = 1;
            files_remaining--;
        }
        current_file_idx = (current_file_idx + 1) % num_files;
        if (files_remaining == 0) break; // Exit if all files are processed
    }

    printf(GREEN "\nPrueba Bonus '%s' completada. Verifique el log: %s\n" RESET, test_name, log_file_path);
    fprintf(log_fp, "--- Resultado ---\n");
    // Basic check: ensure all files were attempted to be read.
    // A more sophisticated check would compare against expected interleaved output.
    // For now, manual log inspection is key for bonus tests.
    fprintf(log_fp, "%sCOMPLETED%s (Revisar el log para la secuencia detallada de lectura)%s\n", GREEN, YELLOW, RESET);

    // Free resources
    free(files);
    fclose(log_fp);

    return 0;
}
