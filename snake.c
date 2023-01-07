#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DELAY 220000
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
   direction CurrentDir;
} snake_part;

typedef struct g_board {
   int fruit_y;
   int fruit_x;
   int game_over;
   int height; 
   int width;
} game_board;

// Function prototypes
void spawn_fruit(WINDOW *game);
void input(int ch);
void movement(int rows, int cols);
void draw(WINDOW *game);

// Global variables
game_board board = {.game_over = 0};
snake_part head;
snake_part tail;
int rows, cols;
int part_count;

int main(void)
{
    srand(time(NULL));
    initscr();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    cbreak();
    timeout(TIMEOUT);
    head.CurrentDir = RIGHT;
    part_count = 1;

    //establish height/width for game area and center it
   int rows, cols;
   getmaxyx(stdscr, rows, cols);
   board.height = rows;
   board.width = cols / 3;
   int start_y = (rows - board.height) / 2; 
   int start_x = (cols - board.width) / 2; 
   refresh();

   // Create window for game area and put a border around it
   WINDOW *game = newwin(board.height, board.width, start_y, start_x);
   box(game, 0, 0); 
   wrefresh(game);

    

    // Set initial coordinates for snake head and print
    head.location_y = (board.height / 2);
    head.location_x = (board.width / 2);
    mvwprintw(game, head.location_y, head.location_x, "0");
    wrefresh(game);
    spawn_fruit(game);

    int ch;
    while(board.game_over == 0)
    {

        // Take user input
        input(ch);
        
        // Move snake
        movement(rows, cols);

        // Collision detection
        if (head.location_y == 0 || head.location_y == board.height - 1 || head.location_x == 0|| head.location_x == board.width - 1)
        {
            
            board.game_over++;
        }

        if (head.location_y == board.fruit_y && head.location_x == board.fruit_x)
        {
            spawn_fruit(game);
            part_count++;
        }
        

        // draw stuff
        draw(game);

        if (board.game_over == 1)
        {
            sleep(2);
        }
        
    }

    //apparently the getchar() here was fucking everything up? But I had to include it initially to get the program to run? Idk man I'm so confused
    endwin();
    return 0;
}

// Function to spawn fruit within board parameters
void spawn_fruit(WINDOW *game)
{
    board.fruit_y = rand() % (board.height - 10) + 5;
        
    board.fruit_x = rand() % (board.width - 20) + 10;
}

// Function to read keyboard input from user
void input(int ch)
{
    ch = getch();
        if (ch == KEY_RIGHT)
        {
            head.CurrentDir = RIGHT;
        }
        else if (ch == KEY_UP)
        {
            head.CurrentDir = UP;
        }
        else if (ch == KEY_LEFT)
        {
            head.CurrentDir = LEFT;
        }
        else if (ch == KEY_DOWN)
        {
            head.CurrentDir = DOWN;
        }
}

// Function to set which direction the snake moves in
void movement(int rows, int cols)
{
    if (head.CurrentDir == RIGHT)
    {
        head.location_x++;
    }
    else if (head.CurrentDir == UP)
    {
        head.location_y--;
    }
    else if (head.CurrentDir == LEFT)
    {
        head.location_x--;
    }
    else if (head.CurrentDir == DOWN)
    {
        head.location_y++;
    }
    
}

// Function to draw graphics on screen
void draw(WINDOW* game)
{
    // Print fruit
    mvwprintw(game, head.location_y, head.location_x, "#");
    
    //Print snake 
    mvwprintw(game, board.fruit_y, board.fruit_x, "o");
    
    // Erase previous snake part
    switch (head.CurrentDir)
    {
        case RIGHT:
            mvwprintw(game, head.location_y, head.location_x - part_count, " ");
        case LEFT:
            mvwprintw(game, head.location_y, head.location_x + part_count, " ");
        case UP:
            mvwprintw(game, head.location_y + part_count, head.location_x, " ");
        case DOWN:
            mvwprintw(game, head.location_y - part_count, head.location_x, " ");


    }

    // Print "Game Over" message
    if (board.game_over == 1)
    {
        wclear(game);
        mvwprintw(game, (board.height / 2), (board.width / 2), "%s", "GAME OVER");
        sleep(1);
    }
    wrefresh(game);
    usleep(DELAY);
}