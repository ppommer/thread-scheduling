#include <pthread.h>

struct list_elem {
    struct list_elem *next;
    void *data;
};

typedef struct list {
    struct list_elem *first;
    struct list_elem *last;
} list_t;

/* function prototypes */
list_t *list_init();

struct list_elem *list_insert(list_t *list, void *data);

struct list_elem *list_append(list_t *list, void *data);

int list_remove(list_t *list, struct list_elem *elem);

void list_finit(list_t *list);

void list_print(list_t *list, void (*print_elem)(void *));

void list_debug_print(list_t *list);

struct list_elem *list_find(list_t *list, void *data, int (*cmp_elem)(const void *, const void *));

