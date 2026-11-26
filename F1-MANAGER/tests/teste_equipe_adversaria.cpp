
#include "EquipeAdversaria.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    Carro carro(
        "MCL40",
        2026,
        110.0,
        f1::Motor(87),
        f1::Freio(82),
        f1::Suspensao(85),
        f1::Chassi(83)
    );

    EquipeAdversaria mclaren(
        "McLaren",
        90,
        80
    );

    assert(mclaren.quantidadePilotos() == 0);
    assert(mclaren.calcularForcaEquipe() == 0.0);

    mclaren.cadastrarPiloto(
        Piloto(
            "Lando Norris",
            "4",
            27,
            carro,
            12,
            50000.0,
            0.0,
            92.0,
            90.0
        )
    );

    mclaren.cadastrarPiloto(
        Piloto(
            "Oscar Piastri",
            "81",
            25,
            carro,
            12,
            50000.0,
            0.0,
            90.0,
            89.0
        )
    );

    assert(mclaren.quantidadePilotos() == 2);
    assert(mclaren.buscarPiloto("4") != nullptr);

    const double forcaAntes =
        mclaren.calcularForcaEquipe();

    mclaren.executarEstrategia();

    const double forcaDepois =
        mclaren.calcularForcaEquipe();

    // Verifica se o bonus estrategico foi calculado corretamente.
    assert(
        std::abs(
            mclaren.getBonusEstrategia() - 4.0
        ) < 1e-9
    );

    assert(
        std::abs(
            forcaDepois - forcaAntes - 4.0
        ) < 1e-9
    );

    // Garante que o bonus nao seja acumulado.
    mclaren.executarEstrategia();

    assert(
        std::abs(
            mclaren.calcularForcaEquipe() - forcaDepois
        ) < 1e-9
    );

    // Verifica o tratamento de atributos invalidos.
    bool bloqueou = false;

    try {
        EquipeAdversaria invalida(
            "Equipe",
            120,
            60
        );
    }
    catch (const std::invalid_argument&) {
        bloqueou = true;
    }

    assert(bloqueou);

    std::cout
        << "TESTE EQUIPE ADVERSARIA: OK\n";

    return 0;
}
