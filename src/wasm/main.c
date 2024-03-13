#include "include/js_bindings.h"
#include "include/utils.h"
#include "game.c"

#define KEY_UP 38
#define KEY_DOWN 40
#define KEY_ACCEPT 32
#define KEY_RESTART 82

#define CNV_W 800
#define CNV_H 600

#define TIME_SPEED 1

#define BALL_SPEED (4 * TIME_SPEED)
#define PADDLE_SPEED (5 * TIME_SPEED)

#define BALL_SIZE 20

#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define PADDLE_MARGIN BALL_SIZE

#define RESET_ON_HIT_WALL
#define CONTROL_LEFT_PADDLE_IN_REVERSE

// ball
int ball_x = CNV_W / 2 - BALL_SIZE / 2;
int ball_y = CNV_H / 2 - BALL_SIZE / 2;
int ball_dx = BALL_SPEED;
int ball_dy = BALL_SPEED;

// paddles
int left_paddle_x = PADDLE_MARGIN;
int right_paddle_x = CNV_W - PADDLE_WIDTH - PADDLE_MARGIN;
int left_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
int right_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
int left_paddle_dy = 0;
int right_paddle_dy = 0;

int left_paddle_started = 0;

// scores
short int left_score = 0;
short int right_score = 0;

void on_key_up(int key)
{
    if (key == KEY_UP && key == KEY_DOWN)
    {
        right_paddle_dy = 0;
    }
}

void on_key_down(int key)
{
    if (key == KEY_UP)
    {
        right_paddle_dy = -PADDLE_SPEED;

        if (left_paddle_started == 0)
        {
            left_paddle_dy = PADDLE_SPEED;
            left_paddle_started = 1;
        }
    }

    if (key == KEY_DOWN)
    {
        right_paddle_dy = PADDLE_SPEED;

        if (left_paddle_started == 0)
        {
            left_paddle_dy = -PADDLE_SPEED;
            left_paddle_started = 1;
        }
    }

#ifdef CONTROL_LEFT_PADDLE_IN_REVERSE
    if (key == KEY_UP)
    {
        left_paddle_dy = PADDLE_SPEED;
    }

    if (key == KEY_DOWN)
    {
        left_paddle_dy = -PADDLE_SPEED;
    }
#endif
}

void check_win()
{
    if (ball_x <= 0)
    {
#ifdef RESET_ON_HIT_WALL
        left_paddle_started = 0;
        ball_x = CNV_W / 2 - BALL_SIZE / 2;
        ball_y = CNV_H / 2 - BALL_SIZE / 2;

        left_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
        right_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
#else
        ball_dx *= -1;

#endif
        right_score += 1;
    }

    if (ball_x + BALL_SIZE >= CNV_W)
    {

#ifdef RESET_ON_HIT_WALL
        left_paddle_started = 0;
        ball_x = CNV_W / 2 - BALL_SIZE / 2;
        ball_y = CNV_H / 2 - BALL_SIZE / 2;

        left_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
        right_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
#else
        ball_dx *= -1;
#endif
        left_score += 1;
    }
}

int frame()
{
    clear_rect(0, 0, CNV_W, CNV_H);

    render_scores(left_score, right_score, CNV_W);
    render_net(CNV_W, CNV_H);

    fill_rect(left_paddle_x, left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x1212aaff);
    fill_rect(right_paddle_x, right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x12aa12ff);

    fill_rect(ball_x, ball_y, BALL_SIZE, BALL_SIZE, 0xaa1212ff);

    // Update ball position
    if (left_paddle_started == 1)
    {
        ball_x += ball_dx;
        ball_y += ball_dy;
    }

    // Update paddle positions
    if (left_paddle_started)
    {
        left_paddle_y += left_paddle_dy;
        right_paddle_y += right_paddle_dy;
    }

    check_win();

    check_paddle_collision_with_walls(
        &right_paddle_y,
        &right_paddle_dy,
        PADDLE_HEIGHT,
        CNV_H);

    check_paddle_collision_with_walls(
        &left_paddle_y,
        &left_paddle_dy,
        PADDLE_HEIGHT,
        CNV_H);

    check_paddle_collision_with_ball(
        &ball_x,
        &ball_y,
        &ball_dx,
        &ball_dy,
        left_paddle_x,
        left_paddle_y,
        BALL_SIZE,
        PADDLE_WIDTH,
        PADDLE_HEIGHT);

    check_paddle_collision_with_ball(
        &ball_x,
        &ball_y,
        &ball_dx,
        &ball_dy,
        right_paddle_x,
        right_paddle_y,
        BALL_SIZE,
        PADDLE_WIDTH,
        PADDLE_HEIGHT);

    check_ball_collision_with_walls(
        &ball_x,
        &ball_y,
        &ball_dy,
        BALL_SIZE,
        CNV_W,
        CNV_H);

    return 0;
}

int main(void)
{
    create_canvas(CNV_W, CNV_H);

    return 0;
}