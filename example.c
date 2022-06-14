#include <stdio.h>
#include "fdw.h"

void print()
{
    printf("Callback method called\n");
    fflush(stdout);
}

int main()
{
    watch_dir("testdir/", &print);
}
