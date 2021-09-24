#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>

void myfunc(char** param){
  ++param;
}

int main(){
  char* string = (char*)malloc(64);
  strcpy(string, "hello_World");
  myfunc(&string);
  myfunc(&string);
  printf("%s ", string);
  return 0;
}
