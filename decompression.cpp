#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>


using namespace std;


typedef struct node{
char value;
node* left;
node* right;
bool ilist;
}node;

node* head;
node* curr_node;

typedef struct encoded_symbol{
char value;
char code_length;
}e_symbol;


bool compare (e_symbol i, e_symbol j){
if (i.code_length == j.code_length)
    return (i.value < j.value);
return (i.code_length < j.code_length);
}

node* link (node* e, char turn){
node* e_link;
if (turn == '1'){
    if (e->right == NULL){
    e->right = (node*)malloc(sizeof(node));
    e->right->right = NULL;
    e->right->left = NULL;
    }
    e_link = e->right;
}
else{
    if (e->left == NULL){
    e->left = (node*)malloc(sizeof(node));
    e->left->right = NULL;
    e->left->left = NULL;
    }
    e_link = e->left;
}

    return e_link;
}


int main()
{
    FILE* input = fopen("arch2.upa", "rb");
    FILE* output = fopen("newoutput.txt", "w");
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
        fscanf(input, "%c", &symbols[i].code_length);
    }

    stable_sort(symbols, symbols + 256, compare);

    int max_length;
    int max_code = 0;
    int curr_code = -1;

    char code[32];
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
              char k = pow(2, t);
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
              char k = pow(2, t);
              k &= curr_code;
              code[max_length - t - 1] = k != 0;
            }
        }

   printf("%c  ", symbols[i].value);
   for (int v = 0; v < max_length; v++){
        char k = pow(2, v);
        k &= curr_code;
        printf("%d", code[v]);
   }
   printf("\n");

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

    char buff;
    char c_length;

    fscanf(input, "%c", &c_length);

    for (i = 0; i <= c_length; i++)
    {
        fscanf(input, "%c", &buff);
        for (int m = 7; m >= 0; m--){
            if (!(curr_node->ilist)){
            char k = pow(2, m);
            k &= buff;
            if (k == 0){
                curr_node = curr_node->left;}
            else{
                curr_node = curr_node->right;}
        }
        else{
            fprintf(output, "%c", curr_node->value);
            curr_node = head;
        }
        }
    }

    return 0;
}
