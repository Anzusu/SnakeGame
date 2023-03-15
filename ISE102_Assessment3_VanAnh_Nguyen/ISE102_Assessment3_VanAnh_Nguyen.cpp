#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING // Needed with c++17
/******************************************************************************
*
*  ACADEMIC INTEGRITY DECLARATION
*
*  I, Van Anh Nguyen, declare that except where I have referenced,
*  the work I am submitting in this assessment is my own work. I
*  acknowledge and agree that the assessor of this assignment may,
*  for the purpose of authenticating this assignment, reproduce it
*  for the purpose of detecting plagiarism. I have read and am aware
*  of the Think Education Policy and Procedure viewable online at
*  http://www.think.edu.au/studying-at-think/policies-and-procedures/
*
******************************************************************************/

#include <iostream>
#include <string>
#include <Windows.h>
#include "textpixels_enums.h"
#include "textpixels.h"
#include "Snake.h"
#include "Fruit.h"
using namespace std;
using namespace textpixels;

// Screens and gamemodes in the game
enum Screen
{
    MENU = 1,
    PLAY,
    EASY,
    HARD,
    PAUSE,
    GAMEOVER,
    EXIT,
    NONE
};
// The playing area (snake garden) is above the graphic user interface (GUI)
const int LEVEL_WIDTH = 49;
const int LEVEL_HEIGHT = 25;
const int GUI_HEIGHT = 15;

const int WALL_LEFT = 0;
const int WALL_RIGHT = LEVEL_WIDTH - 1;
const int WALL_TOP = 0;
const int WALL_BOTTOM = LEVEL_HEIGHT - 1;
const int GAME_OVER_WIDTH = 29;
const int GAME_OVER_HEIGHT = 21;
const int PAUSED_SCREEN_WIDTH = 20;
const int PAUSED_SCREEN_HEIGHT = 9;

// Function declarations
void drawLevelAndGui(Snake snake, int score, int gameMode);
int displayMenuScreen();
void drawGameover(int score);
void drawGamemodeDescription(int gameMode);
int drawGamemode();
void pausedScreen();
void hitBordermessage();
void atePoisonfruitMessage();
int playSnake(int gameMode);

/* ------------------------------------------------------------------------- MAIN */

int main()
{
    // textpixels creates our 30x30 console window
    textpixels::setupConsole(LEVEL_WIDTH, LEVEL_HEIGHT + GUI_HEIGHT, 15, 15);

    /// I chose `screen` as a term for part of the program with its own visuals+controls
    int screen = Screen::MENU;    // Start the game at the menu.
    int score = 0;
    int gameMode = 0;

    srand(time(0));


    while (screen != Screen::EXIT)   //Outermost loop of the whole program
    {
        if (screen == Screen::MENU)
        {
            screen = displayMenuScreen();
        }
        else if (screen == Screen::PLAY)
        { //If player select PLAY then they can chose between easy or hard game mode
            screen = drawGamemode();
            if (screen == Screen::EASY)
            {
                gameMode = 1;           // Gamemode set to easy
                score = playSnake(gameMode);
                screen = Screen::MENU;      // Requesting menu as the next screen after game
            }
            if (screen == Screen::HARD)
            {
                gameMode = 2;           //Gamemode is set to hard
                score = playSnake(gameMode);
                screen = Screen::MENU;      // Requesting menu as the next screen after game
            }
        }
    }
    textpixels::exit();  // Tell textpixels to clean up after itself.
    return (0);
}

/* -------------------------------------------------------- FUNCTION DEFINITIONS */

// Draw the Level's ground and border wall to top of window,
// draw gui with score and any other info at bottom.
void drawLevelAndGui(Snake snake, int score, int gameMode)
{
    // Draw the background and a wall.
    fillWindow(FG_CYAN);
    fillRect(1, 1, LEVEL_WIDTH - 2, LEVEL_HEIGHT - 2, FG_BLUE);
    // Draw the gui background, starting at bottom of level
    fillRect(0, LEVEL_HEIGHT, LEVEL_WIDTH, GUI_HEIGHT, BLACK);

    drawString(2, LEVEL_HEIGHT + 6, "(Q) ", FG_MAGENTA);
    drawString(6, LEVEL_HEIGHT + 6, "quit", FG_GREY);
    drawString(2, LEVEL_HEIGHT + 9, "(P) ", FG_MAGENTA);
    drawString(6, LEVEL_HEIGHT + 9, "pause", FG_GREY);
    drawString(2, LEVEL_HEIGHT + 12, "(C) ", FG_MAGENTA);
    drawString(6, LEVEL_HEIGHT + 12, "continue", FG_GREY);

    // Fruit Points
    drawString(18, LEVEL_HEIGHT + 6, "Fruit points:", FG_DARK_GREY);
    drawString(18, LEVEL_HEIGHT + 8, "Banana = 1", FG_YELLOW);
    drawString(18, LEVEL_HEIGHT + 9, "Strawberry = 3", FG_RED);
    drawString(18, LEVEL_HEIGHT + 11, "Spider = -2", FG_WHITE);
    drawString(18, LEVEL_HEIGHT + 12, "Toxin Waste = Death", FG_GREEN);

    //to display player's score
    drawString(1, 0, "Score: " + to_string(score), layerColours(FG_BLACK, BG_GREY));

    //displaying fps
    drawString(25, LEVEL_HEIGHT - 1, "fps: ", layerColours(FG_DARK_RED, BG_GREY));
    drawString(30, LEVEL_HEIGHT - 1, textpixels::getFpsString(), layerColours(FG_DARK_RED, BG_GREY));

    //game mode descriptions
    drawGamemodeDescription(gameMode);

}

