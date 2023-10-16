#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <pthread.h>

#include <stdint-gcc.h>
 //functions
void print();

int identifier(char * );

int X_Finder(char * );

int space_Finder(char * );
int main() {
  print();
}
//struct contains the param we want to substitute in the polynom and the partial polynom
typedef struct args {
  int param;
  char * str;
}
args;
//below are three threads each one for a degree, we'll use three threads maximum and that is if the polynom is from the 3rd degree
//else each method will be used simultaneously
void * threadMethod3(void * args) {
  struct args * args2 = args;
  int coefficient;
  int res;
  if (args2 -> str[0] == 'x')
    coefficient = 1;
  else
    coefficient = atoi(args2 -> str);

  res = coefficient * (args2 -> param * args2 -> param * args2 -> param);
  return ((void * )(uintptr_t) res);
}

void * threadMethod2(void * args) {
  struct args * args2 = args;
  int coefficient;
  int res;
  if (args2 -> str[0] == 'x')
    coefficient = 1;
  else
    coefficient = atoi(args2 -> str);

  res = coefficient * (args2 -> param * args2 -> param);
  return ((void * )(uintptr_t) res);
}

void * threadMethod1(void * args) {
  struct args * args2 = args;
  int coefficient;
  int res;
  if (args2 -> str[0] == 'x')
    coefficient = 1;
  else
    coefficient = atoi(args2 -> str);

  res = coefficient * (args2 -> param);
  return ((void * )(uintptr_t) res);
}

//this function takes the input from the user and operates on it according to many things like it's degree and the parameter
//the function does operations that split the string to as many pieces to ease our mission
void print() {
  char input[512];
  int count = 0;
  //each one of the res variables are for the results of each partial polynom
  void * res1;
  void * res2;
  void * res3;
  char * tok;
  int positions[4];

  char * str_param;

  //threads
  pthread_t tid1, tid2, tid3;

  while (1) {
    //dynamic memory allocation for the args struct
    struct args * args1 = malloc(sizeof(args));
    if (args1 == NULL) {
      perror("MALLOC FAILED");
      exit(0);
    }
    for (int i = 0; i < 4; ++i) //the positions array records what degrees are there, for example if the string has x^3 then the index number 3 will be 1
      positions[i] = 0;
    printf("ENTER A STRING ACCORDING TO THE FOLLOWING FORMAT:\n\"POLYNOM, VAL\"\n");
    fgets(input, 512, stdin); //input getter
    fflush(stdin);
    if (strcmp(input, "done\n") == 0) {
      free(args1);
      break;
    }
    if (space_Finder(input) == 1) {
      printf("ERROR: SPACES DETECTED\n");
      continue;
    }

    char * splits[4];
    for (int i = 0; i < 4; ++i) { //each part of the splits array has a polynom, if that polynom's degree isn't in the main polynom then it is null
      splits[i] = NULL;
    }
    str_param = strtok(input, ",");
    str_param = strtok(NULL, ",");

    args1 -> param = atoi(str_param);

    tok = strtok(input, "+");
    while (tok != NULL) { //this loop splits every sub-polynom by the '+' sign using strtok into it's index
      if (identifier(tok) >= 0)
        positions[identifier(tok)] = 1;

      splits[identifier(tok)] = tok;

      tok = strtok(NULL, "+");
    }

    for (int i = 0; i < 4; ++i) {
      if (positions[i] == 0) //if the polynom with the degree i was not entered by the user then continue the loop
        continue;
      if (i >= 1)
        args1 -> str = splits[i];
      //in the following if statements, in every case the methods will be activated according to the powers available in the main polynom that was entered in the input by the user
      if (i == 3) { //if the 3rd degree polynom is there in the main input
        if (pthread_create( & tid1, NULL, threadMethod3, args1) != 0)
          printf("Failed to create thread1\n");
        if (pthread_join(tid1, & res1) != 0) {
          perror("join failed");
        }
        count += (int)(uintptr_t) res1;
      }
      if (i == 2) { //if the 2nd polyonm is there in the main one
        if (pthread_create( & tid2, NULL, threadMethod2, args1) != 0)
          printf("Failed to create thread1\n");
        if (pthread_join(tid2, & res2) != 0) {
          perror("join failed");
        }
        count += (int)(uintptr_t) res2;
      }
      if (i == 1) { //if the 1st degree polynom is there in the main one
        if (pthread_create( & tid3, NULL, threadMethod1, args1) != 0)
          printf("Failed to create thread1\n");
        if (pthread_join(tid3, & res3) != 0) {
          perror("join failed");
        }
        count += (int)(uintptr_t) res3;
      }
      if (i == 0) { // if there is a free member in the main polynom
        count += atoi(splits[0]);
      }
    }
    printf("Final answer is: %d\n", count);
    count = 0;
    free(args1);
  }
}

//this function takes a polynom and returns it's power
//it takes the string as a parameter
int identifier(char * str) {
  int ans;
  char c;
  if (X_Finder(str) == 1)
    return 0;
  if (X_Finder(str) == 0 && str[strlen(str) - 1] == 'x')
    return 1;

  c = str[strlen(str) - 1];
  ans = c - '0';
  return ans;
}
//this function checks if the string is the free member part of the polynom or not
//it returns 0 if it is not and 1 if it is the free member
int X_Finder(char * str) {
  for (int i = 0; i < strlen(str); ++i) {
    if (str[i] == 'x')
      return 0;
  }
  return 1;
}
//this function checks if there are spaces in the input string, it takes the string as an argument and
//returns 1 if spaces were found and 0 if there are none
int space_Finder(char * str) {
  for (int i = 0; str[i] != ','; ++i) {
    if (str[i] == ' ')
      return 1;
  }
  return 0;
}