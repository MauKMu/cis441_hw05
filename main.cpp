#include "mbed.h"

#define PADDLE_X_INC        2
#define PADDLE_MIN          3
#define PADDLE_MAX          58
#define PADDLE_LEFT_STOP    PADDLE_MIN + PADDLE_X_INC
#define PADDLE_RIGHT_STOP   PADDKE_MAX - PADDLE_X_INC

#define BALL_X_MIN          1.0f
#define BALL_X_MAX          60.0f
#define BALL_Y_MIN          1.0f
#define BALL_Y_MAX          30.0f

#define SPEED_INC           1.05f

InterruptIn left(p5);
InterruptIn right(p6);
Ticker ticker;

int paddle_x = 30;

float ball_x = 30.0f;
float ball_y = 5.0f;

float ball_speed_x = (float) rand() / RAND_MAX;
float ball_speed_y = (float) rand() / RAND_MAX;

int moving_left = false;
int moving_right = false;

void handle_button_change() {
    // update status of paddle
    moving_left = left.read();
    moving_right = right.read();
}

void update_board() {
    // update paddle
    if (moving_left) {
        paddle_x -= PADDLE_X_INC;
    }

    if (moving_right) {
        paddle_x += PADDLE_X_INC;
    }

    if (paddle_x <= PADDLE_MIN) {
        paddle_x = PADDLE_MIN;
    }
    else if (paddle_x >= PADDLE_MAX) {
        paddle_x = PADDLE_MAX;
    }

    // update ball position
    ball_x += ball_speed_x;
    ball_y += ball_speed_y;
    
    // check for collisions
    
    // vertical wall collisions
    if (ball_x > BALL_X_MAX) {
        ball_x = BALL_X_MAX;
        ball_speed_y *= -1.0f;
    }
    else if (ball_x < BALL_X_MIN) {
        ball_x = BALL_X_MIN;    
        ball_speed_y *= -1.0f;
    }
    
    // horizontal (top) wall collision
    if (ball_y < BALL_Y_MIN) {
        ball_y = BALL_Y_MIN; 
        ball_speed_x *= -1.0f;  
    }
    else if (ball_y > BALL_Y_MAX) {
        // check if paddle is there!    
        if ((float)(paddle_x - 2) <= ball_x && ball_x <= (float)(paddle_x + 2)) {
            // paddle is under ball, so just reflect
            ball_y = BALL_Y_MAX;
            // increase speed (and reflect y)
            ball_speed_y = abs(ball_speed_y) * (-SPEED_INC);
            ball_speed_x *= SPEED_INC;
        }
        else {
            // player lost
            __disable_irq();
        }
    }

    // print coordinates of things

    // paddle
    printf("p%02d\n", paddle_x);

    // ball
    printf("b%02d,%02d\n", (int)ball_x, (int)ball_y);
}

int main() {
    left.rise(&handle_button_change);    
    left.fall(&handle_button_change);    
    right.rise(&handle_button_change);    
    right.fall(&handle_button_change); 
    ticker.attach(&update_board, 0.033f);
    for(;;);
}

