// Function to draw graphics on screen
void draw(WINDOW* game)
{
    // Print fruit
    mvwprintw(game, head.location_y, head.location_x, "#");
    
    //Print snake 
    mvwprintw(game, board.fruit_y, board.fruit_x, "o");
    
    // Erase previous snake
    switch (head.CurrentDir)
    {
        case RIGHT:
            mvwprintw(game, head.location_y, head.location_x - 1, " ");
        case LEFT:
            mvwprintw(game, head.location_y, head.location_x + 1, " ");
        case UP:
            mvwprintw(game, head.location_y + 1, head.location_x, " ");
        case DOWN:
            mvwprintw(game, head.location_y - 1, head.location_x, " ");


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