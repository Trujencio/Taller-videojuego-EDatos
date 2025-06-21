#include "Ficha.h"

Texture2D Ficha::texturaRambo;
Texture2D Ficha::texturaTerminator;
Texture2D Ficha::texturaSoldado;
Texture2D Ficha::texturaCyborg;

void Ficha::cargarTexturas() {
    texturaRambo = LoadTexture("sprites/rambo.png");
    texturaTerminator = LoadTexture("sprites/terminator.png");
    texturaSoldado = LoadTexture("sprites/soldado.png");
    texturaCyborg = LoadTexture("sprites/cyborg.png");
}

void Ficha::liberarTexturas() {
    UnloadTexture(texturaRambo);
    UnloadTexture(texturaTerminator);
    UnloadTexture(texturaSoldado);
    UnloadTexture(texturaCyborg);
}

Ficha::Ficha(int jugador, int filaInicial, int colInicial) {
    this->jugador = jugador;
    this->filaActual = filaInicial;
    this->colActual = colInicial;
    this->esDama = false;

    if (jugador == 1) {
        this->texturaActual = &texturaRambo;
    } else {
        this->texturaActual = &texturaTerminator;
    }
}

void Ficha::dibujarFicha(int centroX, int centroY, int radio,int tamanoCasilla) {
    float factorEscala = 2.0f;
    float escala = factorEscala * tamanoCasilla / (float)texturaActual->width;

    Vector2 posicion = {
        centroX - (texturaActual->width * escala) / 2.0f,
        centroY - (texturaActual->height * escala) / 2.0f
    };

    DrawTextureEx(*texturaActual, posicion, 0.0f, escala, WHITE);
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
void Ficha::setEsDama(bool nuevoEstado) {
    this->esDama = nuevoEstado;

    if (jugador == 1) {
        this->texturaActual = &texturaSoldado;
    } else {
        this->texturaActual = &texturaCyborg;
    }
}

void Ficha::setFila(int filaDestino){this->filaActual = filaDestino;}
void Ficha::setColumna(int colDestino){this->colActual = colDestino;}
int Ficha::getFila(){return this -> filaActual;}
int Ficha::getCol(){return this -> colActual;}
int Ficha::getJugador(){return this -> jugador;}
bool Ficha::getEsDama(){return this -> esDama;}


