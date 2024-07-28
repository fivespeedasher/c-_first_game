struct Button_State {
    bool is_down;
    bool changed;
};

enum { // 可以将下面的变量代表着0、1、2、3、4
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_W,
    BUTTON_S,
    BUTTON_COUNT
};

struct Input {
    Button_State buttons[BUTTON_COUNT];
};