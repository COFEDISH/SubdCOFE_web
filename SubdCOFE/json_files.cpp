#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* extract_json_data(char* request) {
    char* json_start = strstr(request, "{"); // ������� ������ ������ JSON
    if (json_start == NULL) {
        printf("������ JSON �� �������");
        return NULL;
    }

    char* json_end = strrchr(request, '}') + 1; // ������� ����� ������ JSON

    int json_length = json_end - json_start;
    char* json_data = (char*)malloc(json_length + 1); // �������� ������ ��� ������ JSON (+1 ��� ������������ ����)
    if (json_data == NULL) {
        printf("������ ��������� ������");
        return NULL;
    }

    strncpy(json_data, json_start, json_length); // �������� ������ JSON � ��������� �����
    json_data[json_length] = '\0'; // ������������� ����������� ����

    return json_data;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_json_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // ���� �� ����������, ������� ����� ���� � ���������� � ���� ������ ������ JSON
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("������ �������� �����");
            return NULL;
        }
        fprintf(file, "[{}]"); // ����� ������ ������ JSON
        fclose(file);

        // �������� ��������� ���� ��� ������
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("������ �������� ����� ��� ������");
            return NULL;
        }
    }

    // ��������� ������ ��� �������� ����������� �����
    size_t buffer_size = 1024; // ��������� ������ ������
    char* buffer = (char*)malloc(buffer_size * sizeof(char));
    buffer[0] = '\0'; // ��������� �������� - ������ ������

    char line[1024]; // ����� ��� ������ ������ �� �����
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t line_len = strlen(line);

        // �������� � ���������� ������� ������, ���� ����������
        while (strlen(buffer) + line_len >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = (char*)realloc(buffer, buffer_size * sizeof(char));
        }

        // �������� �������� ����� ������ � ��������� �� ������ � ���������� � ������
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
        printf("������ �������� ����� ��� ������");
        return;
    }
    // ���������� ������� "[" � ������ ������
    fputc('[', file);
    fputc('\n', file);
    // ������������ ����� ������ � ����
    int i = 0;
    while (json_data[i] != '\0') {
        fputc(json_data[i], file);
        if (json_data[i] == '}' || json_data[i] == ',') {
            // ������� ������ � ���������� ������� ����� ������ ����������� �������� ������ ��� �������
            fputc('\n', file);
            fputc('\t', file);
        }
        i++;
    }
    // ���������� ������� "]" � ����� ������
    fputc('\n', file);
    fputc(']', file);
    fclose(file);
}
