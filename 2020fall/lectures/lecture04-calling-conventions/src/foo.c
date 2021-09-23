void foo(int a) {
    if (a == 0)
        return;
    a--;
    foo(a);
    return;
}

void main(void) {
    foo(4);
    return;
};
