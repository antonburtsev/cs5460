volatile int bar(int a, int b, int c) {
    int x; 
    x = a + b + c;
    x += 6;
    return x;
}

int main(void){
    int x = 1, y = 2, z = 3;

    x = bar(x, y, z);
    x += 5; 
    return x;
};
