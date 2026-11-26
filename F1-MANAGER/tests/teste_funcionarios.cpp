
#include "funcionario.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

int main() {
    Contrato contrato(12, 20000.0, 5000.0);

    // Teste de heranca e polimorfismo.
    std::vector<std::unique_ptr<Funcionario>> equipe;

    equipe.push_back(
        std::make_unique<Engenheiro>(
            "Ana", "E1", 30,
            contrato, 5, "Aerodinamica"
        )
    );

    equipe.push_back(
        std::make_unique<Mecanico>(
            "Bruno", "M1", 27,
            contrato, 4, 90
        )
    );

    equipe.push_back(
        std::make_unique<Estrategista>(
            "Carla", "S1", 38,
            contrato, 6, 12
        )
    );

    equipe.push_back(
        std::make_unique<ChefeEquipe>(
            "Daniel", "C1", 45,
            contrato, 10, 15
        )
    );

    assert(equipe.size() == 4);

    assert(equipe[0]->getTipo() == "Engenheiro");
    assert(equipe[1]->getTipo() == "Mecanico");
    assert(equipe[2]->getTipo() == "Estrategista");
    assert(equipe[3]->getTipo() == "ChefeEquipe");

    double salarios = 0.0;

    for (const auto& funcionario : equipe) {
        salarios +=
            funcionario->getContrato().getSalario();

        double impacto = funcionario->calcularImpacto();

        assert(impacto >= 0.0);
        assert(impacto <= 5.0);

        std::cout
            << funcionario->getNome()
            << " (" << funcionario->getCargo() << "): "
            << impacto << " pontos de impacto\n";
    }

    assert(salarios == 80000.0);

    // Verifica a validacao dos atributos.
    bool rejeitou = false;

    try {
        Mecanico invalido(
            "Teste", "X", 25,
            contrato, 1, 101
        );
    }
    catch (const std::invalid_argument&) {
        rejeitou = true;
    }

    assert(rejeitou);

    std::cout << "TESTE FUNCIONARIOS: OK\n";

    return 0;
}
