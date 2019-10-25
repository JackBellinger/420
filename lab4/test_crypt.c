#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>  //crypt
#include <crypt.h>
int main()
{
    printf("%s\n", crypt("testpassword", "$5$ab$"));

    printf("%s\n", crypt("testpassword0", "$5$ab$"));
        
    printf("%s\n", crypt("testpassword\n", "$5$ab$"));
        
    return 0;
}

