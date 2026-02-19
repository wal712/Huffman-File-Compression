#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "Priorityqueue.h"
#define BYTE_MAX 256

typedef struct
{
  int length;
  unsigned long code;
} huff_code;

void gen_huff_tree(Node *tree, huff_code *code_table, unsigned long code, int code_length)
{
  assert(tree);
  assert((size_t) code_length < sizeof(code) * 8); // sanity check for myself

  // base case
  if (!tree->left && !tree->right) // Leaf node
  {
    assert(code_table[tree->val].length == 0); // should only be true if this index hasn't been used yet
    code_table[tree->val].code = code;
    code_table[tree->val].length = code_length;
    return;
  }

  assert(tree->left);
  assert(tree->right);

  gen_huff_tree(tree->left, code_table, (code << 1), code_length + 1);
  gen_huff_tree(tree->right, code_table, (code << 1) + 1, code_length + 1);
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

  huff_code code_table[BYTE_MAX] = {{0}};

  gen_huff_tree(huff_tree, code_table, 0, 0);

  // int sum = 0;
  // int product_sum = 0;
  // for (int i = 0; i < BYTE_MAX; i++)
  // {
  //   if (code_table[i].length != 0)
  //   {
  //     printf("code: %lb\t length: %d\n", code_table[i].code, code_table[i].length);
  //     sum += freq_table[i];
  //     product_sum += (freq_table[i] * code_table[i].length);
  //   }
  // }

  // printf("Product sum: %d\t sum: %d\n", product_sum, sum);
  // printf("Average num bits: %f\n", (float) product_sum / sum);


  
  // TODO: make a byte writer

  // TODO: make a byte reader
  fclose(input_file);
  // close(fd);j
  return 0;
}
