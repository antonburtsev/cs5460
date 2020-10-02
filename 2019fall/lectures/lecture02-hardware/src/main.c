
int x = 1, y = 2, z = 3;

volatile int bar() {
    return x + y;
}


volatile int baz(int a, int b, int c) {
    int x; 
    x = a + b + c;
    x += 6;
    return x;
}


int main(void){
    
    x = bar();

    x = baz(x, y, z);
    x += 5; 
    return x;
};
