#include <stdio.h>
#include <stdlib.h>

// Define a structure to represent a neural network
typedef struct {
    int I;     // Number of input units
    int H;     // Number of hidden units
    int O;     // Number of output units
    double *weights; // Array to store the weights
} NeuralNetwork;

// Function to initialize a neural network
NeuralNetwork *initializeNeuralNetwork(int I, int H, int O) {
    NeuralNetwork *nn = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));
    if (nn == NULL) {
        return NULL; // Memory allocation failed
    }

    nn->I = I;
    nn->H = H;
    nn->O = O;
    
    // Initialize weights to zero
    int numWeights = I * H + H * O;
    nn->weights = (double *)calloc(numWeights, sizeof(double));
    if (nn->weights == NULL) {
        free(nn);
        return NULL; // Memory allocation failed
    }

    return nn;
}

// Function to load weights from a file
int loadWeightsFromFile(NeuralNetwork *nn, const char *filename) {
    FILE *file = fopen("weights.txt", "r");
    if (file == NULL) {
        printf("Falhou a abrir Ficheiro \n");
        return 0; // File opening failed
    }
/*
    int i, j;
    double weight;
    while (fscanf(file, "%d:%d %lf", &i, &j, &weight) == 3=) {
        if (i >= 1 && i <= nn->I && j >= 1 && j <= nn->H + nn->O) {
            // Adjust indices for zero-based array
            i--;
            j--;
            nn->weights[i * (nn->H + nn->O) + j] = weight;
  
        }
        */
    int i, j, k, l;
    double weight;
    while (fscanf(file, "%d:%d %d:%d %lf", &i, &j, &k, &l, &weight) == 5) {
        if (i >= 1 && i <= nn->I && j >= 1 && j <= nn->H + nn->O) {
            // Adjust indices for zero-based array
            i--;
            j--;
            nn->weights[i * (nn->H + nn->O) + j] = weight;
        }

    }

    fclose(file);
    return 1; // Successfully loaded weights
}

// Function to propagate input through the network
void propagateInput(NeuralNetwork *nn, double *input, double *output) {
    for (int j = 0; j < nn->H + nn->O; j++) {
        output[j] = 0;
        for (int i = 0; i < nn->I; i++) {
            output[j] += input[i] * nn->weights[i * (nn->H + nn->O) + j];
        }
    }
}

// Function to save the neural network to a file
int saveNeuralNetwork(NeuralNetwork *nn, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0; // File creation failed
    }

    fprintf(file, "%d %d %d\n", nn->I, nn->H, nn->O);
    for (int i = 0; i < nn->I; i++) {
        for (int j = 0; j < nn->H ; j++) {
            fprintf(file, "%d:%d %lf\n", i + 1, j + 1, nn->weights[i * (nn->H + nn->O) + j]);
        }
    }

    fclose(file);
    return 1; // Successfully saved neural network
}

// Function to free memory associated with the neural network
void freeNeuralNetwork(NeuralNetwork *nn) {
    free(nn->weights);
    free(nn);
}

int main() {
    // Example usage:
    NeuralNetwork *nn = initializeNeuralNetwork(2, 3, 1);
    if (loadWeightsFromFile(nn, "weights.txt")) {
        double input[2] = {0.5, 0.8};
        double output[1];
        propagateInput(nn, input, output);
        printf("Output: %lf\n", output[0]);
        saveNeuralNetwork(nn, "saved_weights.txt");
    }
    freeNeuralNetwork(nn);

    return 0;
}