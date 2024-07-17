#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 13
#define WALL '#'
#define PATH ' '
#define ENTRY 'E'
#define EXIT 'S'

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

void initMaze(char maze[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            maze[i][j] = WALL;
        }
    }
}

void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void generateMaze(char maze[N][N], int x, int y) {
    maze[x][y] = PATH;

    int dirs[] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[dirs[i]] * 2;
        int ny = y + dy[dirs[i]] * 2;

        if (nx > 0 && ny > 0 && nx < N-1 && ny < N-1 && maze[nx][ny] == WALL) {
            maze[x + dx[dirs[i]]][y + dy[dirs[i]]] = PATH;
            generateMaze(maze, nx, ny);
        }
    }
}

void ensureSinglePath(char maze[N][N]) {
    for (int i = 1; i < N-1; i++) {
        for (int j = 1; j < N-1; j++) {
            if (maze[i][j] == WALL) {
                int pathCount = 0;
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (maze[ni][nj] == PATH) {
                        pathCount++;
                    }
                }
                if (pathCount > 1) {
                    maze[i][j] = PATH;
                }
            }
        }
    }
}

void printMaze(char maze[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    char maze[N][N];
    initMaze(maze);

    int startX = 1, startY = 1;
    maze[startX][startY] = ENTRY;

    generateMaze(maze, startX, startY);

    maze[N-2][N-2] = EXIT;

    ensureSinglePath(maze);

    printMaze(maze);

    return 0;
}
