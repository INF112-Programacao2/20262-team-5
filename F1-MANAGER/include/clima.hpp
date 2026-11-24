
#ifndef CLIMA_HPP
#define CLIMA_HPP

#include <stdexcept>

// Representa as condicoes climaticas da corrida.
// Os valores sao compativeis com a classe Corrida original.
enum class Clima : int {
    SOL = 1,
    NUBLADO = 2,
    CHUVA = 3
};

// Converte o valor inteiro utilizado em Corrida para Clima.
// Impede que valores invalidos entrem no simulador.
inline Clima converterClima(int valor) {
    switch (valor) {
        case 1:
            return Clima::SOL;

        case 2:
            return Clima::NUBLADO;

        case 3:
            return Clima::CHUVA;

        default:
            throw std::invalid_argument(
                "Clima invalido: use 1, 2 ou 3."
            );
    }
}

#endif
