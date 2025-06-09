#include "Ficha.h"

Ficha::Ficha(int jugador, int filaInicial, int colInicial) {
    this->jugador = jugador;
    this->filaActual = filaInicial;
    this->colActual = colInicial;

    if (jugador == 1){textura = LoadTexture("sprites/rambo.png");}
    if (jugador == 2){textura = LoadTexture("sprites/terminator.png");}
}

Ficha::~Ficha(){UnloadTexture(textura);}

void Ficha::dibujarFicha(int centroX, int centroY, int radio,int tamanoCasilla) {
    float factorEscala = 2.0f;
    float escala = factorEscala * tamanoCasilla / (float)textura.width;


    Vector2 posicion = {
        centroX - (textura.width * escala) / 2.0f,
        centroY - (textura.height * escala) / 2.0f
    };

    DrawTextureEx(textura, posicion, 0.0f, escala, WHITE);
}


bool Ficha::movimientoFicha(int nuevaFila, int nuevaCol) {
    int deltaFila = nuevaFila - this->filaActual;
    int deltaCol = nuevaCol - this->colActual;

    // Debe moverse en diagonal
    if (abs(deltaFila) != abs(deltaCol)) return false;

    // Solo puede moverse 1 o 2 casillas (movimiento normal o captura)
    if (abs(deltaFila) != 1 && abs(deltaFila) != 2) return false;

    // Jugador 1 solo puede subir
    if (jugador == 1 && deltaFila >= 0) return false;

    // Jugador 2 solo puede bajar 
    if (jugador == 2 && deltaFila <= 0) return false;

    return true;
}


//Getters y setters.
void Ficha::setFila(int nuevaFila){this -> filaActual = nuevaFila;}
void Ficha::setColumna(int nuevaColumna){this -> colActual = nuevaColumna;}

void Ficha::setEsDama(bool nuevoEstado){
    this -> esDama = nuevoEstado;

    UnloadTexture(this->textura);
    if (this ->jugador == 1){textura = LoadTexture("sprites/soldado.png");}
    if (this ->jugador == 2){textura = LoadTexture("sprites/cyborg.png");}
}

int Ficha::getFila(){return this -> filaActual;}
int Ficha::getCol(){return this -> colActual;}
int Ficha::getJugador(){return this -> jugador;}
bool Ficha::getEsDama(){return this -> esDama;}