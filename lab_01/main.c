// main.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> // Нужно для проверки на пробельные символы, функция isspace
#include "tree.h"
#include "tree_service.h"

/*
Функция для очистки буфера ввода.
*/
void ClearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/*
Функция строгого ввода.
Читает целое число и проверяет, что после него нет мусора.
Разрешает только пробельные символы, после целого числа.
Значение, которое удалось считать записывает в перемнную по указателю.
Возвращает true, если число введено корректно, и false, если строка невалидна.
*/
bool GetSafeInt(int *result)
{
    // Если не удалось считать целое число
    if (scanf("%d", result) != 1)
    {
        ClearInputBuffer(); // Очищаем мусор в буфере ввода
        return false;
    }

    int next;
    // Читаем все символы до конца строки, т.к. пробельные разрешены
    while ((next = getchar()) != '\n' && next != EOF)
    {
        // Если попался непробельный символ
        if (!isspace(next))
        {
            ClearInputBuffer();
            return false;
        }
    }

    return true;
}

void MenuAddNode(Node **root)
{
    int value, parent_value;

    if (*root == NULL)
    {
        printf("Дерево пустое. Введите целое число для корня: ");
        if (!GetSafeInt(&value))
        {
            printf("Ошибка: введено нецелое число или текст.\n");
            return;
        }

        *root = CreateNode(value);
        if (!(*root))
        {
            printf("Ошибка: не удалось создать корень.\n");
            return;
        }
        printf("Корень успешно создан.\n");
    }
    else
    {
        printf("Введите значение родителя: ");
        if (!GetSafeInt(&parent_value))
        {
            printf("Ошибка: некорректное значение родителя.\n");
            return;
        }

        printf("Введите значение нового узла: ");
        if (!GetSafeInt(&value))
        {
            printf("Ошибка: некорректное значение нового узла.\n");
            return;
        }

        if (AddChildByValue(*root, parent_value, value))
        {
            printf("Узел успешно добавлен.\n");
        }
        else
        {
            printf("Ошибка: родитель не найден или значение уже существует.\n");
        }
    }
}

/*
Обрамление для печати дерева.
*/
void MenuPrintTree(const Node *root)
{
    if (!root)
    {
        printf("Дерево пустое.\n");
        return;
    }
    printf("\n----- СТРУКТУРА ДЕРЕВА -----\n");
    PrintTree(root, 0, 0ULL);
    printf("----------------------------\n");
}

/*
Меню удаления поддерева.
При удалении всего дерева, значение по указателю перезаписывается.
*/
void MenuDeleteNode(Node **root)
{
    if (!*root)
    {
        printf("Дерево пустое.\n");
        return;
    }
    int value;

    printf("Введите значение узла для удаления (вместе с поддеревом): ");
    if (!GetSafeInt(&value))
    {
        printf("Ошибка: введите целое число.\n");
        return;
    }

    if (DeleteSubtreeByValue(root, value))
    {
        printf("Узел успешно удален.\n");
    }
    else
    {
        printf("Узел с таким значением не найден.\n");
    }
}

/*
Обрамление для вывода степени дерева.
*/
void MenuGetDegree(const Node *root)
{
    if (!root)
    {
        printf("Дерево пустое, степень 0.\n");
        return;
    }
    printf("Степень дерева: %u\n", GetDegree(root));
}

int main()
{
    Node *root = NULL;
    int choice = -1;

    printf("Добро пожаловать в программу работы с деревом!\n");

    while (true)
    {
        printf("\n------ ТЕКСТОВОЕ МЕНЮ ------\n");
        printf("1. Добавить узел\n");
        printf("2. Визуализировать дерево\n");
        printf("3. Удалить узел (поддерево)\n");
        printf("4. Определить степень дерева\n");
        printf("0. Выход\n");
        printf("----------------------------\n");
        printf("Выберите действие: ");

        if (!GetSafeInt(&choice))
        {
            printf("Ошибка: выберите пункт меню (целое число)!\n");
            continue;
        }

        if (choice == 0)
        {
            printf("Завершение работы...\n");
            DeleteSubtree(&root, root);
            break;
        }

        switch (choice)
        {
        case 1:
            MenuAddNode(&root);
            break;
        case 2:
            MenuPrintTree(root);
            break;
        case 3:
            MenuDeleteNode(&root);
            break;
        case 4:
            MenuGetDegree(root);
            break;
        default:
            printf("Неверный пункт меню.\n");
        }
    }

    return 0;
}