#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

#define INF 100000000

long frequency[256];

typedef struct symb{
    int p = 0;
    unsigned char n;
    struct symb* left;
    struct symb* right;
}symb;

bitset <32> str;

typedef struct new_symb{
    unsigned int c;
    unsigned int l;
    unsigned char value;
}new_symb;

new_symb nm[256];
new_symb* s_nm[256];
vector <unsigned char> result;

typedef bool (*comp)(symb*, symb*);
bool compare(symb* x1, symb* x2)
{
    return (x1->p > x2->p);
}
priority_queue <symb*, vector<symb*>, comp> pq(compare);

void c_symbs(symb* e, unsigned char n, int length)
{
    symb* t = e;
    int is_print = 0;
    if (length > 0)
        str[length - 1] = n;
    else
        str[length] = n;

    if (t->left)
        c_symbs(t->left, 0, length + 1);
    else
    {
        if (length == 0)
            length = 1;
        nm[e->n].l = length;
    }

    if (t->right)
        c_symbs(t->right, 1, length + 1);

    return;
}

long getFileSize(FILE *file)
{
    long lCurPos, lEndPos;
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    fseek(file, lCurPos, 0);
    fseek(file, 0, 0);
    return lEndPos;
}

unsigned char invert_char(unsigned char x)
{
    int base = 256;
    unsigned char  res = 0;
    while (x != 0)
    {
        res += (x & 1) * (base >>= 1);
        x >>= 1;
    }
    return res;
}

unsigned int invert_int(unsigned int x)
{
    long long int base = 4294967296;
    unsigned int  res = 0;
    while (x != 0)
    {
        res += (x & 1) * (base >>= 1);
        x >>= 1;
    }
    return res;
}

symb* create_symb(int p, unsigned char n, symb* t1 = NULL, symb* t2 = NULL)
{
    symb* e = (symb*)malloc(sizeof(symb));
    e->left = t1;
    e->right = t2;
    e->n = n;
    e->p = p;
    return e;
}

void fill_priority_queue(void)
{
    for(int i = 0; i < 256; i++)
    {
        if (frequency[i])
           pq.push(create_symb(frequency[i], i));
        nm[i].value = i;
    }
}

void fill_frequency(FILE* target, int filesize)
{
    for (int i = 0; i < filesize; i++)
    {
        unsigned char k;
        fscanf(target, "%c", &k);
        frequency[k]++;
    }
}

void restore_symbs_order(void)
{
    for (int i = 0; i < 256; i++)
        s_nm[nm[i].value] = &nm[i];
}

symb* create_tree(void)
{
    while(pq.size() != 1)
    {
        symb* t1 = pq.top();
        pq.pop();
        symb* t2 = pq.top();
        pq.pop();
        pq.push(create_symb(t1->p + t2->p, 0, t1, t2));
    }
    return pq.top();
}

void cache_byte(unsigned char b)
{
    unsigned char k = invert_char(b);
    result.push_back(k);
}

unsigned char calculate_file_name_length(char* file_path)
{
    int i = -1;
    while (file_path[++i] != '\0');
    int k = i;
    while (file_path[--k] != '\\' && (k >= 0));
    return i - k - 1 ;
}

void encode_symbs(void)
{
    int max_length;
    int curr_code = -1;
    int i = -1;
    while (nm[++i].l == 0);
    max_length = nm[i].l;
    while (i <= 255)
    {
        if (max_length == nm[i].l)
        {
            curr_code += 1;
            nm[i].c = (invert_int((int)curr_code)) >> (32 - nm[i].l);
        }
        else
        {
            curr_code += 1;
            curr_code <<= nm[i].l - max_length;
            max_length = nm[i].l;
            nm[i].c = (invert_int((int)curr_code)) >> (32 - nm[i].l);
        }
        i++;
    }
}

long offset(FILE* file)
{
    long current_cursos = ftell(file);
}

void huff_compression(FILE* target, int filesize)
{
    fseek(target, 0, SEEK_SET);
    int buf_length = 0;
    long long buffer = 0;
    for (int j = 0; j < filesize; j++)
    {
        unsigned char t;
        fscanf(target, "%c", &t);
        new_symb* tmp_nm = s_nm[t];
        long long k = tmp_nm->c;
        k <<= buf_length;
        buffer += k;
        buf_length += tmp_nm->l;
        while(buf_length > 8)
        {
            cache_byte((unsigned char)buffer);
            buffer >>= 8;
            buf_length -= 8;
        }
    }
    if (buf_length > 0)
        cache_byte((unsigned char)buffer);
}

void set_filename(char* file_path, char* file_name, int file_name_length)
{
    int i = -1;
    while(file_path[++i] != '\0');
    for (;(file_name_length >= 0) && (i >= 0); file_name_length--, i--)
        file_name[file_name_length] = file_path[i];
}

bool compare_nm(new_symb i, new_symb j)
{
    return (i.l < j.l);
}

void archive(char* files[], unsigned short int files_count)
{
    FILE* output = fopen(files[files_count - 1], "wb");
    long offsets[files_count - 1];
    unsigned short int fc = files_count - 1;
    fprintf(output, "UPA");
    fprintf(output, "HUFF");
    if (files_count > 2 )
        fprintf(output, "%c", 0);
    else
        fprintf(output, "%c", 1);
    fwrite(&fc, sizeof(unsigned short int), 1, output);

    for (int i = 0; i < files_count - 1; i++)
    {
        FILE* target = fopen(files[i], "rb");

        char filename_length = calculate_file_name_length(files[i]);
        char filename[filename_length + 1];
        //strncat(filename, "\\", 2);
        set_filename(files[i], filename, filename_length);

        fprintf(output, "%c", filename_length - 1);
        for (int i = 0; i < filename_length; i++)
            fprintf(output, "%c", filename[i]);


        offsets[i] = ftell(output);
        fseek(output, 16, SEEK_CUR);
        fclose(target);
    }

    for (int i = 0; i < files_count - 1; i++)
    {
        FILE* target = fopen(files[i], "rb");
        if (!pq.empty())
            pq.pop();

        for (int i = 0; i < 256; i++)
        {
            frequency[i] = 0;
            nm[i].c = 0;
            nm[i].l = 0;
            nm[i].value = 0;
        }

        result.clear();

        unsigned long long filesize = getFileSize(target);

        if (filesize != 0)
        {
            fill_frequency(target, filesize);

            fill_priority_queue();

            symb* root = create_tree();
            c_symbs(root, 0, 0);
            stable_sort(nm, nm + 256, compare_nm);

            encode_symbs();

            restore_symbs_order();

            huff_compression(target, filesize);
        }

        unsigned long long length = result.size();

        long current_position = ftell(output);
        fseek(output, offsets[i], SEEK_SET);
        fwrite(&length, sizeof(unsigned long long), 1, output);
        fwrite(&filesize, sizeof(unsigned long long), 1, output);

        fseek(output, current_position, SEEK_SET);
        if (filesize == 0)
        {
            for (int i = 0; i < 256; i++)
                fprintf(output, "%c", 0);
            continue;
        }

        for (int i = 0; i < 256; i++)
            fprintf(output, "%c", (unsigned char)s_nm[i]->l);

        for (int i = 0; i < result.size(); i++)
            fprintf(output, "%c", result[i]);
    }

    return;
}
