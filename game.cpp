#define is_down(b) input->buttons[b].is_down // 结构体以指针传入要用->
#define changed(b) input->buttons[b].changed
#define pressed(b) (is_down(b) && changed(b))
#define released(b) (!is_down(b) && changed(b))

float player1_p = .0f, player1_dp = .0f; //位置，速度，加速度
float player2_p = .0f, player2_dp = .0f;

float arena_half_size_x = 85.f, arena_half_size_y = 45.f; 
float player_half_size_x = 2.f, player_half_size_y = 10.f;

float ball_p_x = .0f, ball_p_y = .0f, ball_dp_x = 100.f, ball_dp_y = 0, ball_half_size = 1.f;

float player1_x = arena_half_size_x - player_half_size_x, player2_x = -arena_half_size_x + player_half_size_x;

internal void
simulate_player(float* p, float* dp, float ddp, float dt) {
    ddp -= *dp * 2.5; //摩擦力
    *dp = *dp + ddp * dt;
    *p += *dp * dt + .5 * ddp * dt * dt;
    // 玩家边界检测
    if(*p + player_half_size_y > arena_half_size_y ||
     *p - player_half_size_y < -arena_half_size_y) {
        *dp = -.8 * *dp;    
    }
}

internal bool
aabb_vs_aabb(float ball_p_x, float ball_p_y, float ball_half_size, 
    float player_x, float player_p, float player_half_size_x, float player_half_size_y) {
    return (ball_p_x + ball_half_size > player_x - player_half_size_x && 
    ball_p_x - ball_half_size < player_x + player_half_size_x &&
    player_p - player_half_size_y <= ball_p_y - ball_half_size &&
    ball_p_y + ball_half_size <= player_p + player_half_size_y); 
}

internal void
simulate_game(Input* input, float dt) {
    clear_screen(0x696969);
    float player1_ddp = .0f, player2_ddp = .0f; //要确保每次停下按就停止加速
    if(is_down(BUTTON_UP)) player1_ddp += 300.f;
    if(is_down(BUTTON_DOWN)) player1_ddp -= 300.f;
    if(is_down(BUTTON_W)) player2_ddp += 300.f;
    if(is_down(BUTTON_S)) player2_ddp -= 300.f;

    simulate_player(&player1_p, &player1_dp, player1_ddp, dt);
    simulate_player(&player2_p, &player2_dp, player2_ddp, dt);

    ball_p_x += ball_dp_x * dt;
    ball_p_y += ball_dp_y * dt;

    // simulate ball
    {
        // 球碰撞检测
        if(aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, player1_x, player1_p, player_half_size_x, player_half_size_y)) {
            ball_p_x = player1_x - player_half_size_x - ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = player1_dp * .5 + (ball_p_y - player1_p) * 2;
        }
        else if(aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, player2_x, player2_p, player_half_size_x, player_half_size_y)) {
            ball_p_x = player2_x + player_half_size_x + ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = player2_dp * .5 + (ball_p_y - player2_p) * 2;
        }

        // 球边界检测
        if(ball_p_x - ball_half_size < -arena_half_size_x || ball_p_x + ball_half_size > arena_half_size_x) {
            ball_p_x = .0f;
            ball_p_y = .0f;
            ball_dp_x = -ball_dp_x;
            ball_dp_y = .0f;
        }
        else if(ball_p_y - ball_half_size < -arena_half_size_y) {
            ball_p_y = -arena_half_size_y + ball_half_size; // 以免程序过了太多才判断
            ball_dp_y *= -1;
        }
        else if(ball_p_y + ball_half_size > arena_half_size_y){
            ball_p_y = arena_half_size_y - ball_half_size;
            ball_dp_y *= -1;
        }
    }

    // rendering
    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xFFEBCD);
    draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xCD2626);
    draw_rect(player1_x, player1_p, player_half_size_x, player_half_size_y, 0x8B4513);
    draw_rect(player2_x, player2_p, player_half_size_x, player_half_size_y, 0x8B4513);
}