#include "projectile.h"

struct pNode *pnode_new(int x, int y, int type) {
    struct pNode *node = (struct pNode *) malloc (sizeof(struct pNode));
    node->x = x;
    node->y = y;
    node->type = type;
    node->next = NULL;
    return node;
}


struct pNode* pnode_add(struct pNode *head, int x, int y, int type) {
    struct pNode *new_node = pnode_new(x, y, type);
    new_node->next = head;
    return new_node;
}


struct pNode* pnode_display(struct pNode *head) {
    if (head == NULL) {
        return NULL;
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, head->x, head->y);

    head->y-= 10;


    if (head->y < 0) {
        struct pNode *temp = head->next;
        free(head);
        return pnode_display(temp);
    }


    head->next = pnode_display(head->next);
    return head;
}

void pnode_free(struct pNode *node) {
    if (node == NULL) {
        return;
    }
    
    if (node->next != NULL) {
        pnode_free(node->next);
    }
    
    free(node);
}
