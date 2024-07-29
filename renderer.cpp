internal void 
clear_screen(u32 color) {
    u32* pixel = (u32*)render_state.memory; // 以防render_state.memory是空指针
    for(int y  = 0; y < render_state.height; y++) {
        for(int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}

internal void 
draw_rect_pixels(int x0, int y0, int x1, int y1, u32 color) {
    x0 = claim(x0, 0, render_state.width);
    x1 = claim(x1, 0, render_state.width);
    y0 = claim(y0, 0, render_state.height);
    y1 = claim(y1, 0, render_state.height);
    for(int y = y0; y < y1; y++) {
        u32* pixel  =(u32*)render_state.memory + x0 + y * render_state.width; //每次换行都要扫过一整行
        for(int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

global_variable float render_scale = 0.01f;
internal void 
draw_rect(float x, float y, float half_x, float half_y, u32 color) {
    // x属于[-100, 100]为中点，y∈[-50, 50]。half_x * render_state.width为宽度一半

    //宽度只受高度影响
    x *= render_state.height * render_scale;
    y *= render_state.height * render_scale;
    half_x *= render_state.height * render_scale;
    half_y *= render_state.height * render_scale;

    x += render_state.width / 2.f;
    y += render_state.height / 2.f;

    int x0 = x - half_x;
    int y0 = y - half_y;
    int x1 = x + half_x;
    int y1 = y + half_y;

    draw_rect_pixels(x0, y0, x1, y1, color);
}

internal void
draw_number(int number, float x, float y, float size, u32 color) {
    // x,y是字的中心
    float half_size = size / 2.f;
    while(number) {
        int digit = number % 10;
        switch(digit) {
            case 0: {
                draw_rect(x - size, y, half_size, 2.5f * size, color);
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x, y + 2 * size, half_size, half_size, color);
                draw_rect(x, y - 2 * size, half_size, half_size, color);}break;
            case 1: {
                draw_rect(x + size, y, half_size, 2.5f * size, color);}break;
            case 2: {
                draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
                draw_rect(x, y, 1.5f * size, half_size, color);
                draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
                draw_rect(x + size, y + size, half_size, half_size, color);
                draw_rect(x - size, y - size, half_size, half_size, color);} break;

            case 3: {
                draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
                draw_rect(x - half_size, y, size, half_size, color);
                draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
                draw_rect(x + size, y, half_size, 2.5f * size, color);} break;

            case 4: {
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x - size, y + size, half_size, 1.5f * size, color);
                draw_rect(x, y, half_size, half_size, color);} break;

            case 5: {
                draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
                draw_rect(x, y, 1.5f * size, half_size, color);
                draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
                draw_rect(x - size, y + size, half_size, half_size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);} break;

            case 6: {
                draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
                draw_rect(x + half_size, y, size, half_size, color);
                draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
                draw_rect(x - size, y, half_size, 2.5f * size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);} break;

            case 7: {
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x - half_size, y + size * 2.f, size, half_size, color);} break;

            case 8: {
                draw_rect(x - size, y, half_size, 2.5f * size, color);
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x, y + size * 2.f, half_size, half_size, color);
                draw_rect(x, y - size * 2.f, half_size, half_size, color);
                draw_rect(x, y, half_size, half_size, color);} break;

            case 9: {
                draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
                draw_rect(x - half_size, y, size, half_size, color);
                draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x - size, y + size, half_size, half_size, color);} break;
        }
        x -= 4 * size;
        number /= 10;
    }
}