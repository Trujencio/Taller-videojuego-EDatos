#include "raylib.h"
#include "Tablero.h"

enum Pantalla {
    MENU,
    JUEGO,
    SALIR
};

// Función para mostrar el menú y devolver la pantalla elegida
Pantalla MostrarMenu() {
    DrawText("MENU PRINCIPAL", 200, 100, 30, BLACK);

    Rectangle btnJugar = {220, 200, 200, 50};
    Rectangle btnSalir = {220, 270, 200, 50};

    DrawRectangleRec(btnJugar, DARKGRAY);
    DrawText("JUGAR", 275, 215, 20, WHITE);

    DrawRectangleRec(btnSalir, DARKGRAY);
    DrawText("SALIR", 285, 285, 20, WHITE);

    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, btnJugar)) {
            return JUEGO;
        } else if (CheckCollisionPointRec(mouse, btnSalir)) {
            return SALIR;
        }
    }

    return MENU;
}

int main() {
    srand(time(NULL));
    const int anchoVentana = 640;
    const int altoVentana = 640;
    const int columnas = 8;
    const int tamanoCasilla = (anchoVentana / columnas);

    InitWindow(anchoVentana, altoVentana, "Tablero");
    SetTargetFPS(60);

    // Estado del programa
    Pantalla pantallaActual = MENU;

    // Inicialización de juego (solo si se entra al juego)
    Tablero tablero(tamanoCasilla);
    Ficha* fichaSeleccionada = nullptr;

    while (!WindowShouldClose() && pantallaActual != SALIR) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (pantallaActual == MENU) {
            pantallaActual = MostrarMenu();
        } 
        else if (pantallaActual == JUEGO) {
            tablero.dibujarTablero();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    int col = GetMouseX() / tamanoCasilla;
                    int fila = GetMouseY() / tamanoCasilla;

                    if (fichaSeleccionada == nullptr) {
                        Ficha* ficha = tablero.obtenerFicha(fila, col);
                        if (ficha != nullptr) {
                            fichaSeleccionada = ficha;
                        }
                    } else {
                        tablero.moverFicha(fichaSeleccionada, fila, col);
                        fichaSeleccionada = nullptr;

                        if (tablero.juegoTerminado()){
                            pantallaActual = MENU;
                        }
                        
                    }
                }
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
