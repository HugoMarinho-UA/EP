#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
    char sizeoffBoard [3][3];
    int choice;
    int move;
    int itsTie;
    int winner;
    char currentPlayer;

}Tic_tac_toe;

int resetBoard(Tic_tac_toe *game);  //Reset board
int printBoard(Tic_tac_toe *game);  //Make board
int playerMove(Tic_tac_toe *game, int move); //Player move
int computerMove(Tic_tac_toe *game);   //Computer move
char checkWinner(Tic_tac_toe *game); //Chek for Winner
void printWinner(char winner); //Print winner of game 
int checkFreeSpaces(Tic_tac_toe *game); //Check for free spaces

int main (void)
{
    Tic_tac_toe game;
    game.choice = 0;
    game.move = 0;
    game.currentPlayer = 'X';
    char response = ' ';

    printf("1. Play against another player?\n");
    printf("2. Play against computer\n");
    scanf("%d", &game.choice);

    do
    {
        resetBoard(&game);
        game.winner = ' ';  // Initialize winner for a new game

        if (game.choice == 1)
        {
            printBoard(&game);
            while (game.winner == ' ' && game.itsTie == 0)
            {
                printf("Player %c, enter your move: ", game.currentPlayer);
                scanf("%d", &game.move);

                // Validate player move
                while (playerMove(&game, game.move) == 0)
                {
                    printf("Invalid move. Please try again: ");
                    scanf("%d", &game.move);
                }

                printBoard(&game);
                game.winner = checkWinner(&game);

                // Check for a tie
                if (checkFreeSpaces(&game) == 0 && game.winner == ' ')
                {
                    game.itsTie = 1;
                    printf("It's a tie!\n");
                }

                // Switch player turn
                game.currentPlayer = (game.currentPlayer == 'X') ? 'O' : 'X';
            }
        }
        else if (game.choice == 2)
        {
            // Implement computer vs player logic
            printBoard(&game);
            while (game.winner == ' ' && game.itsTie == 0)
            {
                if (game.currentPlayer == 'X')
                {
                    printf("Player %c, enter your move: ", game.currentPlayer);
                    scanf("%d", &game.move);

                    // Validate player move
                    while (playerMove(&game, game.move) == 0)
                    {
                        printf("Invalid move. Please try again: ");
                        scanf("%d", &game.move);
                    }
                }
                else
                {
                    printf("Computer's move:\n");
                    computerMove(&game);
                }

                printBoard(&game);
                game.winner = checkWinner(&game);

                // Check for a tie
                if (checkFreeSpaces(&game) == 0 && game.winner == ' ')
                {
                    game.itsTie = 1;
                    printf("It's a tie!\n");
                }

                // Switch player turn
                game.currentPlayer = (game.currentPlayer == 'X') ? 'O' : 'X';
            }
        }
        printWinner(game.winner);
        printf("Would you like to play again? (Y/N): ");
        scanf(" %c", &response);
        response = toupper(response);
    } while (response == 'Y');

    printf("\nThank you for playing!\n");

    return 0;
}

int resetBoard(Tic_tac_toe *game)
{
    game->itsTie = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            game->sizeoffBoard[i][j] = ' '; // Initialize to empty space
        }
    }
    return 0;

}

int printBoard(Tic_tac_toe *game)
{

    printf("\n");
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%c", game->sizeoffBoard[i][j]); 
            if (j < 2) 
            {
                printf(" |");
            }
        }
        printf("\n");
        if (i < 2) 
        {
            printf("--------\n");
        }
    }
    printf("\n");
}

int playerMove(Tic_tac_toe *game,int move)
{
    if (move < 1 || move > 9) {
        printf("Invalid move. Please enter a number between 1 and 9.\n");
        return 0;
    }

    int row = (move - 1) / 3;
    int col = (move - 1) % 3;

    if (game->sizeoffBoard[row][col] == ' ') 
    {
        game->sizeoffBoard[row][col] = game->currentPlayer;
        return 1;
    } else 
    {
        printf("Invalid move. Cell already taken. Please try again.\n");
        return 0;
    }
}

int computerMove(Tic_tac_toe *game)
{
    int empty_cells[9][2];
    int num_empty_cells = 0;

    // Get the coordinates of empty cells
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->sizeoffBoard[i][j] == ' ') {
                empty_cells[num_empty_cells][0] = i;
                empty_cells[num_empty_cells][1] = j;
                num_empty_cells++;
            }
        }
    }

    if (num_empty_cells > 0) {
        // Randomly choose an empty cell
        int random_index = rand() % num_empty_cells;
        int row = empty_cells[random_index][0];
        int col = empty_cells[random_index][1];

        // Make the move
        game->sizeoffBoard[row][col] = 'O';
    }

    return 0;
}

char checkWinner(Tic_tac_toe *game)
{

    //check rows
    for(int i = 0; i < 3; i++)
    {
        if(game->sizeoffBoard[i][0] == game->sizeoffBoard[i][1] && game->sizeoffBoard[i][0] == game->sizeoffBoard[i][2])
        {
             return game->sizeoffBoard[i][0];
        }
    }

    //check columns
    for(int i = 0; i < 3; i++)
    {
        if(game->sizeoffBoard[0][i] == game->sizeoffBoard[1][i] && game->sizeoffBoard[0][i] == game->sizeoffBoard[2][i])
        {
            return game->sizeoffBoard[0][i];
        }
    }

    //check diagonals
    if(game->sizeoffBoard[0][0] == game->sizeoffBoard[1][1] && game->sizeoffBoard[0][0] == game->sizeoffBoard[2][2])
    {
        return game->sizeoffBoard[0][0];
    }
    if(game->sizeoffBoard[0][2] == game->sizeoffBoard[1][1] && game->sizeoffBoard[0][2] == game->sizeoffBoard[2][0])
    {
        return game->sizeoffBoard[0][2];
    }

    return ' ';
}

int checkFreeSpaces(Tic_tac_toe *game)
{
    int freespaces = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if(game->sizeoffBoard [i][j] == ' ')
            {
                freespaces++;
            }
        }
    }
    return freespaces;
}

void printWinner (char winner)
{
    if (winner == 'X')
    {
        printf("Player X wins! \n");
    }else if (winner == 'O')
    {
        printf("Player 'O' wins! \n");
    } else
    {
        
    }
    
}
