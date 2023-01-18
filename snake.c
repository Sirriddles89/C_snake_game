#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DELAY 120000
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
   struct s_head *next;
   direction CurrentDir;
} snake_piece;

typedef struct g_board {
   int fruit_y;
   int fruit_x;
   int game_over;
   int score;
   int height; 
   int width;
} game_board;

// Function prototypes
void spawn_fruit(WINDOW *game);
void input(int ch);
void movement(int rows, int cols);
void grow_snake(WINDOW *game);
void collision();
void draw(WINDOW *game);

// Global variables
game_board board = {.game_over = 0, .score = 0};
snake_piece head;
snake_piece *tail;
int rows, cols;
int next_x;
int next_y;
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
   board.height = rows - 2;
   board.width = cols / 3;
   int start_y = (rows - board.height) / 2; 
   int start_x = (cols - board.width) / 2; 
   refresh();

   // Create window for game area
   WINDOW *game = newwin(board.height, board.width, start_y, start_x); 
 
    // Set initial coordinates for snake head
    head.location_y = (board.height / 2);
    head.location_x = (board.width / 2);

    // Spawn initial fruit
    spawn_fruit(game);

    int ch;
    while(board.game_over == 0)
    {

        // Take user input
        input(ch);
        
        // Move snake / tail
        movement(rows, cols);

        // grow snake
        grow_snake(game);

        // Collision detection
        collision();
        

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
    state1:
        board.fruit_y = rand() % (board.height - 10) + 1; 
        board.fruit_x = rand() % (board.width - 20) + 1;
        for (snake_piece *tmp = &head; tmp != NULL; tmp = tmp->next)
        {
            if (tmp->location_y == board.fruit_y && tmp->location_x == board.fruit_x)
            {
                goto state1;
            }
        }
}

// Function to read keyboard input from user
void input(int ch)
{
    ch = getch();
        if (ch == KEY_RIGHT && head.CurrentDir != LEFT)
        {
            head.CurrentDir = RIGHT;
        }
        else if (ch == KEY_UP && head.CurrentDir != DOWN)
        {
            head.CurrentDir = UP;
        }
        else if (ch == KEY_LEFT && head.CurrentDir != RIGHT)
        {
            head.CurrentDir = LEFT;
        }
        else if (ch == KEY_DOWN && head.CurrentDir != UP)
        {
            head.CurrentDir = DOWN;
        }
}

// Function to set which direction the snake moves in
void movement(int rows, int cols)
{
    // Move head
    int temp_y;
    int temp_x;
    direction previous_d;
    direction temp_d;
    if (head.CurrentDir == RIGHT)
    {
        next_x = head.location_x;
        next_y = head.location_y;
        head.location_x++;

    }
    else if (head.CurrentDir == UP)
    {
        next_x = head.location_x;
        next_y = head.location_y;
        head.location_y--;
    }
    else if (head.CurrentDir == LEFT)
    {
        next_y = head.location_y;
        next_x = head.location_x;
        head.location_x--;
    }
    else if (head.CurrentDir == DOWN)
    {
        next_x = head.location_x;
        next_y = head.location_y;
        head.location_y++;
    }

    previous_d = head.CurrentDir;
    // Move body 
    for (snake_piece *tmp = head.next; tmp != NULL; tmp = tmp->next)
    {
        //remember current location and direction
        temp_y = tmp->location_y;
        temp_x = tmp->location_x;
        temp_d = tmp->CurrentDir;
        //update location and direction
        tmp->location_y = next_y;
        tmp->location_x = next_x;
        tmp->CurrentDir = previous_d;
        //update variables
        next_y = temp_y;
        next_x = temp_x;
        previous_d = temp_d;
        
    }
       
}

// Function to grow snake (also spawns new fruit when current fruit is eaten)
void grow_snake(WINDOW *game)
{
    snake_piece *part = NULL;
    snake_piece *temp = NULL;
    if (head.location_y == board.fruit_y && head.location_x == board.fruit_x)
    {        
        part = malloc(sizeof(snake_piece));

        if (part != NULL && part_count == 1)
        {
            switch (head.CurrentDir)
            {
                case RIGHT: 
                    part->location_y = head.location_y;
                    part->location_x = head.location_x - 1;
                    break;
                case UP: 
                    part->location_y = head.location_y + 1;
                    part->location_x = head.location_x;
                    break;
                case LEFT: 
                    part->location_y = head.location_y;
                    part->location_x = head.location_x + 1;
                    break;
                case DOWN:
                    part->location_y = head.location_y - 1;
                    part->location_x = head.location_x;
                    break;
            }

            part->CurrentDir = head.CurrentDir;
            part->next = NULL;
            head.next = part;
            tail = part;
        }

        else if (part != NULL)
        {
            switch (tail->CurrentDir)
            {
                case RIGHT: 
                    part->location_y = tail->location_y;
                    part->location_x = tail->location_x - 1;
                    break;
                case UP: 
                    part->location_y = tail->location_y + 1;
                    part->location_x = tail->location_x;
                    break;
                case LEFT: 
                    part->location_y = tail->location_y;
                    part->location_x = tail->location_x + 1;
                    break;
                case DOWN:
                    part->location_y = tail->location_y - 1;
                    part->location_x = tail->location_x;
                    break;
            }
            part->CurrentDir = tail->CurrentDir;
            part->next = NULL;
            tail->next = part;
            tail = part;
            
        }
        
        spawn_fruit(game);
        board.score += 10;
        part_count++;
    }
}

// Function to detect running into walls or self
void collision()
{
    // Collision detection (border)
    if (head.location_y == 0 || head.location_y == board.height - 1 || head.location_x == 0|| head.location_x == board.width - 1)
    {
        
        board.game_over++;
    }

    // Collision detection (body)
   
    for (snake_piece *tmp = head.next; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->location_y == head.location_y && tmp->location_x == head.location_x)
        {
            board.game_over++;
        }
    }

    

}

// Function to draw graphics on screen
void draw(WINDOW* game)
{
    // clear screen of former snake parts
    werase(game);

    // draw border
    box(game, 0, 0);

    //update score
    mvprintw(0, 0, "Score: %i", board.score);

    // Print snake

    for (snake_piece *tmp = &head; tmp != NULL; tmp = tmp->next)
    {
        mvwprintw(game, tmp->location_y, tmp->location_x, "#");
    }
    
    //Print fruit 
    mvwprintw(game, board.fruit_y, board.fruit_x, "o");
    
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