//description of the game mode to the player
void drawGamemodeDescription(int gameMode)
{
    string easy = "Easy peasy, eat the fruits, avoid the poison.";
    string hard = "Don't let the wall crush you! Avoid at all costs :3";

    if (gameMode == 1)
    {
        drawString(0, LEVEL_HEIGHT + 1, "Gamemode Easy: " + easy, layerColours(FG_WHITE, BG_BLACK));
    }
    if (gameMode == 2)
    {
        drawString(0, LEVEL_HEIGHT + 1, "Gamemode Hard: " + hard, layerColours(FG_WHITE, BG_BLACK));
    }
}

//window displaying games for players to select
int drawGamemode()
{
    textpixels::setFps(60);
    int choice = Screen::NONE;
    int counter = 0;
    do
    {
        textpixels::startFrame();
        fillWindow(FG_BLACK);
        fillRect(1, 1, LEVEL_WIDTH - 2, LEVEL_HEIGHT - 2, FG_DARK_MAGENTA);
        fillRect(0, 13, LEVEL_WIDTH + 2, LEVEL_HEIGHT + 2, FG_BLACK);

        drawString(15, 5, "PICK YOUR POISON ", layerColours(BG_DARK_MAGENTA, FG_BLACK));
        drawString(18, 20, " Easy  (E) ", layerColours(FG_WHITE, BG_BLACK));
        drawString(18, 25, " Hard  (H) ", layerColours(FG_WHITE, BG_BLACK));
        drawString(18, 30, " Back  (B)", layerColours(FG_WHITE, BG_BLACK));
        drawString(18, 35, " Exit (X)", layerColours(FG_WHITE, BG_BLACK));


        if (keyIsDown('E'))
        {
            choice = Screen::EASY;
        }
        else if (keyIsDown('H'))
        {
            choice = Screen::HARD;
        }
        else if (keyIsDown('B'))
        {
            choice = Screen::MENU;
        }
        else if (keyIsDown('X'))
        {
            choice = Screen::EXIT;
        }
        textpixels::endFrame();
    } while (choice == Screen::NONE);
    return(choice);

}

// Draws the menu screen, waits for user input.
// Returns the player's choice using a constant from enum Screen.
int displayMenuScreen()
{
    //60fps is plenty fast
    textpixels::setFps(60);
    int choice = Screen::NONE;
    do                            // Keeps looping, waiting for input
    {
        textpixels::startFrame();   // Needed always at start of game loop
        fillWindow(FG_DARK_MAGENTA);
        fillRect(2, 2, LEVEL_WIDTH - 4, LEVEL_HEIGHT + 11, FG_BLACK);
        fillRect(2, 2, LEVEL_WIDTH - 4, 10, GREEN);

        drawString(14, 10, "SEVERUS SNAKE THE GAME", layerColours(FG_BLACK, BG_GREEN));
        drawString(15, 22, "(P) Play Snake", layerColours(FG_BLACK, BG_DARK_GREEN));
        drawString(15, 25, "(X) Exit", layerColours(FG_BLACK, BG_DARK_GREEN));
        drawString(39, 39, "ISE102", layerColours(FG_WHITE, BG_BLACK));

        // Check if any valid keys are down. `else if` makes sure we only take one
        // input at a time. 
        if (keyIsDown('P'))
        {
            choice = Screen::PLAY;

        }
        else if (keyIsDown('X'))
        {
            choice = Screen::EXIT;
        }
        textpixels::endFrame();     // Needed always at end of game loop.
    } while (choice == Screen::NONE);     // Only stop when playerHasQuit  
    return(choice);
}


