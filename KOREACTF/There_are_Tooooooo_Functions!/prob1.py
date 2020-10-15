import random

string = b'KOREA{WELCOME_TO_EASY_REVERSING_CHALLENGE}'
xored = ''
for i in range(len(string)):
    xored += chr(string[i]^i)


tmp = sorted([random.randint(0,10000) for i in range(len(string))])
#print(tmp)
count = 0


t = '''#include <stdio.h>
char tmp = 0;
char count = 0;
'''
print(t)

for i in range(10000):
    if i in tmp:
        t = '''int foo'''+str(i)+'''(void){
    tmp = \''''+xored[count]+"\'"+''';
    printf("%c\\n",tmp^count);
    count++;
    return 0;
}'''
        count += 1
    
    else:
        t = '''int foo'''+str(i)+'''(void){
    count++;
    printf("%s!\\n",\"Find The Flag\");
    count--;
    return 0;
}'''

    print(t)


t = '''
int main(void){
    printf("%s!\","Find The Flag");
    return 0;
}
'''
print(t)