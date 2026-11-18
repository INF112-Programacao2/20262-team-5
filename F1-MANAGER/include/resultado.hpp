#ifndef RESULTADO_H
#define RESULTADO_H

//#include "piloto.hpp"
// esperar a implementacao completa de piloto
class Piloto {};

class Resultado {
    private:
        Piloto _piloto;
        int _colocacao;
        double _tempo;
        bool _completouCorrida;
        int _pontos;

    public:
        Resultado(Piloto piloto, int colocacao, int tempo, bool completouCorrida, int pontos);
        ~Resultado();

        Piloto getPiloto();
        void setPiloto(Piloto piloto);

        int getColocacao() const;
        void setColocacao(int colocacao);

        double getTempo();
        void setTempo(double tempo);

        bool getCompletouCorrida();
        void setCompletouCorrida(bool completou);

        int getPontos();
        void setPontos(int pontos);
};

#endif