#include <assert.h>
#include <stdlib.h>

#include "Priorityqueue.h"

void init_leaf(Node *node, unsigned char val, int weight) {
  assert(node);
  node->val = val;
  node->weight = weight;
  node->left = NULL;
  node->right = NULL;
}

void init_queue(PQueue *q, int size) {
  assert(q);
  assert(size > 0);

  q->arr = (Node **) malloc(sizeof(Node *) * size);
  assert(q->arr);

  for (int i = 0; i < size; i++) {
    q->arr[i] = NULL;
  }

  q->capacity = size;
  q->length = 0;
  q->head = 0;
}

void queue_add(PQueue *q, Node *item) {
  assert(q);
  assert(q->arr);
  assert(item);
  assert(q->capacity > q->length);
  
  q->arr[q->length++] = item;

  for (int i = q->length - 1; i > q->head; i--) {
    if (q->arr[i]->weight < q->arr[i - 1]->weight) {
      Node *temp = q->arr[i - 1];
      q->arr[i - 1] = q->arr[i];
      q->arr[i] = temp;
    } else {
      break;
    }
  }
}

Node *dequeue(PQueue *q) {
  assert(q);
  assert(q->arr);
  assert(q->length > 0);

  q->length--;
  return q->arr[q->head++];
}