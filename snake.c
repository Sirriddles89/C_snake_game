#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//game variables
int game_over, score;
int length = 28;
int width = 40;
int X, Y, food_x, food_y, flag;

//function prototypes
void boundary();
void setup();

int main(void)
{
    
    // call setup to generate fruit
    setup();
    //call boundary to draw game area around fruit
    boundary();
}

//boundary function - draws the space where gameplay will take place
void boundary()
{
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j <= width; j++)
        {
            
            if (i == 0 || i == length)
            {
                printf("*");
            }
            else if (i > 0 && j == 0 || i > 0 && j == width)
            {
                printf("*");
            }
            else
            {
                if (i == X && j == Y)
                {
                    printf("0");
                }
                else if (i == food_x && j == food_y)
                {
                    printf("#");
                }
                else 
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

//setup function - generates food within boundary
void setup()
{
    //initialize random number generator
    time_t t;
    srand((unsigned) time(&t));
    game_over = 0;

    X = length / 2;
    Y = width / 2;

    state1:
        food_x = rand() % 20;
        if (food_x == 0)
        {
            goto state1;
        }
    state2:
        food_y = rand() % 30;
        if (food_y == 0)
        {
            goto state2;
        }
    score = 0;            
}