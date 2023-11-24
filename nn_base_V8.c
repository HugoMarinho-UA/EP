//---------------------------------------//
//               Libraries
//---------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//---------------------------------------//
//            Global Variables
//---------------------------------------//

struct NeuralNetwork 
{
    int I, H, O;
    double **inputArrayWeight;
    double **hiddenArrayWeight;
    double *inputArrayBias;
    double *hiddentArrayBias; 
};

//Function to initialize a neural network
void initializeNeuralNetwork(struct NeuralNetwork *nn)
{
    nn->inputArrayWeight = malloc(nn->H * sizeof(double *));
    nn->hiddenArrayWeight = malloc(nn->O * sizeof(double *));
    nn->inputArrayBias = malloc(nn->H * sizeof(double));
    nn->hiddentArrayBias = malloc(nn->O * sizeof(double));
}

// Function to free the memory allocated for a neural network
void freeNeuralNetwork(struct NeuralNetwork *nn) {
    for (int i = 0; i < nn->H; i++) {
        free(nn->inputArrayWeight[i]);
    }
    free(nn->inputArrayWeight);

    for (int i = 0; i < nn->O; i++) {
        free(nn->hiddenArrayWeight[i]);
    }
    free(nn->hiddenArrayWeight);

    free(nn->inputArrayBias);
    free(nn->hiddentArrayBias);
}

//---------------------------------------//
//           Open/Read File
//---------------------------------------//

int readFILE(FILE *fp, struct NeuralNetwork *nn){

    char name_file[30]; 

    printf("Type the name of the file to read\n");

    scanf("%s", name_file);

    // Open file to read
    fp = fopen(name_file, "r");

    // if doesnt open will send message to user
    if(fp == NULL){
        fprintf(stderr, "Couldn't open the file\n");

        return 1;
    }
     // Read first line that contains the number of I, H, O
    fscanf(fp, "%d %d %d", &nn->I, &nn->H, &nn->O);

    // Initialize the neural network
    initializeNeuralNetwork(nn);

    // Close file
    fclose(fp);

    return 0;
}

//---------------------------------------//
//           Open/Write File
//---------------------------------------//

int writeFILE(FILE *fp, struct NeuralNetwork *nn){
    

    char name_file[30]; 

    printf("Type the name of the file to write\n");

    scanf("%s", name_file);

    fp = fopen(name_file, "w");

    // if doesnt open will send message to the user
    if(fp == NULL){
        fprintf(stderr, "Couldn't open the file\n");

        return 1;
    }
    // Write the information to the file
    fprintf(fp, "%d %d %d\n", nn->I, nn->H, nn->O);

    // Write the weights for input to hidden
    for (int i = 0; i < nn->H; i++) {
        for (int j = 0; j < nn->I; j++) {
            fprintf(fp, "%lf ", nn->inputArrayWeight[i][j]);
        }
    }

    // Write the weights for hidden to output
    for (int i = 0; i < nn->H * nn->O; i++) {
        fprintf(fp, "%lf ", nn->hiddenArrayWeight[0][i]);
    }

    // Close file
    fclose(fp);

    return 0;
}

//---------------------------------------//
//          Calculate sigmoid
//---------------------------------------//

double sigmoid(double value){
    return 1/(1 + exp(-value));
}

//---------------------------------------//
//      Calculate sigmoid derivative
//---------------------------------------//

double sigmoid_derivative(double value){
    // returns the derivative value
    return value * (1 - value);
}

//-----------------------------//
//         Propagation
//-----------------------------//

