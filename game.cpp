#define is_down(b) input->buttons[b].is_down // 结构体以指针传入要用->
#define changed(b) input->buttons[b].changed
#define pressed(b) (is_down(b) && changed(b))
#define released(b) (!is_down(b) && changed(b))

float player_x = 0.5f;
float player_y = 0.5f;

internal void
simulate_game(Input* input, float dt) {
    clear_screen(0xff5500ff);

    float speed = 10.f; //每秒移动的距离
    draw_rect(.2, .2, .05, .05, 0xffff0000); 
    if(pressed(BUTTON_UP)) player_y += speed * dt; //为什么is_down会快很多呢？
    if(pressed(BUTTON_DOWN)) player_y -= speed * dt;
    if(pressed(BUTTON_LEFT)) player_x -= speed * dt;
    if(pressed(BUTTON_RIGHT)) player_x += speed * dt;
    draw_rect(player_x, player_y, .005, .005, 0xff00ff00);
}