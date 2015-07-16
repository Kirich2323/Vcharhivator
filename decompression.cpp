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
bool ilist;
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

node* link (node* e, unsigned char turn){
node* e_link;
if (turn == 1)
{
    if (e->right == NULL)
    {
    e->right = (node*)malloc(sizeof(node));
    e->right->right = NULL;
    e->right->left = NULL;
    }
    e_link = e->right;
}
else
{
    if (e->left == NULL)
    {
    e->left = (node*)malloc(sizeof(node));
    e->left->right = NULL;
    e->left->left = NULL;
    }
    e_link = e->left;
}
    return e_link;
}


void decompression_extract(FILE* target, char output_path[])
{
    FILE* output = fopen(output_path, "wb");
    int i, t;
    e_symbol symbols[256];
    head = (node*)malloc(sizeof(node));
    head->left = NULL;
    head->right = NULL;
    node* tmp;
    curr_node = head;

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
        tmp->ilist = 0;
    }
    tmp->value = symbols[i].value;
    tmp->ilist = 1;

    i++;
    }

    unsigned char buff;
    unsigned int c_length;

    for (int i = 0 ; i < 4; i++)
    {
        unsigned int k = 0;
        fscanf(target, "%c", &k);
        k *= pow(2, 8*i);
        c_length += k;
    }

    for (i = 0; i < c_length; i++)
    {
        fscanf(target, "%c", &buff);
        for (int m = 7; m >= 0; m--){
            if (curr_node->ilist)
        {
            fprintf(output, "%c", curr_node->value);
            curr_node = head;
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
