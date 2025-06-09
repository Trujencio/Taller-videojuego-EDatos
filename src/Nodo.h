#pragma once
#include "Ficha.h"
#include <iostream>

class Nodo{

    public:
        Ficha* ficha = nullptr;
        Nodo(Ficha* ficha);
        ~Nodo(){};
        Nodo* sig;
        Ficha* getFicha();
        static Nodo* insertarNodo(Nodo* head, Nodo* nuevo);
        static Nodo* buscarNodoRandom(Nodo* head);
};