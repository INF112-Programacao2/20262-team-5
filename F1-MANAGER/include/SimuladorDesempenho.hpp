
#pragma once

#include "carro.hpp"
#include "clima.hpp"
#include "corrida.hpp"
#include "piloto.hpp"

#include <cstdint>
#include <random>

namespace f1 {

struct ConfiguracaoSimulacao {
    double variacaoVolta{0.006};

    double probabilidadeFalhaBase{0.0003};

    double falhaPorPontoDeDesgaste{0.00001};

    double consumoPorKm{0.25};
};

class SimuladorDesempenho {
public:
    explicit SimuladorDesempenho(
        std::uint32_t semente = std::random_device{}(),
        ConfiguracaoSimulacao config = {}
    );

    double calcularDesempenho(
        const Piloto& piloto,
        const Carro& carro,
        Clima clima
    ) const;

    void simularCorrida(Corrida& corrida);

private:
    std::mt19937 gerador_;
    ConfiguracaoSimulacao config_;
};

} // namespace f1
