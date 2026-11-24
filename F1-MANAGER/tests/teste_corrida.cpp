
#include "carro.hpp"
#include "circuito.hpp"
#include "corrida.hpp"
#include "piloto.hpp"
#include "RegraPontuacao.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    // Confere a regra de pontos.
    assert(RegraPontuacao::calcularPontos(1) == 25);
    assert(RegraPontuacao::calcularPontos(2) == 18);
    assert(RegraPontuacao::calcularPontos(11) == 0);

    auto carroMercedes = []() {
        return Carro(
            "W17", 2026, 100.0,
            f1::Motor(92),
            f1::Freio(90),
            f1::Suspensao(91),
            f1::Chassi(93)
        );
    };

    Piloto russell(
        "George Russell", "63", 28,
        carroMercedes(), 12, 100000, 500000
    );

    Piloto antonelli(
        "Kimi Antonelli", "12", 20,
        carroMercedes(), 12, 100000, 500000
    );

    Piloto rival(
        "Adversario", "99", 27,
        carroMercedes(), 12, 100000, 500000
    );

    Circuito pista(
        "Interlagos", "Brasil", 4.309, 15
    );

    Corrida gp(
        "2026-11-24", pista, 3, 20
    );

    // Inscreve os participantes.
    gp.adicionarPiloto(russell);
    gp.adicionarPiloto(antonelli);
    gp.adicionarPiloto(rival);

    gp.iniciar();

    // Resultados de exemplo, definidos manualmente.
    // A simulacao automatica vira na proxima etapa.
    gp.adicionarResultado(
        Resultado(russell, 0, 1800.0, true, 0, 20, "Finalizou")
    );

    gp.adicionarResultado(
        Resultado(antonelli, 0, 1790.0, true, 0, 20, "Finalizou")
    );

    gp.adicionarResultado(
        Resultado(rival, 0, 1200.0, false, 0, 13, "Falha")
    );

    gp.finalizar();

    assert(gp.getEstado() == EstadoCorrida::FINALIZADA);

    assert(gp.getResultados().size() == 3);

    // Antonelli terminou primeiro.
    assert(gp.getResultado(0).getPiloto().getId() == "12");

    assert(antonelli.getPontos() == 25);
    assert(russell.getPontos() == 18);
    assert(rival.getPontos() == 0);

    assert(antonelli.getVitorias() == 1);

    // O abandono deve permanecer no historico.
    assert(gp.getResultado(2).getVoltasConcluidas() == 13);

    // Nao pode finalizar a mesma corrida novamente.
    bool repeticaoBloqueada = false;

    try {
        gp.finalizar();
    }
    catch (const std::logic_error&) {
        repeticaoBloqueada = true;
    }

    assert(repeticaoBloqueada);
    assert(antonelli.getPontos() == 25);

    gp.exibirResultados();

    std::cout << "TESTE ETAPA 2.2: OK\n";

    return 0;
}
