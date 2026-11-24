
#include "carro.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

Carro::Carro(
    std::string modelo,
    int ano,
    double combustivel,
    f1::Motor motor,
    f1::Freio freio,
    f1::Suspensao suspensao,
    f1::Chassi chassi
)
    : _modelo(std::move(modelo)),
      _ano(ano),
      _combustivel(combustivel),
      _motor(std::move(motor)),
      _freio(std::move(freio)),
      _suspensao(std::move(suspensao)),
      _chassi(std::move(chassi)) {

    if (_modelo.empty() || _ano < 1950 ||
        !std::isfinite(_combustivel) ||
        _combustivel < 0.0) {

        throw std::invalid_argument(
            "Dados do carro invalidos."
        );
    }
}

double Carro::calcularDesempenho() const {

    // Pesos didaticos, configuraveis futuramente para cada GP.
    return 0.35 * _motor.calcularDesempenho()
         + 0.25 * _freio.calcularDesempenho()
         + 0.20 * _suspensao.calcularDesempenho()
         + 0.20 * _chassi.calcularDesempenho();
}

bool Carro::consumirCombustivel(double quantidade) {

    if (!std::isfinite(quantidade) || quantidade < 0.0) {
        throw std::invalid_argument(
            "Consumo de combustivel invalido."
        );
    }

    if (quantidade > _combustivel) {
        return false;
    }

    _combustivel -= quantidade;
    return true;
}

void Carro::melhorarComponente(
    const std::string& nome,
    double incremento
) {
    if (nome == "motor" || nome == "Motor") {
        _motor.melhorar(incremento);
    }
    else if (nome == "freio" || nome == "Freio") {
        _freio.melhorar(incremento);
    }
    else if (nome == "suspensao" || nome == "Suspensao") {
        _suspensao.melhorar(incremento);
    }
    else if (nome == "chassi" || nome == "Chassi") {
        _chassi.melhorar(incremento);
    }
    else {
        throw std::invalid_argument(
            "Componente desconhecido: " + nome
        );
    }
}
