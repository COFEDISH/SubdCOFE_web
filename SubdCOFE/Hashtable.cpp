#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Hashtable.h"
#include "Save_and_Output.h"
#include <locale.h>
#define SIZE 1000



// ������� ���-�������
int hash_function(const char* key) {
    int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash += key[i];
    }
    return hash % SIZE;
}

// ������������� ���-�������
void initialize_hash(HashTable* hashtable) {
    for (int i = 0; i < SIZE; i++) {
        hashtable->array_hash[i] = NULL;
    }
}

// ������� ��� ������ ����������� �������
void printHashTable(HashTable* hashtable) {
    for (int i = 0; i < SIZE; i++) {
        Node_hash* current = hashtable->array_hash[i];
        printf("Index %d: ", i);
        while (current != NULL) {
            printf("(%s, %s) ", current->key_hash, current->value_hash);
            current = current->next_hash;
        }
        printf("\n");
    }
}

void insert_hash(HashTable* hashtable, const char* key, const char* value) {
    int index = hash_function(key);
    Node_hash* new_node = (Node_hash*)malloc(sizeof(Node_hash));
    new_node->key_hash = _strdup(key);
    new_node->value_hash = _strdup(value);
    new_node->next_hash = NULL;

    if (hashtable->array_hash[index] == NULL) {
        hashtable->array_hash[index] = new_node;
    }
    else {
        Node_hash* current = hashtable->array_hash[index];
        while (current->next_hash != NULL) {
            if (strcmp(current->key_hash, key) == 0) {
                // ���������� ������ �������� � ������ ��� ������������� �����
                Node_hash* new_value_node = (Node_hash*)malloc(sizeof(Node_hash));
                new_value_node->key_hash = _strdup(key);
                new_value_node->value_hash = _strdup(value);
                new_value_node->next_hash = NULL;

                Node_hash* temp = current;
                while (temp->next_hash != NULL) {
                    temp = temp->next_hash;
                }
                temp->next_hash = new_value_node;

                return;
            }
            current = current->next_hash;
        }
        // ���������� ������ ����, ���� ������ ��� ����� ����
        current->next_hash = new_node;
    }

}


// ��������� �������� �� ����� �� ���-�������
const char* getbykey_hash(HashTable* hashtable, const char* key) {
    if (key == NULL || strlen(key) == 0) {
        return NULL; // ���� ���� ���� ��� NULL, ������� NULL
    }

    int index = hash_function(key);
    Node_hash* current = hashtable->array_hash[index];
    while (current != NULL) {
        if (strcmp(current->key_hash, key) == 0) {
            return current->value_hash;
        }
        current = current->next_hash;
    }
    return NULL;
}


// ���������� ����������� ���-������� � ����
void save_table_to_file(HashTable* myTable, const char* file_name, const char* table_name) {
    remove_table_from_file(file_name, table_name);
    FILE* file = fopen(file_name, "a");
    if (file == NULL) {

        return;
    }

    fprintf(file, "Hash Table Data (%s):\n", table_name);
    for (int i = 0; i < SIZE; i++) {
        Node_hash* current = myTable->array_hash[i];
        while (current != NULL) {
            fprintf(file, "%s %s\n", current->key_hash, current->value_hash);
            current = current->next_hash;
        }
    }

    fclose(file);
}

// �������� �������� �� ����� �� ���-�������
void deletebykey_hash(HashTable* hashtable, const char* key) {
    int index = hash_function(key);
    Node_hash* current = hashtable->array_hash[index];
    Node_hash* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key_hash, key) == 0) {
            if (prev == NULL) {
                hashtable->array_hash[index] = current->next_hash;
            }
            else {
                prev->next_hash = current->next_hash;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next_hash;
    }
}



