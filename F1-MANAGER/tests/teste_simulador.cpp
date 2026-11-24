
#include "SimuladorDesempenho.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

// Cria um carro para os testes.
Carro criarCarro(
    double qualidade,
    double combustivel = 100.0
) {
    return Carro(
        "W17",
        2026,
        combustivel,
        f1::Motor(qualidade),
        f1::Freio(qualidade),
        f1::Suspensao(qualidade),
        f1::Chassi(qualidade)
    );
}

// Executa uma corrida e devolve seus tempos.
// Isso permite verificar se a semente reproduz os resultados.
std::vector<double> executarTeste(
    unsigned semente,
    bool exibir
) {
    Piloto russell(
        "George Russell", "63", 28,
        criarCarro(94),
        12, 100000, 500000,
        94, 91
    );

    Piloto antonelli(
        "Kimi Antonelli", "12", 20,
        criarCarro(91),
        12, 100000, 500000,
        91, 88
    );

    Piloto adversario(
        "Rival automatico", "99", 26,
        criarCarro(90),
        12, 100000, 500000,
        89, 90
    );

    Circuito pista(
        "Interlagos",
        "Brasil",
        4.309,
        15
    );

    Corrida gp(
        "2026-11-24",
        pista,
        3,
        20
    );

    gp.adicionarPiloto(russell);
    gp.adicionarPiloto(antonelli);
    gp.adicionarPiloto(adversario);

    const double combustivelInicial =
        russell.getCarro().getCombustivel();

    SimuladorDesempenho motor(semente);

    motor.simularCorrida(gp);

    // Confere estado e quantidade de resultados.
    assert(
        gp.getEstado() == EstadoCorrida::FINALIZADA
    );

    assert(gp.getResultados().size() == 3);

    // A simulacao nao deve consumir o carro cadastrado.
    assert(
        russell.getCarro().getCombustivel() ==
        combustivelInicial
    );

    int pontosDistribuidos = 0;
    std::vector<double> tempos;

    for (const Resultado& r : gp.getResultados()) {
        tempos.push_back(r.getTempo());

        pontosDistribuidos += r.getPontos();

        assert(
            r.getVoltasConcluidas() <= gp.getQtdVoltas()
        );

        assert(r.getTempo() >= 0.0);

        if (r.getCompletouCorrida()) {
            assert(r.getVoltasConcluidas() == 20);
        }
    }

    assert(pontosDistribuidos <= 25 + 18 + 15);

    // Uma corrida finalizada nao pode ser executada novamente.
    bool bloqueou = false;

    try {
        motor.simularCorrida(gp);
    }
    catch (const std::logic_error&) {
        bloqueou = true;
    }

    assert(bloqueou);

    if (exibir) {
        gp.exibirResultados();
    }

    return tempos;
}

int main() {
    // Mesmos dados + mesma semente = mesmos resultados
    // neste ambiente de testes.
    assert(
        executarTeste(42, true) ==
        executarTeste(42, false)
    );

    // Verifica abandono por falta de combustivel.
    Piloto a(
        "A", "1", 25,
        criarCarro(95, 1),
        12, 1000, 5000
    );

    Piloto b(
        "B", "2", 25,
        criarCarro(95, 100),
        12, 1000, 5000
    );

    Circuito pista(
        "Teste",
        "Brasil",
        4.3,
        12
    );

    Corrida gp(
        "2026-11-24",
        pista,
        1,
        5
    );

    gp.adicionarPiloto(a);
    gp.adicionarPiloto(b);

    ConfiguracaoSimulacao config;

    config.probabilidadeFalhaBase = 0;
    config.variacaoVolta = 0;

    SimuladorDesempenho motor(7, config);

    motor.simularCorrida(gp);

    // O piloto A nao tinha combustivel suficiente.
    assert(
        !gp.getResultado(1).getCompletouCorrida()
    );

    assert(
        gp.getResultado(1).getPiloto().getId() == "1"
    );

    assert(gp.getResultado(1).getPontos() == 0);

    // Seu carro cadastrado permanece intacto.
    assert(a.getCarro().getCombustivel() == 1.0);

    assert(b.getPontos() == 25);

    std::cout << "TESTE ETAPA 3: OK\n";

    return 0;
}
