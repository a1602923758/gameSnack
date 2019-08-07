#ifndef __TANCHISHE_H__
#define __TANCHISHE_H__
#include <iostream>
#include <curses.h>
#include <map>
#include <memory>
#include <random>
#include <unistd.h>
#include <thread>
#include <mutex>

using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Food{
    public:
        Food(int a,int b):x(a),y(b),val('#'){}
        void set_value(int a,int b);
        int x,y;
        char val;
};


class Snake{
    public:
        Snake(int a,int b,int c):val('@'),x(a),y(b),z(c){}
        void move();
        void turn(int c);
        bool is_over();
        bool is_win(int len);
        char val;
        int x,y;
        int z;


};

extern int row;
extern int col;
extern int score;
extern int max_score;
extern char cur_level;
extern int direct;
extern int new_direct;
extern mutex m;
extern bool quit_flag;
extern bool win_flag;
extern bool over_flag;
extern bool reset_flag;
extern double delta;

extern shared_ptr<Food> food;




#endif
