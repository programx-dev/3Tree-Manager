#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static const int INFINITY_WEIGHT = INT_MAX;
static const int NO_INDEX = -1;

// Неориентированный взвешенный граф в форме матрицы инцидентности
typedef struct
{
    int vertex_count;
    int edge_count;
    int **incidence_matrix; // Размерность [vertex_count][edge_count]
    int *edge_weights;
} Graph;

Graph *CreateGraph(int vertex_count, int edge_count)
{
    if (vertex_count <= 0 || edge_count <= 0)
    {
        return NULL;
    }

    Graph *graph = malloc(sizeof(Graph));
    if (!graph)
    {
        return NULL;
    }
    graph->vertex_count = vertex_count;
    graph->edge_count = edge_count;

    graph->incidence_matrix = malloc((size_t)vertex_count * sizeof(int *));
    if (!graph->incidence_matrix)
    {
        free(graph);
        return NULL;
    }

    for (int i = 0; i < vertex_count; ++i)
    {
        graph->incidence_matrix[i] = calloc((size_t)edge_count, sizeof(int));
        if (!graph->incidence_matrix[i])
        {
            for (int k = 0; k < i; ++k)
            {
                free(graph->incidence_matrix[k]);
            }
            free(graph->incidence_matrix);
            free(graph);
            return NULL;
        }
    }

    graph->edge_weights = malloc((size_t)edge_count * sizeof(int));
    if (!graph->edge_weights)
    {
        for (int i = 0; i < vertex_count; ++i)
        {
            free(graph->incidence_matrix[i]);
        }
        free(graph->incidence_matrix);
        free(graph);
        return NULL;
    }

    return graph;
}

void DestroyGraph(Graph *graph)
{
    if (!graph)
    {
        return;
    }
    for (int i = 0; i < graph->vertex_count; ++i)
    {
        free(graph->incidence_matrix[i]);
    }
    free(graph->incidence_matrix);
    free(graph->edge_weights);
    free(graph);
}

void SetEdge(Graph *graph, int edge_index, int u, int v, int weight)
{
    graph->incidence_matrix[u][edge_index] = 1;
    graph->incidence_matrix[v][edge_index] = 1;
    graph->edge_weights[edge_index] = weight;
}

// Находит вершины - концы ребра, просматривая столбец в матрице инцидентности
void GetEdgeEndpoints(const Graph *graph, int edge_index, int *u, int *v)
{
    *u = NO_INDEX;
    *v = NO_INDEX;
    for (int i = 0; i < graph->vertex_count; ++i)
    {
        if (graph->incidence_matrix[i][edge_index] != 1)
        {
            continue;
        }
        if (*u == NO_INDEX)
        {
            *u = i;
        }
        else
        {
            *v = i;
            return;
        }
    }
}

void PrintIncidenceMatrix(const Graph *graph)
{
    printf("Матрица инцидентности (%d вершин, %d рёбер):\n",
           graph->vertex_count, graph->edge_count);
    printf("      ");
    for (int e = 0; e < graph->edge_count; ++e)
    {
        printf("e%-3d", e + 1);
    }
    printf("\n");
    for (int i = 0; i < graph->vertex_count; ++i)
    {
        printf("v%-4d ", i + 1);
        for (int e = 0; e < graph->edge_count; ++e)
        {
            printf("%-4d", graph->incidence_matrix[i][e]);
        }
        printf("\n");
    }
    printf("w:    ");
    for (int e = 0; e < graph->edge_count; ++e)
    {
        printf("%-4d", graph->edge_weights[e]);
    }
    printf("\n");
}

