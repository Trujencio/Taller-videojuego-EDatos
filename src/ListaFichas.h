#pragma once
#include "Ficha.h"

class ListaFichas{
    public:
        Ficha* ficha;
        ListaFichas* siguiente;
        ListaFichas(Ficha* ficha);
        int cantFichas = 0;

        ListaFichas* guardarFicha(ListaFichas* head,ListaFichas* fichaGuardar);
        ListaFichas* borrarFichaDeLista(ListaFichas* head,Ficha* fichaBorrar);
        void liberarLista(ListaFichas* head);
        Ficha* getFicha();
        int getCantFichas();
        static ListaFichas* clonarLista(ListaFichas* original);
};