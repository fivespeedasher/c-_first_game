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

internal void 
draw_rect(float x, float y, float half_x, float half_y, u32 color) {
    // x属于[0,1]为中点，half_x * render_state.width为宽度一半

    //宽度只受高度影响
    x *= render_state.width;
    y *= render_state.height;
    half_x *= render_state.height;
    half_y *= render_state.height;

    int x0 = x - half_x;
    int y0 = y - half_y;
    int x1 = x + half_x;
    int y1 = y + half_y;

    draw_rect_pixels(x0, y0, x1, y1, color);
}