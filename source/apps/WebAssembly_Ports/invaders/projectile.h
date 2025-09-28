#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include "sdl.h"


struct pNode {
    int x,y,type;
    struct pNode *next;
};


struct pNode *pnode_new(int x, int y, int type);
struct pNode* pnode_add(struct pNode *head, int x, int y, int type);
struct pNode* pnode_display(struct pNode *head);
void pnode_free(struct pNode *node);


#endif