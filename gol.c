#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

// Dimensions of the grid of life
#define ROWS 50
#define COLS 60

// Probability of life in any cell
#define LIFE_PROB 0.1

// For every step of evolution, we compute the sum of every
// cell in a separate grid with identical dimensions. We use
// that sum later to evolve every cell in the original array.
void evolve(int **grid, int **gridsum) {
    // 1. Compute all sums for every cell
    for (int rows = 0; rows < ROWS; rows++) {
        for (int cols = 0; cols < COLS; cols++) {
            int sum = 0;
            for (int i = rows-1; i <= rows + 1; i++) {
                for (int j = cols - 1; j <= cols + 1; j++) {
                    if (i == rows && j == cols) continue;
                    if (i >= 0 && i < ROWS && j>= 0 && j < COLS) sum += grid[i][j];
                }
            }
            gridsum[rows][cols] = sum;
        }
    }

    // 2. Based on the already computed sums, evolve the cells
    for (int rows = 0; rows < ROWS; rows++) {
        for (int cols = 0; cols < COLS; cols++) {
            if (grid[rows][cols] == 1) {
                if ((gridsum[rows][cols] < 2) | (gridsum[rows][cols] > 3)) grid[rows][cols] = 0;
            }
            else if (grid[rows][cols] == 0 && gridsum[rows][cols] == 3) grid[rows][cols] = 1;
        }
    }
}

int main() {

    // Choose how to represent life state and death state
    char life = 'O';
    char death = ' ';

    // Seed the random number generator with the current process ID and the time
    int seed = time(0) + getpid();
    //srand(time(0) + getpid());
    srand(seed);

    // Initialize our two main grids
    int **grid = malloc(sizeof(int*) * ROWS);
    int **gridsum = malloc(sizeof(int*) * ROWS);
    for (int i = 0; i < ROWS; i++) {
        grid[i] = malloc(sizeof(int) * COLS);
        gridsum[i] = malloc(sizeof(int) * COLS);
    }

    // 1. Life is born
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (((float)rand()/(float)RAND_MAX) < (1.0 - LIFE_PROB)) {
                grid[i][j] = 0;
                printf("%c ", death);
            }
            else {
                grid[i][j] = 1;
                printf("%c ", life);
            }
        }
        printf("\n");
    }

    fflush(stdout);

    // 2. Loop of life
    while (1) {
        // Clear the terminal (LINUX)
        printf("\033[H\033[J");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == 1) printf("%c ", life);
                else printf("%c ", death);
            }
            printf("\n");
            fflush(stdout);
        }

        printf("SEED: %d\n", seed);

        // Perform the evolution
        evolve(grid, gridsum);

        // Microseconds per evolution step
        usleep(90000);
    }

    // Free memory
    for (int i = 0; i < ROWS; i++) {
        free(gridsum[i]);
        free(grid[i]);
    }
    free(grid);
    free(gridsum);

    return 0;
}
