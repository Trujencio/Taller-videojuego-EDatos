#pragma once
#include <raylib.h>
#include <cmath>
#include <iostream>
#include <vector>


class Ficha{
    private:
        int filaActual;
        int colActual;
        int jugador;
        bool esDama = false;

        // Textura actual que apunta a la textura compartida
        Texture2D* texturaActual;

        // Texturas estáticas compartidas (una sola por tipo)
        static Texture2D texturaRambo;
        static Texture2D texturaTerminator;
        static Texture2D texturaSoldado;
        static Texture2D texturaCyborg;

    public:
        
        Ficha(int jugador, int filaInicial, int ColInicial);
        ~Ficha(){};
        static void cargarTexturas();
        static void liberarTexturas();
        void dibujarFicha(int centroX, int centroY, int radio,int tamanoCasilla);
        bool movimientoFicha(int nuevaFila, int nuevaCol);
        int getFila();
        int getCol();
        int getJugador();
        bool getEsDama();
        void setEsDama(bool nuevoEstado);
        void setFila(int filaDestino);
        void setColumna(int colDestino);
};
