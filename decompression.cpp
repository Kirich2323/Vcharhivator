#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;

typedef struct node{
unsigned char value;
node* left;
node* right;
bool leaf;
}node;

vector <char*> filenames;

typedef struct encoded_symbol{
unsigned char value;
unsigned char code_length;
}e_symbol;

bool cmp(e_symbol i, e_symbol j){
if (i.code_length == j.code_length)
    return (i.value < j.value);
return (i.code_length < j.code_length);
}

node* node_create(){
node* c_node = (node*)malloc(sizeof(node));
c_node->left = NULL;
c_node->right = NULL;
return c_node;
}

typedef struct FAT
{
    unsigned long long real_size;
    unsigned long long packed_size;
} fat;

vector <fat> files;

node* link (node* current_node, unsigned char turn){
node* link_of_node;
if (turn == 1)
{
    if (current_node->right == NULL)
        current_node->right = node_create();
    link_of_node = current_node->right;
}
else
{
    if (current_node->left == NULL)
        current_node->left = node_create();
    link_of_node = current_node->left;
}
    return link_of_node;
}


void decompression_extract(FILE* target)
{
    int i, t;
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

    unsigned short int files_count;
    fread(&files_count, sizeof(unsigned short int), 1, target);

    unsigned char filename_length;
    char* filename;
    for (int i = 0; i < files_count; i++)
    {
        fread(&filename_length, sizeof(unsigned char), 1, target);
        filename = (char*)malloc(sizeof(char) * (filename_length + 2));
        filename[filename_length + 1] = '\0';
        fread(filename, sizeof(char), filename_length+1, target);
        filenames.push_back(filename);
        unsigned long long packed_size;
        unsigned long long real_size;
        fread(&packed_size, sizeof(unsigned long long), 1, target);
        fread(&real_size, sizeof(unsigned long long), 1, target);
        fat new_fat;
        new_fat.packed_size = packed_size;
        new_fat.real_size = real_size;
        files.push_back(new_fat);
    }

    for (int j = 0; j < files_count; j++)
    {
        int i;
        FILE* output = fopen(filenames[j], "wb");
        node* head = node_create();
        node* tmp;
        node* curr_node = head;
        e_symbol symbols[256];
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
                curr_code += 1;
            else
            {
                curr_code += 1;
                curr_code <<= symbols[i].code_length - max_length;
                max_length = symbols[i].code_length;
            }

            for (t = 0; t < max_length; t++)
            {
                unsigned int k = pow(2, t);
                k &= curr_code;
                code[max_length - t - 1] = k != 0;
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
        unsigned long long byte_count = 0;

        while (byte_count < files[j].real_size)
        {
            fscanf(target, "%c", &buff);
            unsigned char k = 128;
            for (int m = 7; m >= 0; m--){
                if (curr_node->leaf)
                {
                    fprintf(output, "%c", curr_node->value);
                    curr_node = head;
                    byte_count++;
                    if (byte_count == files[j].real_size)
                        break;
                }
                unsigned char t = k & buff;
                k >>= 1;
                if (t == 0)
                    curr_node = curr_node->left;
                else
                    curr_node = curr_node->right;
            }
        }
    }

    return;
}
