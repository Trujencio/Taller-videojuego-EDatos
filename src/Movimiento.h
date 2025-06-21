class Movimiento {
public:
    int filaOrigen;
    int colOrigen;
    int filaDestino;
    int colDestino;
    bool esCaptura;  // Opcional, para marcar si el movimiento implica captura

    Movimiento(int filaOrigen,int colOrigen,int filaDestino,int colDestino,bool esCaptura);
};
