#include "Tablero.h"
#include <raylib.h>

Tablero::Tablero(int tamanoCasilla, int offsetX, int offsetY) {
    this->tamanoCasilla = tamanoCasilla;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    
    iniciarMovPosibles();
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {tableroFichas[fila][col] = nullptr;}
    }
    iniciarFichas();
}
//Copiar matriz de fichas
Tablero::Tablero(const Tablero& otro) {
    // Copiar matriz principal de fichas
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (otro.tableroFichas[i][j] != nullptr) {
                tableroFichas[i][j] = new Ficha(*otro.tableroFichas[i][j]);
            } else {
                tableroFichas[i][j] = nullptr;
            }
        }
    }

    fichasJugador1 = ListaFichas::clonarLista(otro.fichasJugador1);
    fichasCPU = ListaFichas::clonarLista(otro.fichasCPU);

    turnoJugador = otro.turnoJugador;
    vivasJugador = otro.vivasJugador;
    vivasCPU = otro.vivasCPU;
 
}

void Tablero::dibujarTablero() {
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            Color colorCasilla = (fila + col) % 2 == 0 ? RAYWHITE : DARKBROWN;
            DrawRectangle(offsetX + col * tamanoCasilla, offsetY + fila * tamanoCasilla, tamanoCasilla, tamanoCasilla, colorCasilla);
        }
    }
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            if (tableroFichas[fila][col] != nullptr) {
                int centroX = offsetX + col * tamanoCasilla + tamanoCasilla / 2;
                int centroY = offsetY + fila * tamanoCasilla + tamanoCasilla / 2;

                tableroFichas[fila][col]->dibujarFicha(centroX, centroY, (tamanoCasilla / 2) - 4, tamanoCasilla);
            }
        }
    }
}

//Para tener donde son las casillas negras (solo por ahi se puede mover las fichas)
void Tablero::iniciarMovPosibles(){
      for (int fila = 0; fila < 8; fila++) {
            for (int col = 0; col < 8; col++) {
                movPosible[fila][col] = ((fila + col) % 2 == true); //Son casillas negras
            }
        }
}

void Tablero::iniciarFichas() {

    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            if ((fila + col) % 2 == 1) { // Para solo poner fichas en casillas negras
                if (fila < 3) {
                    // Poniendo las fichas del jugador 2 (CPU)
                    Ficha* nuevaFicha = new Ficha(2, fila, col);
                    tableroFichas[fila][col] = nuevaFicha;
                    ListaFichas* ficha = new ListaFichas(nuevaFicha);
                    fichasCPU = fichasCPU->guardarFicha(fichasCPU,ficha);
                } else if (fila > 4) {
                    // Poniendo las fichas del jugador 1
                    Ficha* nuevaFicha = new Ficha(1, fila, col);
                    tableroFichas[fila][col] = nuevaFicha;
                    ListaFichas* ficha = new ListaFichas(nuevaFicha);
                    fichasJugador1 = fichasJugador1->guardarFicha(fichasJugador1,ficha);
                }
            }
        }
    }

    vivasJugador = fichasJugador1->cantFichas;
    vivasCPU = fichasCPU->cantFichas;

}


bool Tablero::movimientoValido(int filaDestino, int colDestino){
    if (filaDestino < 0 || filaDestino >= 8 || colDestino < 0 || colDestino >= 8) {return false;}
    if (!movPosible[filaDestino][colDestino]){return false;} //Si es true es porque se esta eligiendo mover una ficha hacia una casilla blanca.
    return true;
}

void Tablero::eliminarFicha(Ficha* ficha) {
    if (ficha == nullptr) {return;}

    int jugador = ficha->getJugador();
    
    if (jugador == 1){
        fichasJugador1 = fichasJugador1->borrarFichaDeLista(fichasJugador1, ficha);
        vivasJugador--;
    }

    else{
        fichasCPU = fichasCPU->borrarFichaDeLista(fichasCPU, ficha);
        vivasCPU--;
    }
    
    // Quitar del tablero
    int fila = ficha->getFila();
    int col = ficha->getCol();
    tableroFichas[fila][col] = nullptr;


    delete ficha;
}

