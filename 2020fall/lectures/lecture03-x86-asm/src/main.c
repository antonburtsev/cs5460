
int x = 1;
int a[] = {1, 2, 3, 4, 5, 6, 7, 8};

int bar() {
    int x = 0; 
    for (int i = 0; i < 7; i ++) {
	    x += a[i];
    }; 
    return x;
}


int main(void){
    x = bar() + x; 
    x += 5; 
    return x;
};
