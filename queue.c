#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* DONE: Handle insufficient memory to provide */
    if (q == NULL) {
        return NULL;
    }
    q->head = q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* DONE: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (q == NULL || newh == NULL) {
        return false;
    }
    if (q->size == 0) {
        q->head = q->tail = newh;
        newh->next = NULL;
    } else {
        newh->next = q->head;
        q->head = newh;
    }
    /* DONE: Allocate space for the string and copy it */
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh); /* Beware leak! */
        return false;
    }
    memcpy(newh->value, s, strlen(s));
    newh->value[strlen(s)] = '\0';
    ++q->size;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* DONE: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (q == NULL || newt == NULL) {
        return false;
    }
    if (q->size == 0) {
        q->head = q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    newt->next = NULL;
    /* DONE: Allocate space for the string and copy it */
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newt->value == NULL) {
        free(newt); /* Beware leak! */
        return false;
    }
    memcpy(newt->value, s, strlen(s));
    newt->value[strlen(s)] = '\0';
    ++q->size;

    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* DONE: You need to fix up this code. */
    if (q == NULL || q->size == 0) {
        return false;
    }
    if (sp != NULL) {
        int len = bufsize - 1 < strlen(q->head->value) ? bufsize - 1
                                                       : strlen(q->head->value);
        for (int i = 0; i < len; ++i) {
            sp[i] = q->head->value[i];
        }
        sp[len] = '\0';
    }
    list_ele_t *del = q->head;
    q->head = q->head->next;
    free(del);
    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* DONE: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return (q->size);
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* DONE: You need to write the code for this function */
    if (q == NULL || q->size == 0) {
        return;
    }
    list_ele_t *prev = NULL;
    list_ele_t *now = q->head;
    list_ele_t *next = now->next;
    while (next != NULL) {
        list_ele_t *nextnext = next->next;
        now->next = prev;
        next->next = now;
        prev = now;
        now = next;
        next = nextnext;
    }
    /* swap */
    prev = q->head;
    q->head = q->tail;
    q->tail = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

/* Comparison function */
bool cmp(char *a, char *b) /* Return if a <= b */
{
    int len = strlen(a) < strlen(b) ? strlen(a) : strlen(b);
    for (int i = 0; i < len; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }
    return strlen(a) <= strlen(b);
}
list_ele_t *merge_sort(list_ele_t *start, int L, int R)
{
    if (R <= L + 1) {
        return start;
    }
    list_ele_t *left = start;
    list_ele_t *prev = left, *right = left->next;
    int M = (L + R) / 2;
    for (int i = L; i < M - 1; ++i) {
        prev = right;
        right = right->next;
    }
    prev->next = NULL;

    left = merge_sort(left, L, M);
    right = merge_sort(right, M, R);
    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && cmp(left->value, right->value))) {
            if (!merge) {
                start = merge = left;  // LL1;
            } else {
                merge->next = left;  // LL2;
                merge = merge->next;
            }
            left = left->next;  // LL3;
        } else {
            if (!merge) {
                start = merge = right;  // LL4;
            } else {
                merge->next = right;  // LL5;
                merge = merge->next;
            }
            right = right->next;  // LL6;
        }
    }
    return start;
}
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL || q->size == 0) {
        return;
    }
    q->head = merge_sort(q->head, 0, q->size);
    /* Determine tail after sorting */
    list_ele_t *elem = q->head;
    while (elem != NULL) {
        q->tail = elem;
        elem = elem->next;
    }
}
