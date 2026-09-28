/* Structure representing a node in a linked list */
typedef struct node {
    char *name;         /* Name associated with this node */
    char *content;      /* Data content stored in this node */
    int line;           /* Line number related to the content */
    struct node *next;  /* Pointer to the next node in the linked list */
} node;

/**
 * @brief Allocates and initializes a new node for a linked list.
 *
 * This function creates and returns a new node for storing a macro read from the source file.
 * 
 * @param name A string representing the name of the macro.
 * @param content The macro's content, excluding the 'endmacro' directive.
 * @param line_num The line number in the source file where the macro was defined.
 * @return A pointer to the newly created node.
 */
node *create_node(char *name, char *content, int line_num);

/**
 * @brief Searches for a specific node in a linked list.
 *
 * This function searches for a macro by its name within a linked list of macros.
 * 
 * @param head A pointer to the head of the linked list where macros are stored.
 * @param name The name of the macro being searched for.
 * @param found A pointer to an integer that will be set to indicate if the macro was found (non-zero) or not (zero).
 * @return A pointer to the node containing the macro if found, or NULL if not found.
 */
node *find_node_in_list(node *head, char *name, int *found);

/**
 * @brief Appends a new node to the end of a linked list.
 *
 * This function adds a newly created node containing a macro to the end of an existing linked list.
 * 
 * @param head A pointer to the head of the linked list where macros are stored.
 * @param name A string representing the name of the new macro.
 * @param content A string representing the content of the new macro.
 * @param line_num The line number in the source file where the macro was defined.
 */
void append_to_list(node **head, char *name, char *content, int line_num);

/**
 * @brief Frees the memory allocated for a single node.
 *
 * This function releases the memory allocated for a specific node, including its content.
 * 
 * @param node1 A pointer to the node that needs to be deallocated.
 */
void free_node(node *node1);

/**
 * @brief Frees the memory allocated for an entire linked list.
 *
 * This function deallocates the memory used by all nodes in a linked list, starting from the head.
 * 
 * @param head A pointer to the head of the linked list where macros are stored.
 */
void free_list(node *head);
