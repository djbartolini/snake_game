#include <iostream>
#include <unistd.h> // usleep()
#include <cstdio>
#include <cstdlib> // srand()
#include "ncurses.h"// refresh

bool gameOver;
// bounds for game
const int width = 30;
const int height = 30;
// game variables
int x, y, fruitX, fruitY;
int tailX[100], tailY[100];
int nTail;
int score;
enum eDir { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDir dir;

void Setup() {
  // ncurses initialization
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  gameOver = false;

  // initialize new snake (player) in center of map
  dir = STOP;
  x = width / 2;
  y= height / 2;
  
  // random starting fruit
  fruitX = rand() % width;
  fruitY = rand() % height;
  
  score = 0;
}

void Draw() {
  clear();
 
  // draw the map boundaries
  // top boundary `#`
  for (int i = 0; i < width; i++) {
    mvprintw(0, i, "# ");
  }
  // middle portion
  // can contain boundary `#`, snake head `O`, snake tail `o`, or fruit `X`
  // checks the current value of each and draws window accordingly
  for (int i = 1; i < height - 1; i++) {
    mvprintw(i, 0, "#");
    for (int j = 1; j < width - 1; j++) {
      mvprintw(i, j, " ");

      if ((i - 1) == y && (j - 1) == x) {
        mvprintw(i, j, "O");
      } else if ((i - 1) == fruitY && (j - 1) == fruitX) {
        mvprintw(i, j, "X");
      } else {
        bool isTailSegment = false;
        for (int k = 0; k < nTail; k++) {
          if (tailX[k] == (j - 1) && tailY[k] == (i - 1)) {
            isTailSegment = true;
            mvprintw(i, j, "o");
            break;
          }
        }
        if (!isTailSegment) {
          mvprintw(i, j, " ");
        }
      }
    }
    mvprintw(i, width - 1, "#");
  }
  // bottom boundary `#`
  for (int i = 0; i < width; i++) {
    mvprintw(height - 1, i, "#");
  }
  
  // print score
  mvprintw(0, width + 2, "Score: %d", score);

  refresh();
}

// use `ncurses` for keyboard inputs
void Input() {
  int ch = getch();

  switch (ch) {
  case 'a':
    dir = LEFT;
    break;
  case 'd':
    dir = RIGHT;
    break;
  case 'w':
    dir = UP;
    break;
  case 's':
    dir = DOWN;
    break;
  case 'x':
    gameOver = true;
    break;
  }
}

// game logic
void Logic() {
  // tail position made by finding the previous 2 tails coordinates
  int prevX = x;
  int prevY = y;
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;

  // your score increases and tail grows for every fruit eaten
  if (x == fruitX && y == fruitY) {
    score += 10;

    fruitX = rand() % width;
    fruitY = rand() % height;

    // check to ensure new fruit doesn't spawn at the snake
    for (int i = 0; i < nTail; i++) {
      if (fruitX == tailX[i] && fruitY == tailY[i] || fruitX == x && fruitY == y) {
        fruitX = rand() % width;
        fruitY = rand() % height;
      }
    }

    nTail++;
  }

  // shift tail positions in the arrays
  for (int i = 1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }


  // 'listen' for key inputs
  switch (dir) {
    case LEFT:
      x--;
      break;
    case RIGHT:
      x++;
      break;
    case UP:
      y--;
      break;
    case DOWN:
      y++;
      break;
    default:
      break;
  }

  // you die if you hit the boundary
  if (x >= width || x < 0 || y >= height || y < 0) {
    gameOver = true;
  }

  // you die if you eat your tail
  for (int i = 0; i < nTail; i++) {
    if (tailX[i] == x && tailY[i] == y) {
      gameOver = true;
    }
  }
}

// cleanup ncurses functions
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
