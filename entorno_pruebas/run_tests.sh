#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Contadores de pruebas
mandatory_total=0
mandatory_passed=0
error_case_total=0
error_case_passed=0
bonus_total=0
bonus_passed=0

# Limpiar logs antiguos
echo -e "${BLUE}Limpiando logs antiguos...${NC}"
rm -f entorno_pruebas/logs/*.log

echo ""
echo -e "${BLUE}Iniciando pruebas para get_next_line...${NC}"
echo "========================================"

# Compilación de la parte obligatoria
echo -e "${YELLOW}Compilando get_next_line (obligatoria)...${NC}"
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 ../get_next_line.c ../get_next_line_utils.c entorno_pruebas/test_main_mandatory.c -o entorno_pruebas/gnl_tester_mandatory
if [ \$? -eq 0 ]; then
    echo -e "${GREEN}Compilación de la parte obligatoria exitosa.${NC}"
else
    echo -e "${RED}Error en la compilación de la parte obligatoria. Revisa los errores.${NC}"
    # No continuar si la compilación obligatoria falla
    exit 1
fi

echo ""
echo -e "${YELLOW}Ejecutando pruebas obligatorias...${NC}"

# Función para ejecutar una prueba obligatoria
run_mandatory_test() {
    local test_name=\$1
    local file_path=\$2
    shift 2
    local expected_lines=("\$@")

    ((mandatory_total++))
    echo -e "${BLUE}--- Prueba Obligatoria: \$test_name ---${NC}"
    ./entorno_pruebas/gnl_tester_mandatory "\$test_name" "\$file_path" "\${expected_lines[@]}"
    # La comprobación de PASSED/FAILED se basa en el contenido del log generado por el C tester
    if grep -q "${GREEN}PASSED${NC}" "entorno_pruebas/logs/\${test_name}.log"; then
        echo -e "${GREEN}Resultado: PASSED${NC}"
        ((mandatory_passed++))
    else
        echo -e "${RED}Resultado: FAILED${NC}"
        echo "Para más detalles, revisa entorno_pruebas/logs/\${test_name}.log"
    fi
    echo ""
}

# --- Definición de Pruebas Obligatorias ---
run_mandatory_test "empty_file" "entorno_pruebas/test_files/empty_file.txt"
run_mandatory_test "single_line_no_nl" "entorno_pruebas/test_files/single_line.txt" "This is a single line without a newline character at the end."
run_mandatory_test "single_line_with_nl" "entorno_pruebas/test_files/single_line_with_nl.txt" "This is a single line with a newline character at the end."
run_mandatory_test "file_with_no_nl" "entorno_pruebas/test_files/file_with_no_nl.txt" "Last line without newline"
run_mandatory_test "multiple_lines" "entorno_pruebas/test_files/multiple_lines.txt" "First line" "Second line" "Third line"
run_mandatory_test "multiple_nl" "entorno_pruebas/test_files/multiple_nl.txt" "Line 1" "" "" "Line 4"
# Para long_line, la comparación de contenido exacto es compleja aquí.
# El log mostrará el contenido recibido. La prueba contará como 'ejecutada'.
# El C actual lo marcará como FAILED si la string no es "EXPECTED_CONTENT..."
# Para un test real, se debería leer el esperado de un archivo o no pasar string esperada.
run_mandatory_test "long_line" "entorno_pruebas/test_files/long_line.txt" "EXPECTED_CONTENT_FOR_LONG_LINE_NEEDS_HANDLING"


echo ""
echo "========================================"
echo -e "${YELLOW}Compilando get_next_line (bonus)...${NC}"
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 ../get_next_line_bonus.c ../get_next_line_utils_bonus.c entorno_pruebas/test_main_bonus.c -o entorno_pruebas/gnl_tester_bonus
if [ \$? -eq 0 ]; then
    echo -e "${GREEN}Compilación de la parte bonus exitosa.${NC}"
else
    echo -e "${RED}Error en la compilación de la parte bonus. Revisa los errores.${NC}"
    # Decidir si continuar o no. Por ahora, continuaremos y el resumen lo reflejará.
fi

echo ""
echo -e "${YELLOW}Ejecutando pruebas bonus...${NC}"

# Función para ejecutar una prueba bonus
run_bonus_test() {
    local test_name=\$1
    shift
    local files_to_test=("\$@")

    ((bonus_total++))
    echo -e "${BLUE}--- Prueba Bonus: \$test_name ---${NC}"
        ./entorno_pruebas/gnl_tester_bonus "\$test_name" "\${files_to_test[@]}"
        # Para bonus, el C tester escribe "COMPLETED" en el log.
        # La verificación de la correcta intercalación es principalmente manual revisando el log.
        if grep -q "${GREEN}COMPLETED${NC}" "entorno_pruebas/logs/\${test_name}.log"; then
            echo -e "${GREEN}Resultado: COMPLETED (Verifica el log para detalles de intercalación)${NC}"
            ((bonus_passed++))
        else
            echo -e "${RED}Resultado: FAILED o INCOMPLETO${NC}"
            echo "Para más detalles, revisa entorno_pruebas/logs/\${test_name}.log"
        fi
    else
        echo -e "${RED}Ejecutable de prueba bonus no encontrado (falló la compilación?).${NC}"
    fi
    echo ""
}

# --- Definición de Pruebas Bonus ---
run_bonus_test "multiple_fds" "entorno_pruebas/test_files/bonus_file1.txt" "entorno_pruebas/test_files/bonus_file2.txt" "entorno_pruebas/test_files/bonus_file3.txt"
# Se pueden añadir más escenarios de bonus aquí. Por ejemplo, algunos archivos vacíos, otros no, etc.
run_bonus_test "bonus_mix" "entorno_pruebas/test_files/empty_file.txt" "entorno_pruebas/test_files/single_line_with_nl.txt" "entorno_pruebas/test_files/multiple_lines.txt"


echo ""
echo "========================================"
echo -e "${YELLOW}Resumen de las pruebas:${NC}"
echo -e "Pruebas obligatorias: ${GREEN}\$mandatory_passed${NC}/${mandatory_total} pasadas"
echo -e "Pruebas de casos de error: ${GREEN}\$error_case_passed${NC}/${error_case_total} pasadas"
if [ -f ./entorno_pruebas/gnl_tester_bonus ]; then
    echo -e "Pruebas bonus: ${GREEN}\$bonus_passed${NC}/\${bonus_total} completadas (revisar logs para detalle)"
else
    echo -e "Pruebas bonus: ${RED}No ejecutadas (error de compilación)${NC}"
fi
echo "========================================"
echo "Los logs detallados se encuentran en la carpeta 'entorno_pruebas/logs'."
echo -e "${BLUE}Limpiando ejecutables...${NC}"
rm -f entorno_pruebas/gnl_tester_mandatory entorno_pruebas/gnl_tester_bonus entorno_pruebas/gnl_invalid_fd_test
