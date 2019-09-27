#include <stdio.h>
#include <stdlib.h>
int main() {
  struct sandwish{
    int bread_size;
    char content;
    unsigned char taste;
  };

  struct sandwish s0;
  struct sandwish* ptrSan;
  /*
    Recomendation of no casting:
    https://stackoverflow.com/questions/605845/do-i-cast-the-result-of-malloc
    ptrSan = malloc(sizeof(struct sandwish));

    However, I will leave the version seen in the class.
  */
  ptrSan = (struct sandwish*) malloc(sizeof(struct sandwish));
  
  s0.bread_size = 4;
  s0.content = 'H';
  s0.taste = 167;
  
  ptrSan->bread_size = 3;
  ptrSan->content = 'C';
  ptrSan->taste = 192;

  printf("direct     : %d, %c, %d\n",\
	 s0.bread_size, s0.content,s0.taste);
  
  printf("through ptr: %d, %d, %d",\
	 ptrSan->bread_size,\
	 ptrSan->content, ptrSan->taste);
  return 0;
}
