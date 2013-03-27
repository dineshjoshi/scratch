#include "libsample.h"
#include <malloc.h>

struct _node * _create_new_node(void *data) {
    struct _node *tmp = malloc(sizeof(struct _node));

    if (!tmp) return NULL;

    tmp->data = data;
    tmp->next = NULL;

    return tmp;
}

void _delete_node(struct _node *node) {
    free(node);                 /* should free() data too */
}


/**
 * sum function
 */
int smpl_sum(int a, int b) {
  return a+b;
}

int smpl_sub(int a, int b) {
  return a-b;
}

/**
 * Create a new list
 */
struct _node * list_new() {
    struct _node *tmp = NULL;
    tmp = _create_new_node(NULL);
    return tmp;                 /* NULL check is the client's responsibility */
}


struct _node * list_insert(struct _node *root, void *data) {
    struct _node *tmp = root;

    if (!root) return NULL;

    while (tmp->next) {
        tmp = tmp->next;
    }

    tmp->next = _create_new_node(data);

    return tmp->next;
}

void list_free(struct _node *root) {
    struct _node *tmp = root, *todelete = NULL;

    if (!root) return;

    while (tmp->next) {
        todelete = tmp;
        tmp = tmp->next;        
        _delete_node(todelete);
    }
}

struct _node * list_next   (struct _node * node) {
    return node->next;
}

int list_node_getint (struct _node * node) {
    return (int)(*((int *)(node->data)));
}
