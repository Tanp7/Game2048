#include <algorithm>
#include <bits/stdc++.h>
#include <curses.h>
#include <vector>
using namespace std;

#define N 4
#define WIDTH 5
#define TARGET 2042
#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3

class Game2048 {
public:
    Game2048() : status(S_NORMAL){
        setTestData();
    }

    int getStatus(){ return status; }

    void processInput(){
        char ch = getch();
        if (ch>='a' && ch <= 'z'){
            ch-=32;
        }
        if(status == S_NORMAL){
            bool updated = false;
            if(ch=='A'){
                updated = moveLeft();
            } else if(ch=='S'){
                rotate();
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
            } else if(ch == 'D'){
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
            } else if(ch=='W'){
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
                rotate();
            }

            if (updated){
                randNew();
                if(isOver()){
                    status = S_FAIL;
                }
            }
        }

        if (ch=='Q'){
            status = S_QUIT;
        } else if (ch == 'R'){
            restart();
        }
    }

    void draw(){
        clear();
        const int offset = 12;
        for(int i = 0; i<=N; ++i){
            for(int j = 0; j<=N;++j){
                drawItem(i*2,1+j*WIDTH + offset, '+');

                if (i != N){
                    drawItem(i*2+1,1+j*WIDTH + offset,'|');
                }

                for (int k = 1; j!=N && k < WIDTH; ++k){
                    drawItem(i*2, 1+j*WIDTH +k + offset, '-');
                }

                if (i!=N && j != N){
                    drawNum(i*2 + 1, (j+1)*WIDTH+offset, data[i][j]);
                }
            }
        }


        mvprintw(2*N+2,(5*(N-4)-1)/2,"W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
        mvprintw(2*N+3,12+5*(N-4)/2,"Game2048");

        if (status == S_WIN){
            mvprintw(N,5*N/2-1,"YOU WIN,PRESS R TO CONTINUE ");
        } else if(status == S_FAIL){
            mvprintw(N, 5*N/2-1,"YOU LOSE,PRESS R TO CONTINUE ");
        }
    }
    void setTestData(){
        for(int i = 0; i<=N; ++i){
            for (int j = 0; j < N; ++j){
                data[i][j] = 16 << (i+j);
            }
        }
    }

private:
    bool isOver(){
        for(int i = 0; i<N; ++i){
            for (int j = 0; j < N; ++j){
                if ((j+1<N) && (data[i][j] * data[i][j+1] == 0 || data[i][j] == data [i][j+1]))
                    return false;
                if ((i+1<N) && (data[i][j] * data[i+1][j] == 0 || data[i][j] == data [i+1][j]))
                    return false;
            }
        }
        return true;
    }

    bool moveLeft(){
        int tmp[N][N];
        for(int i = 0; i<N; ++i){
            int currentWritePos = 0;
            int lastValue = 0;
            for (int j = 0; j<N; ++j){
                tmp[i][j]=data[i][j];
                if (data[i][j] == 0){
                    continue;
                }

                if(lastValue == 0){
                    lastValue = data[i][j];
                } else {
                    if (lastValue == data[i][j]){
                        data[i][currentWritePos] = lastValue * 2;
                        lastValue = 0;
                        if (data[i][currentWritePos] == TARGET){
                            status = S_WIN;
                        }
                    }else {
                        data[i][currentWritePos] = lastValue;
                        lastValue = data[i][j];
                    }
                    ++currentWritePos;
                }
                data[i][j]=0;
            }
            if (lastValue != 0){
                data[i][currentWritePos] = lastValue;
            }
        }

        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j){
                if(data[i][j]!=tmp[i][j]) return true;
            }
        }
        return false;
    }

    void rotate(){
        int tmp[N][N] = {0};
        for (int i = 0; i< N; ++i){
            for (int j = 0;j<N; j++){
                tmp[i][j] = data[j][N-1-i];
            }
        }
        for(int i = 0; i<N; i++){
            for (int j = 0; j<N; j++){
                data[i][j] = tmp[i][j];
            }
        }
    }


    void restart(){
        for(int i = 0;i < N; i++){
            for (int j = 0; j<N; j++){
                data[i][j]=0;
            }
        }
        randNew();
        randNew();
        status = S_NORMAL;
    }



    bool randNew(){
        vector<int> emptyPos;
        for(int i = 0; i < N; i++){
            for(int j = 0; j<N; j++){
                if (data[i][j] == 0){
                    emptyPos.push_back(i*N+j);
                }
            }
        }

        if (emptyPos.size() == 0){
            return false;
        }

        int value = emptyPos[rand() % emptyPos.size()];
        data[value / N][value % N] = rand() % 10 == 1 ? 4 : 2;
        return true;
    }



    void drawItem(int row, int col, int c){
        move(row,col);
        addch(c);
    }


    void drawNum(int row, int col, int num){
        while(num > 0){
            drawItem(row,col, num%10+'0');
            num /= 10;
            --col;
        }
    }



private:
    int data[N][N];
    int status;
};

void initialize(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    srand(time(NULL));
}


void shutdown(){
    endwin();
}


int main(){
    initialize();

    Game2048 game;
    do{
        game.draw();
        game.processInput();
    } while (S_QUIT != game.getStatus());

    shutdown();
    return 0;
}
