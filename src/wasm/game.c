void render_net(int cnv_w, int cnv_h)
{
    for (int i = 0; i < cnv_h / 20; ++i)
    {
        fill_rect(cnv_w / 2 - 2, 20 * i + 5, 4, 10, 0xffffffff);
    }
}

void render_scores(int left_score, int right_score, int cnv_w)
{

    char left_score_str[5];
    int_to_cstr(left_score, left_score_str);
    char right_score_str[5];
    int_to_cstr(right_score, right_score_str);

    fill_text(0, 24, left_score_str);
    fill_text(cnv_w - cstr_len(right_score_str) * 16, 24, right_score_str);
}

void prevent_paddle_stuck_in_wall(
    int *paddle_y,
    int paddle_height,
    int cnv_h)
{
    if (*paddle_y < 0)
    {
        *paddle_y = 1;
    }
    if (*paddle_y > cnv_h - paddle_height)
    {
        *paddle_y = cnv_h - paddle_height - 1;
    }
}

void prevent_ball_stuck_in_wall(
    int *ball_x,
    int *ball_y,
    int ball_size,
    int cnv_w,
    int cnv_h)
{
    if (*ball_x < 0)
    {
        *ball_x = 1;
    }
    if (*ball_y < 0)
    {
        *ball_y = 1;
    }

    if (*ball_x > cnv_w - ball_size)
    {
        *ball_x = cnv_w - ball_size - 1;
    }

    if (*ball_y > cnv_h - ball_size)
    {
        *ball_y = cnv_h - ball_size - 1;
    }
}

void check_paddle_collision_with_walls(
    int *paddle_y,
    int *paddle_dy,
    int paddle_height,
    int cnv_h)
{
    if (*paddle_y <= 0 || *paddle_y + paddle_height >= cnv_h)
    {
        *paddle_dy *= -1;
    }

    prevent_paddle_stuck_in_wall(paddle_y, paddle_height, cnv_h);
}

void check_ball_collision_with_walls(
    int *ball_x,
    int *ball_y,
    int *ball_dy,
    int ball_size,
    int cnv_w,
    int cnv_h)
{
    if (*ball_y <= 0 || *ball_y + ball_size >= cnv_h)
    {
        *ball_dy *= -1; // Reflect the ball vertically
    }

    prevent_ball_stuck_in_wall(ball_x, ball_y, ball_size, cnv_w, cnv_h);
}

void check_paddle_collision_with_ball(
    int *ball_x,
    int *ball_y,
    int *ball_dx,
    int *ball_dy,
    int paddle_x,
    int paddle_y,
    int ball_size,
    int paddle_width,
    int paddle_height)
{
    // Define the ball's bounding box
    int ball_left = *ball_x;
    int ball_right = *ball_x + ball_size;
    int ball_top = *ball_y;
    int ball_bottom = *ball_y + ball_size;

    // Define the paddle's bounding box
    int paddle_left = paddle_x;
    int paddle_right = paddle_x + paddle_width;
    int paddle_top = paddle_y;
    int paddle_bottom = paddle_y + paddle_height;

    // Check for AABB collision
    if (ball_right >= paddle_left && ball_left <= paddle_right &&
        ball_bottom >= paddle_top && ball_top <= paddle_bottom)
    {

        // Determine the side of the collision based on velocity
        // If moving down or up, it's a top/bottom collision
        if (*ball_dy > 0 && ball_bottom - paddle_top < ball_size)
        {
            // Collision with the top side of the paddle
            *ball_dy = -*ball_dy;             // Reverse Y-direction
            *ball_y = paddle_top - ball_size; // Reposition ball to avoid sticking
        }
        else if (*ball_dy < 0 && paddle_bottom - ball_top < ball_size)
        {
            // Collision with the bottom side of the paddle
            *ball_dy = -*ball_dy;    // Reverse Y-direction
            *ball_y = paddle_bottom; // Reposition ball to avoid sticking
        }

        // If moving right or left, it's a left/right collision
        if (*ball_dx > 0 && ball_right - paddle_left < ball_size)
        {
            // Collision with the left side of the paddle
            *ball_dx = -*ball_dx;              // Reverse X-direction
            *ball_x = paddle_left - ball_size; // Reposition ball to avoid sticking
        }
        else if (*ball_dx < 0 && paddle_right - ball_left < ball_size)
        {
            // Collision with the right side of the paddle
            *ball_dx = -*ball_dx;   // Reverse X-direction
            *ball_x = paddle_right; // Reposition ball to avoid sticking
        }
    }
}
