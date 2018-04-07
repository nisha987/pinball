#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>

#define GAME_WIDTH 480
#define GAME_HEIGHT 320

#define GAME_LOOP_DELAY 80

#define BRICK_W 50
#define BRICK_H 20

#define PADDLE_W 70
#define PADDLE_H 20
#define PADDLE_INIT_X 205
#define PADDLE_INIT_Y 280

#define NUM_ROWS 3
#define NUM_COLS 5

#define X_MARGIN 20
#define Y_MARGIN 10
#define X_GAP 20
#define Y_GAP 10
#define BALL_R 8
#define INIT_BALL_X 240
#define INIT_BALL_Y 270

#define BALL_DIRECTION -55 //in degrees
#define BALL_MOVEMENT 5

bool gameover = false;

class Ball
{
public:
    int radius,x,y;
    int prevX, prevY;
    int angleOfMovement;
    Ball()
    {
        x=INIT_BALL_X;
        y=INIT_BALL_Y;
        radius=BALL_R;
        angleOfMovement = BALL_DIRECTION;
    }
    void draw()
    {
        setcolor(15);
        circle(x, y,radius);
    }
    void erase()
    {
        int previousColor = getcolor();
        setcolor(0);
        circle(x, y, radius);
        setcolor(previousColor);
    }

    int getRightBound()
    {
        return x + radius;
    }

    int getLeftBound()
    {
        return x - radius;
    }

    int getTopBound()
    {
        return y - radius;
    }

    int getBottomBound()
    {
        return y + radius;
    }



    void move()
    {
        float directionInRadian = angleOfMovement * (3.14f / 180);
        int nextX = x + BALL_MOVEMENT * cos(directionInRadian);
        float dy = BALL_MOVEMENT * sin(directionInRadian);
        int nextY = y + dy;

        //check horizontal boundaries
        if (nextX >= GAME_WIDTH || nextX < 0)
        {
            angleOfMovement = 180 - angleOfMovement;
        }
        //check bottom boundary
        if (nextY >= GAME_HEIGHT)
        {
            //Game Over
            gameover = true;
        }
        //check top boundary
        if (nextY < 0)
        {
            angleOfMovement = angleOfMovement-90;
        }

        prevX = x;
        prevY = y;
        x = nextX;
        y = nextY;
    }
};

Ball ballObj;

class Paddle
{
public:
    int x, y, w, h;

    Paddle()
    {
        x = PADDLE_INIT_X;
        y = PADDLE_INIT_Y;
        w = PADDLE_W;
        h = PADDLE_H;
    }

    void draw()
    {
        setfillstyle(SOLID_FILL, 5);
        bar(x, y, x+w, y+h);
    }

    void erase()
    {
        setfillstyle(SOLID_FILL, 0);
        bar(x, y, x+w, y+h);
    }

    void moveLeft()
    {
        x -= 10;
    }

    void moveRight()
    {
        x += 10;
    }

    bool checkCollision()
    {
        int L1 = x;
        int R1 = x+w;
        int T1 = y;
        int B1 = y+h;

        int L2 = ballObj.getLeftBound();
        int R2 = ballObj.getRightBound();
        int T2 = ballObj.getTopBound();
        int B2 = ballObj.getBottomBound();

        if (R1 < L2)
        {
            return false;
        }

        if (R2 < L1)
        {
            return false;
        }

        if (B2 < T1)
        {
            return false;
        }

        if (B1 < T2)
        {
            return false;
        }

        return true;
    }
};


class Brick
{
public:
    int x, y, w, h, id;
    bool isDestroyed;

    Brick()
    {
        isDestroyed = false;
        w = BRICK_W;
        h = BRICK_H;
    }

    void initialize(int _id, int _x, int _y)
    {
        id = _id;
        x = _x;
        y = _y;
    }

    void destroy()
    {
        isDestroyed = true;
    }

    void draw()
    {
        if (isDestroyed == true)
        {
            setcolor(0);
            rectangle(x, y, x+w, y+h);
        }
        else
        {
            setcolor(2);
            rectangle(x, y, x+w, y+h);
        }

    }

    void erase()
    {
        int previousColor = getcolor();
        setcolor(0);
        rectangle(x,y,x+w,y+h);
        setcolor(previousColor);
    }

    bool checkCollision()
    {
        if (isDestroyed == true)
        {
            return false;
        }

        int L1 = x;
        int R1 = x+w;
        int T1 = y;
        int B1 = y+h;

        int L2 = ballObj.getLeftBound();
        int R2 = ballObj.getRightBound();
        int T2 = ballObj.getTopBound();
        int B2 = ballObj.getBottomBound();

        if (R1 < L2)
        {
            return false;
        }

        if (R2 < L1)
        {
            return false;
        }

        if (B2 < T1)
        {
            return false;
        }

        if (B1 < T2)
        {
            return false;
        }

        return true;
    }
};

