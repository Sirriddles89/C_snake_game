#include <stdio.h>
#include <stdlib.h>

//game variables
int i, j, game_over, score;
int length = 28;
int width = 40;

//prototype for Boundary function
void boundary();

int main(void)
{
    //calling the boundary function draws a game window

    boundary();
}

//boundary function - draws the space where gameplay will take place
void boundary()
{
    for (i = 0; i <= length; i++)
    {
        for (j = 0; j <= width; j++)
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
                printf(" ");
            }
        }
        printf("\n");
    }
}