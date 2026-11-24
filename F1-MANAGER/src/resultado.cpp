
#include "resultado.hpp"
#include "piloto.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

Resultado::Resultado(Piloto& piloto, int colocacao, double tempo,
                     bool completouCorrida, int pontos,
                     int voltasConcluidas, std::string ocorrencia,
                     bool desclassificado)
    : _piloto(&piloto),
      _colocacao(colocacao),
      _tempo(tempo),
      _completouCorrida(completouCorrida),
      _pontos(pontos),
      _voltasConcluidas(voltasConcluidas),
      _ocorrencia(std::move(ocorrencia)),
      _desclassificado(desclassificado) {

    // Evita armazenar valores impossiveis nos resultados.
    if (colocacao < 0 || pontos < 0 || voltasConcluidas < 0 ||
        !std::isfinite(tempo) || tempo < 0.0) {
        throw std::invalid_argument(
            "Dados do resultado invalidos."
        );
    }
}

// Getters e setters
const Piloto& Resultado::getPiloto() const {
    return *_piloto;
}

void Resultado::setPiloto(Piloto& piloto) {
    _piloto = &piloto;
}

int Resultado::getColocacao() const {
    return _colocacao;
}

void Resultado::setColocacao(int colocacao) {
    if (colocacao < 0) {
        throw std::invalid_argument("Colocacao invalida.");
    }
    _colocacao = colocacao;
}

double Resultado::getTempo() const {
    return _tempo;
}

void Resultado::setTempo(double tempo) {
    if (!std::isfinite(tempo) || tempo < 0.0) {
        throw std::invalid_argument("Tempo invalido.");
    }
    _tempo = tempo;
}

bool Resultado::getCompletouCorrida() const {
    return _completouCorrida;
}

void Resultado::setCompletouCorrida(bool completou) {
    _completouCorrida = completou;
}

int Resultado::getPontos() const {
    return _pontos;
}

void Resultado::setPontos(int pontos) {
    if (pontos < 0) {
        throw std::invalid_argument("Pontos invalidos.");
    }
    _pontos = pontos;
}

// Informacoes adicionais usadas pelo motor de simulacao.
int Resultado::getVoltasConcluidas() const {
    return _voltasConcluidas;
}

const std::string& Resultado::getOcorrencia() const {
    return _ocorrencia;
}

bool Resultado::getDesclassificado() const {
    return _desclassificado;
}
