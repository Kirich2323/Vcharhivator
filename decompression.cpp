#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>


using namespace std;


typedef struct node{
unsigned char value;
node* left;
node* right;
bool leaf;
}node;

node* head;
node* curr_node;

typedef struct encoded_symbol{
unsigned char value;
unsigned char code_length;
}e_symbol;


bool cmp(e_symbol i, e_symbol j){
if (i.code_length == j.code_length)
    return (i.value < j.value);
return (i.code_length < j.code_length);
}

void node_create(node* c_node){
c_node = (node*)malloc(sizeof(node));
c_node->left = NULL;
c_node->right = NULL;
}

node* link (node* current_node, unsigned char turn){
node* link_of_node;
if (turn == 1)
{
    if (current_node->right == NULL)
        node_create(current_node->right);
    link_of_node = current_node->right;
}
else
{
    if (current_node->left == NULL)
        node_create(current_node->left)
    link_of_node = current_node->left;
}
    return link_of_node;
}


void decompression_extract(FILE* target, char output_path[])
{
    FILE* output = fopen(output_path, "wb");
    int i, t;
    e_symbol symbols[256];
    node_create(head);
    node* tmp;
    curr_node = head;

    char UPA[4] = {'U', 'P', 'A', '\0'};
    char sign[4];
    sign[3] = '\0';

    fread(sign, sizeof(char), 3, target);

    if (strcmp(sign, UPA) != 0)
    {
        cout << "Wrong file format\n";
        return;
    }

    char Alg[5];
    Alg[4] = '\0';
    fread(Alg, sizeof(char), 4, target);

    if (strcmp(Alg, "HUFF") != 0)
    {
        cout << "Wrong Algoritm";
        return;
    }

    char solid;
    fread(&solid, sizeof(char), 1, target);

    if (solid)
        cout << "Solid\n";
    else
        cout << "Not solid\n";

    unsigned short int files_count;
    fread(&files_count, sizeof(unsigned short int), 1, target);

    unsigned char filename_length;
    fread(&filename_length, sizeof(unsigned char), 1, target);

    char* filename = (char*)malloc(filename_length + 2);
    filename[filename_length] = '\0';

    fread(filename, sizeof(char), filename_length+1, target);

    unsigned long long packed_size = 0;
    unsigned long long real_size = 0;

    fread(&packed_size, sizeof(unsigned long long), 1, target);
    fread(&real_size, sizeof(unsigned long long), 1, target);

    for (i = 0; i <= 255; i++)
    {
        symbols[i].value = i;
        fscanf(target, "%c", &symbols[i].code_length);
    }

    stable_sort(symbols, symbols + 256, cmp);

    int max_length;
    int max_code = 0;
    int curr_code = -1;

    unsigned char code[32];
    i = -1;

    while (symbols[++i].code_length == 0);

    max_length = symbols[i].code_length;

    while (i <= 255)
    {
        if (max_length == symbols[i].code_length)
        {
            curr_code += 1;
            for (t = 0; t < max_length; t++)
            {
                  unsigned int k = pow(2, t);
                  k &= curr_code;
                  code[max_length - t - 1] = k != 0;
            }
        }
        else
        {
            curr_code += 1;
            curr_code <<= symbols[i].code_length - max_length;
            max_length = symbols[i].code_length;

            for (t = 0; t < max_length; t++)
            {
                  unsigned int k = pow(2, t);
                  k &= curr_code;
                  code[max_length - t - 1] = k != 0;
            }
        }

    tmp = head;
    for (int t = 0; t < max_length; t++)
    {
        tmp = link(tmp, code[t]);
        tmp->leaf = 0;
    }
    tmp->value = symbols[i].value;
    tmp->leaf = 1;

    i++;
    }

    unsigned char buff;
    unsigned long long c_length;

    while (byte_count < real_size)
    {
        fscanf(target, "%c", &buff);
        for (int m = 7; m >= 0; m--){
            if (curr_node->leaf)
        {
            fprintf(output, "%c", curr_node->value);
            curr_node = head;
            byte_count++;
            if (byte_count == real_size)
                break;
        }
            unsigned char k = pow(2, m);
            k &= buff;
            if (k == 0)
                curr_node = curr_node->left;
            else
                curr_node = curr_node->right;
        }
    }
    return;
}
