#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *ptr = q->head;
    while (ptr) {
        list_ele_t *previous = ptr;
        ptr = ptr->next;
        free(previous->value);
        free(previous);
    }
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
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    size_t len = strlen(s) + 1;
    newh->value = (char *) malloc(len);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len);
    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;

    q->size++;
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
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->next = NULL;

    size_t len = strlen(s) + 1;
    newh->value = (char *) malloc(len);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len);

    if (q->size == 0) {
        q->head = newh;
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size++;
    return true;
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
    if (!q || !q->head)
        return false;

    if (!sp)
        return false;

    strncpy(sp, q->head->value, bufsize);
    sp[bufsize - 1] = '\0';
    list_ele_t *head = q->head;
    q->head = q->head->next;
    free(head->value);
    free(head);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
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
    if (!q || !q->head)
        return;

    list_ele_t *ptr = q->head, *previous = NULL, *next = NULL;
    q->tail = q->head;
    while (ptr) {
        next = ptr->next;
        ptr->next = previous;
        previous = ptr;
        ptr = next;
    }
    q->head = previous;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    merge_sort(&q->head);

    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}

void merge_sort(list_ele_t **head)
{
    if (!(*head) || !(*head)->next)
        return;

    list_ele_t *fast = (*head)->next;
    list_ele_t *slow = *head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    fast = slow->next;
    slow->next = NULL;

    list_ele_t **left = head;
    list_ele_t **right = &fast;

    merge_sort(left);
    merge_sort(right);

    list_ele_t *newh = NULL;
    list_ele_t **tmp = &newh;

    while (*left && *right) {
        if (strcmp((*left)->value, (*right)->value) < 0) {
            *tmp = *left;
            *left = (*left)->next;
        } else {
            *tmp = *right;
            *right = (*right)->next;
        }
        tmp = &((*tmp)->next);
    }

    if (*left)
        *tmp = *left;
    else if (*right)
        *tmp = *right;

    *head = newh;
}