int brickCount = NUM_ROWS * NUM_COLS;
Brick bricks[NUM_ROWS * NUM_COLS];
Paddle paddleObj;

void processInput()
{
    char input ='k';
    if(kbhit())
        {
            input = getch();
            switch(input)
            {
                case 'x':
                    exit(0);
                case 'a':
                    paddleObj.moveLeft();
                    break;
                case 'd':
                    paddleObj.moveRight();
                    break;
            }
        }
}

void updateGame()
{
    //check collision of ball with each brick
    for (int i = 0; i < brickCount; i++)
    {
        if (bricks[i].checkCollision() == true)
        {
            bricks[i].destroy();

            int L1 = bricks[i].x;
            int R1 = bricks[i].x + bricks[i].w;
            int T1 = bricks[i].y;
            int B1 = bricks[i].y + bricks[i].h;

            int L2 = ballObj.getLeftBound();
            int R2 = ballObj.getRightBound();
            int T2 = ballObj.getTopBound();
            int B2 = ballObj.getBottomBound();

            int prevL2 = ballObj.prevX - ballObj.radius;
            int prevR2 = ballObj.prevX + ballObj.radius;
            int prevT2 = ballObj.prevY - ballObj.radius;
            int prevB2 = ballObj.prevY + ballObj.radius;

            if (prevR2 < L1 && R2 >= L1)
            {
                //collision occured at the left edge of
                //brick
                ballObj.angleOfMovement = 180 - ballObj.angleOfMovement;
            }

            else if (R1 < prevL2 && L2 <=R1)
            {
                //collision occured at the right edge of
                //brick
                ballObj.angleOfMovement = 180 - ballObj.angleOfMovement;
            }

            else if (prevB2 < T1 && B2 >= T1)
            {
                //collision occrued at the top edge of
                //brick
                ballObj.angleOfMovement = ballObj.angleOfMovement-90;
            }

            else if(B1 < prevT2 && B1 >= T2)
            {
                //collision occured at the bottom edge of
                //brick
                ballObj.angleOfMovement = ballObj.angleOfMovement-90;
            }
        }
    }

    if (paddleObj.checkCollision() == true)
    {


        int L1 = paddleObj.x;
        int R1 = paddleObj.x + paddleObj.w;
        int T1 = paddleObj.y;
        int B1 = paddleObj.y + paddleObj.h;

        int L2 = ballObj.getLeftBound();
        int R2 = ballObj.getRightBound();
        int T2 = ballObj.getTopBound();
        int B2 = ballObj.getBottomBound();

        int prevL2 = ballObj.prevX - ballObj.radius;
        int prevR2 = ballObj.prevX + ballObj.radius;
        int prevT2 = ballObj.prevY - ballObj.radius;
        int prevB2 = ballObj.prevY + ballObj.radius;

        if (prevR2 < L1 && R2 >= L1)
        {
            //collision occured at the left edge of
            //brick
            ballObj.angleOfMovement = 180 - ballObj.angleOfMovement;
        }

        else if (R1 < prevL2 && L2 <=R1)
        {
            //collision occured at the right edge of
            //brick
            ballObj.angleOfMovement = 180 - ballObj.angleOfMovement;
        }

        else if (prevB2 < T1 && B2 >= T1)
        {
            //collision occrued at the top edge of
            //brick
            ballObj.angleOfMovement = -ballObj.angleOfMovement;
        }

        else if(B1 < prevT2 && B1 >= T2)
        {
            //collision occured at the bottom edge of
            //brick
            ballObj.angleOfMovement = ballObj.angleOfMovement-90;
        }
    }


    ballObj.move();
}

void drawGame()
{
    int i;
    for (i = 0; i < brickCount; i++)
    {
        bricks[i].draw();
    }
    ballObj.draw();
    paddleObj.draw();
}

void eraseGame()
{
    ballObj.erase();
    paddleObj.erase();
}

int main()
{
    initwindow(480, 320, "Second");

    int row, col;
    int brickID = 0;
    int x, y;
    x = X_MARGIN;
    y = Y_MARGIN;
    for (row = 1; row <= NUM_ROWS; row++)
    {
        for (col = 1; col <= NUM_COLS; col++)
        {
            bricks[brickID].initialize(brickID, x, y);
            brickID++;
            x = x + BRICK_W + X_GAP;
        }
        x = X_MARGIN;
        y = y + BRICK_H + Y_GAP;
    }

    setcolor(2);

    //The following while loop
    //defines the GameLoop

    while(1)
    {
        if (gameover == true)
        {

            break;
        }

        eraseGame();
        processInput();
        updateGame();
        drawGame();
        delay(GAME_LOOP_DELAY);
    }

    getch();
}
