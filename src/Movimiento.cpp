#include "Movimiento.h"

Movimiento::Movimiento(int filaOrigen,int colOrigen,int filaDestino,int colDestino,bool esCaptura){
    this->filaOrigen = filaOrigen;
    this->colOrigen = colOrigen;
    this->filaDestino = filaDestino;
    this->colDestino = colDestino;
    this->esCaptura = false;
}