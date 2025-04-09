#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Ne pas modifier
void init_file()
{
    FILE* f = fopen("data.txt", "w");
    if (f) {
        fprintf(f, "5\n12\n8\n1\n19\n");
        fclose(f);
    }
    f = fopen("data2.txt", "w");
    if (f) {
        fprintf(f, "10\n20\n30\n");
        fclose(f);
    }
    f = fopen("data3.txt", "w");
    if (f) {
        fprintf(f, "1\n2\n3\n4\n5\n");
        fclose(f);
    }
    f = fopen("data_vide.txt", "w");
    if (f) {
        fclose(f);
    }
}

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} List;

void append(List* list, int value)
{
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    if (!list->head) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

void free_list(List* list)
{
    Node* current = list->head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        free(tmp);
    }
    list->head = list->tail = NULL;
}

void print_list(const List* list)
{
    printf("Liste :");
    Node* current = list->head;
    if (!current) {
        printf("\n");
        return;
    }
    while (current) {
        printf(" %d", current->value);
        if (current->next) printf(" ->");
        current = current->next;
    }
    printf("\n");
}

void reverse_list(List* list)
{
    Node *prev = NULL, *curr = list->head, *next = NULL;
    list->tail = list->head;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}

int sum_list(const List* list)
{
    int sum = 0;
    Node* current = list->head;
    while (current) {
        sum += current->value;
        current = current->next;
    }
    return sum;
}

int min_list(const List* list)
{
    if (!list->head) return 0;
    int min = list->head->value;
    Node* current = list->head->next;
    while (current) {
        if (current->value < min) min = current->value;
        current = current->next;
    }
    return min;
}

int max_list(const List* list)
{
    if (!list->head) return 0;
    int max = list->head->value;
    Node* current = list->head->next;
    while (current) {
        if (current->value > max) max = current->value;
        current = current->next;
    }
    return max;
}

void filter_list(List* list, int threshold)
{
    Node *current = list->head, *prev = NULL;
    while (current) {
        if (current->value < threshold) {
            Node* to_delete = current;
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }
            current = current->next;
            free(to_delete);
        } else {
            prev = current;
            current = current->next;
        }
    }
    // Réajuster la queue
    list->tail = prev;
}

bool read_file(const char* filename, List* list)
{
    FILE* f = fopen(filename, "r");
    if (!f) return false;
    int value;
    while (fscanf(f, "%d", &value) == 1) {
        append(list, value);
    }
    fclose(f);
    return true;
}

bool add_to_file(const char* filename, int value)
{
    FILE* f = fopen(filename, "a");
    if (!f) return false;
    fprintf(f, "%d\n", value);
    fclose(f);
    return true;
}

void help()
{
    printf("Utilisation : ./app <fichier> [options]\n\n");
    printf("Options disponibles :\n");
    printf("  --reverse         Inverse l'ordre des éléments\n");
    printf("  --sum             Affiche la somme des éléments\n");
    printf("  --filter <val>    Filtre les éléments >= val\n");
    printf("  --add <val>       Ajoute une valeur à la fin du fichier\n");
    printf("  --help            Affiche ce message d'aide\n");
    printf("  --version, -v     Affiche la version du programme\n");
    printf("  --min             Affiche la valeur minimale de la liste\n");
    printf("  --max             Affiche la valeur maximale de la liste\n");
}

void version()
{
    printf("version 1.0\n");
}

int main(int argc, char* argv[])
{
    // Ne pas modifier
    init_file();
    // ---------------

    if (argc < 2) 
        return 1;
        
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0) {
            help(stdout);
            return 0;
        }
        if (strcmp(argv[i], "--version") == 0) {
            version(stdout);
            return 0;
        }
    }    

    // Option seule sans fichier
    if (strncmp(argv[1], "--", 2) == 0) 
        return 1;

    const char* filename = argv[1];
    List list = {NULL, NULL};

    // --add doit s'exécuter même si on ne lit pas la liste
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--add") == 0 && i + 1 < argc) {
            int val = atoi(argv[i + 1]);
            if (add_to_file(filename, val)) {
                printf("Valeur %d ajoutée\n", val);
            }
            return 0;
        }
    }

    if (!read_file(filename, &list)) {
        return 2;
    }

    bool list_modified = false;

    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--reverse") == 0) {
            reverse_list(&list);
            list_modified = true;
        } else if (strcmp(argv[i], "--sum") == 0) {
            printf("Somme : %d\n", sum_list(&list));
        } else if (strcmp(argv[i], "--min") == 0) {
            printf("Minimum : %d\n", min_list(&list));
        } else if (strcmp(argv[i], "--max") == 0) {
            printf("Maximum : %d\n", max_list(&list));
        } else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
            int threshold = atoi(argv[i + 1]);
            filter_list(&list, threshold);
            list_modified = true;
            ++i;
        }
    }

    // On affiche la liste si elle a été modifiée ou s’il n’y a pas d’option
    if (argc == 2 || list_modified) {
        print_list(&list);
    }
    free_list(&list);
    return 0;
}
