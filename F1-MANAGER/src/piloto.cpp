
#include "piloto.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

Piloto::Piloto(
    std::string nome,
    std::string id,
    int idade,
    const Carro& carro,
    int duracao,
    double salario,
    double rescisao,
    double habilidade,
    double consistencia
)
    : Pessoa(std::move(nome), std::move(id), idade),
      _carro(carro),
      _contrato(duracao, salario, rescisao),
      _habilidade(habilidade),
      _consistencia(consistencia) {

    if (idade < 18) {
        throw std::invalid_argument(
            "O piloto precisa ter pelo menos 18 anos."
        );
    }

    if (!std::isfinite(_habilidade) ||
        !std::isfinite(_consistencia) ||
        _habilidade < 0.0 || _habilidade > 100.0 ||
        _consistencia < 0.0 || _consistencia > 100.0) {

        throw std::invalid_argument(
            "Habilidade e consistencia devem estar entre 0 e 100."
        );
    }
}

//getters
const Carro& Piloto::getCarro() const {
    return _carro;
}

Carro& Piloto::getCarro() {
    return _carro;
}

const Contrato& Piloto::getContrato() const {
    return _contrato;
}

int Piloto::getTitulos() const {
    return _titulos;
}

int Piloto::getVitorias() const {
    return _vitorias;
}

int Piloto::getPontos() const {
    return _pontos;
}

//setters
void Piloto::setTitulos(int titulos) {
    if (titulos < 0) {
        throw std::invalid_argument(
            "Titulos nao podem ser negativos."
        );
    }

    _titulos = titulos;
}

void Piloto::setVitorias(int vitorias) {
    if (vitorias < 0) {
        throw std::invalid_argument(
            "Vitorias nao podem ser negativas."
        );
    }

    _vitorias = vitorias;
}

void Piloto::setPontos(int pontos) {
    if (pontos < 0) {
        throw std::invalid_argument(
            "Pontos nao podem ser negativos."
        );
    }

    _pontos = pontos;
}

//outras funcoes
void Piloto::registrarResultado(
    int pontos,
    bool vitoria
) {
    if (pontos < 0) {
        throw std::invalid_argument(
            "Pontos nao podem ser negativos."
        );
    }

    // Impede overflow na pontuacao acumulada.
    if (pontos > std::numeric_limits<int>::max() - _pontos) {
        throw std::overflow_error(
            "Pontuacao acumulada excede o limite."
        );
    }

    if (vitoria &&
        _vitorias == std::numeric_limits<int>::max()) {
        throw std::overflow_error(
            "Contagem de vitorias excede o limite."
        );
    }

    _pontos += pontos;

    if (vitoria) {
        ++_vitorias;
    }
}

void Piloto::zerarTemporada() {
    _pontos = 0;

    // Vitorias e titulos sao estatisticas de carreira,
    // nao da temporada.
}
