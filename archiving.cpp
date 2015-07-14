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
bitset <16> buffer;
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
        nm[e->n].c = (char)str.to_ulong();
        nm[e->n].l = length;
    }

    if (t->right)
        c_symbs(t->right, 1, length + 1);

    return;
}

void archive(int frequency[256], char* target, int size_target)
{
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
            s[count]->left = s[i]->right = NULL;
            count++;
        }

    symb* last;
    for (;;)
    {
        int min1 = INF;
        int min2 = INF;
        int min1_index = INF;
        int min2_index = INF;
        for(i = 0; i < N; i++)
        {
            if (s[i]->p <= min1 && s[i]->used != 1)
            {
                if (min1_index != INF)
                    s[min1_index]->used = 0;
                min1_index = i;
                s[i]->used = 1;
                min1 = s[i]->p;
            }
        }

        for(i = 0; i < N; i++)
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

    int j = 0;
    for (j = 0; j < size_target; j++)
    {
        new_symb* tmp_nm = &nm[target[j]];
        for (i = 0; i < tmp_nm->l; i++)
        {
            char buf = (char)pow(2, i);
            buf = buf & tmp_nm->c;
            if (buf)
                buffer[buf_length + i] = 1;
            else
                buffer[buf_length + i] = 0;
        }
        buf_length += tmp_nm->l;
        if (buf_length > 8)
        {
            result.push_back((char)buffer.to_ulong());
            buffer >>= 8;
            buf_length -= 8;
        }
    }

    if (buf_length > 0)
        result.push_back((char)buffer.to_ulong());

    return ;
}
