
#include "EquipeControlavel.hpp"
#include "EquipeAdversaria.hpp"
#include "funcionario.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    Carro w17(
        "W17", 2026, 110.0,
        f1::Motor(80),
        f1::Freio(75),
        f1::Suspensao(70),
        f1::Chassi(78)
    );

    EquipeControlavel mercedes("Mercedes", 1000000.0);

    mercedes.cadastrarPiloto(
        Piloto("George Russell", "63", 28, w17,
               12, 10000, 0, 90, 88)
    );

    mercedes.cadastrarPiloto(
        Piloto("Kimi Antonelli", "12", 20, w17,
               12, 9000, 0, 85, 86)
    );

    const Contrato contratoEngenheiro(
        12, 20000.0, 5000.0
    );

    const Contrato contratoMecanico(
        12, 12000.0, 2000.0
    );

    // Contratacao de funcionarios.
    mercedes.cadastrarFuncionario(
        std::make_unique<Engenheiro>(
            "Ana", "E1", 30,
            contratoEngenheiro, 5, "Aerodinamica"
        )
    );

    mercedes.cadastrarFuncionario(
        std::make_unique<Mecanico>(
            "Bruno", "M1", 27,
            contratoMecanico, 4, 90
        )
    );

    assert(mercedes.quantidadePilotos() == 2);
    assert(mercedes.quantidadeFuncionarios() == 2);

    // Consulta de funcionarios pelo ID.
    assert(mercedes.buscarFuncionario("E1") != nullptr);
    assert(mercedes.buscarFuncionario("NAO_EXISTE") == nullptr);

    assert(
        mercedes.buscarFuncionario("M1")->getTipo()
        == "Mecanico"
    );

    // Salarios: 10000 + 9000 + 20000 + 12000.
    assert(mercedes.folhaSalarialMensal() == 51000.0);

    // Soma dos impactos individuais.
    assert(
        std::abs(
            mercedes.calcularImpactoFuncionarios() - 5.5
        ) < 1e-9
    );

    // Verifica a rejeicao de IDs duplicados.
    bool duplicado = false;

    try {
        mercedes.cadastrarFuncionario(
            std::make_unique<Estrategista>(
                "Carla", "E1", 38,
                contratoMecanico, 6, 12
            )
        );
    }
    catch (const std::invalid_argument&) {
        duplicado = true;
    }

    assert(duplicado);

    // Um piloto tambem nao pode repetir o ID de um funcionario.
    duplicado = false;

    try {
        mercedes.cadastrarPiloto(
            Piloto(
                "Duplicado", "E1", 21, w17,
                12, 1000, 0, 70, 70
            )
        );
    }
    catch (const std::invalid_argument&) {
        duplicado = true;
    }

    assert(duplicado);

    // Remocao e atualizacao da folha salarial.
    mercedes.removerFuncionario("M1");

    assert(mercedes.quantidadeFuncionarios() == 1);
    assert(mercedes.folhaSalarialMensal() == 39000.0);
    assert(mercedes.buscarFuncionario("M1") == nullptr);

    // Teste de consulta constante pela classe base.
    const Equipe& visualizacao = mercedes;

    assert(
        visualizacao.buscarFuncionario("E1") != nullptr
    );

    // Remover um funcionario inexistente deve gerar excecao.
    bool naoEncontrado = false;

    try {
        mercedes.removerFuncionario("M1");
    }
    catch (const std::invalid_argument&) {
        naoEncontrado = true;
    }

    assert(naoEncontrado);

    // As equipes adversarias tambem podem ter funcionarios.
    EquipeAdversaria mclaren("McLaren", 90, 80);

    mclaren.cadastrarFuncionario(
        std::make_unique<ChefeEquipe>(
            "Daniel", "C1", 45,
            contratoMecanico, 10, 15
        )
    );

    assert(mclaren.quantidadeFuncionarios() == 1);
    assert(mclaren.folhaSalarialMensal() == 12000.0);

    std::cout
        << "TESTE INTEGRACAO EQUIPE-FUNCIONARIOS: OK\n";

    return 0;
}
