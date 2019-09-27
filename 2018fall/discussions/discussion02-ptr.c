#include <stdio.h>


int main() {
  char* chPt = "hgfedcba";    // char chPt[3] = {'c','b','a'};
  long chPt2Int = (int) chPt; //convertion loss
  int lett = (int) *chPt;
  int x = (int) *((int*)chPt);
  int y = (int) *((int*)chPt + 1);
  
  printf("chPt     = %p               \n",chPt);
  printf("chPt2Int = %p               \n",chPt2Int);
  printf("lett     = %p ('%c')        \n",lett,lett);
  printf("x        = %p               \n", x);
  printf("y        = %p             \n\n", y);
  
  printf("size of a char:    %ld bytes   \n",sizeof(char));
  printf("size of an int:    %ld bytes   \n",sizeof(int));
  printf("size of a double:  %ld bytes \n\n",sizeof(double));
  
  printf("size of a char*:   %ld bytes   \n",sizeof(char*));
  printf("size of an int*:   %ld bytes   \n",sizeof(int*));
  printf("size of a double*: %ld bytes   \n",sizeof(double*));
  return 0;
}
