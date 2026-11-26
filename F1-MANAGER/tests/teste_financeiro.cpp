
#include "financeiro.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    Financeiro mercedes(1000000.0);

    // Verifica o saldo inicial.
    assert(mercedes.getSaldo() == 1000000.0);

    // Compra uma melhoria de R$ 250.000.
    bool comprou = mercedes.removerFundos(
        250000.0,
        "26/11/2026",
        "Melhoria do motor"
    );

    assert(comprou);
    assert(mercedes.getSaldo() == 750000.0);
    assert(mercedes.getDespesasTotais() == 250000.0);

    // Tenta comprar algo sem dinheiro suficiente.
    bool segundaCompra = mercedes.removerFundos(
        900000.0,
        "26/11/2026",
        "Nova melhoria"
    );

    assert(!segundaCompra);
    assert(mercedes.getSaldo() == 750000.0);

    // Recebimento de patrocinio.
    mercedes.adicionarFundos(
        200000.0,
        "26/11/2026",
        "Patrocinio"
    );

    assert(mercedes.getSaldo() == 950000.0);
    assert(mercedes.getReceitasTotais() == 200000.0);

    // Apenas duas operacoes foram realizadas.
    assert(mercedes.quantidadeTransacoes() == 2);

    // Despesas negativas nao podem ser realizadas.
    bool bloqueou = false;

    try {
        mercedes.removerFundos(
            -100.0,
            "26/11/2026",
            "Operacao invalida"
        );
    }
    catch (const std::invalid_argument&) {
        bloqueou = true;
    }

    assert(bloqueou);

    mercedes.exibirHistoricoDeTransacoes();

    std::cout << "TESTE FINANCEIRO: OK\n";

    return 0;
}
