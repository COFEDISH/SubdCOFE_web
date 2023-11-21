#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "Save_and_Output.h"
#define MAX_SIZE 100

// ������������� �����
void initialize_stack(Stack* stack) {
    stack->top_stack = -1; // ������������� ������� ����� � -1, ��� ��������� �� ������ ����
}

// �������� �� ������� �����
bool is_empty_stack(Stack* stack) {
    return stack->top_stack == -1; // ���������� true, ���� ���� ����, ����� false
}

// �������� �� ������������ �����
bool is_full_stack(Stack* stack) {
    return stack->top_stack == MAX_SIZE - 1; // ���������� true, ���� ���� ����������, ����� false
}

// ��������� �������� �� ������� �����
void push_stack(Stack* stack, int value) {
    if (is_full_stack(stack)) {
        printf("���� ����������\n"); // ������� ���������, ���� ���� ����������
    }
    else {
        stack->top_stack++;
        stack->elem_stack[stack->top_stack] = value; // �������� ������� �� ������� �����
    }
}

// ���������� �������� � ������� �����
int pop_stack(Stack* stack, const char* file_name, const char* table_name) {
    if (is_empty_stack(stack)) {
        printf("���� ����\n"); // ������� ���������, ���� ���� ����
        return -1;
    }
    else {
        int value = stack->elem_stack[stack->top_stack];
        stack->top_stack--;
        save_stack_to_file(stack, file_name, table_name); // ��������� ���� � ���� ����� ���������� ��������
        return value;
    }
}

// ���������� �������� � ������� ����� ��� ����������
int pop_stack_save(Stack* stack) {
    if (is_empty_stack(stack)) {
        printf("���� ����\n"); // ������� ���������, ���� ���� ����
        return -1;
    }
    else {
        int value = stack->elem_stack[stack->top_stack];
        stack->top_stack--;
        return value;
    }
}

// ��������� �������� � ������� ����� ��� ��� ��������
int top_stack(Stack* stack) {
    if (is_empty_stack(stack)) {
        printf("���� ����\n"); // ������� ���������, ���� ���� ����
        return -1;
    }
    else {
        return stack->elem_stack[stack->top_stack]; // ���������� ������� � ������� �����
    }
}

// ������ �����
void print_stack(Stack* stack) {
    if (is_empty_stack(stack)) {
        printf("���� ����\n"); // ������� ���������, ���� ���� ����
        return;
    }

    printf("����: ");
    while (!is_empty_stack(stack)) {
        int value = pop_stack_save(stack); // ��������� �������� �� ����� ��� ������
        printf("%d ", value); // �������� ����������� ��������
    }
    printf("\n");
}
