
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    chdir("/..");

    char buffer[200] ={0};
    getcwd(buffer , 200);

    printf("buffer:%s \n" , buffer );

    return 0 ;
}
