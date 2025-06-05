#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcmp

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42 // Default BUFFER_SIZE if not provided by compiler flag
#endif

// Helper to print test results consistently
void run_and_print_test(int fd, const char *expected_line, const char *test_name) {
    char *line = get_next_line(fd);
    if (line == NULL && expected_line == NULL) {
        printf("[PASS] %s: Expected NULL, Got NULL\n", test_name);
    } else if (line == NULL && expected_line != NULL) {
        printf("[FAIL] %s: Expected \"%s\", Got NULL\n", test_name, expected_line);
    } else if (line != NULL && expected_line == NULL) {
        printf("[FAIL] %s: Expected NULL, Got \"%s\"\n", test_name, line);
    } else if (strcmp(line, expected_line) == 0) {
        printf("[PASS] %s: Expected \"%s\", Got \"%s\"\n", test_name, expected_line, line);
    } else {
        // Print with explicit newlines for clarity
        char clean_expected[100] = {0};
        char clean_line[100] = {0};
        // Use strncpy to prevent buffer overflows if lines are too long
        if (expected_line) strncpy(clean_expected, expected_line, 99);
        if (line) strncpy(clean_line, line, 99);

        for(int i=0; clean_expected[i] && i<99; i++) if(clean_expected[i]=='\n') clean_expected[i]='N';
        for(int i=0; clean_line[i] && i<99; i++) if(clean_line[i]=='\n') clean_line[i]='N';

        printf("[FAIL] %s: Expected (esc) \"%s\", Got (esc) \"%s\"\n", test_name, clean_expected, clean_line);
    }
    if (line) {
        free(line);
    }
}

int main(void) {
    int fd1, fd4, fd5, fd6, fd7, fd8, fd9, fd10; // Removed fd2, fd3

    printf("--- GNL Bonus Test Suite --- MAX_FD: %d, BUFFER_SIZE: %d ---\n", MAX_FD, BUFFER_SIZE);

    // Test Case 5 (Original problematic one)
    printf("\n--- Test Case 5: Single Newline (test5.txt) ---\n");
    fd5 = open("test5.txt", O_RDONLY);
    if (fd5 == -1) { perror("Error opening test5.txt"); return 1; }
    run_and_print_test(fd5, "\n", "test5.txt, Line 1");
    run_and_print_test(fd5, NULL, "test5.txt, EOF");
    close(fd5);

    // Test Case 6: Two Newlines
    printf("\n--- Test Case 6: Two Newlines (test6_two_nl.txt) ---\n");
    fd6 = open("test6_two_nl.txt", O_RDONLY);
    if (fd6 == -1) { perror("Error opening test6_two_nl.txt"); return 1; }
    run_and_print_test(fd6, "\n", "test6.txt, Line 1");
    run_and_print_test(fd6, "\n", "test6.txt, Line 2");
    run_and_print_test(fd6, NULL, "test6.txt, EOF");
    close(fd6);

    // Test Case 7: "a\n"
    printf("\n--- Test Case 7: 'a\\n' (test7_a_nl.txt) ---\n");
    fd7 = open("test7_a_nl.txt", O_RDONLY);
    if (fd7 == -1) { perror("Error opening test7_a_nl.txt"); return 1; }
    run_and_print_test(fd7, "a\n", "test7.txt, Line 1");
    run_and_print_test(fd7, NULL, "test7.txt, EOF");
    close(fd7);

    // Test Case 8: "\na"
    printf("\n--- Test Case 8: '\\na' (test8_nl_a.txt) ---\n");
    fd8 = open("test8_nl_a.txt", O_RDONLY);
    if (fd8 == -1) { perror("Error opening test8_nl_a.txt"); return 1; }
    run_and_print_test(fd8, "\n", "test8.txt, Line 1");
    run_and_print_test(fd8, "a", "test8.txt, Line 2"); // Assuming GNL returns "a" if no trailing NL
    run_and_print_test(fd8, NULL, "test8.txt, EOF");
    close(fd8);

    // Test Case 9: Multiple short lines (a\nb\nc\n)
    printf("\n--- Test Case 9: Multi-short lines (test9_multiple_lines_short.txt) ---\n");
    fd9 = open("test9_multiple_lines_short.txt", O_RDONLY);
    if (fd9 == -1) { perror("Error opening test9.txt"); return 1; }
    run_and_print_test(fd9, "a\n", "test9.txt, Line 1");
    run_and_print_test(fd9, "b\n", "test9.txt, Line 2");
    run_and_print_test(fd9, "c\n", "test9.txt, Line 3");
    run_and_print_test(fd9, NULL, "test9.txt, EOF");
    close(fd9);

    // Test Case 10: Long line, no newline (42 chars)
    printf("\n--- Test Case 10: Long line no NL (test10_long_no_nl.txt) ---\n");
    fd10 = open("test10_long_no_nl.txt", O_RDONLY);
    if (fd10 == -1) { perror("Error opening test10.txt"); return 1; }
    run_and_print_test(fd10, "123456789012345678901234567890123456789012", "test10.txt, Line 1");
    run_and_print_test(fd10, NULL, "test10.txt, EOF");
    close(fd10);

    // Re-test original fd1 and fd4 for good measure (multi-FD check)
    printf("\n--- Test Case 1 & 4: Interleaved (test1.txt, test4.txt) ---\n");
    fd1 = open("test1.txt", O_RDONLY);
    fd4 = open("test4.txt", O_RDONLY);
    if (fd1 == -1 || fd4 == -1) { perror("Error opening test1/test4.txt"); return 1; }
    run_and_print_test(fd1, "line 1 file 1\n", "test1.txt, Line 1");
    run_and_print_test(fd4, "single line file 4", "test4.txt, Line 1");
    run_and_print_test(fd1, "line 2 file 1\n", "test1.txt, Line 2");
    run_and_print_test(fd4, NULL, "test4.txt, EOF"); // fd4 is now EOF
    run_and_print_test(fd1, "line 3 file 1\n", "test1.txt, Line 3");
    run_and_print_test(fd1, NULL, "test1.txt, EOF"); // fd1 is now EOF
    close(fd1);
    close(fd4);

    printf("\n--- Test Suite Complete ---\n");
    return 0;
}
