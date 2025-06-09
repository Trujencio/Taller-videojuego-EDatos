#pragma once
#include "Ficha.h"
#include <iostream>
#include "Nodo.h"
class Tablero{

public:
    Nodo* listaCPU;
    int tamanoCasilla;
    bool movPosible[8][8];
    Ficha* tableroFichas[8][8];
    int turnoJugador;
    int vivasJugador = 12;
    int vivasCPU = 12;


    Tablero(int tamanoCasilla);
    void dibujarTablero();
    void iniciarMovPosibles();
    void iniciarFichas();
    Ficha* obtenerFicha(int fila, int col);
    bool movimientoValido(Ficha* ficha, int filaDestino, int colDestino);
    void moverFicha(Ficha* ficha, int filaDestino, int colDestino);
    int getTurno();
    void jugarCPU();
    bool juegoTerminado();
};