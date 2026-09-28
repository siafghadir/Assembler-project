#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deal_with_data.h"
#include "helper_functions.h"
#include "Errors.h"

/* Utility Function to create a new node */
node *create_node(char *name, char *content, int line_num) {
    node *new_node;

    new_node = handle_malloc(sizeof(node));

    new_node->name = name;
    new_node->content = content;
    new_node->line = line_num;
    new_node->next = NULL;

    return new_node;
}

/* List Manipulation Function to find a node in the list */
node *find_node_in_list(node *head, char *name, int *is_found) {
    *is_found = 0;

    if (head == NULL) {
        return NULL;
    }

    if (strcmp(name, head->name) == 0) {
        *is_found = 1;
        printf("Node %s is already present in the list\n", name);
        return head;
    }

    if (head->next == NULL) {
        return head;
    }

    return find_node_in_list(head->next, name, is_found);
}

/* List Manipulation Function to append a new node to the list */
void append_to_list(node **head, char *name, char *content, int line_num) {
    int is_found;
    node *new_node, *current_node;
    is_found = 0;

    current_node = find_node_in_list(*head, name, &is_found);

    if (is_found && strcmp(current_node->content, content) != 0) {
        display_internal_error(ERROR_CODE_13);
        free(name);
        free(content);
        return;
    }

    if (!is_found) {
        new_node = create_node(name, content, line_num);

        if (current_node == NULL) {
            *head = new_node;
        } else {
            current_node->next = new_node;
        }
    }
}

/* Memory Management Function to free a node */
void free_node(node *node1) {
    free(node1->name);
    free(node1->content);
    free(node1);
}

/* Memory Management Function to free the entire list */
void free_list(node *head) {
    while(head != NULL) {
        node *temp = head;
        head = head->next;
        free_node(temp);
    }
}
