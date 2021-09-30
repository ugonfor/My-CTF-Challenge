
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include "crc32.h"

char map[500][500];
char discovered[500][500];
char mines[500][500];
int mines_cnt;

std::vector<int> x_order;
std::vector<int> seed_all;

char boom;
int x,y;

void init_map(){
    memset(map,'#', sizeof(map));
    memset(discovered,0,sizeof(discovered));
    memset(mines,0,sizeof(mines));
}


// ans
std::vector<std::pair<int,int>> v_ans;
std::pair<int,int> ans;


void set_maps(int level){
    int seed = 1;
    for (auto i = x_order.begin(); i != x_order.end(); i++)
    {
        seed *= *i;
        if ( (seed & 0xff) == 0) {
            seed = seed/0x100;
        }

    }
    x_order.clear();
    srand(seed);
    seed_all.push_back(seed);
    int x = 0; int y = 0;
    for (int i = 0; i < level; i++)
    {
        x = rand()%(level-2) + 1;
        y = rand()%(level-2) + 1;
        
        while (mines[x][y] == '*'){
            x = rand()%(level-2) + 1;
            y = rand()%(level-2) + 1;
        }

        ans.first=x;
        ans.second=y;
        v_ans.push_back(ans);

        mines[x][y] = '*';
        if (mines[x+1][y] != '*') mines[x+1][y] += 1;
        if (mines[x-1][y] != '*') mines[x-1][y] += 1;
        if (mines[x][y+1] != '*') mines[x][y+1] += 1;
        if (mines[x][y-1] != '*') mines[x][y-1] += 1;
    }
    
}

void print_map(int level){
    printf("==== LEVEL %d ====\n", level);
    for (int i = 0; i < level; i++)
    {
        for (int j = 0; j < level; j++)
        {
            if (!discovered[i][j]) printf("%c", map[i][j]);
            else if (mines[i][j] == '*') printf("%c", mines[i][j]);
            else printf("%d", mines[i][j]);
        }
        printf("\n");
    }
}

int Menu(int level){
    int ret = 0;
    while(1){
        print_map(level);
        printf("MENU\n");
        printf("1. Select mine's location\n");
        printf("2. Select safe ground location\n");
        printf("3. Show Me the Map\n");
        printf("4. Exit\n");
        printf("-> ");
        //scanf("%d", &ret);
        ret = 1;
        if(ret < 1) printf("Wrong Menu\n");
        else if(ret > 4) printf("Wrong Menu\n");
        else return ret;
    }
}

void Select_mine(int level){
    // 맵을 보여줌
    print_map(level);

    // 위치 선정
    int X=0; int Y=0;
    printf("MINES LOC\n");
    printf("X: ");
    //scanf("%d", &X);
    printf("Y: ");
    //scanf("%d", &Y);
    X = v_ans[0].first;
    Y = v_ans[0].second;
    v_ans.erase(v_ans.begin());

    // 만약에 마인이 없으면 실패
    if(mines[X][Y] == '*'){
        printf("There is a Mine!!!! ^_^!!!\n");
        discovered[X][Y] = 1;
        mines_cnt++;
        x_order.push_back(X);
    }
    else{
        printf("There is No mines -_-;\n");
        exit(0);
    }
}


void Select_safe(int level){
    // 맵을 보여줌
    print_map(level);

    // 위치 선정
    int X=0; int Y=0;
    printf("SAFE GROUND LOC\n");
    printf("X: ");
    scanf("%d", &X);
    printf("Y: ");
    scanf("%d", &Y);

    // 만약에 마인이 있으면 실패
    if(mines[X][Y] == '*'){
        printf("There is a mine -_-;\n");
        exit(0);
    }
    else{
        printf("There is no Mines!!!! ^_^!!!\n");
        discovered[X][Y] = 1;
    }
}

void play(int level){
    while (mines_cnt != level)
    {
        int menu = Menu(level);
        switch (menu)
        {
        case 1:
            Select_mine(level);
            break;
        case 2:
            Select_safe(level);
            break;
        case 3:
            print_map(level);
            break;
        case 4:
            exit(0);
            break;
        default:
            break;
        }
    }
}

void flag(std::vector<int> seed_All){
    char data[121]= {0,};
    for (int i = 0; i < 121; i++)
    {
        data[i] = seed_All[seed_All.size() - 121 + i] & 0xff;
    }
    
    uint8_t xor_data[121] = {195, 119, 125, 47, 218, 187, 141, 215, 247, 143, 19, 211, 42, 237, 183, 203, 33, 250, 223, 227, 196, 223, 212, 136, 233, 133, 71, 24, 132, 50, 244, 157, 223, 249, 40, 139, 228, 144, 165, 78, 39, 5, 88, 232, 225, 8, 225, 225, 50, 201, 148, 231, 177, 51, 150, 169, 52, 103, 237, 
38, 31, 216, 254, 208, 136, 25, 239, 157, 40, 225, 65, 189, 15, 159, 136, 1, 232, 145, 103, 254, 165, 242, 229, 139, 9, 229, 235, 121, 149, 52, 131, 121, 204, 205, 159, 175, 158, 218, 110, 236, 161, 87, 86, 161, 217, 66, 142, 124, 249, 31, 135, 47, 178, 91, 16, 33, 164, 138, 119, 190, 87};
    
    for (int i = 0; i < 121; i++)
    {
        data[i] ^= xor_data[i];
    }

    if (crc32(data,121) == 0x641f36c4) printf("CLEAR! %s\n", data);
    else printf("FAIL.... CHEER UP!\n");
}

int main(int argc, char const *argv[])
{
    for (int i = 5; i < 150; i++)
    {
        v_ans.clear();
        init_map();
        set_maps(i);
        play(i);
        mines_cnt = 0;
        //sleep(0.5);
    }

    printf("SEED ALL\n");
    for (int i = 0; i < seed_all.size(); i++)
    {
        printf("0x%x,", seed_all[i] & 0xff);
    }
    
    flag(seed_all);

    
    
    return 0;
}
