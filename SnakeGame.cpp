#include <iostream>
#include <unistd.h> // usleep()
#include <cstdio>
#include "ncurses.h"// refresh

bool gameOver;
const int width = 30;
const int height = 30;
int x, y, fruitX, fruitY;
int score;
enum eDir { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDir dir;

using namespace std;

void Setup() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  
  gameOver = false;
  dir = STOP;
  x = width / 2;
  y= height / 2;
  
  fruitX = rand() % width;
  fruitY = rand() % height;
  
  score = 0;
}

void Draw() {
  clear();
 
  for (int i = 0; i < width; i++) {
    mvprintw(0, i, "#");
  }

  for (int i = 1; i < height - 1; i++) {
    mvprintw(i, 0, "#");
    for (int j = 1; j < width - 1; j++) {
      mvprintw(i, j, " ");

      if ((i - 1) == y && (j - 1) == x) {
        mvprintw(i, j, "O");
      } else if ((i - 1) == fruitY && (j - 1) == fruitX) {
        mvprintw(i, j, "X");
      }

    }
    mvprintw(i, width - 1, "#");
  }

  for (int i = 0; i < width; i++) {
    mvprintw(height - 1, i, "#");
  }
  
  refresh();
}


void Input() {
  
}

void Logic() {
  
}
void Cleanup() {
  endwin();
}

int main() {
  Setup();
  
  while (!gameOver) {
    Draw();
    Input();
    Logic();
    usleep(1000);
  }
  
  Cleanup();
  
  return 0;
}