// Алгоритм Прима для минимального остовного дерева
// Возвращает суммарный вес минимального остовного дерева (МОД) или -1 при ошибке или несвязном графе
int PrimMST(const Graph *graph)
{
    int n = graph->vertex_count;

    int *in_mst = calloc((size_t)n, sizeof(int));
    int *min_weight = malloc((size_t)n * sizeof(int));
    int *parent_edge = malloc((size_t)n * sizeof(int));

    if (!in_mst || !min_weight || !parent_edge)
    {
        free(in_mst);
        free(min_weight);
        free(parent_edge);
        fprintf(stderr, "Ошибка выделения памяти\n");
        return -1;
    }

    for (int i = 0; i < n; ++i)
    {
        min_weight[i] = INFINITY_WEIGHT;
        parent_edge[i] = NO_INDEX;
    }
    min_weight[0] = 0;

    int total_weight = 0;

    for (int step = 0; step < n; ++step)
    {
        int u = NO_INDEX;
        for (int i = 0; i < n; ++i)
        {
            if (!in_mst[i] && (u == NO_INDEX || min_weight[i] < min_weight[u]))
            {
                u = i;
            }
        }

        if (u == NO_INDEX || min_weight[u] == INFINITY_WEIGHT)
        {
            printf("Граф несвязный.\n");
            free(in_mst);
            free(min_weight);
            free(parent_edge);
            return -1;
        }

        in_mst[u] = 1;
        total_weight += min_weight[u];

        for (int e = 0; e < graph->edge_count; ++e)
        {
            if (graph->incidence_matrix[u][e] != 1)
            {
                continue;
            }
            int a;
            int b;
            GetEdgeEndpoints(graph, e, &a, &b);
            int neighbor = (a == u) ? b : a;
            if (neighbor == NO_INDEX || in_mst[neighbor])
            {
                continue;
            }
            if (graph->edge_weights[e] < min_weight[neighbor])
            {
                min_weight[neighbor] = graph->edge_weights[e];
                parent_edge[neighbor] = e;
            }
        }
    }

    printf("Минимальное остовное дерево:\n");
    for (int i = 1; i < n; ++i)
    {
        if (parent_edge[i] == NO_INDEX)
        {
            continue;
        }
        int a;
        int b;
        GetEdgeEndpoints(graph, parent_edge[i], &a, &b);
        printf("  v%d - v%d  (вес %d)\n", a + 1, b + 1, graph->edge_weights[parent_edge[i]]);
    }
    printf("Суммарный вес минимального остовного дерева: %d\n", total_weight);

    free(in_mst);
    free(min_weight);
    free(parent_edge);
    return total_weight;
}

int ReadGraph(Graph **out_graph)
{
    printf("Введите количество вершин и рёбер: ");
    int vertex_count;
    int edge_count;
    if (scanf("%d %d", &vertex_count, &edge_count) != 2 ||
        vertex_count < 2 || edge_count < 1)
    {
        fprintf(stderr, "Ошибка: необходимо не менее 2 вершин и 1 ребра.\n");
        return 0;
    }

    Graph *graph = CreateGraph(vertex_count, edge_count);
    if (!graph)
    {
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        return 0;
    }

    printf("Введите рёбра в формате: u v вес (вершины нумеруются от 1 до %d)\n", vertex_count);
    for (int e = 0; e < edge_count; ++e)
    {
        printf("  Ребро %d: ", e + 1);
        int u;
        int v;
        int weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3)
        {
            fprintf(stderr, "Ошибка: некорректный ввод для ребра %d.\n", e + 1);
            DestroyGraph(graph);
            return 0;
        }
        if (u < 1 || u > vertex_count || v < 1 || v > vertex_count || u == v)
        {
            fprintf(stderr, "Ошибка: недопустимое ребро (%d, %d) для графа с %d вершинами.\n",
                    u, v, vertex_count);
            DestroyGraph(graph);
            return 0;
        }
        SetEdge(graph, e, u - 1, v - 1, weight);
    }

    *out_graph = graph;
    return 1;
}

int main(void)
{
    Graph *graph = NULL;
    if (!ReadGraph(&graph))
    {
        return EXIT_FAILURE;
    }

    printf("\n");
    PrintIncidenceMatrix(graph);
    printf("\n");
    PrimMST(graph);

    DestroyGraph(graph);
    return EXIT_SUCCESS;
}