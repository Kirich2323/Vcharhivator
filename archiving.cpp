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
long long buffer;
int buf_length = 0;

typedef bool (*comp)(symb*, symb*);
bool compare(symb* x1, symb* x2)
{
    return (x1->p > x2->p);
}

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
        int k = (int)(pow(2, length) - 1) & (int)str.to_ulong();
        nm[e->n].c = k;
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

unsigned char calculate_file_name_length(char* file_path)
{
    int i = -1;
    while (file_path[++i] != '\0');
    int k = i;
    while (file_path[--k] != '\\');

    return i - k - 1 ;
}
void set_filename(char* file_path, char* file_name, int file_name_length)
{
    int i = -1;
    while(file_path[++i] != '\0');
    for (;file_name_length >= 0; file_name_length--, i--)
        file_name[file_name_length] = file_path[i];
}

bool compare_nm(new_symb i, new_symb j)
{
    return (i.l < j.l);
}

void archive(char* files[], int files_count)
{
    FILE* target = fopen(files[0], "rb");

    char filename_length = calculate_file_name_length(files[0]);
    char* filename = (char*)malloc(filename_length);
    set_filename(files[0], filename, filename_length);

    long filesize = getFileSize(target);

    FILE* output = fopen(files[files_count - 1], "wb");

    for (int i = 0; i < filesize; i++)
    {
        unsigned char k;
        fscanf(target, "%c", &k);
        frequency[k]++;
    }

    priority_queue <symb*, vector<symb*>, comp> pq(compare);
    int i = 0;
    for(i = 0; i < 256; i++)
    {
        if (frequency[i])
        {
            symb* e;
            e = (symb*)malloc(sizeof(symb));
            e->p = frequency[i];
            e->n = i;
            e->left = e->right = NULL;
            pq.push(e);
        }
        nm[i].value = i;
    }

    while(pq.size() != 1)
    {
        symb* t1 = pq.top();
        pq.pop();
        symb* t2 = pq.top();
        pq.pop();
        symb* e = (symb*)malloc(sizeof(symb));
        e->left = t1;
        e->right = t2;
        e->p = t1->p + t2->p;
        pq.push(e);
    }

    c_symbs(pq.top(), 0, 0);
    stable_sort(nm, nm + 256, compare_nm);

    int max_length;
    int max_code = 0;
    int curr_code = -1;

    i = -1;

    while (nm[++i].l == 0);
    max_length = nm[i].l;

    while (i <= 255)
    {
        if (max_length == nm[i].l)
        {
            curr_code += 1;
            unsigned char buff = 0;
            for (int t = 0; t < max_length; t++)
            {
                unsigned char k = pow(2, t);
                k &= curr_code;
                if (k != 0)
                {
                    k = pow(2, max_length - t - 1);
                    buff += k;
                }
            }
            nm[i].c = (invert_int((int)curr_code)) >> (32 - nm[i].l);
        }
        else
        {
            curr_code += 1;
            curr_code <<= nm[i].l - max_length;
            max_length = nm[i].l;
            unsigned char buff = 0;
            for (int t = 0; t < max_length; t++)
            {
                unsigned char k = pow(2, t);
                k &= curr_code;
                if (k != 0)
                {
                    k = pow(2, max_length - t - 1);
                    buff += k;
                }
            }
            nm[i].c = (invert_int((int)curr_code)) >> (32 - nm[i].l);
        }
        i++;
    }
    for (int i = 0; i < 256; i++)
        s_nm[nm[i].value] = &nm[i];

    fprintf(output, "UPA");
    fprintf(output, "HUFF");
    fprintf(output, "%c", 1);
    fprintf(output, "%c", 1);
    fprintf(output, "%c", 0);
    fprintf(output, "%c", filename_length - 1);
    for (int i = 0; i < filename_length; i++)
        fprintf(output, "%c", filename[i]);

    fseek(target, 0, SEEK_SET);
    int j = 0;
    for (j = 0; j < filesize; j++)
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
            unsigned char k = invert_char((unsigned char)buffer);
            result.push_back(k);
            buffer >>= 8;
            buf_length -= 8;
        }
    }

    if (buf_length > 0)
    {
        unsigned char k = invert_char((unsigned char)buffer);
        result.push_back(k);
    }

    int length = result.size();

    for (int i = 0; i < 8; i++)
    {
        unsigned char k = (char)length;
        length >>= 8;
        fprintf(output, "%c", k);
    }

    for (int i = 0; i < 8; i++)
    {
        unsigned char k = (char)filesize;
        filesize >>= 8;
        fprintf(output, "%c", k);
    }

    for (int i = 0; i < 256; i++)
        fprintf(output, "%c", (unsigned char)s_nm[i]->l);

    for (int i = 0; i < result.size(); i++)
        fprintf(output, "%c", result[i]);

    return;
}
