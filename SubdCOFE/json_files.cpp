#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* extract_json_data(char* request) {
    char* json_start = strstr(request, "{"); // Находим начало данных JSON
    if (json_start == NULL) {
        printf("Данные JSON не найдены");
        return NULL;
    }

    char* json_end = strrchr(request, '}') + 1; // Находим конец данных JSON

    int json_length = json_end - json_start;
    char* json_data = (char*)malloc(json_length + 1); // Выделяем память под данные JSON (+1 для завершающего нуля)
    if (json_data == NULL) {
        printf("Ошибка выделения памяти");
        return NULL;
    }

    strncpy(json_data, json_start, json_length); // Копируем данные JSON в отдельный буфер
    json_data[json_length] = '\0'; // Устанавливаем завершающий нуль

    return json_data;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_json_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Файл не существует, создаем новый файл и записываем в него пустую строку JSON
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Ошибка создания файла");
            return NULL;
        }
        fprintf(file, "[{}]"); // Пишем пустую строку JSON
        fclose(file);

        // Повторно открываем файл для чтения
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Ошибка открытия файла для чтения");
            return NULL;
        }
    }

    // Выделение памяти для хранения содержимого файла
    size_t buffer_size = 1024; // начальный размер буфера
    char* buffer = (char*)malloc(buffer_size * sizeof(char));
    buffer[0] = '\0'; // начальное значение - пустая строка

    char line[1024]; // буфер для чтения строки из файла
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t line_len = strlen(line);

        // Проверка и увеличение размера буфера, если необходимо
        while (strlen(buffer) + line_len >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = (char*)realloc(buffer, buffer_size * sizeof(char));
        }

        // Удаление символов новой строки и табуляции из строки и добавление к буферу
        for (size_t i = 0; i < line_len; ++i) {
            if (line[i] != '\n' && line[i] != '\t') {
                strncat(buffer, &line[i], 1);
            }
        }
    }

    fclose(file);
    return buffer;
}


void save_json_to_file(const char* json_data, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи");
        return;
    }
    // Добавление символа "[" в начале данных
    fputc('[', file);
    fputc('\n', file);
    // Посимвольный вывод данных в файл
    int i = 0;
    while (json_data[i] != '\0') {
        fputc(json_data[i], file);
        if (json_data[i] == '}' || json_data[i] == ',') {
            // Перенос строки и добавление отступа после каждой закрывающей фигурной скобки или запятой
            fputc('\n', file);
            fputc('\t', file);
        }
        i++;
    }
    // Добавление символа "]" в конце данных
    fputc('\n', file);
    fputc(']', file);
    fclose(file);
}
