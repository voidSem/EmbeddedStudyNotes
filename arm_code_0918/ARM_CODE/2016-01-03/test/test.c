
#include <stdio.h>


# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))

#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a),&(a)[0]))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

int main(void)
{
    int a ; 
    int b ; 
    float c ; 

    int buffer[0];

    //类型一致返回1   否则返回值
    printf("%d \n" , __same_type(a , b ));

    printf("%d \n" , __same_type(a , c ));
        
    printf("%d \n" , sizeof(buffer));   //0
    printf("%d \n" , sizeof(buffer+0)); //8

    //int buffer[-!!(e)] ; 
    
    printf("struct: %d  \n" , sizeof(struct {int:1;}));   



    return 0 ; 
}
