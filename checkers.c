#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//attributes
#define RESET 0
#define BRIGHT 1
//foreground
#define RED 1
#define GREEN 2
//background
#define BLACK 0
#define WHITE 7
//defining pieces
#define B 66
#define R 82
#define G 71

void color(int attr, int fg, int bg){
  //to see how coloring spaces work https://linuxgazette.net/issue65/padala.html
  char col[13];
  //sprintf stores the string into a variable instead of printing it
  sprintf(col, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  printf("%s", col);
}

void print_header(){
  int j = 65;
  printf("X|");
  for (int i = 0;i < 8;i++){
    printf("%c", j);
    printf("|");
    j++;
  }
}

void print_row(int row, int (*a)[8]){
  //ASCII - B(blank) 66 R(red) 82 G(green) 71
  printf("%d|", row);
  int attr = BRIGHT, fg, bg;
  for (int i = 0; i < 8; i++){
    if(i%2 == 0){
      if(row % 2 == 0){
        bg = WHITE;
      }
      else{
        bg = BLACK;
      }
    }
    else{
      if(row % 2 == 0){
        bg = BLACK;
      }
      else{
        bg = WHITE;
      }
    }
    if((*a)[i] == B){
      color(attr, WHITE, bg);
      printf(" ");
      color(RESET, WHITE, BLACK);
      printf("|");
    }
    else if((*a)[i] == R){
      color(attr, RED, bg);
      printf("O");
      color(RESET, WHITE, BLACK);
      printf("|");
    }
    else{
      color(attr, GREEN, bg);
      printf("O");
      color(RESET, WHITE, BLACK);
      printf("|");
    }
  }
}

void board(int (*matrix)[8]){
  //prints the board with current state of the game
  print_header();
  printf("\n");
  for (int i = 0; i < 8; i++){
    int (*row)[8] = (matrix+i);
    print_row(i+1, row);
    printf("\n");
  }
}

int game_valid(int (*matrix)[8]){
  //sees if the game has ended i.e when one side runs out of pieces
  //TODO implement a draw where if one side has no possible moves its a draw
  int count_r = 0, count_g = 0;
  for (int i = 0; i < 8; i++){
    int (*row)[8] = (matrix + i);
    for (int j = 0; j < 8; j++){
      if((*row)[j] == R){
        count_r++;
      }
      else if((*row)[j] == G){
        count_g++;
      }
      else{}
    }
  }
  if(count_r == 0 || count_g == 0){
    return 0;
  }
  else{
    return 1;
  }
}

int valid_move(char *start, char *end, int (*matrix)[8]){
  //ASCII for A to H is 65 - 72, 1 to 8 is 49 to 56
  //Checks if the starting and ending column is within the board
  if ((int)start[0] < 65 || (int)start[0] > 72 || (int)end[0] < 65 || (int)end[0] > 72){
    return 0;
  }
  //Checks if the starting and ending rows are within the board
  else if((int)start[1] < 49 || (int)start[1] > 56 || (int)end[1] < 49 || (int)end[1] > 56){
    return 0;
  }
  //Checks if the piece lands on an empty square
  else if((*(matrix+atoi(end+1)-1))[(int)end[0]-65] != B){
    return 0;
  }

  else{
    //Checks if the piece moves more than one diagonal square
    if(((int)end[0] - (int)start[0]) > 1 || ((int)start[0]-(int)end[0]) > 1){
      char in_bet_col = (char)((int)start[0]+1);
      int in_bet_row = (atoi(start+1) + atoi(end + 1))/2;
      printf("\n%c %d\n", in_bet_col, in_bet_row);
      (*(matrix+in_bet_row-1))[((int)(in_bet_col))-65] = B;
    }
    int s = atoi(start + 1);
    int e = atoi(end + 1);
    int temp = (*(matrix+s-1))[((int)start[0])-65];
    (*(matrix+s-1))[((int)start[0])-65] = B;
    (*(matrix+e-1))[((int)end[0])-65] = temp;
    return 1;
  }
}

int main(){
  printf("When typing move, write <Col no.><Row no.> eg A2. So you will be prompted starting and ending move. Do not worry about jumping pieces. Just write A8 and B7 for eg.\n");
  int checkers_board[8][8] = {
    {B,G,B,G,B,G,B,G},
    {G,B,G,B,G,B,G,B},
    {B,G,B,G,B,G,B,G},
    {B,B,B,B,B,B,B,B},
    {B,B,B,B,B,B,B,B},
    {R,B,R,B,R,B,R,B},
    {B,R,B,R,B,R,B,R},
    {R,B,R,B,R,B,R,B}
  };
  //declares a string to capture moves last element is '\0'
  char start_move[3];
  char end_move[3];
  //prints the starting board
  board(checkers_board);
  //state is 1 if game hasnt ended, 0 otherwise
  int state = game_valid(checkers_board);
  while (state == 1){
    printf("Enter the position of the piece to move: ");
    scanf("%s", start_move);
    printf("Enter the position of the piece when it lands: ");
    scanf("%s", end_move);
    //move_ok checks if move is valid and changes the board if valid, otherwise returns 0
    int move_ok = valid_move(start_move, end_move, checkers_board);
    if(move_ok == 0){
      printf("Invalid Move. Re-enter.\n");
      continue;
    }
    //checks if game has ended
    state = game_valid(checkers_board);
    //prints the board 
    board(checkers_board);
  }
  printf("Game ended");
  return 0;
}
