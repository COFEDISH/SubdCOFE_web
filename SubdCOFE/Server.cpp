#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "query.h"
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 6379
#define MAX_REQUESTS 100 // Максимальное количество запросов
#define MAX_REQUEST_LENGTH 110000 // Максимальная длина запроса

HANDLE mutex;
char requests[MAX_REQUESTS][MAX_REQUEST_LENGTH]; // Массив для хранения запросов
int current_request_index = 0;

bool contains_russian(const char* query) {
    while (*query) {
        unsigned char c = (unsigned char)(*query);
        if ((c >= 0xD0 && c <= 0xDF) || (c >= 0xE0 && c <= 0xEF)) {
            unsigned char next = (unsigned char)(*(query + 1));
            if ((next >= 0x80 && next <= 0xBF))
                return true; // символ является частью русского алфавита в UTF-8
        }
        ++query;
    }
    return false; // нет русских символов
}


DWORD WINAPI ClientHandler(void* socket) {
    SOCKET client_socket = *(SOCKET*)socket;
    char* recv_buffer = (char*)malloc(MAX_REQUEST_LENGTH * sizeof(char)); // Выделение памяти под запрос
    int recv_size;

    while ((recv_size = recv(client_socket, recv_buffer, strlen(recv_buffer), 0)) != SOCKET_ERROR) {
        recv_buffer[recv_size] = '\0';

        WaitForSingleObject(mutex, INFINITE);
        char* dynamic_request = (char*)malloc((recv_size + 1) * sizeof(char)); // Выделение памяти для хранения запроса
        strcpy(dynamic_request, recv_buffer); // Копирование запроса в новую динамическую память
        strcpy_s(requests[current_request_index], dynamic_request); // Сохранение запроса в массиве
        free(dynamic_request); // Освобождение памяти для хранения запроса
        current_request_index = (current_request_index + 1) % MAX_REQUESTS; // Переход к следующему запросу
        ReleaseMutex(mutex);
        printf("%s", recv_buffer);
        if (!contains_russian(recv_buffer)) {
            const char* answer = get_to_subd(requests[current_request_index - 1]); // Обработка последнего полученного запроса
            if (answer != "Is not exist" and answer != "Invalid query format.") send(client_socket, answer, strlen(answer), 0);
            else continue;
        }
        else {
            continue;
        }
    }

    free(recv_buffer); // Освобождение памяти, выделенной под запрос
    closesocket(client_socket); // Закрытие сокета после обработки

    return 0;
}


int main() {

    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    DWORD thread;
    mutex = CreateMutex(NULL, FALSE, NULL);

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    printf("Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.0.105");
    server.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }

    puts("Bind done");

    if (listen(s, 3) == SOCKET_ERROR) {
        printf("Listen failed with error code : %d", WSAGetLastError());
        return 1;
    }

    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);

    while (new_socket = accept(s, (struct sockaddr*)&client, &c)) {
        printf("\nConnection accepted\n");
        CreateThread(NULL, 0, ClientHandler, &new_socket, 0, &thread);
        // closesocket(new_socket); // Закрытие сокета после обработки
    }
    closesocket(new_socket); // Закрытие сокета после завершения обработки всех запросов



    if (new_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
