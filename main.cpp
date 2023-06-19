#include <iostream> 
#include <conio.h>
#include <windows.h>
#include<time.h>
//include vector
#include <vector>

using namespace std;
bool gameOver;
const int width = 30;
const int height = 30;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;


int frame_cnt = 0;
//max size of the obstacle
const int max_obstacle = 6;
int cnt = max_obstacle;

//lucky item
int luckyitemX, luckyitemY;
bool luckyitem_on = true;
bool lucky_once = false;


// boolean typed game field
char game_real_field[height + 5][width + 5];
//char typed game field
bool game_field[height + 5][width + 5];

//obstacle
struct obstacle{
    int x;
    int y;
};
vector<obstacle> obstacle_set(max_obstacle + 5);

// check if the cell is occcupied or not
bool check_occupied(int y, int x) {
    if (game_field[y][x]) {
        return true;
    }
    return false;
}

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;

    bool ng = true;
    while (ng) {
        fruitX = rand() % width;
        fruitY = rand() % height;
        if (fruitX > 0 && fruitY > 0 && fruitX < width - 1 && fruitY < height - 1) {
            ng = false;
        }
    }
    // lucky item set up 
     luckyitemX = rand() % width;
     luckyitemY = rand() % height;

    //obstacle set up
 
    for (int i = 0; i < max_obstacle; i++) {
        int obstacle_x = rand() % width;
        int obstacle_y = rand() % height;
        obstacle_set[i].x = obstacle_x;
        obstacle_set[i].y = obstacle_y;
    }

    score = 0;
    nTail = 0;
    for (int i = 0; i < height + 2; i++) {
        for (int j = 0; j < width + 2; j++) {
            game_field[i][j] = false;
            game_real_field[i][j] = ' ';
        }
    }
}



void Draw()
{
    system("cls");
    cout << endl;
    cout << " ";
   

    game_real_field[y][x] = '@';
    game_field[y][x] = true;
    game_real_field[fruitY][fruitX] = 'F';
    game_field[fruitY][fruitX] = true;

    for (int i = 0; i < max_obstacle; i++) {
        int obstacle_x = obstacle_set[i].x;
        int obstacle_y = obstacle_set[i].y;
        if (!check_occupied(obstacle_y, obstacle_x)) {
            game_real_field[obstacle_y][obstacle_x] = '*';
            game_field[obstacle_y][obstacle_x] = true;
        }
    }

    for (int i = 0; i < height+2; i++) {
        for (int j = 0; j < width+2; j++) {
            if (i == 0 || j == 0 || i == height + 1 || j == width + 1)
            {
                game_real_field[i][j] = '#';
                game_field[i][j] = true;
            }
            else if (luckyitem_on && i == luckyitemY && j == luckyitemX) {
                game_real_field[i][j] = 'L';
                game_field[i][j] = true;
            }
            
        }
    }


    //ntail
    if (nTail > 0) {
        for (int i = 0; i < nTail; i++) {
            int tx = tailX[i];
            int ty = tailY[i];
            game_real_field[ty][tx] = 'o';
            game_field[ty][tx] = true;
        }
    }

    for (int i = 0; i < height + 2; i++) {
        for (int j = 0; j < width + 2; j++) {
            cout << game_real_field[i][j];
        }
        cout << endl;
    }
    
    cout << endl;
    cout << "Score:" << score << endl;
}



void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
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
        default:
            break;
        }
    }
}


void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case RIGHT:
        x++;
        break;
    default:
        break;
    }

    if (x > width || x < 0 || y > height || y < 0) {
         gameOver = true;
    }
    /*/for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
         gameOver = true;
        }
    }

    */

    for (int i = 0; i < max_obstacle; i++) {
        if (obstacle_set[i].x == x && obstacle_set[i].y == y) {
            gameOver = true;
        }
    }



    if (x == fruitX && y == fruitY)
    {
        srand(time(0));
        score += 10;
        bool ng_fruit = false;
        while (!ng_fruit) 
        {
            fruitX = rand() % width;
            fruitY = rand() % height;
            if (fruitX != 0 && fruitY != 0) {
                ng_fruit = true;
            }
        }
        nTail++;
    }

    //lucky item 

    if (!lucky_once && luckyitem_on && luckyitemX == x && luckyitemY == y)
    {
        srand(time(0));
        score += 50;
        bool ng_lucky = true;
        lucky_once = true;
        while (ng_lucky)
        {
            luckyitemX = rand() % width;
            luckyitemY = rand() % height;
            for (int i = 0; i < height + 2; i++) {
                for (int j = 0; j < width + 2; j++) {
                    if (luckyitemX == i && luckyitemY == j && !check_occupied(i, j)) {
                        ng_lucky = false;
                   }
                }
            }
        }
        if (nTail > 0) { nTail = 0; }
    }
    

    for (int i = 0; i < height + 2; i++) {
        for (int j = 0; j < width + 2; j++) {
            game_field[i][j] = false;
            game_real_field[i][j] = ' ';
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    system("MODE con cols = 24 lines = 25");
    Setup();
    while (!gameOver)
    {
        frame_cnt += 1;
        if (frame_cnt > 30) {
            if (luckyitem_on == false) {
                luckyitem_on = true;
                lucky_once = false;
                bool ng_lucky = true;
                while (ng_lucky)
                {
                    luckyitemX = rand() % width;
                    luckyitemY = rand() % height;
                    for (int i = 0; i < height + 2; i++) {
                        for (int j = 0; j < width + 2; j++) {
                            if (luckyitemX == i && luckyitemY == j && !check_occupied(i, j)) {
                                ng_lucky = false;
                            }
                        }
                    }
                }
            }
            else {
                luckyitem_on = false;
                bool ng_lucky = true;
                while (ng_lucky)
                {
                    luckyitemX = rand() % width;
                    luckyitemY = rand() % height;
                    for (int i = 0; i < height + 2; i++) {
                        for (int j = 0; j < width + 2; j++) {
                            if (luckyitemX == i && luckyitemY == j && !check_occupied(i, j)) {
                                ng_lucky = false;
                            }
                        }
                    }
                }
            }
            frame_cnt = 0;
        }
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
    return 0;
}