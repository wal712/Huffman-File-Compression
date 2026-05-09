#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Priorityqueue.h"
#include "ByteIO.h"

#define BYTE_MAX 256
#define FILE_NAME_MAX 256

typedef struct
{
  int length;
  unsigned long code;
} huff_code;

void gen_huff_tree(Node *tree, huff_code *code_table, unsigned long code, int code_length)
{
  assert(tree);
  assert((size_t)code_length < sizeof(code) * 8); // sanity check for myself

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

  Byte_Buffer buffer = {0};
  // TODO: create new file to write to
  char new_filename[FILE_NAME_MAX] = "compressed_";
  int max_length = FILE_NAME_MAX - strlen(new_filename);
  strncat(new_filename, argv[1], max_length);

  FILE *compressed_file = fopen(new_filename, "r");
  if (compressed_file != NULL)
  {
    printf("Error: %s already exists as a file here. Please either rename or remove said file and try again\n", new_filename);
    fclose(input_file);
    // close(fd);j
    return 0;
  }

  printf("compressed file name is: %s\n", new_filename);
  compressed_file = fopen(new_filename, "w+");

  assert(compressed_file);

  // TODO: write out freq_table as header of compressed file:
  /*
   * I'm thinking write out frequency, then space, then byte, and end header with two newlines
   * There should always be at least one frequency byte pair in header
   * So we check for a newline after each character?
   * Hmm, in that case we only need one newline, even if encoded character is newline, we don't care, we just check the byte after it
   * So: read till space, interpret as int, read byte as byte, pair int and byte in freq table
   * check if next character is space or newline
   * If space, Repeat decoding steps above, if newline, end decoding
   */

  for (int i = 0; i < BYTE_MAX; i++)
  {
    if (freq_table[i] == 0)
    {
      continue;
    }

    fputc(' ', compressed_file);
    fprintf(compressed_file, "%d ", freq_table[i]);
    fputc(i, compressed_file);
  }

  int compressed_freq_table[BYTE_MAX] = {0};

  rewind(compressed_file);

  c = fgetc(compressed_file);
  assert(c == ' ');

  char num_buffer[256] = {0};
  while ((c = fgetc(compressed_file)) != ' ')
  {
    strncat(num_buffer, &c, 1);
  }

  printf("%s\n", num_buffer);
  // TODO: make a byte reader
  fclose(input_file);
  // close(fd);j
  return 0;
}
