#include "ListaFichas.h"

ListaFichas::ListaFichas(Ficha* ficha){
    this->ficha = ficha;
    this->siguiente = nullptr;
}

ListaFichas* ListaFichas::guardarFicha(ListaFichas* head,ListaFichas* fichaGuardar){
    if (head == nullptr && fichaGuardar != nullptr){return fichaGuardar;}

    if (head != nullptr && fichaGuardar == nullptr){return head;}
    
    ListaFichas* aux = head;

    while (aux -> siguiente != nullptr){
        aux = aux -> siguiente;
    }

    aux -> siguiente = fichaGuardar;
    cantFichas++;
    return head;
}

//Esta funcion como tal no borra, solo elimina las referencias.
ListaFichas* ListaFichas::borrarFichaDeLista(ListaFichas* head,Ficha* fichaBorrar){
    if (head == nullptr){return nullptr;}

    //Si lo que se quiere borrar es el head.
    if (fichaBorrar == head ->getFicha()){return head -> siguiente;}

    if (fichaBorrar == nullptr){return head;}
    
    ListaFichas* aux = head;
    
    while (aux->siguiente != nullptr && aux->siguiente->getFicha() != fichaBorrar){
        aux = aux ->siguiente;
    }
    
    // Si no se encontró la ficha
    if (aux->siguiente == nullptr) {return head;}

    
    ListaFichas* fichaABorrar = aux->siguiente;
    aux->siguiente = fichaABorrar->siguiente;

    this->cantFichas--;
    return head;
}

void ListaFichas::liberarLista(ListaFichas* head) {
    while (head != nullptr) {
        ListaFichas* siguiente = head->siguiente;
        delete head;  // Solo borras el nodo, NO la ficha
        head = siguiente;
    }
}

//Para evitar que al borrar fichas en el tablero copia no afecte al tablero original
ListaFichas*  ListaFichas::clonarLista(ListaFichas* original) {
    if (original == nullptr) return nullptr;


    ListaFichas* copiaHead = new ListaFichas(new Ficha(*original->ficha));
    ListaFichas* actualOriginal = original->siguiente;
    ListaFichas* actualCopia = copiaHead;

    while (actualOriginal != nullptr) {
        ListaFichas* nuevoNodo = new ListaFichas(new Ficha(*actualOriginal->ficha));
        actualCopia->siguiente = nuevoNodo;

        actualCopia = nuevoNodo;
        actualOriginal = actualOriginal->siguiente;
    }

    return copiaHead;
}

Ficha* ListaFichas::getFicha(){return this->ficha;}
int ListaFichas::getCantFichas(){return this->cantFichas;}
