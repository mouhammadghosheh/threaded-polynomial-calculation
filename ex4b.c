#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <unistd.h>

#include <sys/wait.h>

//Functions
void print();

int identifier(char * );

int X_Finder(char * );

int space_Finder(char * );

int calcAns(char * , int);

int main() {
  print();
  return 0;
}

//this function calculates the final answer of the partial polynom according to it's degree.
//it takes the polynom as a string and the general parameter as an int.
//it returns an int which is the result.
int calcAns(char * str, int param) {
  if (str == NULL)
    return 0;
  int power = identifier(str);
  int coefficient;
  int res = 0;
  if (str[0] == 'x')
    coefficient = 1;
  else
    coefficient = atoi(str);
  if (power == 3) {
    res = coefficient * (param * param * param);
  } else if (power == 2) {
    res = coefficient * (param * param);
  } else if (power == 1) {
    res = coefficient * (param);
  } else if (power == 0) {
    return coefficient;
  }

  return res;

}
//this function takes the input from the user and operates on it according to many things like it's degree and the parameter
//the function does operations that split the string to as many pieces to ease our mission
void print() {
  //useful initialization
  char input[512];
  char * tok;
  int param;
  int positions[4];

  char * str_param;

  while (1) {
    for (int i = 0; i < 4; ++i) //the positions array records what degrees are there, for example if the string has x^3 then the index number 3 will be 1
      positions[i] = 0;
    printf("ENTER A STRING ACCORDING TO THE FOLLOWING FORMAT:\n\"POLYNOM, VAL\"\n");
    fgets(input, 512, stdin); //input getter
    fflush(stdin);
    if (strcmp(input, "done\n") == 0)
      break;
    if (space_Finder(input) == 1) {
      printf("ERROR: SPACES DETECTED\n");
      continue;
    }
    char temp[512];
    for (int i = 0; i < strlen(input); ++i) { //remove the \n at the end of the string
      if (input[i] != '\n') {
        temp[i] = input[i];
      } else {
        temp[i] = '\0';

        break;
      }
    }

    char * splits[4]; //each part of the splits array has a polynom, if that polynom's degree isn't in the main polynom then it is null
    for (int i = 0; i < 4; ++i) {
      splits[i] = NULL;
    }
    str_param = strtok(input, ",");
    str_param = strtok(NULL, ",");

    param = atoi(str_param); //to get the parameter

    tok = strtok(input, "+");
    while (tok != NULL) { //this loop splits every sub-polynom by the '+' sign using strtok into it's index
      if (identifier(tok) >= 0)
        positions[identifier(tok)] = 1;

      splits[identifier(tok)] = tok;

      tok = strtok(NULL, "+");
    }

    int * sum;
    int stat;
    int cur;
    int ID;
    key_t key = ftok("/tmp", 'y'); //key creation for shared memory
    ID = shmget(key, sizeof(int), IPC_CREAT | 0600); // shared memory id
    if (ID == -1)
      perror("FAILURE");
    sum = (int * ) shmat(ID, 0, 0); //sum is the shared variable and shmat gets  it from the shared memory
    if (sum < 0)
      perror("ATTACHING FAILED");
    * sum = 0;
    pid_t forks_Arr[identifier(temp)];

    for (int i = 0; i < 4; ++i) {

      forks_Arr[i] = fork();
      if (forks_Arr[i] < 0) {
        perror("FORK NOT WORKING");
        exit(0);

      } else if (forks_Arr[i] == 0) { //every child calculates a polynom according to it's respectable degree
        cur = calcAns(splits[i], param);
        * sum += cur;
        exit(0);
      }

    }
    for (int j = 0; j < identifier(input); ++j) {
      waitpid(forks_Arr[j], & stat, 0);
    }
    if (positions[0] == 1 && X_Finder(splits[0]) == 1) // the free member in the polynom
      *
      sum += atoi(splits[0]);
    int res = * sum;
    shmdt(sum);
    shmctl(ID, IPC_RMID, 0);
    printf("final answer is  %d\n", res);

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
  for (int i = 0; str[i] == ','; ++i) {
    if (str[i] == ' ')
      return 1;
  }
  return 0;
}