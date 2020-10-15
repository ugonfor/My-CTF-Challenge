#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ptrace.h>

#include "crc32.h"

void init(void) __attribute__((constructor (101)));
void init(void){
    if (ptrace(PTRACE_TRACEME,0,NULL,NULL) != 0){
        printf("input: ");
        exit(EXIT_FAILURE);
    }
}
int size_f[5] = {0xa9,0xcb,0x80,0xb4,0};
int stage1();
int stage2();
int stage3();
int stage4();
int main(int argc, char const *argv[]);


char flag[55] = "You_could_feel_the_various_flavors_of_reversing,right?";

int stage1(){ //parent
    printf("You have to pass the anti-debugging\n");
    printf("Then, change the ZF\n");
    return 0;
}

int stage2(){ //child
    char buf[20];
    printf("input: ");
    scanf("%s",buf);
    printf("Key : %s\n",buf);
    if(strlen(buf) != 13){
        return 1;
    }

    xordata(&stage3,buf,size_f[1]);
    stage3();
    return 0;
}


int stage3(){ 
    char buf[20] = {0};
    printf("input: ");
    scanf("%s",buf);
    printf("Key : %s\n",buf);
    if(strlen(buf) != 19){
        return 1;
    }
    xordata(&stage4,buf,size_f[2]);
    stage4();
    return 0;
}

int stage4(){
    int buf[2] = {crc32(0x4008E0,0x52c),0};
    xordata(flag,buf,55);
    printf("\n\tFlag: %s\n",flag);
    return 0;
}

int xordata(char* addr, char key[], size_t size){
    size_t len = strlen(key);
    for (size_t i = 0; i < size; i++)
    {
        *(addr+i) = *(addr+i) ^ key[i%len];
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    mprotect(0x400000,0x5000, 7);
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    setvbuf(stderr,0,2,0);

    char buf[10];
    printf("input: ");
    scanf("%s",buf);
    if(strlen(buf) != 2){
        return 1;
    }
    printf("Key : %s\n",buf);
    xordata(0x400CC4,buf,size_f[0]);            // key: GO

    printf("\nHey, you complete the Step1. Good luck!\n");

    int pid = fork();
    if(pid)
    {
        stage1();
    }
    else
    {       
        char buf2[10];
        printf("input: ");
        scanf("%s",buf2);
        if(strlen(buf2) != 5){
            return 1;
        }
        xordata(&stage2,buf2,size_f[3]);
        stage2();
    }
    return 0;
}

int foo(void){
    printf("foo");
    return 0;
}
