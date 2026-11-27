
#include "GestorEtapa.hpp"
#include "EquipeControlavel.hpp"
#include "EquipeAdversaria.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

static Carro criarCarro(double qualidade) {
    return Carro(
        "W17",
        2026,
        100.0,
        f1::Motor(qualidade),
        f1::Freio(qualidade),
        f1::Suspensao(qualidade),
        f1::Chassi(qualidade)
    );
}

static double tempoPiloto(
    const Corrida& corrida,
    const std::string& id
) {
    for (const Resultado& resultado : corrida.getResultados()) {
        if (resultado.getPiloto().getId() == id) {
            return resultado.getTempo();
        }
    }

    throw std::logic_error(
        "Piloto nao encontrado no resultado."
    );
}

int main() {
    EquipeControlavel mercedes(
        "Mercedes",
        1000000.0
    );

    EquipeAdversaria mclaren(
        "McLaren",
        85,
        20
    );

    const Contrato contrato(
        12,
        1000.0,
        0.0
    );

    mercedes.cadastrarPiloto(
        Piloto(
            "George Russell",
            "63",
            28,
            criarCarro(85),
            12,
            1000.0,
            0.0,
            90,
            90
        )
    );

    mclaren.cadastrarPiloto(
        Piloto(
            "Piloto adversario",
            "99",
            28,
            criarCarro(85),
            12,
            1000.0,
            0.0,
            90,
            90
        )
    );

    mercedes.cadastrarFuncionario(
        std::make_unique<Estrategista>(
            "Estrategista",
            "S1",
            40,
            contrato,
            5,
            12
        )
    );

    mercedes.cadastrarFuncionario(
        std::make_unique<ChefeEquipe>(
            "Chefe",
            "C1",
            55,
            contrato,
            5,
            15
        )
    );

    const double bonus =
        mercedes.calcularBonusEstrategia();

    assert(bonus > 0.0 && bonus <= 3.0);

    Circuito pista(
        "Interlagos",
        "Brasil",
        4.309,
        15
    );

    Corrida gp(
        "2026-11-26",
        pista,
        1,
        20
    );

    Piloto* russell =
        mercedes.buscarPiloto("63");

    Piloto* adversario =
        mclaren.buscarPiloto("99");

    assert(russell != nullptr);
    assert(adversario != nullptr);

    gp.adicionarPiloto(*russell);
    gp.adicionarPiloto(*adversario);

    ConfiguracaoSimulacao config;

    config.variacaoVolta = 0.0;
    config.probabilidadeFalhaBase = 0.0;

    SimuladorDesempenho simulador(
        42,
        config
    );

    const double condicaoInicial =
        russell->getCarro().getCondicao();

    const double combustivelInicial =
        russell->getCarro().getCombustivel();

    GestorEtapa::executar(
        gp,
        simulador,
        {&mercedes, &mclaren}
    );

    // Verifica a finalizacao e os resultados.
    assert(
        gp.getEstado() == EstadoCorrida::FINALIZADA
    );

    assert(gp.getResultados().size() == 2);

    // A IA adversaria prepara sua estrategia.
    assert(mclaren.getBonusEstrategia() == 1.0);

    // Pilotos equivalentes: a diferenca vem da estrategia.
    assert(
        tempoPiloto(gp, "63") <
        tempoPiloto(gp, "99")
    );

    // O desgaste afeta o carro cadastrado na equipe.
    assert(
        russell->getCarro().getCondicao() <
        condicaoInicial
    );

    assert(
        russell->getCarro().getCondicao() >= 0.0
    );

    // O simulador usa uma copia temporaria do combustivel.
    assert(
        russell->getCarro().getCombustivel() ==
        combustivelInicial
    );

    const double condicaoDepois =
        russell->getCarro().getCondicao();

    // A mesma corrida nao pode ser executada novamente.
    bool repetida = false;

    try {
        GestorEtapa::executar(
            gp,
            simulador,
            {&mercedes, &mclaren}
        );
    }
    catch (const std::logic_error&) {
        repetida = true;
    }

    assert(repetida);

    assert(
        russell->getCarro().getCondicao() ==
        condicaoDepois
    );

    // O desgaste permanece acumulado para o proximo GP.
    Corrida gp2(
        "2026-12-10",
        pista,
        3,
        20
    );

    gp2.adicionarPiloto(*russell);
    gp2.adicionarPiloto(*adversario);

    GestorEtapa::executar(
        gp2,
        simulador,
        {&mercedes, &mclaren}
    );

    assert(
        russell->getCarro().getCondicao() <
        condicaoDepois
    );

    // Valida a ausencia de uma equipe.
    Corrida gp3(
        "2026-12-20",
        pista,
        1,
        2
    );

    gp3.adicionarPiloto(*russell);
    gp3.adicionarPiloto(*adversario);

    bool semEquipe = false;

    try {
        GestorEtapa::executar(
            gp3,
            simulador,
            {&mercedes}
        );
    }
    catch (const std::invalid_argument&) {
        semEquipe = true;
    }

    assert(semEquipe);

    assert(
        gp3.getEstado() == EstadoCorrida::NAO_INICIADA
    );

    std::cout
        << "TESTE GESTOR ETAPA: OK\n";

    return 0;
}
