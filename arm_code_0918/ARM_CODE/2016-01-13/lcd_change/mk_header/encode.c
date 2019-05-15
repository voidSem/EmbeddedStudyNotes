
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define   RGB(r,g,b)      ((r<<16)|(g<<8)|b)

int main(int argc , char *argv[])
{

    FILE *bmpfile = NULL ; 

    FILE *headfile = NULL ; 

    bmpfile = fopen(argv[1],"r");
    if(NULL ==  bmpfile)
    {
        perror("Open bmpfile fail");
        return -1 ; 
    }
    
    fseek(bmpfile , 54 , SEEK_SET);

    headfile = fopen(argv[2],"a");
    if(NULL == headfile)
    {
        perror("Open headfile fail");
        return -2 ; 
    }

    unsigned char buffer[3] ; 
    unsigned int Value ; 
    int row , col ; 
    for(row = 0 ; row < 480 ; row++)
    {
        for(col = 0 ; col < 800 ; col++)   
        {
            fread(buffer, 3 , 1 , bmpfile);
            Value = RGB(buffer[2],buffer[1],buffer[0]);
            fprintf(headfile , "%#8x , " , Value);
            if(col % 10 == 0)
                fprintf(headfile , "\n");
        }
    }

    fclose(bmpfile);
    fclose(headfile);

    return 0 ; 
}
