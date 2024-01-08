#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <fann.h>

typedef struct
{
    char sizeoffBoard [3][3];
    int choice;
    int move;
    int itsTie;
    int winner;
    char currentPlayer;

}Tic_tac_toe;

int resetBoard(Tic_tac_toe *game);
int printBoard(Tic_tac_toe *game);
int playerMove(Tic_tac_toe *game, int move);
int computerMove(Tic_tac_toe *game);
char checkWinner(Tic_tac_toe *game);
void printWinner(char winner);
int checkFreeSpaces(Tic_tac_toe *game);
void PlyaAgainstplayer(Tic_tac_toe *game);
void playagainstcomputer(Tic_tac_toe *game, struct fann *ann);
void computeragainstcomputer(Tic_tac_toe *game);
void computerXMove(Tic_tac_toe *game);
void train_neural_network(struct fann *ann, float moves[], int num_moves);
int neural_network_move(struct fann *ann, Tic_tac_toe *game);

int main (void)
{
    Tic_tac_toe game;
    game.choice = 0;
    game.move = 0;
    game.currentPlayer = 'X';
    char response = ' ';
    struct fann *ann = fann_create_standard(3, 9, 10, 9);

    printf("1. Play against another player?\n");
    printf("2. Play against computer\n");
    printf("3. Training mode\n");
    scanf("%d", &game.choice);

        do
    {
        resetBoard(&game);
        game.winner = ' '; // Initialize winner for a new game

        switch (game.choice)
        {
        case 1:
            PlyaAgainstplayer(&game);
            break;
        case 2:
            playagainstcomputer(&game, ann);
            break;
        case 3:
            computeragainstcomputer(&game);
            break;
        

        default:
            printf("Invalid choice. Please choose again.\n");
            break;
        }

        printWinner(game.winner);
        printf("Would you like to play again? (Y/N): ");
        scanf(" %c", &response);
        response = toupper(response);
    } while (response == 'Y');

    fann_destroy(ann);
    printf("\nThank you for playing!\n");

    return 0;
}

void PlyaAgainstplayer (Tic_tac_toe *game)
{
    printBoard(game);
    while (game->winner == ' ' && game->itsTie == 0)
    {
        printf("Player %c, enter your move: ", game->currentPlayer);
        scanf("%d", &game->move);

        // Validate player move
        while (playerMove(game, game->move) == 0)
        {
            printf("Invalid move. Please try again: ");
            scanf("%d", &game->move);
        }

        printBoard(game);
        game->winner = checkWinner(game);

        // Check for a tie
        if (checkFreeSpaces(game) == 0 && game->winner == ' ')
        {
            game->itsTie = 1;
            printf("It's a tie!\n");
        }

        // Switch player turn
        game->currentPlayer = (game->currentPlayer == 'X') ? 'O' : 'X';
    }
}

void playagainstcomputer (Tic_tac_toe *game, struct fann *ann)
{
    printBoard(game);
    while (game->winner == ' ' && game->itsTie == 0)
    {
        if (game->currentPlayer == 'X')
        {
            printf("Player %c, enter your move: ", game->currentPlayer);
            scanf("%d", &game->move);

            // Validate player move
            while (playerMove(game, game->move) == 0)
            {
                printf("Invalid move. Please try again: ");
                scanf("%d", &game->move);
            }
        }
        else
        {
            printf("Computer's move:\n");
            int move = neural_network_move(ann, game);

            // Validate and make the move
            while (playerMove(game, move) == 0)
            {
                printf("Invalid move. Please try again: ");
                move = neural_network_move(ann, game);
            }
        }
        printBoard(game);
        game->winner = checkWinner(game);

        // Check for a tie
        if (checkFreeSpaces(game) == 0 && game->winner == ' ')
        {
            game->itsTie = 1;
            printf("It's a tie!\n");
        }

        // Switch player turn
        game->currentPlayer = (game->currentPlayer == 'X') ? 'O' : 'X';
    }
}

void computeragainstcomputer (Tic_tac_toe *game)
{
    printBoard(game);
    while (game->winner == ' ' && game->itsTie == 0)
    {
        // Computer X's move
        printf("Computer X's move:\n");
        computerXMove(game);

        printBoard(game);
        game->winner = checkWinner(game);

        // Check for a tie
        if (checkFreeSpaces(game) == 0)
        {
            game->itsTie = 1;
            printf("It's a tie!\n");
            break;  // Exit the loop if it's a tie
        }

        // Computer O's move
        printf("Computer O's move:\n");
        computerMove(game);

        printBoard(game);
        game->winner = checkWinner(game);
    }
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

void computerXMove(Tic_tac_toe *game)
{
    int empty_cells[9][2];
    int num_empty_cells = 0;

    // Get the coordinates of empty cells
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (game->sizeoffBoard[i][j] == ' ')
            {
                empty_cells[num_empty_cells][0] = i;
                empty_cells[num_empty_cells][1] = j;
                num_empty_cells++;
            }
        }
    }

    if (num_empty_cells > 0)
    {
        // Randomly choose an empty cell
        int random_index = rand() % num_empty_cells;
        int row = empty_cells[random_index][0];
        int col = empty_cells[random_index][1];

        // Make the move
        game->sizeoffBoard[row][col] = 'X';
    }
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

void train_neural_network(struct fann *ann, float moves[], int num_moves)
{
    /// Create a dataset for training
    struct fann_train_data *data = fann_create_train(num_moves, 9, 9);

    // Set the input and output patterns for the training data
    for (int i = 0; i < num_moves; ++i)
    {
        // Input: Current board state, Output: Next board state
        fann_type *input = &moves[i * 18];
        fann_type *output = &moves[i * 18 + 9];

        // Add the pattern to the training data set
        fann_train_data_set_input(data, i, input);
        fann_train_data_set_output(data, i, output);
    }

    // Train the neural network
    fann_train_on_data(ann, data, 10000, 1000, 0.01);

    // Clean up the training data
    fann_destroy_train(data);
}

int neural_network_move(struct fann *ann, Tic_tac_toe *game)
{
    int empty_cells[9][2];
    int num_empty_cells = 0;

    // Get the coordinates of empty cells
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (game->sizeoffBoard[i][j] == ' ')
            {
                empty_cells[num_empty_cells][0] = i;
                empty_cells[num_empty_cells][1] = j;
                num_empty_cells++;
            }
        }
    }

    if (num_empty_cells > 0)
    {
        // Convert the current board state to a 1D array for input to the neural network
        int moves[9 * 18];  // Assuming 9 moves per game and 18 elements per move (9 input, 9 output)
        int index = 0;

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                moves[index++] = (int)(game->sizeoffBoard[i / 3][i % 3] == 'X');
            }
        }

        uintptr_t predicted_move_ptr = (uintptr_t)fann_run(ann, (fann_type*)moves);
        int predicted_move = 1 + (int)predicted_move_ptr;

        return predicted_move;
    }

    return 0;
}


void printWinner (char winner)
{
    if (winner == 'X')
    {
        printf("Player X wins! \n");
    }else if (winner == 'O')
    {
        printf("Player 'O' wins! \n");
    } else{}
}
