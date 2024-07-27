#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "string.h"

#define MATRIX_SIZE 13

typedef char labyrinth_child;
typedef labyrinth_child labyrinth[MATRIX_SIZE][MATRIX_SIZE];

const int posicionInicial[2] = {0, 0};
const int posicionFinal[2] = {MATRIX_SIZE - 1, MATRIX_SIZE - 1};
bool solved = false;

const labyrinth_child pared = '#';
const labyrinth_child camino = '.';
const labyrinth_child meta = 'M';
const labyrinth_child personaje = 'O';
const labyrinth_child bloqueado = 'x';

const char direcciones[] = "DRLU";
const int dr[] = {1, 0, 0, -1}; // Movimientos en fila { derecha, abajo, izquierda, arriba }
const int dc[] = {0, 1, -1, 0}; // Movimientos en columna { derecha, abajo, izquierda, arriba }

void mostrarLaberinto(labyrinth matrix, size_t rows, size_t cols);
void resolverLaberinto(labyrinth laberinto, size_t rows, size_t cols);
void generarLaberinto(labyrinth matrix, size_t rows, size_t cols);
bool generarCamino(labyrinth matrix, int x, int y, int rows, int cols);

int main() {
  srand(time(NULL));
  labyrinth laberinto = {};
  generarLaberinto(laberinto, MATRIX_SIZE, MATRIX_SIZE);
  resolverLaberinto(laberinto, MATRIX_SIZE, MATRIX_SIZE);

  return 0;
}


void generarLaberinto(labyrinth matrix, size_t rows, size_t cols) {
    // Inicializar el laberinto con paredes
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = pared;
        }
    }

    // Generar el camino desde la posición inicial
    generarCamino(matrix, posicionInicial[0], posicionInicial[1], rows, cols);

    // Establecer la posición inicial y final
    matrix[posicionInicial[0]][posicionInicial[1]] = personaje;
    matrix[posicionFinal[0]][posicionFinal[1]] = meta;
}

bool generarCamino(labyrinth matrix, int x, int y, int rows, int cols) {
    int SHUFFLE_COUNT = 4;
    int directions[4] = {0, 1, 2, 3};

    // Mezclar las direcciones
    for (int i = 0; i < SHUFFLE_COUNT; i++) {
        int r = rand() % SHUFFLE_COUNT;
        int temp = directions[i];
        directions[i] = directions[r];
        directions[r] = temp;
    }

    for (int i = 0; i < SHUFFLE_COUNT; i++) {
        int dir = directions[i];
        int nx = x + dr[dir];
        int ny = y + dc[dir];
        int nx2 = x + 2 * dr[dir];
        int ny2 = y + 2 * dc[dir];

        if (nx2 >= 0 && ny2 >= 0 && nx2 < rows && ny2 < cols && matrix[nx2][ny2] == pared) {
            matrix[nx][ny] = camino;
            matrix[nx2][ny2] = camino;
            if (generarCamino(matrix, nx2, ny2, rows, cols)) {
                return true;
            }
        }
    }

    if (x == posicionFinal[0] && y == posicionFinal[1]) {
        return true;
    }

    return false;
}

void mostrarLaberinto(labyrinth matrix, size_t rows, size_t cols) {
  printf("Mostrando Laberinto: \n");
  for (int i = 0; i <= rows * 2; i++) {
    printf("_");
  }
  printf("\n");
  for (int i = 0; i < rows; i++) {
    printf("|");
    for (int j = 0; j < cols; j++) {
      if (j == MATRIX_SIZE - 1) {
        printf("%c|\n", matrix[i][j]);
      } else {
        printf("%c ", matrix[i][j]);
      }
    }
  }
  for (int i = 0; i <= rows * 2; i++) {
    printf("-");
  }
  printf("\n");
}


bool esValido(int row, int col, labyrinth matrix) {
  return row >= 0 && col >= 0 &&
    row < MATRIX_SIZE && col < MATRIX_SIZE &&
    (matrix[row][col] == camino || matrix[row][col] == meta);
}

void solucionar(
  int row,
  int col,
  labyrinth matrix,
  int* count
) {
  if (solved) return;
  if (row == MATRIX_SIZE - 1 && col == MATRIX_SIZE - 1) return;

  matrix[row][col] = bloqueado;

  for (int i = 0; i < 4; i++) {
    int nextRow = row + dr[i];
    int nextCol = col + dc[i];

    if (esValido(nextRow, nextCol, matrix)) {
      (*count)++;
      if (matrix[nextRow][nextCol] == meta) solved = true;

      matrix[nextRow][nextCol] = personaje;
      mostrarLaberinto(matrix, MATRIX_SIZE, MATRIX_SIZE);
      solucionar(nextRow, nextCol, matrix, count);
    }

    if (solved) break;
  }
};

void resolverLaberinto(labyrinth matrix, size_t rows, size_t cols) {
  const bool canResolveMeta =
    matrix[rows - 1][cols - 2] == camino || matrix[rows - 2][cols - 1] == camino;
  const bool canResolveInicio =
    matrix[1][0] == camino || matrix[0][1] == camino;

  if (!canResolveMeta || !canResolveInicio) {
    printf("No se puede resolver el laberinto\n");
    return;
  }

  int count = 0;

  solucionar(0, 0, matrix, &count);

  if (!solved) {
    printf("No se puede resolver el laberinto\n");
  } else {
    printf("Hubo %d pasos hasta la solución\n", count);
  }
}
