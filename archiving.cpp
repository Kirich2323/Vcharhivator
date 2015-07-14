#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>

using namespace std;

int N = 0;
#define INF 100000000

long frequency[256];

typedef struct symb{
    int p, used = 0;
    char n;
    struct symb* left;
    struct symb* right;
}symb;

bitset <8> str;

typedef struct new_symb{
    char c;
    int l;
}new_symb;

new_symb nm[256];
vector <unsigned char> result;
long long buffer;
int buf_length = 0;

void c_symbs(symb* e, char n, int length)
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
        unsigned char k = (char)(pow(2, length) - 1) & (char)str.to_ulong();
        nm[e->n].c = k;
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

unsigned char invert(unsigned char x)
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

void archive(FILE * target)
{
    long filesize = getFileSize(target);
    for (int i = 0; i < filesize; i++)
    {
        unsigned char k;
        fscanf(target, "%c", &k);
        frequency[k]++;
    }
    symb* s[256];
    int i = 0;
    int count = 0;
    for(i = 0; i < 256; i++)
        if (frequency[i])
        {
            s[count] = (symb*)malloc(sizeof(symb));
            s[count]->used = 0;
            s[count]->p = frequency[i];
            s[count]->n = i;
            s[count]->left = s[count]->right = NULL;
            count++;
        }
    symb* last;
    for (;;)
    {
        int min1 = INF;
        int min2 = INF;
        int min1_index = INF;
        int min2_index = INF;
        for(i = 0; i < count; i++)
            if (s[i]->p <= min1 && s[i]->used != 1)
            {
                if (min1_index != INF)
                    s[min1_index]->used = 0;
                min1_index = i;
                s[i]->used = 1;
                min1 = s[i]->p;
            }

        for(i = 0; i < count; i++)
            if (s[i]->p <= min2 && s[i]->used != 1)
            {
                if (min2_index != INF)
                    s[min2_index]->used = 0;
                min2_index = i;
                s[i]->used = 1;
                min2 = s[i]->p;
            }
        if (min2 == INF)
            break;

        symb* e = (symb*)malloc(sizeof(symb));
        e->left = s[min1_index];
        e->right = s[min2_index];
        e->p = s[min1_index]->p + s[min2_index]->p;
        e->used = 0;
        e->n = -1;
        s[min1_index] = e;
        last = e;
    }

    if (last)
        c_symbs(last, 0, 0);

    fseek(target, 0, SEEK_SET);
    int j = 0;
    for (j = 0; j < filesize; j++)
    {
        char t;
        fscanf(target, "%c", &t);
        new_symb* tmp_nm = &nm[t];
        long long k = tmp_nm->c;
        k <<= buf_length;
        buffer += k;
        buf_length += tmp_nm->l;
        while(buf_length > 8)
        {
            char k = invert((unsigned char)buffer);
            result.push_back(k);
            buffer >>= 8;
            buf_length -= 8;
        }
    }

    if (buf_length > 0)
    {
        char k = invert((unsigned char)buffer);
        result.push_back(k);
    }

    return ;
}