//let player know they died 
//show player their score
void drawGameover(int score)
{
    fillRect(8, 3, GAME_OVER_WIDTH, GAME_OVER_HEIGHT, CYAN);
    drawString(15, 3, "Your final score :)" + to_string(score), layerColours(FG_BLACK, BG_WHITE));
    drawString(14, 15, "Select to play again", layerColours(FG_BLACK, BG_WHITE));
    drawString(21, 17, "(Y)", layerColours(FG_BLACK, BG_WHITE));
    drawString(21, 19, "(N)", layerColours(FG_BLACK, BG_WHITE));
}

//function that generates within the border of the game 
int randInsideborder(int size)
{
    int safeRand = rand() % (size - 2) + 1;
    return safeRand;
}

// Play the game! Responsible for core game loop: listening for input,
// simulating changes in the game world, drawing the new state to screen.
// Returns score 
int playSnake(int gameMode)
{
    textpixels::setFps(200);      // Set a high frame rate for input sensitivity.
    // Set up whatever needs to persist throughout the loop
    bool playerHasQuit = false;
    bool playerHasPaused = false;
    bool hitWall = false;
    bool poisonFruitActive = false;
    int fruitCountdown = 2 * 50;
    int score = 0;

    // Declare a snake object using the new Snake type (see Snake.h in header files)
    // Note on moveInterval: Moving slithers every 12th frame is still quick at 200fps.
    Snake slithers;
    slithers.colour = FG_DARK_MAGENTA;
    slithers.moveInterval = 20;
    slithers.location.x = randInsideborder(LEVEL_WIDTH);
    slithers.location.y = randInsideborder(LEVEL_HEIGHT);
    slithers.xDir = Direction::RIGHT;
    slithers.yDir = Direction::NONE;

    //for drawing the snake
    slithers.length = 1;


    //fruits
    Fruit banana;
    banana.location = Point2d(randInsideborder(LEVEL_WIDTH), randInsideborder(LEVEL_HEIGHT));
    banana.colour = YELLOW;
    banana.pointsValue = 1;

    Fruit strawberry;
    strawberry.location = Point2d(randInsideborder(LEVEL_WIDTH), randInsideborder(LEVEL_HEIGHT));
    strawberry.colour = RED;
    strawberry.pointsValue = 3;

    Fruit spider;
    spider.location = Point2d(randInsideborder(LEVEL_WIDTH), randInsideborder(LEVEL_HEIGHT));
    spider.colour = WHITE;
    spider.pointsValue = -2;

    Fruit toxic_waste;
    toxic_waste.location.x = rand() % (WALL_RIGHT - 1) + 1;
    toxic_waste.location.y = rand() % (WALL_RIGHT - 1) + 1;
    toxic_waste.colour = GREEN;


    do
    {
        textpixels::startFrame();   // Let textpixels know we're doing stuff in this frame.
        if (fruitCountdown > 0) fruitCountdown -= 1;
        srand(time(NULL));      // Seeds the random number

        /* ---------------------------------------------------------------------- INPUT */
        if (keyIsDown('Q'))
        {
            playerHasQuit = true;
            break;  // End the DO loop now, go to the line after it ends
        }

        if (playerHasPaused == false)
        {
            Direction xDirectionInput = textpixels::getLeftRightInput();
            Direction yDirectionInput = textpixels::getUpDownInput();

            /* ----------------------------------------------------------------- PROCESSING */
            if (xDirectionInput != Direction::NONE)
            {
                slithers.xDir = xDirectionInput;
                slithers.yDir = Direction::NONE;
            }
            else if (yDirectionInput != Direction::NONE)
            {
                slithers.yDir = yDirectionInput;
                slithers.xDir = Direction::NONE;
            }

            if (textpixels::frame() % slithers.moveInterval == 0 && slithers.expired == false)
            {
                slithers.history.push_back(slithers.location);

                slithers.location.x += slithers.xDir; // xDir is -1, 0 or 1 (see Direction enum) 
                slithers.location.y += slithers.yDir;

                if (slithers.location == banana.location)
                {
                    score += 1;
                    slithers.length += 1;
                    banana.location.x = randInsideborder(LEVEL_WIDTH);
                    banana.location.y = randInsideborder(LEVEL_HEIGHT);
                }

                if (slithers.location == strawberry.location)
                {
                    score += 3;
                    slithers.length += 1;
                    strawberry.location.x = randInsideborder(LEVEL_WIDTH);
                    strawberry.location.y = randInsideborder(LEVEL_HEIGHT);
                }

                if (slithers.location == spider.location)
                {
                    score += -2;
                }

                if (slithers.location == toxic_waste.location)
                {
                    slithers.expired = true;
                    slithers.poisoned = true;
                }
                //severus grows by comparing the history of the snake (each past location) to length of snake
                if (slithers.history.size() > slithers.length + 2)
                {
                    slithers.history.erase(slithers.history.begin());   // removes last segment from the snake
                }

                //gamemode, easy
                if (gameMode == 1)
                {
                    if (slithers.location.y == WALL_TOP)
                    {
                        slithers.location.y += LEVEL_HEIGHT - 2;
                    }
                    if (slithers.location.y == WALL_BOTTOM)
                    {
                        slithers.location.y -= LEVEL_HEIGHT - 2;
                    }
                    if (slithers.location.x == WALL_LEFT)
                    {
                        slithers.location.x += LEVEL_WIDTH - 2;
                    }
                    if (slithers.location.x == WALL_RIGHT)
                    {
                        slithers.location.x -= LEVEL_WIDTH - 2;
                    }
                }

                //gamemode, hard
                if (gameMode == 2)
                {
                    if (slithers.xDir != Direction::NONE)
                    {
                        //checks if snake hits left or right wall
                        if (slithers.location.x == WALL_LEFT)
                        {
                            hitWall = true;
                        }
                        if (slithers.location.x == WALL_RIGHT)
                        {
                            hitWall = true;
                        }
                    }
                    else if (slithers.yDir != Direction::NONE)
                    {
                        //checks if snake hits bottom or top wall
                        if (slithers.location.y == WALL_TOP)
                        {
                            hitWall = true;
                        }
                        if (slithers.location.y == WALL_BOTTOM)
                        {
                            hitWall = true;
                        }
                    }
                    // If the snake has hit a wall
                    // the snake is now dead
                    if (hitWall)
                    {
                        slithers.colour = RED;
                        slithers.expired = true;
                    }
                }
            }
        }


        drawLevelAndGui(slithers, score, gameMode);
        if (fruitCountdown <= 0)
        {
            drawPixel(banana.location, banana.colour);
            drawPixel(strawberry.location, strawberry.colour);
            drawPixel(spider.location, spider.colour);
            drawPixel(toxic_waste.location, toxic_waste.colour);
        }
        // Controls how the fruit appears and disappears
        if (frame() % 3000 == 0)
        {
            toxic_waste.location.x = randInsideborder(LEVEL_WIDTH);
            toxic_waste.location.y = randInsideborder(LEVEL_HEIGHT);
        }

        // Controls how the fruit appears and disappears
        if (frame() % 1500 == 0)
        {
            spider.location.x = randInsideborder(LEVEL_WIDTH);
            spider.location.y = randInsideborder(LEVEL_HEIGHT);
        }

        // Draws the body of the snake
        for (int i = 0; i < slithers.history.size(); i++)
        {
            drawPixel(slithers.history[i], BLACK);
        }

        // Draws the head of the snkae
        drawPixel(slithers.location, slithers.colour);  // Draw slithers: read her internal properties


        // Checks if the player has paused the game
        if (keyIsDown('P'))
        {
            playerHasPaused = true;
        }

        if (playerHasPaused)
        {
            pausedScreen();
        }

        if (keyIsDown('C'))
        {
            playerHasPaused = false;
        }

        //show cause of death
        if (hitWall == true)
        {
            drawGameover(score);
            hitBordermessage();
        }

        //show cause of death
        if (slithers.poisoned == true)
        {
            drawGameover(score);
            atePoisonfruitMessage();
        }

        // ask player if they would like to play again
        if (slithers.expired == true)
        {

            if (keyIsDown('Y'))
            {
                playSnake(gameMode);
            }
            if (keyIsDown('N'))
            {
                playerHasQuit = true;
                break;
            }
        }
        textpixels::endFrame();     // Tell textpixel it can draw the screen and sleep
    } while (!playerHasQuit || !playerHasPaused);     // Only stop when playerHasQuit or playerHasPaused

    return score; //return placeholder score
}

//pause screen when player paused the game
void pausedScreen()
{
    fillRect(14, 5, PAUSED_SCREEN_WIDTH, PAUSED_SCREEN_HEIGHT, GREY);
    drawString(21, 6, "PAUSED", layerColours(FG_BLACK, BG_GREY));
    drawString(18, 10, "(C) ", layerColours(FG_BLACK, BG_GREY));
    drawString(21, 10, " continue", layerColours(FG_BLACK, BG_GREY));
}

//show the reason for death
void hitBordermessage()
{
    drawString(15, 7, "I told you to avoid the walls :'(", layerColours(FG_BLACK, BG_GREY));
    drawString(15, 8, "Better luck next time", layerColours(FG_BLACK, BG_GREY));
}

//show reason for death
void atePoisonfruitMessage()
{
    drawString(11, 7, "Don't eat the toxin waste!!!", layerColours(FG_BLACK, BG_GREY));
    drawString(15, 8, "YOU HAD ONE JOB", layerColours(FG_BLACK, BG_GREY));
}
