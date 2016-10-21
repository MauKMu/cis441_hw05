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

InterruptIn left(p5);
InterruptIn right(p6);

int paddle_x = 30;

float ball_x = 30.0f;
float ball_y = 5.0f;

float ball_speed_x = (float) rand() / RAND_MAX;
float ball_speed_y = (float) rand() / RAND_MAX;

bool wall_collided;
bool paddle_collided;


void handle_button_press() {
    if (left.read()) {
        // move left
        if (paddle_x <= PADDLE_LEFT_STOP) {
            paddle_x = PADDLE_MIN;
        }     
        else {
            paddle_x -= PADDLE_X_INC;
        }    
    }
    else {
        // move right
        if (paddle_x >= PADDLE_RIGHT_STOP) {
            paddle_x = PADDLE_MAX;
        }     
        else {
            paddle_x += PADDLE_X_INC;
        }
    }    
}

void update_board() {
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
    }
}

int main() {
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}

