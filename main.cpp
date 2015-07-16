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
    int files_count = 0;
    char* files[argc - 2];
    if (argc < 3)
    {
        cout << "Wrong amount of arguments" << endl;
        return 0;
    }

    if (*argv[1] == 'x')
    {
        for (int i = 2; i < argc - 1; i++)
            if (chek_exist(argv[i]))

        if (chek_exist(argv[2]))
        {
            cout << "Extracting\n";
            decompression_extract(target, argv[argc - 1]);
        }
    }
    else if (*argv[1] == 'a')
    {
        for (int i = 2; i < argc - 1; i++)
            if (chek_exist(argv[i]))
                files[files_count++] = argv[i];
            files[files_count++] = argv[argc - 1];
            cout << "Archiving\n";
            archive(files, files_count);
    }
    return 0;
}
