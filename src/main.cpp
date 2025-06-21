#include "raylib.h"
#include "Tablero.h"

enum Pantalla {
    MENU,
    JUEGO,
    SALIR
};

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
        if (CheckCollisionPointRec(mouse, btnJugar)) return JUEGO;
        else if (CheckCollisionPointRec(mouse, btnSalir)) return SALIR;
    }

    return MENU;
}

int main() {
    srand(time(NULL));
    const int anchoVentana = 1280;
    const int altoVentana = 720;
    const int columnas = 8;
    const int tamanoCasilla = 70;
    const int tableroAncho = columnas * tamanoCasilla;
    const int tableroAlto = columnas * tamanoCasilla;
    const int espacioSuperior = 80;
    int offsetX = (anchoVentana - tableroAncho) / 2;
    int offsetY = espacioSuperior + (altoVentana - espacioSuperior - tableroAlto) / 2;

    InitWindow(anchoVentana, altoVentana, "Tablero");
    Ficha::cargarTexturas();
    SetTargetFPS(60);

    Pantalla pantallaActual = MENU;
    Tablero tablero(tamanoCasilla, offsetX, offsetY);
    Ficha* fichaSeleccionada = nullptr;

    const int jugadorHumano = 1;
    const int jugadorCPU = 2;

    // Sniper
    Texture2D mira = LoadTexture("sprites/miraSniper.png");
    int turnosJugador = 0;
    const int turnosParaSniper = 3;
    bool sniperDisponible = false;
    bool sniperActivo = false;
    bool sniperMensaje = false;
    float sniperMensajeDuracion = 3.0f;
    float sniperMensajeTimer = 0.0f;

    // CPU retraso
    bool cpuPensando = false;
    float tiempoEspera = 0.5f;
    float tiempoTranscurrido = 0.0f;

    while (!WindowShouldClose() && pantallaActual != SALIR) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (pantallaActual == MENU) {
            pantallaActual = MostrarMenu();
        } 
        else if (pantallaActual == JUEGO) {
            DrawText(("Turno: Jugador " + std::to_string(tablero.getTurno())).c_str(), 20, 20, 30, BLACK);
            tablero.dibujarTablero();

            // Cartel centrado del sniper
            if (sniperMensaje) {
                const int anchoCartel = 500;
                const int altoCartel = 40;
                int posXCartel = (anchoVentana - anchoCartel) / 2;
                int posYCartel = 50;

                DrawRectangle(posXCartel, posYCartel, anchoCartel, altoCartel, Fade(DARKBLUE, 0.85f));
                DrawText("¡SNIPER DISPONIBLE! Presiona 'S' para activar", posXCartel + 10, posYCartel + 10, 20, WHITE);
                sniperMensajeTimer += GetFrameTime();
                if (sniperMensajeTimer >= sniperMensajeDuracion) {
                    sniperMensaje = false;
                }
            }

            // Activar sniper
            if (sniperDisponible && IsKeyPressed(KEY_S)) {
                sniperActivo = true;
                sniperDisponible = false;
            }

            // Mostrar mira del sniper
            if (sniperActivo) {
                HideCursor();
                Vector2 mouse = GetMousePosition();
                DrawTextureEx(mira, {mouse.x - mira.width, mouse.y - mira.height}, 0.0f, 2.0f, WHITE);
            } else {
                ShowCursor();
            }

            // Turno del jugador humano
            if (tablero.getTurno() == jugadorHumano) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    int fila = (GetMouseY() - offsetY) / tamanoCasilla;
                    int col = (GetMouseX() - offsetX) / tamanoCasilla;

                    if (sniperActivo) {
                        Ficha* objetivo = tablero.getFicha(fila, col);
                        if (objetivo != nullptr && objetivo->getJugador() == jugadorCPU) {
                            tablero.eliminarFicha(objetivo);
                            sniperActivo = false;
                            sniperMensaje = false;
                            tablero.turnoJugador = jugadorCPU;

                            if (tablero.juegoTerminado()) pantallaActual = MENU;
                        }
                    } else {
                        if (fichaSeleccionada == nullptr) {
                            Ficha* ficha = tablero.getFicha(fila, col);
                            if (ficha != nullptr && ficha->getJugador() == jugadorHumano) {
                                fichaSeleccionada = ficha;
                            }
                        } else {
                            tablero.moverFicha(fichaSeleccionada, fila, col);
                            fichaSeleccionada = nullptr;

                            turnosJugador++;
                            if (turnosJugador >= turnosParaSniper) {
                                sniperDisponible = true;
                                sniperMensaje = true;
                                sniperMensajeTimer = 0.0f;
                                turnosJugador = 0;
                            }

                            if (tablero.juegoTerminado()) pantallaActual = MENU;
                        }
                    }
                }
            }

            // Turno de la CPU con retraso simulado
            if (tablero.getTurno() == jugadorCPU) {
                if (!cpuPensando) {
                    cpuPensando = true;
                    tiempoTranscurrido = 0.0f;
                } else {
                    DrawText("CPU pensando...", 20, 60, 20, GRAY);
                    tiempoTranscurrido += GetFrameTime();
                    if (tiempoTranscurrido >= tiempoEspera) {
                        tablero.turnoCPU();  // Asegúrate de que usar Minimax correctamente
                        cpuPensando = false;
                        if (tablero.juegoTerminado()) pantallaActual = MENU;
                    }
                }
            }
        }

        EndDrawing();
    }

    Ficha::liberarTexturas();
    UnloadTexture(mira);
    CloseWindow();
    return 0;
}
