typedef unsigned int u32;

void render_background() {
    u32* pixel = (u32*)render_state.memory; // 以防render_state.memory是空指针
    for(int y  = 0; y < render_state.height; y++) {
        for(int x = 0; x < render_state.width; x++) {
            *pixel++ = 0xff5500ff;
        }
    }
}