int propagation(struct NeuralNetwork *nn, double lr, double median_error_value, int epoch_value) {
    
    int epoch = 0;
    int start = 0;
    double median_error;
    double ***inputArrayWeight3D;
    double ***hiddenArrayWeight3D;
    double ***inputArrayBias3D;
    double ***hiddentArrayBias3D;
    double inputs[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    double target[4][1] = {{0},{1},{1},{0}};
    double **hiddenActivation;
    double **hiddenOutput;
    double **outputActivation;
    double **output;
    double **error;
    double **d_hiddenweight;
    double **d_hiddenbias;
    double **d_inputweight;
    double **d_inputbias;
    double **error_prop_hidden;
    double **error_hidden;
    double **weights;

    // Allocate memory for 3D arrays
    inputArrayWeight3D = malloc(4 * sizeof(double **));
    hiddenArrayWeight3D = malloc(4 * sizeof(double **));
    inputArrayBias3D = malloc(4 * sizeof(double **));
    hiddentArrayBias3D = malloc(4 * sizeof(double **));

    hiddenActivation = malloc(4 * sizeof(double *));
    hiddenOutput = malloc(4 * sizeof(double *));
    outputActivation = malloc(4 * sizeof(double *));
    output = malloc(4 * sizeof(double *));
    error = malloc(4 * sizeof(double *));
    d_hiddenweight = malloc(4 * sizeof(double *));
    d_hiddenbias = malloc(4 * sizeof(double *));
    d_inputweight = malloc(4 * sizeof(double *));
    d_inputbias = malloc(4 * sizeof(double *));
    error_prop_hidden = malloc(4 * sizeof(double *));
    error_hidden = malloc(4 * sizeof(double *));

    for (int i = 0; i < 4; i++) {
        inputArrayWeight3D[i] = malloc(nn->H * sizeof(double *));
        hiddenArrayWeight3D[i] = malloc(nn->O * sizeof(double *));
        inputArrayBias3D[i] = malloc(1 * sizeof(double *));
        hiddentArrayBias3D[i] = malloc(1 * sizeof(double *));

        hiddenActivation[i] = malloc(nn->H * sizeof(double));
        hiddenOutput[i] = malloc(nn->H * sizeof(double));
        outputActivation[i] = malloc(nn->O * sizeof(double));
        output[i] = malloc(nn->O * sizeof(double));
        error[i] = malloc(nn->O * sizeof(double));
        d_hiddenweight[i] = malloc(nn->O * sizeof(double));
        d_hiddenbias[i] = malloc(nn->O * sizeof(double));
        d_inputweight[i] = malloc(nn->H * sizeof(double));
        d_inputbias[i] = malloc(nn->H * sizeof(double));
        error_prop_hidden[i] = malloc(nn->O * sizeof(double));
        error_hidden[i] = malloc(nn->H * sizeof(double));
    }

    // Reallocate weights in 3D arrays
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < nn->H; j++){
            for (int k = 0; k < nn->I; k++){
                inputArrayWeight3D[i][j][k] = nn->inputArrayWeight[j][k];
            }
        inputArrayBias3D[i][0][j] = nn->inputArrayBias[j];
        }
        for (int j = 0; j < nn->O; j++){
            for (int k = 0; k < nn->H; k++){
                hiddenArrayWeight3D[i][j][k] = nn->hiddenArrayWeight[j][k];
            }
            hiddentArrayBias3D[i][0][j] = nn->hiddentArrayBias[j];
        }
    }

    // start calculation
    start = 1;

    // condition the time of calculation until gets the error objective or a max number of tentaives
    while ((epoch <= epoch_value && median_error > median_error_value) || start == 1){

        start = 0;
        printf("epoch = %d\n", epoch);
        printf("start = %d\n", start);

    //---------------------------------------//
    //         Forward Propagation
    //---------------------------------------//

        // calculate to the 4 solutions
        for(int i = 0; i < 4; i++){

            // calculate hidden output
            for (int j = 0; j < nn->H; j++){
                hiddenActivation[i][j] = 0;
                for (int k = 0; k < nn->I; k++){
                    hiddenActivation[i][j] += inputArrayWeight3D[i][j][k] * inputs[i][k];
                }
                hiddenActivation[i][j] += inputArrayBias3D[i][0][j];
                hiddenOutput[i][j] = sigmoid(hiddenActivation[i][j]);
                printf("%lf ", inputs[i][j]);
                //printf("%lf ", hiddenOutput[i][j]);
            }

            // calculate outputs
            for (int j = 0; j < nn->O; j++){
                outputActivation[i][j] = 0;
                for (int k = 0; k < nn->H; k++){
                    outputActivation[i][j] += hiddenArrayWeight3D[i][j][k] * hiddenOutput[i][k];
                }
                outputActivation[i][j] += hiddentArrayBias3D[i][0][j];
                output[i][j] = sigmoid(outputActivation[i][j]);
                printf("output = %lf\n", output[i][j]);
            }

            // calculate error
            error[i][0] = (target[i][0] - output[i][0]) * sigmoid_derivative(output[i][0]);
            printf("error = %lf\n", error[i][0]);

            // sum the error do divide later
            median_error += fabs(error[i][0]);
        }

        // median error
        median_error = median_error/4;
        printf("median error = %lf", median_error);

        printf("\n");

    //---------------------------------------//
    //            Back Propagation
    //---------------------------------------//

        // calculate to the 4 solutions
        for(int i = 0; i < 4; i++){

            // calculate changes in weights between hidden and output layer
            for (int j = 0; j < nn->H; j++){
                d_hiddenweight[j][0] = lr * error[i][0] * hiddenOutput[i][j];
                //printf("%lf ", d_hiddenweight[j][0]);
            }
            d_hiddenbias[i][0] = lr * error[i][0];
            //printf("%lf\n", d_hiddenbias[i][0]);

            // propagate error to hidden layer
            for(int j = 0; j < nn->H; j++){
                error_prop_hidden[i][j] = error[i][0] * nn->hiddenArrayWeight[0][j];
            }

            // calculate propagated error form output
            for(int j = 0; j < nn->H; j++){
                error_hidden[i][j] = error_prop_hidden[i][j] * hiddenOutput[i][j];
            }
            
            // calculate changes in weights between input and hidden layer
            for(int j = 0; j < nn->H; j++){
                for (int k = 0; k < nn->I; k++){
                    d_inputweight[k][j] = lr * error_hidden[i][j] * inputs[i][k];
                    //printf("%lf ", d_inputweight[k][j]);
                }
                d_inputbias[i][j] = lr * error_hidden[i][j];
                //printf("%lf\n", d_inputbias[i][j]);
            }

            // update weights of input to hidden layer and bias
            for (int j = 0; j < nn->H; j++){
                for (int k = 0; k < nn->I; k++){
                    inputArrayWeight3D[i][j][k] = inputArrayWeight3D[i][j][k] + d_inputweight[k][j];
                    //printf("%lf ", inputArrayWeight[j][k]);
                }
                inputArrayBias3D[i][0][j] = inputArrayBias3D[i][0][j] + d_inputbias[i][j];
                //printf("%lf\n", inputArrayBias[0][j]);
            }
            
            // update weights of hidden to ouput layer and bias
            for (int j = 0; j < nn->O; j++){
                for (int k = 0; k < nn->H; k++){
                    hiddenArrayWeight3D[i][j][k] = hiddenArrayWeight3D[i][j][k] + d_hiddenweight[k][j];
                    //printf("%lf ", hiddenArrayWeight[j][k]);
                }
                hiddentArrayBias3D[i][0][j] = hiddentArrayBias3D[i][0][j] + d_hiddenbias[i][j];
                //printf("%lf\n", hiddentArrayBias[0][j]);
            }
        }

        // one more season
        epoch++;
    }
}

