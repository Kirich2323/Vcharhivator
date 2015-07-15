#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "archiving.cpp"
#include "decompression.cpp"

using namespace std;

FILE* target;

bool chek_exist(char path[])
{
    if ((bool)(target = fopen(path, "rb")) == 0)
    {
        cout << "Some Error with open file" << endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Wrong amount of arguments" << endl;
        return 0;
    }

    if (*argv[1] == 'x')
    {
        if (chek_exist(argv[2]))
        {
            cout << "Extracting\n";
        }
    }
    else if (*argv[1] == 'a')
    {
        if (chek_exist(argv[2]))
        {
            cout << "Archiving\n";
            archive(target, argv[argc - 1]);
        }
    }
    return 0;
}
