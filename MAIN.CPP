#include "TANCHISHE.h"
#include <vector>
map<char,double> level{{'D',0.5},{'C',0.4},{'B',0.3},{'A',0.2},{'S',0.1}};
vector<shared_ptr<Snake>> snake;

void init_win(void){
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr,false);
    keypad(stdscr,true);
    if(has_colors()) start_color();
    curs_set(0);
}
void close_win(void){
    endwin();
}
void draw_borden(){
    attron(A_REVERSE);
	box(stdscr,0,0);
    mvprintw(0,COLS/2-16,"Score:%d,Max Score:%d,Level:%c,length:%d",score,max_score,cur_level,snake.size());

}
void draw(){
    clear();
    draw_borden();
    auto iter=snake.begin();
    while (iter!=snake.end()){
		auto ptr=*iter++;
        mvaddch(ptr->x,ptr->y,ptr->val);
    }
    mvaddch(food->x,food->y,food->val);
    refresh();
    attrset(A_NORMAL);
}
void init_snake(){
    snake.push_back(shared_ptr<Snake>(new Snake({LINES/2,COLS/2,direct})));
}

void del_snake(){
	while(snake.begin()!=snake.end()){
		snake.pop_back();
	}
}

bool is_valid(int a,int b){
	auto iter=snake.begin();
	while (iter!=snake.end()){
		auto ptr=*iter++;
		if (ptr->x==a && ptr->y==b) return false;
	}
	return true;
    //if (head->x==a && head->y==b) return false;
    //auto iter=head->next;
    //while (iter!=NULL){
    //    if (iter->x==a && iter->y==b) return false;
    //    iter=iter->next;
    //}
    //return true;
}
void create_food(){
    int a=rand()%(LINES-2)+1;
    int b=rand()%(COLS-2)+1;
    while(!is_valid(a,b)){
        a=rand()%(LINES-2)+1;
        b=rand()%(COLS-2)+1;
    }
    if (food==NULL) food=shared_ptr<Food>(new Food({a,b}));
    else food->set_value(a,b);
}
void get_keypad(){
    char choice;
    while ((choice=getch())){
        if (choice=='q'){
            m.lock();
            quit_flag=true;
            m.unlock();
            break;
        }
		if (choice=='r'){
			m.lock();
			reset_flag=true;
			m.unlock();
		}
        m.lock();
        switch(choice){
            case 'a':new_direct=LEFT;break;
            case 's':new_direct=DOWN;break;
            case 'd':new_direct=RIGHT;break;
            case 'w':new_direct=UP;break;
        }
        m.unlock();
    }
}


void change_delta(){
	switch(snake.size()/20){
		case 0:delta=level['D'];cur_level='D';break;
		case 1:delta=level['C'];cur_level='C';break;
		case 2:;
		case 3:delta=level['B'];cur_level='C';break;
		case 4:;
		case 5:;
		case 6:delta=level['A'];cur_level='A';break;
		default:delta=level['S'];cur_level='S';
	} 

}


void move(){
    auto iter=snake.begin();
	int cur_direct=direct;
	int temp;
	auto ptr=*iter++;
	int a=snake.back()->x;
	int b=snake.back()->y;
	int c=snake.back()->z;
	ptr->turn(cur_direct);
	ptr->move();
	while (iter!=snake.end()){
		ptr=*iter++;
		temp=ptr->z;
		ptr->move();
		ptr->turn(cur_direct);
		cur_direct=temp;
	}
	if ((*snake.begin())->x==food->x && (*snake.begin())->y==food->y){
		if (snake.size()==1) c=direct;
		snake.push_back(shared_ptr<Snake>(new Snake({a,b,c})));
		create_food();
		score+=10;
		change_delta();
	}
}

void move_and_show(){
    while (true){
		win_flag=(*snake.begin())->is_win(int(snake.size()));
		over_flag=(*snake.begin())->is_over();
		

        if ((direct<=1 && new_direct>=2) || (direct>=2 && new_direct<=1)){
            m.lock();
            direct=new_direct;
            m.unlock();
        }
        if (quit_flag){
            m.lock();
            quit_flag=false;
            m.unlock();
            break;
        }
		if (reset_flag){
			m.lock();
			reset_flag=false;
			win_flag=false;
			over_flag=false;
			direct=UP;
			delta=0.5;
			cur_level='D';
			if (score>max_score) max_score=score;
			score=0;
			m.unlock();
			del_snake();
			init_snake();
			create_food();
		}
    	if (win_flag){
        	clear();
        	draw_borden();
        	mvprintw(LINES/2,COLS/2-8,"you are winner.");
			mvprintw(LINES/2+1,COLS/2-18,"Please enter \'q\' exit or \'r\' continue");
    		refresh();
    		attrset(A_NORMAL);
    	}
    	else if (over_flag){
        	clear();
        	draw_borden();
        	mvprintw(LINES/2,COLS/2-5,"game over.");
			mvprintw(LINES/2+1,COLS/2-18,"Please enter \'q\' exit or \'r\' continue");
    		refresh();
    		attrset(A_NORMAL);
    	}
		else{
			move();
			draw();
		}
        usleep(delta*1000000);
    }


}




int main(){
    srand((int)time(0));
    init_win();
    init_snake();
    create_food();
    thread t1(get_keypad),t2(move_and_show);
    t1.join();
    t2.join();
    close_win();

}