//---------------------------------------//
//              Main Cycle
//---------------------------------------//

int main(){

//---------------------------------------//
//           Initialization
//---------------------------------------//

    double lr = 0;
    double median_error_value = 0;
    int epoch_value = 0;

    //---------------------------------------//
    //       Print information from a file
    //---------------------------------------//

    // file Pointer
    FILE *fp= NULL;

    // initialize neural network parameters
    struct NeuralNetwork nn;
    nn.I=0;
    nn.H=0;
    nn.O=0;

    // Oopen file to read
    if(readFILE(fp, &nn)!=0){
        return 1;   // exit if file reading fail 
    }

    // read first line that contain the number of I,H,O
    fscanf(fp, "%d %d %d", &nn.I, &nn.H, &nn.O);

    // print the values of I,H,O
    printf("I = %d, H = %d, O = %d\n", nn.I, nn.H, nn.O);

    // declare and Initialize variables for the loop
    int li, ui, lo, uo;
    double w;

    // read the lines that contain the weight of every link between layers
    while (fscanf(fp, "%d:%d %d:%d %lf", &li, &ui, &lo, &uo, &w) == 5){
        // Print the information
        printf("Li -> %d:%d %d:%d %f\n", li, ui, lo, uo, w);
    }

    printf("\n");

    // Close file
    fclose(fp);

    // Neural network parameters
    double inputArrayWeight[nn.H][nn.I];
    double hiddenArrayWeight[nn.O][nn.H];
    double inputArrayBias[1][nn.H];
    double hiddentArrayBias[1][nn.O];

    // clear weights
    freeNeuralNetwork(&nn);

//---------------------------------------//
//              Execution
//---------------------------------------//

    //---------------------------------------------------------------------//
    //        Get information from file to calculate output of network
    //---------------------------------------------------------------------//

    // Open file to read
    readFILE(fp, &nn);

    // Gives the information to the user about the initialization of the reading of the neural network
    printf("Reading neural network...\n");

    // Read first line that contain the number of I,H,O
    fscanf(fp, "%d %d %d", &nn.I, &nn.H, &nn.O);

    // Read the lines that contain the weight of every link between in and hidden layer
    for (int k = 0; k < (nn.I * nn.H);k++){
        fscanf(fp, "%d:%d %d:%d %lf", &li, &ui, &lo, &uo, &w);
        // Transfer the weights to the array
        inputArrayWeight[k%nn.H][k/nn.H] = w;         
    }

    // Read the lines that contain the weight of every link between hidden and output layer
    for (int i = 0; i < (nn.H * nn.O);i++){
        fscanf(fp, "%d:%d %d:%d %lf", &li, &ui, &lo, &uo, &w);
        // Transfer the weights to the array
        hiddenArrayWeight[0][i] = w;
    }

    printf("\n");

    // Close file
    fclose(fp);

    // Inform the user that the file as been read
    printf("Finished reading the file!\n");

    //---------------------------------------//
    //            Random Weights
    //---------------------------------------//

    // random weights for input to hidden
    for(int i = 0; i < nn.I; i++){
        for(int j = 0; j < nn.H; j++){
            inputArrayWeight[i][j] = ((double)rand()/RAND_MAX);
        }
    }

    // random weights for bias to hidden
    for(int i = 0; i < nn.H; i++){
        inputArrayBias[0][i] = ((double)rand()/RAND_MAX);
    }

    // random weights for hidden to output
    for(int i = 0; i < nn.H; i++){
        for(int j = 0; j < nn.O; j++){
            hiddenArrayWeight[i][j] = ((double)rand()/RAND_MAX);
        }
    }

    // random weights for bias to output
    for(int i = 0; i < nn.O; i++){
        hiddentArrayBias[0][i] = ((double)rand()/RAND_MAX);
    }

    //-----------------------------//
    //            Debug
    //-----------------------------//

    // Print initial weights and biases
    printf("Initial hidden weights: ");
    for (int i = 0; i < nn.I; ++i) {
        for (int j = 0; j < nn.H; ++j) {
            printf("%lf ", inputArrayWeight[i][j]);
        }
    }
    printf("\n");

    printf("Initial hidden biases: ");
    for (int i = 0; i < nn.H; ++i) {
        printf("%lf ", inputArrayBias[0][i]);
    }
    printf("\n");

    printf("Initial output weights: ");
    for (int i = 0; i < nn.H; ++i){
        for (int j = 0; j < nn.O; ++j){
            printf("%lf ", hiddenArrayWeight[i][j]);
        }
    }
    printf("\n");

    printf("Initial output biases: ");
    for (int i = 0; i < nn.O; ++i) {
        printf("%lf ", hiddentArrayBias[0][i]);
    }
    printf("\n");

    //---------------------------------------//
    //        Training Algorithm
    //---------------------------------------//

    printf("Select a learn rate for the neural network\n");
    scanf("%lf", &lr);

    printf("\n");

    printf("Select how much median_error you want for the neural network\n");
    scanf("%lf", &median_error_value);

    printf("\n");

    printf("Select how much epoch you want for the neural network to run\n");
    scanf("%d", &epoch_value);

    propagation(&nn, lr, median_error_value, epoch_value);

    //---------------------------------------------------------------------//
    //        Write information on file about the network parameters
    //---------------------------------------------------------------------//

    // Open file to write
    writeFILE(fp, &nn);

    // Tells the user to type the number of input nodes
    printf("Type de number of input nodes\n");
    scanf("%d", &nn.I);

    printf("\n");

    // Tells the user to type the number of hidden nodes
    printf("Type de number of hidden nodes\n");
    scanf("%d", &nn.H);
    
    printf("\n");

    // Tells the user to type the number of output nodes
    printf("Type de number of output nodes\n");
    scanf("%d", &nn.O);

    printf("\n");

    // Write tthe information to a new file
    fprintf(fp, "%d %d %d\n", nn.I, nn.H, nn.O);

    // Structurize the neural network based the numbers typed before
    for (int z = 1; z <= nn.I; z++){
        for (int a = 1; a <= nn.H; a++){
            fprintf(fp, "1:%d 2:%d %lf\n", z, a, w);
        } 
    }
    
    for (int z = 1; z <= nn.H; z++){
        for (int a = 1; a <= nn.O; a++){
            fprintf(fp, "2:%d 3:%d %lf\n", z, a, w);
        } 
    }

    // Close file
    fclose(fp);

    // Tells the user that the file as been created and saved
    printf("Neural net work saved!!!\n");

    return 0;
}