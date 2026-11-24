
#ifndef SIMULADOR_DESEMPENHO_HPP
#define SIMULADOR_DESEMPENHO_HPP

#include "clima.hpp"
#include "corrida.hpp"
#include "carro.hpp"
#include "piloto.hpp"

#include <cstdint>
#include <random>

// Parametros de simulacao.
// Sao regras do jogo, nao dados oficiais da Formula 1.
struct ConfiguracaoSimulacao {

    // Desvio proporcional dos tempos de cada volta.
    double variacaoVolta = 0.007;

    // Chance de falha por volta para carro com qualidade 100.
    double probabilidadeFalhaBase = 0.0004;

    // Unidade didatica de combustivel consumida por km.
    double consumoPorKm = 0.25;
};

class SimuladorDesempenho {
private:
    // Gerador de numeros pseudoaleatorios.
    // Com uma semente fixa, podemos reproduzir os testes.
    std::mt19937 _gerador;

    ConfiguracaoSimulacao _config;

public:
    explicit SimuladorDesempenho(
        std::uint32_t semente = std::random_device{}(),
        ConfiguracaoSimulacao config = {}
    );

    // Retorna um indice de desempenho de 0 a 100.
    // Nao altera o piloto ou o carro.
    double calcularDesempenho(
        const Piloto& piloto,
        const Carro& carro,
        Clima clima
    ) const;

    // Executa o GP e registra classificacao e pontuacao.
    // A corrida deve estar no estado NAO_INICIADA.
    void simularCorrida(Corrida& corrida);
};

#endif
