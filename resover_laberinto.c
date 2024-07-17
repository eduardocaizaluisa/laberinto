#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

// Dimensiones del laberinto (ancho y alto)
#define ANCHO 5
#define ALTO 5

// Estructura para representar el laberinto
typedef struct {
    bool esPared;
    bool visitado;
    bool esEntrada;
    bool esSalida;
} Celda;

// Funciones para generar números aleatorios
int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Función para verificar si una posición está dentro del laberinto
bool dentroDelLaberinto(int x, int y) {
    return x >= 0 && x < ANCHO && y >= 0 && y < ALTO;
}
//guandad en archivo de texto y leer wasd

// Prototipo de la función para generar el laberinto recursivamente
void generarLaberintoRecursivo(Celda laberinto[ANCHO][ALTO], int x, int y);

// Función para generar un laberinto utilizando el algoritmo Recursive Backtracking
void generarLaberinto(Celda laberinto[ANCHO][ALTO]) {
    // Inicializar todas las celdas
    for (int i = 0; i < ANCHO; ++i) {
        for (int j = 0; j < ALTO; ++j) {
            laberinto[i][j].esPared = true;
            laberinto[i][j].visitado = false;
            laberinto[i][j].esEntrada = false;
            laberinto[i][j].esSalida = false;
        }
    }

    // Definir entrada en la primera casilla
    laberinto[0][0].esEntrada = true;
    laberinto[0][0].esPared = false;

    // Definir salida en la última casilla
    laberinto[ALTO - 1][ANCHO - 1].esSalida = true;
    laberinto[ALTO - 1][ANCHO - 1].esPared = false;

    // Implementar el algoritmo Recursive Backtracking para generar el laberinto
    generarLaberintoRecursivo(laberinto, 0, 0);
}

// Función auxiliar recursiva para generar el laberinto utilizando Recursive Backtracking
void generarLaberintoRecursivo(Celda laberinto[ANCHO][ALTO], int x, int y) {
    // Marcar la celda como visitada
    laberinto[x][y].visitado = true;

    // Direcciones posibles: derecha, abajo, izquierda, arriba
    int direcciones[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Barajar las direcciones para obtener un orden aleatorio
    for (int i = 0; i < 4; ++i) {
        int aleatorio = randInt(i, 3);
        int tempX = direcciones[i][0];
        int tempY = direcciones[i][1];
        direcciones[i][0] = direcciones[aleatorio][0];
        direcciones[i][1] = direcciones[aleatorio][1];
        direcciones[aleatorio][0] = tempX;
        direcciones[aleatorio][1] = tempY;
    }

    // Explorar las celdas vecinas en orden aleatorio
    for (int i = 0; i < 4; ++i) {
        int nuevo_x = x + direcciones[i][0];
        int nuevo_y = y + direcciones[i][1];
        int paso_x = x + direcciones[i][0] / 2;
        int paso_y = y + direcciones[i][1] / 2;

        if (dentroDelLaberinto(nuevo_x, nuevo_y) && !laberinto[nuevo_x][nuevo_y].visitado) {
            // Romper la pared entre la celda actual y la vecina
            laberinto[nuevo_x][nuevo_y].esPared = false;
            laberinto[paso_x][paso_y].esPared = false;

            // Llamar recursivamente a la función para la celda vecina
            generarLaberintoRecursivo(laberinto, nuevo_x, nuevo_y);
        }
    }
}

// Función para imprimir el laberinto en la consola
void imprimirLaberinto(Celda laberinto[ANCHO][ALTO], int posX, int posY) {
    system("cls");  // Limpiar pantalla (Windows)

    printf("Laberinto:\n");
    for (int i = 0; i < ALTO; ++i) {
        for (int j = 0; j < ANCHO; ++j) {
            if (i == posY && j == posX) {
                printf("X "); // Posición actual del elemento
            } else if (laberinto[i][j].esSalida) {
                printf("S "); // Salida
            } else if (laberinto[i][j].esEntrada) {
                printf("E "); // Entrada
            } else if (laberinto[i][j].esPared) {
                printf("# "); // Pared
            } else {
                printf(". "); // Camino libre
            }
        }
        printf("\n");
    }
}

// Función para mover el elemento por el laberinto
bool moverElemento(Celda laberinto[ANCHO][ALTO], int *posX, int *posY) {
    // Direcciones posibles: derecha, abajo, izquierda, arriba
    int direcciones[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    bool movido = false;

    // Explorar las direcciones en orden aleatorio
    for (int i = 0; i < 4; ++i) {
        int aleatorio = randInt(i, 3);
        int tempX = direcciones[i][0];
        int tempY = direcciones[i][1];
        direcciones[i][0] = direcciones[aleatorio][0];
        direcciones[i][1] = direcciones[aleatorio][1];
        direcciones[aleatorio][0] = tempX;
        direcciones[aleatorio][1] = tempY;
    }

    for (int i = 0; i < 4; ++i) {
        int nuevo_x = *posX + direcciones[i][0];
        int nuevo_y = *posY + direcciones[i][1];

        if (dentroDelLaberinto(nuevo_x, nuevo_y) && !laberinto[nuevo_y][nuevo_x].esPared) {
            *posX = nuevo_x;
            *posY = nuevo_y;
            movido = true;
            break;
        }
    }

    return movido;
}

int main() {
    srand(time(NULL)); // Semilla para números aleatorios

    Celda laberinto[ANCHO][ALTO]; // Declaración del laberinto

    generarLaberinto(laberinto); // Generar laberinto utilizando Recursive Backtracking

    int posX = 0, posY = 0; // Posición inicial del elemento
    int pasos = 0; // Contador de pasos

    // Encontrar la posición inicial (entrada)
    for (int i = 0; i < ANCHO; ++i) {
        for (int j = 0; j < ALTO; ++j) {
            if (laberinto[i][j].esEntrada) {
                posX = j;
                posY = i;
                break;
            }
        }
    }

    // Bucle principal para mover el elemento hasta la salida
    while (!laberinto[posY][posX].esSalida) {
        imprimirLaberinto(laberinto, posX, posY);

        if (!moverElemento(laberinto, &posX, &posY)) {
            // Si no se puede mover, el laberinto está mal generado
            printf("¡Laberinto mal generado! No hay salida accesible.\n");
            return 1;
        }

        pasos++; // Incrementar contador de pasos
        Sleep(500); // Esperar 500 ms
    }

    imprimirLaberinto(laberinto, posX, posY); // Imprimir el laberinto final

    // Mostrar mensaje de salida y número de pasos
    printf("\n¡El elemento ha cruzado el laberinto!\n");
    printf("Numero de pasos: %d\n", pasos);

    return 0;
}
