#include <iostream>
#include <stdio.h>

using namespace std;
typedef unsigned char BYTE;

long getFileSize(FILE *file)
{
    long lCurPos, lEndPos;
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    fseek(file, lCurPos, 0);
    return lEndPos;
}

long frequency[256];


int main()
{
    FILE* target;
    BYTE *fileBuf;
    if ((bool)(target = fopen("test.txt", "rb")) == 0)
    {
        cout << "Some Error with open file" << endl;
        return 0;
    }

    long filesize = getFileSize(target);
    for (int i = 0; i < filesize; i++)
    {
        unsigned char k;
        fscanf(target, "%c", &k);
        frequency[k]++;
    }

    for (int i = 0; i < 256; i++)
        printf("frequency[%d] = %d\n", i, frequency[i]);

    char k = 0;
    //fscanf(target, "%c", &k);

    //ifstream target ("test.txt", ios::in | ios::binary);
    //fscanf(target, "%s", &S);

    return 0;
}
