// tree.h

#pragma once

#include <stdbool.h>

// Структура узла
typedef struct Node
{
    int value;                // Значение, является ключом
    struct Node *first_child; // Указатель на самого левого ребенка
    struct Node *last_child;  // Указатель на самого правого ребенка
    struct Node *next;        // Указатель на следующего соседа
    struct Node *prev;        // Указатель на предыдущего соседа
    struct Node *parent;      // Указатель на родителья
} Node;

Node *CreateNode(int value);

Node *AddChild(Node *parent, int value);

void FreeMemory(Node *root);

bool DeleteSubtree(Node **root, Node *target);
