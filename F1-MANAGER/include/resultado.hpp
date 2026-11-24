
#ifndef RESULTADO_H
#define RESULTADO_H

#include <string>

// A classe Piloto real fica em piloto.hpp.
// Declaracao antecipada evita inclusoes circulares.
class Piloto;

class Resultado {
private:
    Piloto* _piloto; // Ponteiro nao-dono: o piloto precisa continuar existindo.
    int _colocacao;
    double _tempo; // Tempo acumulado em segundos.
    bool _completouCorrida;
    int _pontos;
    int _voltasConcluidas;
    std::string _ocorrencia;
    bool _desclassificado;

public:
    Resultado(Piloto& piloto, int colocacao, double tempo,
              bool completouCorrida, int pontos,
              int voltasConcluidas = 0,
              std::string ocorrencia = "",
              bool desclassificado = false);

    ~Resultado() = default;

    // Getters e setters
    const Piloto& getPiloto() const;
    void setPiloto(Piloto& piloto);

    int getColocacao() const;
    void setColocacao(int colocacao);

    double getTempo() const;
    void setTempo(double tempo);

    bool getCompletouCorrida() const;
    void setCompletouCorrida(bool completou);

    int getPontos() const;
    void setPontos(int pontos);

    // Informacoes adicionais para o simulador.
    int getVoltasConcluidas() const;
    const std::string& getOcorrencia() const;
    bool getDesclassificado() const;
};

#endif
