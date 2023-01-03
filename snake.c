#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DELAY 30000

typedef struct s_head
{
   int location[2];
   struct s_head* next;
   int head;
   int direction;
   int next_x;
}
snake_head;

typedef struct g_board
{
   int fruit[2];
   int game_over;
   int height; 
   int width;
}
game_board;

// Function prototypes
void spawn_fruit(WINDOW *game);
void input(WINDOW *game, int rows, int cols);

game_board board = {.height = 53, .width = 63, .game_over = 0};
snake_head head;

int main(void)
{
   initscr();
   noecho();
   curs_set(FALSE);

   //establish height/width for game area and center it
   int rows, cols;
   int start_y = (LINES - board.height) / 2; 
   int start_x = (COLS - board.width) / 2; 
   refresh();

   // Create window for game area and put a border around it
   WINDOW *game = newwin(board.height, board.width, start_y, start_x);
   getmaxyx(game, rows, cols);
   box(game, 0, 0); 
   wrefresh(game);

   // Call spawn_fruit function to spawn initial fruit
   spawn_fruit(game); 

   // Set initial coordinates for snake head and print
   head.location[1] = (board.height / 2);
   head.location[2] = (board.width / 2);
   mvwprintw(game, head.location[1], head.location[2], "0");
   wrefresh(game);

   // Call input function to move snake
   while(true)
   {
      input(game, rows, cols);
      wrefresh(game);
   }
   

   getchar();
   endwin();
   return 0;
}

// Spawn fruit on game board
void spawn_fruit(WINDOW *game)
{
   //initialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    state1:
        board.fruit[0] = rand() % 50;
        if (board.fruit[0] == 0)
        {
            goto state1;
        }
    state2:
        board.fruit[1] = rand() % 60;
        if (board.fruit[1] == 0)
        {
            goto state2;
        }
   
   mvwprintw(game, board.fruit[0], board.fruit[1], "#");
   wrefresh(game);
}

void input(WINDOW *game, int rows, int cols)
{
   keypad(game, TRUE);
   int ch;
   int y = head.location[1];
   int x = head.location[2];
   cbreak();
   while((ch = wgetch(game)) != KEY_END)
   {
      
      switch (ch)
        {
            case KEY_UP:
                if (y > 0)
                {
                    --y;
                    break;
                }
            
            case KEY_DOWN:
                if (y < (rows - 2))
                {
                    ++y;
                    break;
                }
            
            case KEY_LEFT:
                if (x > 0)
                {
                    --x;
                    break;
                }
            
            case KEY_RIGHT:
                if (x < (cols - 2))
                {
                    ++x;
                    break;
                }

            case KEY_HOME:
                x = 0;
                y = 0;
                break;
        }

      mvwprintw(game, y, x, "0");
      wrefresh(game);
   }
}