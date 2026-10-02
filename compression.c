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
    printf("Usage: %s <filenames>\n", argv[0]);
    return 1;
  }

  // int fd = open(argv[1], O_RDONLY);
  // if (fd == -1)
  //{
  // fprintf(stderr, "Coudldn't open file: %s\n", argv[1]);
  // return 1;
  // }

  FILE *input_file = fopen(argv[1], "rb");
  if (!input_file)
  {
    fprintf(stderr, "Coudldn't open file: %s\n", argv[1]);
    return 1;
  }

  int freq_table[256] = {0};
  char c;
  int num_chars = 0;
  int file_char;

  while ((file_char = fgetc(input_file)) != EOF)
  {
    // printf("idx: %u\n", (unsigned char)c);
    freq_table[(unsigned char)file_char]++;
    num_chars++;
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

  char newer_filename[FILE_NAME_MAX] = "uncompressed_";
  max_length = FILE_NAME_MAX - strlen(newer_filename);
  strncat(newer_filename, argv[1], max_length);

  FILE *uncompressed_file = fopen(newer_filename, "r");
  if (uncompressed_file != NULL)
  {
    printf("Error: %s already exists as a file here. Please either rename or remove said file and try again\n", newer_filename);
    fclose(input_file);
    // close(fd);j
    return 0;
  }

  printf("uncompressed file name is: %s\n", newer_filename);
  uncompressed_file = fopen(newer_filename, "wb+");

  assert(uncompressed_file);

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

  fputc('\n', compressed_file);
  // TODO: write out length of file? or figure out how to ensure extra zero bytes after writer flush don't get interpreted
  fprintf(compressed_file, "%d ", num_chars);

  // TODO: write encoded file contents
  // I think literally just read each byte from original file and find it in code table
  rewind(input_file);
  Byte_Buffer buffer = {0};
  while ((file_char = fgetc(input_file)) != EOF)
  {
    huff_code current = code_table[(unsigned char)file_char];
    write_bytes(&buffer, compressed_file, current.code, current.length);
  }

  flush_write(&buffer, compressed_file);

  int compressed_freq_table[BYTE_MAX] = {0};

  rewind(compressed_file);

  // c = fgetc(compressed_file);
  // assert(c == ' ');

  char num_buffer[256] = {0};
  int key;

  while (1)
  {
    if ((char)(file_char = fgetc(compressed_file)) == '\n')
    {
      // header over
      break;
    }

    assert(file_char == ' ');
    int frequency = 0;
    while ((char)(file_char = fgetc(compressed_file)) != ' ')
    {
      printf("%c", file_char);
      assert(file_char != EOF);
      // strncat(num_buffer, &c, 1);
      frequency = frequency * 10 + (file_char - '0');
    }
    printf("Num buffer: %d", frequency);
    printf("\n");
    key = fgetc(compressed_file);
    assert(key != EOF);

    // printf("%s \t %c\n", num_buffer, key);
    compressed_freq_table[(unsigned char)key] = frequency;
    num_buffer[0] = '\0';
  }

  while ((c = fgetc(compressed_file)) != ' ')
  {
    assert(c != EOF);
    strncat(num_buffer, &c, 1);
  }

  // TODO: did I do this in the right place??
  int new_num_chars = strtol(num_buffer, NULL, 0);

  // you can get rid of this later obviously
  //  TODO: write actual tests bro
  for (int i = 0; i < BYTE_MAX; i++)
  {
    // printf("%d passed\n", i);
    // printf("original: %ld, new one: %ld\n", freq_table[i], compressed_freq_table[i]);
  }

  // TODO: make a byte reader
  /*
   * Ok what do we need:
   * We probably want to use the huff_tree, and the byte buffer
   * Probably a nested loop, where inner loop reads a bit at a time, pick left or right branch of current node in huff_tree ->
   * -> and stop loop when isLeaf(currnode) = 1 (true), at which point we write the given value at the node into the uncompressed file
   * we continue with this loop until our reader function returns EOF
   * to do this, we need to read in each byte (char) as an int, so we can intepret EOF correctly
   * Then, we wrap said inner loop in an outer loop that checks if our reader function returns -1, at which point we are done
   * TODO: handle excess 0 bits at the end -> Either add length of expected number of bits to header, or might just resolve itself
   * -> That last bit needs explaining, basically we might not hit a leaf with excess zero bits, and then we terminate so its fine, not sure tho
   */

  // TODO: Need to generalize creation of huff tree and code table so I can use seperately for compression and decompression
  int read_bit;
  int chars_read = 0;
  Node *currnode = huff_tree;
  while (((read_bit = read_bytes(&buffer, compressed_file)) != EOF) && chars_read < new_num_chars) // TODO: add check if weve reached end of length specified in header
  {
    // TODO: HANDLE EXCESS BITS AT END OF FILE
    //  printf("Read_bit is: %d\n", read_bit);
    if (read_bit == 1)
    {
      currnode = currnode->right;
      // printf("Going right\n");
    }
    else
    {
      currnode = currnode->left;
      // printf("Going left\n");
    }
    if (is_leaf(currnode))
    {
      fputc(currnode->val, uncompressed_file);
      currnode = huff_tree; // reset to root of tree, we hit a leaf node
      chars_read++;
      // printf("Found a leaf!\n");
    }
    else
    {
      // printf("Didn't find a leaf\n");
    }
  }
  // assert(read_bit != EOF);
  fclose(input_file);
  // close(fd);j
  return 0;
}
