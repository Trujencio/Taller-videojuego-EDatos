#include "Tablero.h"
#include <raylib.h>

Tablero::Tablero(int tamanoCasilla){
    
    this -> tamanoCasilla = tamanoCasilla;
    turnoJugador = 1;
    iniciarMovPosibles();

    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            tableroFichas[fila][col] = nullptr;
        }
    }

    iniciarFichas();
}

void Tablero::dibujarTablero() {
    // Dibujar casillas (tablero)
    
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            Color colorCasilla = (fila + col) % 2 == 0 ? RAYWHITE : DARKBROWN;
            DrawRectangle(col * tamanoCasilla, fila * tamanoCasilla, tamanoCasilla, tamanoCasilla, colorCasilla);
        }
    }

    // Dibujar fichas
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            if (tableroFichas[fila][col] != nullptr) {
                int centroX = col * tamanoCasilla + tamanoCasilla / 2;
                int centroY = fila * tamanoCasilla + tamanoCasilla / 2;
                
                tableroFichas[fila][col] -> dibujarFicha(centroX, centroY, tamanoCasilla / 2 - 4,tamanoCasilla);
            }
        }
    }
}


void Tablero::iniciarMovPosibles(){
      for (int fila = 0; fila < 8; fila++) {
            for (int col = 0; col < 8; col++) {
                movPosible[fila][col] = ((fila + col) % 2 == true);
            }
        }
}
void Tablero::iniciarFichas() {
    listaCPU = nullptr;  // Suponiendo que tienes Nodo* listaCPU como atributo de Tablero

    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            if ((fila + col) % 2 == 1) { // Solo en casillas negras
                if (fila < 3) {
                    // Fichas del jugador 2 (CPU)
                    Ficha* nuevaFicha = new Ficha(2, fila, col);
                    tableroFichas[fila][col] = nuevaFicha;
                    Nodo* nuevoNodo = new Nodo(nuevaFicha);
                    listaCPU = Nodo::insertarNodo(listaCPU, nuevoNodo);
                } else if (fila > 4) {
                    // Fichas del jugador 1
                    tableroFichas[fila][col] = new Ficha(1, fila, col);
                }
            }
        }
    }
}


bool Tablero::movimientoValido(Ficha* ficha, int filaDestino, int colDestino){
    if (filaDestino < 0 || filaDestino >= 8 || colDestino < 0 || colDestino >= 8) {
        return false;
    }
    
    // if (!ficha -> movimientoFicha(filaDestino,colDestino)){
    //     //TraceLog(LOG_INFO, "Segundo if");
    //     return false;
    // }
    
    if (!movPosible[filaDestino][colDestino]){return false;} //lo que esta en esa posicion es false.

    return true;
}

void Tablero::moverFicha(Ficha* ficha, int filaDestino, int colDestino){
    if (ficha == nullptr){
        TraceLog(LOG_INFO, "nullptr");
        return;
    }
    
    if (ficha -> getJugador() != this -> turnoJugador){return;}
    
    if (!movimientoValido(ficha, filaDestino, colDestino)){
        TraceLog(LOG_INFO, "movimientoValido");
        return;
        
    }
    
    if (tableroFichas[filaDestino][colDestino] != nullptr){
        return;
    } //Porque no se pueden poner directamente encima.

    int filaOrigen = ficha -> getFila();
    int colOrigen = ficha -> getCol();
    int deltaFila = filaDestino - filaOrigen;
    int deltaCol = colDestino - colOrigen;
    TraceLog(LOG_INFO, "despues de fila origen");

    if (!ficha->getEsDama()) {
        if ((ficha->getJugador() == 1 && deltaFila >= 0) ||
            (ficha->getJugador() == 2 && deltaFila <= 0)) {return;}
    }


    if (!((abs(deltaFila) == 1 && abs(deltaCol) == 1) || (abs(deltaFila) == 2 && abs(deltaCol) == 2))) {
        TraceLog(LOG_INFO, "if raro.");
        return;
    } //Movimiento que no es ni captura, ni simple.
    
    if (abs(deltaFila) == 2 && abs(deltaCol) == 2){
        TraceLog(LOG_INFO, "segundo if.");
        int filaMedio = filaOrigen + deltaFila / 2;
        int colMedio = colOrigen + deltaCol / 2;
        Ficha* intermedia = tableroFichas[filaMedio][colMedio];

        if (intermedia == nullptr) return;
        if (intermedia -> getJugador() == ficha -> getJugador()) return; // ficha es del mismo jugador.
        if (intermedia != nullptr && intermedia != ficha && intermedia->getJugador() != ficha->getJugador()) {
            if (intermedia ->getJugador() == 1){vivasJugador--;}
            if (intermedia ->getJugador() == 2){vivasCPU--;}
                
            
            delete intermedia;
            tableroFichas[filaMedio][colMedio] = nullptr;
        }

    } 

    if (filaOrigen < 0 || filaOrigen >= 8 || colOrigen < 0 || colOrigen >= 8) {
    TraceLog(LOG_ERROR, "Índices fuera de rango: fila %d, col %d", filaOrigen, colOrigen);
    return; // o manejar error
    }

    tableroFichas[filaOrigen][colOrigen] = nullptr;
    tableroFichas[filaDestino][colDestino] = ficha;
    
    ficha -> setFila(filaDestino);
    ficha -> setColumna(colDestino);

    
    if ((ficha->getJugador() == 1 && filaDestino == 0) ||
        (ficha->getJugador() == 2 && filaDestino == 7)) {
            ficha ->setEsDama(true);
            TraceLog(LOG_INFO, "Ficha coronada como dama");
    }


    if (turnoJugador == 1){this -> turnoJugador = 2;}
    else if (turnoJugador == 2){this -> turnoJugador = 1;}
    
}

Ficha* Tablero::obtenerFicha(int fila, int col) {
    if (fila < 0 || fila >= 8 || col < 0 || col >= 8) return nullptr;
    return tableroFichas[fila][col];
}

int Tablero::getTurno(){return this -> turnoJugador;}
void Tablero::jugarCPU() {
    Nodo* nodo = listaCPU;

    while (nodo != nullptr) {
        Ficha* ficha = nodo->ficha;
        if (ficha != nullptr) {
            int fila = ficha->getFila();
            int col = ficha->getCol();

            TraceLog(LOG_INFO, "CPU ficha en (%d, %d)", fila, col);

            int filaDestino = fila + 1;
            int colIzq = col - 1;
            int colDer = col + 1;

            if (movimientoValido(ficha, filaDestino, colIzq)) {
                TraceLog(LOG_INFO, "moviendo a (%d, %d)", filaDestino, colIzq);
                moverFicha(ficha, filaDestino, colIzq);
                return;
            }

            if (movimientoValido(ficha, filaDestino, colDer)) {
                TraceLog(LOG_INFO, "moviendo a (%d, %d)", filaDestino, colDer);
                moverFicha(ficha, filaDestino, colDer);
                return;
            }

            TraceLog(LOG_INFO, "Ficha en (%d, %d) no puede moverse", fila, col);
        } else {
            TraceLog(LOG_INFO, "Nodo sin ficha");
        }

        nodo = nodo->sig;
    }

    TraceLog(LOG_INFO, "La CPU no pudo mover ninguna ficha.");
}


bool Tablero::juegoTerminado() {
    if (vivasJugador == 0) {
        TraceLog(LOG_INFO, "Perdiste. No te quedan fichas.");
        return true;
    }

    if (vivasCPU == 0) {
        TraceLog(LOG_INFO, "¡Ganaste! La CPU no tiene más fichas.");
        return true;
    }

    return false;
}

