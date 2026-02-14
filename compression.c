#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "Priorityqueue.h"
#define BYTE_MAX 256

int get_max(const void *num1, const void *num2)
{
  int a = *(int *)num1;
  int b = *(int *)num2;
  if (a > b)
  {
    // printf("%d is bigger than %d\n", a, b);
    return 1;
  }
  else if (a < b)
  {
    // printf("%d is bigger than %d\n", b, a);
    return -1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: %s <filenames\n", argv[0]);
    return 1;
  }

  // int fd = open(argv[1], O_RDONLY);
  // if (fd == -1)
  //{
  // fprintf(stderr, "Coudldn't open file: %s\n", argv[1]);
  // return 1;
  // }

  FILE *input_file = fopen(argv[1], "r");
  if (!input_file)
  {
    fprintf(stderr, "Coudldn't open file: %s\n", argv[1]);
    return 1;
  }

  int freq_table[256] = {0};
  char c;

  while ((c = fgetc(input_file)) != EOF)
  {
    // printf("idx: %u\n", (unsigned char)c);
    freq_table[(unsigned char)c]++;
  }

  int sum = 0;
  for (int i = 0; i < BYTE_MAX; i++)
  {
    sum += freq_table[i];
  }

  // printf("Total sum: %d\n", sum);

  // qsort(&freq_table, BYTE_MAX, sizeof(int), get_max);

  PQueue q;
  init_queue(&q, BYTE_MAX);

  for (int i = 0; i < BYTE_MAX; i++)
  {
    if (freq_table[i] == 0)
    {
      continue;
    }
    Node *curr = (Node *)malloc(sizeof(Node));
    assert(curr);
    init_leaf(curr, (unsigned char)i, freq_table[i]);
    queue_add(&q, curr);
  }

  // for (int i = 0; i < q.length; i++)
  //{
  // printf("%d\n", q.arr[i]->weight);
  //}

  // printf("length before dequeue: %d\t head before dequeue %d\n", q.length, q.head);
  // Node *test = dequeue(&q);
  // printf("length after dequeue: %d\t head after dequeue %d\n", q.length, q.head);
  //  TODO: build huffman tree
  while (q.length > 1)
  {
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *pop1 = dequeue(&q);
    Node *pop2 = dequeue(&q);

    assert(new_node);
    assert(pop1);
    assert(pop2);

    // Left is smaller, right is bigger
    if (pop1 < pop2)
    {
      init_node(new_node, pop1, pop2, pop1->weight + pop2->weight);
    }
    else
    {
      init_node(new_node, pop2, pop1, pop1->weight + pop2->weight);
    }

    queue_add(&q, new_node);
  }

  Node *huff_tree = dequeue(&q);
  assert(huff_tree);
  assert(q.length == 0);

  fclose(input_file);
  // close(fd);j
  return 0;
}
