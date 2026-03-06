// tree_service.c

#include "tree_service.h"
#include <stdlib.h>
#include <stdio.h>

/*
Получить степень дерева.
Степень дерева - максимальная из степеней узлов.
Степень узлов - количество прямых потомков.
Вызывается рекурсивно в глубину.
*/
unsigned int GetDegree(const Node *target)
{
    if (!target)
        return 0;

    unsigned int max_degree = 0;

    unsigned int count = 0;
    for (const Node *child = target->first_child; child; child = child->next)
    {
        count++;
        unsigned int child_degree = GetDegree(child);
        if (child_degree > max_degree)
        {
            max_degree = child_degree;
        }
    }

    if (count > max_degree)
    {
        max_degree = count;
    }

    return max_degree;
}

/*
Поиск в глубину (DFS) по значению в дереве.
Возвращает указатель на подходящий узел.
*/
Node *FindByValue(Node *root, int value)
{
    if (!root)
    {
        return NULL;
    }

    if (root->value == value)
    {
        return root;
    }

    for (Node *child = root->first_child; child; child = child->next)
    {
        Node *found = FindByValue(child, value);
        if (found)
        {
            return found;
        }
    }
    return NULL;
}

/*
Добавить к узлу с указанным значением нового ребенка с заданным значением.
Выполняет поиск родительского узла по значениею.
Затем выполняет проверку в дереве на существование узла с таким же значеним.
Возвращает ссылку на созданный узел.
*/
Node *AddChildByValue(Node *root, int parent_value, int value)
{

    if (!root)
    {
        return NULL;
    }

    Node *parent = FindByValue(root, parent_value);

    if (!parent)
    {
        return NULL;
    }

    if (FindByValue(root, value))
    {
        return NULL;
    }

    Node *new_node = AddChild(parent, value);

    return new_node;
}

/*
Удалить поддерево по заданному значению.
Принимает указатель(2) на корень дерева и значение корня удаляемого поддерева.
Возвращает true, если удалось успешно удалить поддерево, иначе false.
При удалении всего дерева, в указатель записывается NULL.
*/
bool DeleteSubtreeByValue(Node **root, int value)
{
    Node *target = FindByValue(*root, value);

    return DeleteSubtree(root, target);
}

/*
Красивая печать дерева в консоли.
Выполняется рекурсивно, за каждый вызов печатает один уровень.
Принимает ссылку на корень печатаемого поддерева,
текущую глубину (влияет на количесвто отступов) и
битовую маску (bitset), которая хранит состояния по уровням:
законсилась ли на этом уровне ветка, или еще тянется, а также
состояние является ли печатамый узел последним ребенком или нет.
*/
void PrintTree(const Node *root, int depth, unsigned long long int mask)
{
    if (!root)
    {
        return;
    }

    // Рисуем отступы и вертикальные  линии предков, т.е.
    // уровни от корня и до дедушки
    for (int i = 0; i < depth - 1; i++)
    {
        if (i < 64) // Ограничение маски в 64 бита
        {
            // Проверяем i-ый бит маски
            // Если бит = 1, то на этом уровне ветка закончилась => будут пробелы
            if (mask & (1ULL << i))
            {
                printf("    ");
            }
            // Если бит = 0, то ветка на этом уровне продолжается => нужна линия
            else
            {
                printf("|   ");
            }
        }
        // Защита от переполнения глубины. Если глубина > 64, то
        // печатаем линию, независимо от того закончилась ветка или нет
        else
        {
            printf("|   ");
        }
    }

    // Рисуем соединитель, перед самим печатаемым элементом, т.е.
    // уровень родителя
    if (depth > 0) // Если это не корень
    {
        if ((depth - 1) < 64) // Ограничение маски в 64 бита
        {
            // Проверяем бит текущего уровня (depth - 1)
            // Если бит 1, то это последний ребенок
            if (mask & (1ULL << (depth - 1)))
            {
                printf("+-- ");
            }
            // Если бит 0, то ветка продолжается дальше
            else
            {
                printf("|-- ");
            }
        }
        // Больше обозримой глубины, просто печатаем линию
        else
        {
            printf("|-- ");
        }
    }
    // Печатам значение узла
    printf("%d\n", root->value);

    // Рекурсимный переход к детям
    const Node *child = root->first_child;
    while (child)
    {
        // Копируем текущую маску для ребенка
        unsigned long long int new_mask = mask;
        if (depth < 64) // Ограничение маски в 64 бита
        {
            // Если у текущего ребенка нет соседа справа, то
            // продолжать ветку не нужно, т.е. на этом уровне будут пробелы
            if (!child->next)
            {
                new_mask |= (1ULL << depth);
            }
        }
        PrintTree(child, depth + 1, new_mask);
        child = child->next;
    }
}
