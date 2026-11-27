
#include "EquipeControlavel.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    Carro w17(
        "W17", 2026, 110.0,
        f1::Motor(80),
        f1::Freio(80),
        f1::Suspensao(80),
        f1::Chassi(80)
    );

    EquipeControlavel mercedes(
        "Mercedes", 15000.0
    );

    mercedes.cadastrarPiloto(
        Piloto(
            "George Russell", "63", 28, w17,
            12, 1000.0, 0.0, 91, 90
        )
    );

    Piloto* russell = mercedes.buscarPiloto("63");

    assert(russell != nullptr);

    // Verifica a perda de desempenho causada pelo desgaste.
    const double desempenhoInicial =
        russell->getCarro().calcularDesempenho();

    russell->getCarro().desgastar(40.0);

    assert(russell->getCarro().getCondicao() == 60.0);
    assert(
        russell->getCarro().calcularDesempenho()
        < desempenhoInicial
    );

    // Contrata um mecanico com habilidade 90.
    Contrato contrato(12, 2000.0, 0.0);

    mercedes.cadastrarFuncionario(
        std::make_unique<Mecanico>(
            "Bruno", "M1", 30,
            contrato, 4, 90
        )
    );

    // Impacto do mecanico: 4 pontos.
    // Bonus na recuperacao: 20%.
    const double saldoAntes =
        mercedes.getFinanceiro().getSaldo();

    assert(
        mercedes.realizarManutencao(
            "63", 20.0, 3000.0, "2026-11-26"
        )
    );

    assert(
        std::abs(
            russell->getCarro().getCondicao() - 84.0
        ) < 1e-9
    );

    assert(
        mercedes.getFinanceiro().getSaldo()
        == saldoAntes - 3000.0
    );

    // O desempenho melhora apos a manutencao.
    assert(
        russell->getCarro().calcularDesempenho()
        > desempenhoInicial * 0.88
    );

    // Saldo insuficiente nao pode alterar o carro.
    const double condicaoAntes =
        russell->getCarro().getCondicao();

    assert(
        !mercedes.realizarManutencao(
            "63", 10.0, 999999.0, "2026-11-27"
        )
    );

    assert(
        russell->getCarro().getCondicao()
        == condicaoAntes
    );

    // Recupera a condicao maxima.
    assert(
        mercedes.realizarManutencao(
            "63", 20.0, 1000.0, "2026-11-28"
        )
    );

    assert(russell->getCarro().getCondicao() == 100.0);

    // Manutencoes desnecessarias nao geram despesas.
    const double saldoFinal =
        mercedes.getFinanceiro().getSaldo();

    assert(
        !mercedes.realizarManutencao(
            "63", 10.0, 1000.0, "2026-11-29"
        )
    );

    assert(
        mercedes.getFinanceiro().getSaldo()
        == saldoFinal
    );

    // Valores invalidos sao rejeitados.
    bool rejeitou = false;

    try {
        mercedes.realizarManutencao(
            "63", -5.0, 1000.0, "2026-11-30"
        );
    }
    catch (const std::invalid_argument&) {
        rejeitou = true;
    }

    assert(rejeitou);

    std::cout << "TESTE MANUTENCAO: OK\n";

    return 0;
}
