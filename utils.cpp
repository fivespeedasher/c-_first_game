typedef unsigned int u32;
#define global_variable static
#define internal static
int claim(int x, int min, int max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}