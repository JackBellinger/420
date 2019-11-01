#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>  //crypt
#include <crypt.h>
int main()
{
    printf("120password: %s\n", crypt("120password", "$1$ab$"));

    printf("password123: %s\n", crypt("password123", "$1$ab$"));

    return 0;
}

/*
1testpassword:$1$ab$pB70lXqwCgb5UsfmZSxEQ0
testpassword2:$1$ab$cItnXSJ/K7kB0mF6AuTpl0
*/
