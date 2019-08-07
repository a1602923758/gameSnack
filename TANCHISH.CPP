#include "TANCHISHE.h"

int row=0;
int col=0;
int score=0;
int max_score=0;
char cur_level='D';
int direct=UP;
int new_direct=UP;
mutex m;
bool quit_flag=false;
bool win_flag=false;
bool over_flag=false;
bool reset_flag=false;
double delta=0.5;
shared_ptr<Food> food;

void Food::set_value(int a,int b){
    x=a;
    y=b;
}


void Snake::move(){
    switch(z){
        case 0:x-=1;break;
        case 1:x+=1;break;
        case 2:y-=1;break;
        case 3:y+=1;break;
    }
}

void Snake::turn(int c){
    z=c;
}


bool Snake::is_over(){
    if (x==0 || x==LINES-1) return true;
    if (y==0 || y==COLS-1) return true;
    return false;
}

bool Snake::is_win(int len){
    if (len>=(LINES-1)*(COLS-1)) return true;
    return false;
}

