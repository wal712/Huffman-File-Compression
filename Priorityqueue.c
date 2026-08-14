#include <assert.h>
#include <stdlib.h>

#include "Priorityqueue.h"

void init_leaf(Node *node, unsigned char val, int weight)
{
  assert(node);
  node->val = val;
  node->weight = weight;
  node->left = NULL;
  node->right = NULL;
}

void init_node(Node *node, Node *left, Node *right, int weight)
{
  assert(node);
  assert(left);
  assert(right);
  assert(weight > 0);

  node->left = left;
  node->right = right;
  node->weight = weight;
}

void init_queue(PQueue *q, int size)
{
  assert(q);
  assert(size > 0);

  q->arr = (Node **)malloc(sizeof(Node *) * size);
  assert(q->arr);

  for (int i = 0; i < size; i++)
  {
    q->arr[i] = NULL;
  }

  q->capacity = size;
  q->length = 0;
  q->head = 0;
}

void queue_add(PQueue *q, Node *item)
{
  assert(q);
  assert(q->arr);
  assert(item);
  assert(q->capacity > q->length);

  int tail = (q->head + q->length++) % q->capacity;

  q->arr[tail] = item;

  for (int i = 0; i < q->length - 1; i++)
  {
    int idx1 = (((tail - i) % q->capacity) + q->capacity) % q->capacity;     // One liner to get positive remainder in the range [0, capacity - 1]
    int idx2 = (((tail - i - 1) % q->capacity) + q->capacity) % q->capacity; // One liner to get positive remainder in the range [0, capacity - 1]

    if (q->arr[idx1]->weight < q->arr[idx2]->weight)
    {
      Node *temp = q->arr[idx2];
      q->arr[idx2] = q->arr[idx1];
      q->arr[idx1] = temp;
    }
    else
    {
      break;
    }
  }
}

Node *dequeue(PQueue *q)
{
  assert(q);
  assert(q->arr);
  assert(q->length > 0);

  q->length--;
  return q->arr[q->head++];
}

int is_leaf(Node *node)
{
  if (!node->left && !node->right)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
