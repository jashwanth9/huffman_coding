#ifndef HUFFMAN_H
#define HUFFMAN_H

struct node
{
	int data;
	struct node * left;
	struct node * right;
	char character[128];
	unsigned bit : 1 ;
};
typedef struct node node_t;

struct list
{
	struct node * link_node;
	struct list * link_list;
};
typedef struct list list_t;

struct bits_t
{
	unsigned char temp_bits;
};
typedef struct bits_t bits;

void compress(char * source, char * destination);

FILE * frequency_finder(FILE * fp, int * freq_array);

list_t * make_tree(int * freq);
list_t * insert(list_t * head, list_t * temp);
list_t * make_sub_tree(list_t *);
void make_compressed_file(list_t * head, FILE * compressed, FILE * source);

void display_frequency(int * freq_array);
void display_list(list_t * head);
void inorder(node_t * root);
void disp_bits(unsigned char x);
void recreate_file(node_t * head, FILE * compressed, FILE * destination);

#endif


