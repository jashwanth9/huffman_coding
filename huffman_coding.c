#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman_coding.h"

int count_last=0;

void compress(char * source, char * destination)
{
    int freq_array[128]= {0};
    FILE * fp_source;
    FILE * fp_compressed;

    fp_source = fopen(source,"r");
    if(fp_source==NULL)
    {
        printf("FILE not present.\n");
    }
    else
    {
        fp_compressed = fopen("compressed.txt", "wb");

        fp_source = frequency_finder(fp_source, freq_array);
        display_frequency(freq_array);
        list_t * head =make_tree(freq_array);
        inorder(head->link_node);

        make_compressed_file(head, fp_compressed, fp_source);
        fclose(fp_source);
        fclose(fp_compressed);
	printf("---> A compressed file named \"compressed.txt\" is created.\n");
        FILE * fp_recreate = fopen(destination, "wb");
        FILE * fp_compressed_read = fopen("compressed.txt", "r");

        recreate_file(head->link_node, fp_compressed_read, fp_recreate);
	printf("---> The decoded file is created.\n");    
        fclose(fp_compressed_read);
        fclose(fp_recreate);
    }
}

FILE * frequency_finder(FILE * fp, int * freq_array)
{
	int index;
	char ch;
	ch=fgetc(fp);
	while(ch!=EOF)
	{
		index  = (int)ch;
		freq_array[index] += 1;
		ch = fgetc(fp);
	}
	rewind(fp);
	return fp;
}

void  display_frequency(int * freq_array)
{
	int i;
	for(i = 0 ; i < 128 ; ++i)
	{
		if(freq_array[i]!=0)
		{
			printf("%2c(%2d)  ---  %4d\n", (char)i,i, freq_array[i]);
		}
	}
	printf("\n");
}

list_t * make_tree(int * freq_array)
{
    printf("\n---> Making a TREE of frequency\n");
	list_t * head = 0;
	list_t * temp = 0;
	node_t * new_node = 0;

	int i;
	for(i=0 ; i < 128; ++i)
	{
		if(freq_array[i]!=0)
		{
			temp = (list_t *)malloc(sizeof(list_t));
			new_node = (node_t *)malloc(sizeof(node_t));

			temp->link_list=0;
			temp->link_node = new_node;

			new_node->data = freq_array[i];
			new_node->character[0]= (char)i;
			new_node->left= new_node->right = 0;

			head = insert(head, temp);
		}
	}
//	display_list(head);
	head= make_sub_tree(head);
	return head;
}

list_t *  insert(list_t * head, list_t * temp)
{
	if(head==0)
	{
		head = temp;
	}
	else
	{
		list_t * prev=0;
		list_t * pres=head;

		while(pres && (temp->link_node->data > pres->link_node->data))
		{
			prev=pres;
			pres = pres->link_list;
		}
		if(prev==0)
		{
			temp->link_list=head;
			head=temp;
		}
		else
		{
			prev->link_list=temp;
			temp->link_list=pres;
		}
	}
	return head;
}

void display_list(list_t * head)
{
	while(head)
	{
		printf("%d  ", head->link_node->data);
		head=head->link_list;

	}
}

list_t * make_sub_tree(list_t * head)
{
	list_t * sub_list;
	list_t * temp_free_list;
	node_t * sub_node;
	while(head->link_list)
	{

		sub_list = (list_t *)malloc(sizeof(list_t));
		sub_node = (node_t *)malloc(sizeof(node_t));

		head->link_node->bit=0;
		head->link_list->link_node->bit=1;
		sub_list->link_list=0;
		sub_list->link_node= sub_node;
		sub_node->data = head->link_node->data + head->link_list->link_node->data;
		sub_node->left = head->link_node;
		sub_node->right = head->link_list->link_node;
		strcat(sub_node->character, sub_node->left->character);
		strcat(sub_node->character, sub_node->right->character);

		temp_free_list=head;

		head=head->link_list->link_list;

		free(temp_free_list->link_list);
		free(temp_free_list);

		head=insert(head, sub_list);
	}
	printf("---> TREE of frequency created\n");
	return head;
}

void  make_compressed_file(list_t * head, FILE * compressed, FILE * source)
{
    printf("---> Making a compressed file\n");
	bits b;
	b.temp_bits=0;

	char ch=fgetc(source);
	int count = 0;

	node_t * temp = head->link_node;
	node_t * temp1 = temp;

	while(ch != EOF)
	{
		while(temp->left && temp->right)
		{
            if(count<8)
			{
				if(strchr(temp->left->character, ch))
				{
					//printf("%u", temp->left->bit);
					if(count!=7)
					b.temp_bits <<= 1;
					temp=temp->left;
					count++;
					count_last++;
				}
				else if(strchr(temp->right->character, ch))
				{
					//printf("%u", temp->right->bit);
					(int)b.temp_bits++;
					if(count!=7)
					b.temp_bits <<= 1;
					temp=temp->right;
					count++;
					count_last++;
				}
			}
			else
			{
//				disp_bits(b.temp_bits);
				fprintf(compressed,"%c", b.temp_bits);
				b.temp_bits=0;
				count=0;
			}
		}
		ch= fgetc(source);
//		printf("\n%c", ch);
		temp=temp1;
	}
	b.temp_bits <<= 1;
//	disp_bits(b.temp_bits);
    fprintf(compressed, "%c", b.temp_bits);

}

void inorder(node_t * root)
{
	if(root!=0)
	{
		inorder(root->left);
		printf("%3d %3u %s\n", root->data, root->bit, root->character);
		inorder(root->right);
	}
}

void disp_bits(unsigned char x)
{
	printf("-------");
	int i, k, mask;
	for(i=7; i>=0; --i)
	{
		mask=1<<i;
		k= x&mask;
		if(k==0)
		printf("0");
		else
		printf("1");
	}
	printf("\n");
}

void recreate_file(node_t * head, FILE * compressed, FILE * destination)
{
	int n=count_last/8;
	int remaingbits=count_last%8;
	int count,bi=0;
	bits b;
	b.temp_bits=0;
	char ch;
	node_t * temp = head->link_node;
	node_t * temp1 = temp;
	while(n)
	{
		ch=fgetc(compressed);
		count=(int)ch;
		b.temp_bits=count;
		if(temp->left && temp->right)
		{
			if()
		}
		--n;	
	}


}

/*int dec_bin(int count,int bi)
{
	int m;
	while(count)
	{
		m=count%2;
		bi=bi*10+m;
		count=count/2;
	}
	if(count==0)
	return bi;
}
*/