//Más enfocado para que nosotros movamos
void Tablero::moverFicha(Ficha* ficha, int filaDestino, int colDestino){
    if (ficha == nullptr){return;}
    
    if (ficha -> getJugador() != this -> turnoJugador){return;}
    
    if (!movimientoValido(filaDestino, colDestino)){return;}
    
    if (tableroFichas[filaDestino][colDestino] != nullptr){return;} 

    int filaOrigen = ficha -> getFila();
    int colOrigen = ficha -> getCol();
    int deltaFila = filaDestino - filaOrigen;
    int deltaCol = colDestino - colOrigen;

    //Verificar que el movimiento no sea retrocediendo si no es dama.
    if (!ficha->getEsDama()) {
        if ((ficha->getJugador() == 1 && deltaFila >= 0) ||
            (ficha->getJugador() == 2 && deltaFila <= 0)) {return;}
    }

    //El movimiento no es ni captura ni simple.
    if (!((abs(deltaFila) == 1 && abs(deltaCol) == 1) || (abs(deltaFila) == 2 && abs(deltaCol) == 2))) {return;}
    
    //Logica para validar el "comer" una ficha rival.
    if (abs(deltaFila) == 2 && abs(deltaCol) == 2){

        int filaMedio = filaOrigen + deltaFila / 2;
        int colMedio = colOrigen + deltaCol / 2;
        Ficha* fichaIntermedia = tableroFichas[filaMedio][colMedio];
        
        if (fichaIntermedia == nullptr) {return;}

        if (fichaIntermedia -> getJugador() == ficha -> getJugador()) {return;} //Si retorna es porque se intentó comer una ficha rival. 

        eliminarFicha(fichaIntermedia);
    } 

    if (filaOrigen < 0 || filaOrigen >= 8 || colOrigen < 0 || colOrigen >= 8) {return;}

    tableroFichas[filaOrigen][colOrigen] = nullptr;
    tableroFichas[filaDestino][colDestino] = ficha;
    
    ficha -> setFila(filaDestino);
    ficha -> setColumna(colDestino);

    //Si llega al otro lado se convierte en dama.
    if ((ficha->getJugador() == 1 && filaDestino == 0) ||(ficha->getJugador() == 2 && filaDestino == 7)) {
            ficha ->setEsDama(true);
    }

    if (turnoJugador == 1){this -> turnoJugador = 2;}
    else if (turnoJugador == 2){this -> turnoJugador = 1;}
}

void Tablero::turnoCPU() {
    if (juegoTerminado()) {return;}

    Movimiento mejorMov = obtenerMejorMovimientoCPU(4); 
    aplicarMovimiento(mejorMov);
}


Ficha* Tablero::getFicha(int fila, int col) {
    if (fila < 0 || fila >= 8 || col < 0 || col >= 8) {return nullptr;}

    return tableroFichas[fila][col];
}

int Tablero::getTurno(){return this -> turnoJugador;}

bool Tablero::juegoTerminado() {
    if (vivasJugador == 0) {return true;}

    if (vivasCPU == 0) {return true;}

    return false;
}

void Tablero::setTurno(int turno) {this->turnoJugador = turno;}


vector<Movimiento> Tablero::obtenerMovimientosJugador(int jugador, ListaFichas* head) {

    vector<Movimiento> movimientos;
    ListaFichas* aux = head;

    while (aux != nullptr) {
        Ficha* ficha = aux->getFicha();
        int fila = ficha->getFila();
        int col = ficha->getCol();
        bool esDama = ficha->getEsDama();

        vector<pair<int, int>> direcciones;

        if (esDama) {
            direcciones = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
        } else if (jugador == 1) {
            direcciones = { {-1, -1}, {-1, 1} };
        } else {
            direcciones = { {1, -1}, {1, 1} };
        }

        for (pair<int,int>& dir : direcciones) {
            int df = dir.first;
            int dc = dir.second;

            int filaDestino = fila + df;
            int colDestino = col + dc;

            if (movimientoValido(filaDestino, colDestino) && tableroFichas[filaDestino][colDestino] == nullptr) {
                movimientos.push_back(Movimiento(fila, col, filaDestino, colDestino, false));
            }

            int filaIntermedia = fila + df;
            int colIntermedia = col + dc;
            int filaSalto = fila + 2 * df;
            int colSalto = col + 2 * dc;

            if (movimientoValido(filaSalto, colSalto) &&
                tableroFichas[filaIntermedia][colIntermedia] != nullptr &&
                tableroFichas[filaIntermedia][colIntermedia]->getJugador() != jugador &&
                tableroFichas[filaSalto][colSalto] == nullptr) {
                movimientos.push_back(Movimiento(fila, col, filaSalto, colSalto, true));
            }
        }

        aux = aux->siguiente;
    }

    return movimientos;
}

