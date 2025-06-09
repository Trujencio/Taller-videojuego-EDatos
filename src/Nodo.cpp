#include "Nodo.h"

Nodo::Nodo(Ficha* ficha){
    this -> ficha = ficha;
    this -> sig = nullptr;
}

Ficha* Nodo::getFicha(){return this -> ficha;}

Nodo* Nodo::insertarNodo(Nodo* head, Nodo* nuevo){

    if (head == nullptr && nuevo != nullptr){return nuevo;}

    if (nuevo == nullptr){return head;}
    
    Nodo* nodoAux = head;
    while (nodoAux -> sig != nullptr){nodoAux = nodoAux -> sig;}

    nodoAux -> sig = nuevo;
    return head;
}

Nodo* Nodo::buscarNodoRandom(Nodo* head){
    int numRandom = (rand() % 12) + 1;
    int indiceActual = 1;
    Nodo* nodoActual = head;
    while (indiceActual != numRandom && nodoActual != nullptr){nodoActual = nodoActual -> sig;}
    if (nodoActual == nullptr){return nullptr;}
    return nodoActual;
}