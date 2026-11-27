
#include "EquipeControlavel.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    Carro w17(
        "W17", 2026, 110.0,
        f1::Motor(70),
        f1::Freio(70),
        f1::Suspensao(70),
        f1::Chassi(70)
    );

    EquipeControlavel mercedes(
        "Mercedes", 50000.0
    );

    mercedes.cadastrarPiloto(
        Piloto(
            "George Russell", "63", 28, w17,
            12, 10000.0, 0.0, 90, 88
        )
    );

    const Contrato contratoEng(
        12, 20000.0, 0.0
    );

    const Contrato contratoMec(
        12, 12000.0, 0.0
    );

    mercedes.cadastrarFuncionario(
        std::make_unique<Engenheiro>(
            "Ana", "E1", 30,
            contratoEng, 5, "Aerodinamica"
        )
    );

    mercedes.cadastrarFuncionario(
        std::make_unique<Mecanico>(
            "Bruno", "M1", 27,
            contratoMec, 4, 90
        )
    );

    // Verifica o valor total da folha salarial.
    assert(
        mercedes.folhaSalarialMensal() == 42000.0
    );

    // Pagamento de novembro.
    assert(mercedes.pagarFolhaMensal("2026-11"));
    assert(mercedes.folhaJaPaga("2026-11"));

    assert(
        mercedes.getFinanceiro().getSaldo() == 8000.0
    );

    assert(
        mercedes.getFinanceiro().quantidadeTransacoes() == 1
    );

    // O mesmo mes nao pode ser cobrado novamente.
    assert(!mercedes.pagarFolhaMensal("2026-11"));

    assert(
        mercedes.getFinanceiro().getSaldo() == 8000.0
    );

    // Caixa insuficiente nao registra pagamento nem transacao.
    assert(!mercedes.pagarFolhaMensal("2026-12"));
    assert(!mercedes.folhaJaPaga("2026-12"));

    assert(
        mercedes.getFinanceiro().quantidadeTransacoes() == 1
    );

    // Entrada de patrocinio para quitar dezembro.
    mercedes.getFinanceiro().adicionarFundos(
        50000.0,
        "2026-12-01",
        "Patrocinio"
    );

    assert(mercedes.pagarFolhaMensal("2026-12"));

    assert(
        mercedes.getFinanceiro().getSaldo() == 16000.0
    );

    // Compara a melhoria de uma equipe com engenheiro
    // com a de outra equipe sem engenheiro.
    EquipeControlavel semEngenheiro(
        "Equipe de controle", 50000.0
    );

    semEngenheiro.cadastrarPiloto(
        Piloto(
            "Teste", "T1", 25, w17,
            12, 10000.0, 0.0, 90, 88
        )
    );

    const double desempenhoInicial =
        w17.calcularDesempenho();

    assert(
        mercedes.melhorarCarro(
            "63", "motor", 5.0,
            1000.0, "2026-12-03"
        )
    );

    assert(
        semEngenheiro.melhorarCarro(
            "T1", "motor", 5.0,
            1000.0, "2026-12-03"
        )
    );

    const double ganhoMercedes =
        mercedes.buscarPiloto("63")
            ->getCarro().calcularDesempenho()
        - desempenhoInicial;

    const double ganhoSemEngenheiro =
        semEngenheiro.buscarPiloto("T1")
            ->getCarro().calcularDesempenho()
        - desempenhoInicial;

    assert(ganhoMercedes > ganhoSemEngenheiro);

    // Valores invalidos nao podem gerar cobranca.
    const double saldoAntes =
        mercedes.getFinanceiro().getSaldo();

    bool melhoriaInvalida = false;

    try {
        mercedes.melhorarCarro(
            "63", "motor", 0.0,
            1000.0, "2026-12-04"
        );
    }
    catch (const std::invalid_argument&) {
        melhoriaInvalida = true;
    }

    assert(melhoriaInvalida);

    assert(
        mercedes.getFinanceiro().getSaldo() == saldoAntes
    );

    // Competencias invalidas devem ser rejeitadas.
    bool dataInvalida = false;

    try {
        mercedes.pagarFolhaMensal("2026-13");
    }
    catch (const std::invalid_argument&) {
        dataInvalida = true;
    }

    assert(dataInvalida);

    // Equipes sem pilotos ou funcionarios nao possuem folha.
    EquipeControlavel vazia(
        "Vazia", 100.0
    );

    assert(!vazia.pagarFolhaMensal("2026-11"));

    std::cout << "TESTE GESTAO MERCEDES: OK\n";

    return 0;
}