void Tablero::aplicarMovimiento(const Movimiento& mov) {
    int filaOrigen = mov.filaOrigen;
    int colOrigen = mov.colOrigen;
    int filaDestino = mov.filaDestino;
    int colDestino = mov.colDestino;

    Ficha* ficha = tableroFichas[filaOrigen][colOrigen];

    // Mover ficha en matriz
    tableroFichas[filaDestino][colDestino] = ficha;
    tableroFichas[filaOrigen][colOrigen] = nullptr;

    if (mov.esCaptura) {
    int filaCapturada = (filaOrigen + filaDestino) / 2;
    int colCapturada = (colOrigen + colDestino) / 2;
    eliminarFichaEnPosicion(filaCapturada, colCapturada);
}


    // Actualizar posición ficha
    ficha->setFila(filaDestino);
    ficha->setColumna(colDestino);

    // Comprobar coronación
    if (!ficha->getEsDama()) {
        if ((ficha->getJugador() == 1 && filaDestino == 0) ||
            (ficha->getJugador() == 2 && filaDestino == 7)) {
            ficha->setEsDama(true);
        }
    }

    turnoJugador = (turnoJugador == 1) ? 2 : 1;
}

int Tablero::evaluar(){
    int puntaje = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Ficha* ficha = tableroFichas[i][j];
            if (ficha != nullptr) {
                int valorFicha = ficha->getEsDama() ? 3 : 1;
                if (ficha->getJugador() == 2) { // CPU (max)
                    puntaje += valorFicha;
                } else { // Jugador humano (min)
                    puntaje -= valorFicha;
                }
            }
        }
    }

    return puntaje;
}

Ficha* Tablero::obtenerFicha(int fila, int col){return tableroFichas[fila][col];}


int Tablero::minimax(int profundidad, bool esMaximizador, int alpha, int beta) {
    
    if (juegoTerminado() || profundidad == 0) {
        return evaluar();
    }

    if (esMaximizador) {
        int mejorValor = INT_MIN;

        vector<Movimiento> movimientos = obtenerMovimientosJugador(2, fichasCPU);
        for (Movimiento mov : movimientos) {
            Tablero copia = *this; 
            copia.aplicarMovimiento(mov);

            int valor = copia.minimax(profundidad - 1, false, alpha, beta);
            mejorValor = max(mejorValor, valor);
            alpha = max(alpha, mejorValor);
            if (beta <= alpha)
                break;  
        }

        return mejorValor;
    } else {
        int peorValor = INT_MAX;

        vector<Movimiento> movimientos = obtenerMovimientosJugador(1, fichasJugador1);
        for (Movimiento mov : movimientos) {
            Tablero copia = *this;
            copia.aplicarMovimiento(mov);

            int valor = copia.minimax(profundidad - 1, true, alpha, beta);
            peorValor = min(peorValor, valor);
            beta = min(beta, peorValor);
            if (beta <= alpha)
                break; 
        }

        return peorValor;
    }
}

Movimiento Tablero::obtenerMejorMovimientoCPU(int profundidad) {
    int mejorValor = INT_MIN;

    Movimiento mejorMovimiento = Movimiento(0,0,0,0,0);

    vector<Movimiento> movimientos = obtenerMovimientosJugador(2, fichasCPU);
    for (Movimiento mov : movimientos) {
        Tablero copia = *this;
        copia.aplicarMovimiento(mov);

        int valor = copia.minimax(profundidad - 1, false, INT_MIN, INT_MAX);

        if (valor > mejorValor) {
            mejorValor = valor;
            mejorMovimiento = mov;
        }
    }

    return mejorMovimiento;
}


void Tablero::eliminarFichaEnPosicion(int fila, int col) {
    if (fila < 0 || fila >= 8 || col < 0 || col >= 8) return;

    Ficha* ficha = tableroFichas[fila][col];
    if (ficha == nullptr) return;

    int jugador = ficha->getJugador();
    if (jugador == 1) {
        fichasJugador1 = fichasJugador1->borrarFichaDeLista(fichasJugador1, ficha);
        vivasJugador--;
    } else {
        fichasCPU = fichasCPU->borrarFichaDeLista(fichasCPU, ficha);
        vivasCPU--;
    }

    tableroFichas[fila][col] = nullptr;
    delete ficha;
}
