#include <stdio.h>
#include <stdlib.h>

typedef struct llentry_s llentry_t;

struct llentry_s {
    int value;
    llentry_t* next;
};

void ll_add(llentry_t** head, int valtoadd) {
    llentry_t* tmp;
    llentry_t* tmpnew;

    if (head == NULL) {
        printf("ERROR! null head pointer ref\n");
        return;
    }
    // &head is not null

    if (*head == NULL) {
        // printf("head is null - allocating new first entry\n");
        *head = malloc(sizeof(llentry_t));
        (*head)->value = valtoadd;
        (*head)->next = NULL;
        return;
    } else {

        tmp = *head;
        while (tmp->value < valtoadd && 
               tmp->next != NULL) {
            tmp = tmp->next;
        }
        
        // mid-list insertion
        tmpnew = malloc(sizeof(llentry_t));
        tmpnew->next = tmp->next;
        tmpnew->value = tmp->value;
        tmp->next = tmpnew;
        
        if (tmp->value < valtoadd) {
            tmpnew->value = valtoadd;
        } else {
            tmp->value = valtoadd;
        }
    }
}

void ll_remove(llentry_t** head, int valtorm) {
    llentry_t* tmp;
    llentry_t* tmptorm;

    if (head == NULL) {
        printf("ERROR! null head pointer ref\n");
    }

    if (*head == NULL) {
        return; // nothing to remove
    }

    // special case for head entry
    if ((*head)->value == valtorm) {
        tmptorm = *head;
        *head = (*head)->next;
        free(tmp);
        return;
    }

    tmp = *head;
    while (tmp->next != NULL) {
        if (tmp->next->value == valtorm) {
            tmptorm = tmp->next;
            tmp->next = tmp->next->next;
            free(tmptorm);
            return;
        } else {
            tmp = tmp->next;
        }
    }
}

void ll_print(llentry_t* head) {
    while (head != NULL) {
        printf("Value = %d\n", head->value);
        head = head->next;
    }
    printf(".\n");
}

int main(int argc, int **argv) {
    llentry_t* head = NULL;

    ll_add(&head, 3);
    ll_add(&head, 5);
    ll_add(&head, 4);
    ll_add(&head, 7);
    ll_add(&head, 6);
    ll_add(&head, 6);
    ll_add(&head, 6);
    ll_add(&head, 2);
    ll_print(head);
    
    ll_remove(&head, 7);
    ll_print(head);
    ll_remove(&head, 6);
    ll_print(head);
    ll_remove(&head, 3);
    ll_print(head);
    ll_remove(&head, 2);
    ll_print(head);
    ll_remove(&head, 4);
    ll_print(head);
    ll_remove(&head, 5);
    ll_print(head);
    ll_remove(&head, 6);
    ll_print(head);
    ll_remove(&head, 6);
    ll_print(head);

    return(0);
}
