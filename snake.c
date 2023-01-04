#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DELAY 50000
#define TIMEOUT 10

// game structs
typedef enum {
    LEFT, 
    RIGHT, 
    UP, 
    DOWN
} direction;

typedef struct s_head {
   int location_y;
   int location_x;
   struct s_head* next;
} snake_head;

typedef struct g_board {
   int fruit[2];
   int game_over;
   int height; 
   int width;
} game_board;

// Function prototypes
void spawn_fruit(WINDOW *game);

// Global variables
game_board board = {.height = 53, .width = 63, .game_over = 0};
snake_head head;
direction currentDir = RIGHT;
int rows, cols;

int main(void)
{
    initscr();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    cbreak();
    timeout(TIMEOUT);

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
    head.location_y = (board.height / 2);
    head.location_x = (board.width / 2);
    mvwprintw(game, head.location_y, head.location_x, "0");
    wrefresh(game);

    int ch;
    while(1)
    {
        //input
        ch = getch();
        if (ch == KEY_RIGHT)
        {
            currentDir = RIGHT;
        }
        else if (ch == KEY_UP)
        {
            currentDir = UP;
        }
        else if (ch == KEY_LEFT)
        {
            currentDir = LEFT;
        }
        else if (ch == KEY_DOWN)
        {
            currentDir = DOWN;
        }

        //movement

        if (currentDir == RIGHT)
        {
            head.location_x++;
        }
        else if (currentDir == UP)
        {
            head.location_y--;
        }
        else if (currentDir == LEFT)
        {
            head.location_x--;
        }
        else if (currentDir == DOWN)
        {
            head.location_y++;
        }

        //print stuff
        mvwprintw(game, head.location_y, head.location_x, "0");
        mvwprintw(game, head.location_y, head.location_x - 1, " ");
        wrefresh(game);
        usleep(DELAY);
    }

    //apparently the getchar() here was fucking everything up? But I had to include it initially to get the program to run? Idk man I'm so confused
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
