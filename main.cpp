#include "mbed.h"
#include <time.h> 

#define PADDLE_X_INC        2
#define PADDLE_MIN          3
#define PADDLE_MAX          58
#define PADDLE_LEFT_STOP    PADDLE_MIN + PADDLE_X_INC
#define PADDLE_RIGHT_STOP   PADDKE_MAX - PADDLE_X_INC

#define BALL_X_MIN          1.0f
#define BALL_X_MAX          60.0f
#define BALL_Y_MIN          1.0f
#define BALL_Y_MAX          29.0f

#define SPEED_INIT_MAG      0.25f
#define SPEED_INC           1.2f
#define RAND_INT_MAX        1000




AnalogIn ain(p20); // should be floating
InterruptIn left(p5);
InterruptIn right(p6);
Ticker ticker;

uint16_t lfsr_value;
int paddle_x = 30;

float ball_x = 30.0f;
float ball_y = 5.0f;

float ball_speed_x;
// find ball_speed_y to keep magnitude of speed as SPEED_INIT_MAG
// 50/50 chance of being negative
float ball_speed_y;

int moving_left = false;
int moving_right = false;


void seed_lfsr() {
    lfsr_value = ain.read_u16();
}    

uint16_t lfsr() {
    uint16_t new_bit = (lfsr_value >> 15 & 0x0001) ^ (lfsr_value >> 13 & 0x0001) ^ 
                        (lfsr_value >> 12 & 0x0001) ^ (lfsr_value >> 10 & 0x0001);
    lfsr_value = (lfsr_value << 1) | new_bit;
    return lfsr_value;
}    

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
        ball_speed_x *= -1.0f;
    }
    else if (ball_x < BALL_X_MIN) {
        ball_x = BALL_X_MIN;    
        ball_speed_x *= -1.0f;
    }
    
    // horizontal (top) wall collision
    if (ball_y < BALL_Y_MIN) {
        ball_y = BALL_Y_MIN; 
        ball_speed_y *= -1.0f;  
    }
    else if (ball_y > BALL_Y_MAX) {
        // check if paddle is there!    
        if ((float)(paddle_x - 3) <= ball_x && ball_x <= (float)(paddle_x + 3)) {
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
    printf("b%02d,%02d\n", (int)rint(ball_x), (int)rint(ball_y));
}

int main() {
    seed_lfsr();
    ball_speed_x = (float) (lfsr()) * (lfsr() % 2 ? -1.0f : 1.0f) * SPEED_INIT_MAG / 65536.0f;
    ball_speed_y = (lfsr() % 2 ? -1.0f : 1.0f) * sqrt(SPEED_INIT_MAG * SPEED_INIT_MAG - ball_speed_x * ball_speed_x);
    
    left.rise(&handle_button_change);    
    left.fall(&handle_button_change);    
    right.rise(&handle_button_change);    
    right.fall(&handle_button_change); 
    ticker.attach(&update_board, 0.033f);
    for(;;);
}

