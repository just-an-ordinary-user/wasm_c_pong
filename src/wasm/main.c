void print(char *str);

void create_canvas(int width, int height);

void fill_rect(int x, int y, int width, int height, int color);

void clear_rect(int x, int y, int width, int height);

void fill_text(int x, int y, char *text);

#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_ACCEPT ' '
#define KEY_RESTART 'r'

#define CNV_W 800
#define CNV_H 600

#define TIME_SPEED 2
#define BALL_SPEED (4 * TIME_SPEED)
#define BALL_SIZE 20
#define PADDLE_SPEED (5 * TIME_SPEED)
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100

// ball
int ball_x = CNV_W / 2;
int ball_y = CNV_H / 2;
int ball_dx = BALL_SPEED;
int ball_dy = BALL_SPEED;

// paddles
int left_paddle_x = 25;
int right_paddle_x = CNV_W - PADDLE_WIDTH - 25;
int left_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
int right_paddle_y = CNV_H / 2 - PADDLE_HEIGHT / 2;
int left_paddle_dy = 0;
int right_paddle_dy = 0;

int left_paddle_started = 0;

// scores
short int left_score = 0;
short int right_score = 0;

// Function to reverse a string
void reverse_cstr(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
}

// Function to get the length of a string
int cstr_len(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// Function to convert an integer to a string
void int_to_cstr(int num, char *str)
{
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    do
    {
        int digit = num % 10;
        str[i++] = '0' + digit;
        num /= 10;
    } while (num != 0);

    // Add '-' for negative numbers
    if (isNegative)
    {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    reverse_cstr(str, i);
}

void render_net()
{
    for (int i = 0; i < CNV_H / 20; ++i)
    {
        fill_rect(CNV_W / 2 - 2, 20 * i + 5, 4, 10, 0xffffffff);
    }
}

void render_scores()
{

    char left_score_str[5];
    int_to_cstr(left_score, left_score_str);
    char right_score_str[5];
    int_to_cstr(right_score, right_score_str);

    fill_text(0, 24, left_score_str);
    fill_text(CNV_W - cstr_len(right_score_str) * 16, 24, right_score_str);
}

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
}

// Function to check if there is a collision with paddles
int is_paddle_collision(int ball_x, int ball_y, int paddle_x, int paddle_y, int paddle_width, int paddle_height)
{
    return (
        (paddle_x < ball_x && ball_x < paddle_x + paddle_width) &&
        (paddle_y < ball_y && ball_y < paddle_y + paddle_height));
}

int frame()
{
    clear_rect(0, 0, CNV_W, CNV_H);

    render_scores();
    render_net();

    fill_rect(left_paddle_x, left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x1212aaff);
    fill_rect(right_paddle_x, right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x12aa12ff);

    fill_rect(ball_x, ball_y, BALL_SIZE, BALL_SIZE, 0xaa1212ff);

    // Update ball position
    ball_x += ball_dx;
    ball_y += ball_dy;

    // Update paddle positions
    left_paddle_y += left_paddle_dy;
    right_paddle_y += right_paddle_dy;

    // Check paddle collision with walls
    if (right_paddle_y <= 0 || right_paddle_y + PADDLE_HEIGHT >= CNV_H)
    {
        right_paddle_dy *= -1;
    }

    if (left_paddle_y <= 0 || left_paddle_y + PADDLE_HEIGHT >= CNV_H)
    {
        left_paddle_dy *= -1;
    }

    // Check for collision with left paddle
    if (is_paddle_collision(ball_x, ball_y, left_paddle_x, left_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT))
    {
        ball_dx *= -1; // Reflect the ball
        ball_x = left_paddle_x + PADDLE_WIDTH + BALL_SIZE;
    }

    // Check for collision with right paddle
    if (is_paddle_collision(ball_x, ball_y, right_paddle_x - PADDLE_WIDTH, right_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT))
    {
        ball_dx *= -1; // Reflect the ball
        ball_x = right_paddle_x - BALL_SIZE;
    }

    if (ball_x <= 0)
    {
        ball_dx *= -1;
        right_score += 1;
    }

    if (ball_x + BALL_SIZE >= CNV_W)
    {
        ball_dx *= -1;
        left_score += 1;
    }

    // Check collisions with walls
    if (ball_y <= 0 || ball_y + BALL_SIZE >= CNV_H)
    {
        ball_dy *= -1; // Reflect the ball vertically
    }

    return 0;
}

int main(void)
{
    create_canvas(CNV_W, CNV_H);

    return 0;
}