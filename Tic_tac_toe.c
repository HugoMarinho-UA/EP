#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

char board[3][3];
const char PLAYER = 'X';
const char COMPUTER = 'O';

void resetBoard();
void printBoard();
int checkFreeSpaces();
void playerMove();
void computerMove();
char checkWinner();
void printWinner(char);


int main()
{
    char winner = ' ';
    char response = ' ';
    int choice;
    printf("Choose an option: \n");
    printf("1. Play against another player. \n");
    printf("2. Play against the computer. \n");
    scanf("%d", &choice);
    if(choice == 2)
    {
        do
        {
            
            winner = ' ';
            response = ' ';
            resetBoard();

            while(winner == ' ' && checkFreeSpaces() != 0)
            {
                printBoard();

                playerMove();
                winner = checkWinner();
                if(winner != ' ' || checkFreeSpaces() == 0)
                {
                    break;
                }

                computerMove();
                winner = checkWinner();
                if(winner != ' ' || checkFreeSpaces() == 0)
                {
                    break;
                }
                printBoard();
                printWinner(winner);

                printf("\nWould you like to play again? (Y/N): ");
                scanf("%c");
                scanf("%c", &response);
                response = toupper(response);
            } while (response == 'Y');
        }        
        while (response == 'Y');
        printf("Thanks for playing!");
    } 
   return 0;
}

void resetBoard()
{
    for( int i = 0; i < 3; i++) // Reset rows
    {
        for( int j = 0; j < 3; j++) // Reset Colums
        {
            board[i][j] = ' ';
        }
    }

}
void printBoard()
{
    printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}
int checkFreeSpaces()
{
    int freespaces = 9;     //Number off available spaces, 9

}
void playerMove()
{

}
void computerMove()
{

}
char checkWinner()
{

}
void printWinner(char winner)
{

}