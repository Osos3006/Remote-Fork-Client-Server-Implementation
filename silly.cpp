#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "myfork.h"
using namespace std;

int main()
{
    string x = myfork("192.168.1.80", 2002);

    for (int i = 0; i < 10; i++)
    {
        sleep(1);
        cout << i << " " <<  x << endl;
    }

    return 0;
}