#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>  //crypt
#include <crypt.h>
int main()
{
    printf("1testpassword: %s\n", crypt("1testpassword", "$1$ab$"));

    printf("testpassword2: %s\n", crypt("testpassword2", "$1$ab$"));

    return 0;
}

/*
1testpassword:$1$ab$pB70lXqwCgb5UsfmZSxEQ0
testpassword2:$1$ab$cItnXSJ/K7kB0mF6AuTpl0
*/
