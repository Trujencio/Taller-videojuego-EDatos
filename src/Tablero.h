#pragma once
#include "Ficha.h"
#include "ListaFichas.h"
#include "Movimiento.h"
#include <iostream>
#include <vector>
using namespace std;

class Tablero{

    public:
        int tamanoCasilla; //Para dibujo de tablero.
        bool movPosible[8][8]; //Las casillas negras son true.
        Ficha* tableroFichas[8][8];
        Ficha* tableroCopia[8][8];
        int turnoJugador = 1; //Siempre empezaremos nosotros.

        int vivasJugador = 0;
        int vivasCPU = 0;
        ListaFichas* fichasJugador1 = nullptr;
        ListaFichas* fichasCPU = nullptr;

        int offsetX;
        int offsetY; //Ambos para temas de mouse y dibujo de tablero.

        Tablero(int tamanoCasilla, int offsetX, int offsetY);
        Tablero(const Tablero& otro);
        void dibujarTablero();
        void iniciarMovPosibles();
        void iniciarFichas();
        bool juegoTerminado();
        Ficha* getFicha(int fila, int col);
        bool movimientoValido(int filaDestino, int colDestino);
        vector<Movimiento> obtenerMovimientosJugador(int jugador,ListaFichas* head);
        void moverFicha(Ficha* ficha, int filaDestino, int colDestino);
        int getTurno();
        void turnoCPU();
        void setTurno(int turno);
        void eliminarFicha(Ficha* ficha);
        void eliminarFichaEnPosicion(int fila,int col);
        void aplicarMovimiento(const Movimiento& mov);
        Ficha* obtenerFicha(int fila, int col);
        int evaluar();
        int minimax(int profundidad, bool esMaximizador, int alpha, int beta);
        Movimiento obtenerMejorMovimientoCPU(int profundidad);
};