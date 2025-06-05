#include "../get_next_line.h" // Adjust path as necessary
#include <stdio.h>
#include <stdlib.h> // For free()

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

int main(void) {
    char *line;
    int invalid_fd = -1; // A commonly invalid FD
    int very_high_fd = 9999; // Another FD that's likely invalid

    printf(YELLOW "Ejecutando prueba: Invalid File Descriptors\n" RESET);

    // Test with -1
    line = get_next_line(invalid_fd);
    if (line == NULL) {
        printf(GREEN "PASS: get_next_line(-1) retornó NULL como esperado.\n" RESET);
    } else {
        printf(RED "FAIL: get_next_line(-1) debería haber retornado NULL, pero retornó: %s\n" RESET, line);
        free(line);
        return 1; // Indicate failure
    }

    // Test with a very high FD
    line = get_next_line(very_high_fd);
    if (line == NULL) {
        printf(GREEN "PASS: get_next_line(9999) retornó NULL como esperado.\n" RESET);
    } else {
        printf(RED "FAIL: get_next_line(9999) debería haber retornado NULL, pero retornó: %s\n" RESET, line);
        free(line);
        return 1; // Indicate failure
    }

    // Test with a closed FD (more complex to set up here without error, would need to open then close)
    // For now, focusing on inherently invalid FDs.

    return 0; // Indicate success
}
