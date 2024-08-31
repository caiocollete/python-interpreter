#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Adicionada para usar isdigit

#define MAX_VAR_COUNT 100
#define MAX_VAR_NAME_LEN 50
#define MAX_LINE_LEN 256

typedef struct {
    char name[MAX_VAR_NAME_LEN];
    int value;
} Variable;

Variable variables[MAX_VAR_COUNT];
int var_count = 0;

void set_variable(char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    strcpy(variables[var_count].name, name);
    variables[var_count].value = value;
    var_count++;
}

int get_variable(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return 0; // Retorna 0 se a variável não for encontrada
}

int get_value(char *token) {
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
        return atoi(token);
    } else {
        return get_variable(token);
    }
}

void execute_line(char *line) {
    if (strncmp(line, "print(", 6) == 0) {
        char content[MAX_LINE_LEN];
        sscanf(line + 6, "%[^)]", content);
        if (content[0] == '"' && content[strlen(content) - 1] == '"') {
            content[strlen(content) - 1] = '\0'; // Remove o último "
            printf("%s\n", content + 1); // Remove o primeiro "
        } else {
            printf("%d\n", get_value(content));
        }
    } else if (strstr(line, " + ") != NULL) {
        char var1[MAX_VAR_NAME_LEN], var2[MAX_VAR_NAME_LEN], result[MAX_VAR_NAME_LEN];
        sscanf(line, "%s = %s + %s", result, var1, var2);
        set_variable(result, get_value(var1) + get_value(var2));
    } else if (strstr(line, " - ") != NULL) {
        char var1[MAX_VAR_NAME_LEN], var2[MAX_VAR_NAME_LEN], result[MAX_VAR_NAME_LEN];
        sscanf(line, "%s = %s - %s", result, var1, var2);
        set_variable(result, get_value(var1) - get_value(var2));
    } else {
        char var_name[MAX_VAR_NAME_LEN];
        int value;
        sscanf(line, "%s = %d", var_name, &value);
        set_variable(var_name, value);
    }
}

void execute_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        execute_line(line);
    }

    fclose(file);
}

int main() {
    const char *filename = "tentativagpt2.txt"; // Nome do arquivo especificado diretamente no código
    execute_file(filename);
    return EXIT_SUCCESS;
}
