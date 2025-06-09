#pragma once
#include <raylib.h>
#include <cmath>
#include <iostream>
class Ficha{
    private:
        int filaActual;
        int colActual;
        int jugador;
        bool esDama = false;
    public:
        Texture2D textura;
        Ficha(int jugador, int filaInicial, int ColInicial);
        ~Ficha();

        void dibujarFicha(int centroX, int centroY, int radio,int tamanoCasilla);
        void setFila(int nuevaFila);
        void setColumna(int nuevaCol);
        void setEsDama(bool nuevoEstado);
        bool movimientoFicha(int nuevaFila, int nuevaCol);
        int getFila();
        int getCol();
        int getJugador();
        bool getEsDama();
};
