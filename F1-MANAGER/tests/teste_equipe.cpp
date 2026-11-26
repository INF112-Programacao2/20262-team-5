
#include "EquipeControlavel.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    Carro w17(
        "W17",
        2026,
        110.0,
        f1::Motor(80),
        f1::Freio(75),
        f1::Suspensao(70),
        f1::Chassi(78)
    );

    EquipeControlavel mercedes(
        "Mercedes",
        1000000.0
    );

    mercedes.cadastrarPiloto(
        Piloto(
            "George Russell",
            "63",
            28,
            w17,
            12,
            10000,
            0,
            90,
            88
        )
    );

    Piloto* george = mercedes.buscarPiloto("63");
    assert(george != nullptr);

    double antes =
        george->getCarro().calcularDesempenho();

    // Eu verifico se a melhoria realmente altera o carro.
    bool comprou = mercedes.melhorarCarro(
        "63",
        "motor",
        5.0,
        250000.0,
        "26/11/2026"
    );

    assert(comprou);
    assert(
        mercedes.getFinanceiro().getSaldo() ==
        750000.0
    );

    assert(
        george->getCarro().calcularDesempenho() >
        antes
    );

    // Eu verifico se o saldo insuficiente bloqueia a compra.
    bool segundaCompra = mercedes.melhorarCarro(
        "63",
        "freio",
        5.0,
        900000.0,
        "26/11/2026"
    );

    assert(!segundaCompra);

    assert(
        mercedes.getFinanceiro().getSaldo() ==
        750000.0
    );

    // Eu cadastro outro piloto para conferir os enderecos.
    mercedes.cadastrarPiloto(
        Piloto(
            "Kimi Antonelli",
            "12",
            20,
            w17,
            12,
            9000,
            0,
            85,
            86
        )
    );

    assert(mercedes.quantidadePilotos() == 2);
    assert(mercedes.buscarPiloto("63") == george);
    assert(mercedes.folhaSalarialMensal() == 19000.0);

    // Eu verifico se componentes invalidos sao rejeitados.
    bool bloqueou = false;

    try {
        mercedes.melhorarCarro(
            "12",
            "banana",
            5.0,
            10.0,
            "26/11/2026"
        );
    }
    catch (const std::invalid_argument&) {
        bloqueou = true;
    }

    assert(bloqueou);

    george->setPontos(25);
    mercedes.encerrarTemporada();

    assert(george->getPontos() == 0);

    std::cout << "TESTE EQUIPE: OK\n";

    return 0;
}
