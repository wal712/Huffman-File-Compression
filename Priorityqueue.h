#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

typedef struct Node Node;

struct Node
{
    Node *left;
    Node *right;
    int weight;
    unsigned char val;
};

typedef struct
{
    Node **arr;
    int capacity;
    int length;
    int head;
} PQueue;

void init_leaf(Node *node, unsigned char val, int weight);
void init_node(Node *node, Node *left, Node *right, int weight);
void init_queue(PQueue *q, int size);
void queue_add(PQueue *q, Node *item);
Node *dequeue(PQueue *q);
int is_leaf(Node *node);
#endif
