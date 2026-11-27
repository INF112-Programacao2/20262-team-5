
#include "equipe.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

Equipe::Equipe(std::string nome)
    : _nome(std::move(nome)) {

    if (_nome.empty()) {
        throw std::invalid_argument(
            "Nome da equipe vazio."
        );
    }
}

// ==========================================
// CONSULTAS GERAIS
// ==========================================

const std::string& Equipe::getNome() const {
    return _nome;
}

std::size_t Equipe::quantidadePilotos() const {
    return _pilotos.size();
}

std::size_t Equipe::quantidadeFuncionarios() const {
    return _funcionarios.size();
}

// ==========================================
// GERENCIAMENTO DE PILOTOS
// ==========================================

void Equipe::cadastrarPiloto(Piloto piloto) {
    if (
        buscarPiloto(piloto.getId()) ||
        buscarFuncionario(piloto.getId())
    ) {
        throw std::invalid_argument(
            "Identificador ja cadastrado na equipe."
        );
    }

    // O objeto continua no mesmo endereco apos novas insercoes no vetor.
    _pilotos.push_back(
        std::make_unique<Piloto>(
            std::move(piloto)
        )
    );
}

Piloto* Equipe::buscarPiloto(
    const std::string& id
) {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    return it == _pilotos.end() ? nullptr : it->get();
}

const Piloto* Equipe::buscarPiloto(
    const std::string& id
) const {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    return it == _pilotos.end() ? nullptr : it->get();
}

void Equipe::removerPiloto(
    const std::string& id
) {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    if (it == _pilotos.end()) {
        throw std::invalid_argument(
            "Piloto nao encontrado."
        );
    }

    // Ponteiros para este piloto deixam de ser validos apos a remocao.
    _pilotos.erase(it);
}

// ==========================================
// GERENCIAMENTO DE FUNCIONARIOS
// ==========================================

void Equipe::cadastrarFuncionario(
    std::unique_ptr<Funcionario> funcionario
) {
    if (!funcionario) {
        throw std::invalid_argument(
            "Funcionario nulo."
        );
    }

    // Impede identificadores duplicados dentro da equipe.
    if (
        buscarFuncionario(funcionario->getId()) ||
        buscarPiloto(funcionario->getId())
    ) {
        throw std::invalid_argument(
            "Identificador ja cadastrado na equipe."
        );
    }

    // A propriedade do objeto passa para a equipe.
    _funcionarios.push_back(
        std::move(funcionario)
    );
}

Funcionario* Equipe::buscarFuncionario(
    const std::string& id
) {
    const auto it = std::find_if(
        _funcionarios.begin(),
        _funcionarios.end(),
        [&id](const std::unique_ptr<Funcionario>& funcionario) {
            return funcionario->getId() == id;
        }
    );

    return it == _funcionarios.end()
        ? nullptr
        : it->get();
}

const Funcionario* Equipe::buscarFuncionario(
    const std::string& id
) const {
    const auto it = std::find_if(
        _funcionarios.begin(),
        _funcionarios.end(),
        [&id](const std::unique_ptr<Funcionario>& funcionario) {
            return funcionario->getId() == id;
        }
    );

    return it == _funcionarios.end()
        ? nullptr
        : it->get();
}

void Equipe::removerFuncionario(
    const std::string& id
) {
    const auto it = std::find_if(
        _funcionarios.begin(),
        _funcionarios.end(),
        [&id](const std::unique_ptr<Funcionario>& funcionario) {
            return funcionario->getId() == id;
        }
    );

    if (it == _funcionarios.end()) {
        throw std::invalid_argument(
            "Funcionario nao encontrado."
        );
    }

    _funcionarios.erase(it);
}

// ==========================================
// FOLHA SALARIAL
// ==========================================

double Equipe::folhaSalarialMensal() const {
    double total = 0.0;

    // Soma os salarios dos pilotos cadastrados.
    for (const auto& piloto : _pilotos) {
        total += piloto->getContrato().getSalario();
    }

    // Acrescenta os salarios dos funcionarios.
    for (const auto& funcionario : _funcionarios) {
        total += funcionario->getContrato().getSalario();
    }

    return total;
}

// ==========================================
// IMPACTO DOS FUNCIONARIOS
// ==========================================

double Equipe::calcularImpactoFuncionarios() const {
    double total = 0.0;

    // Usa polimorfismo para executar o calculo de cada profissao.
    for (const auto& funcionario : _funcionarios) {
        total += funcionario->calcularImpacto();
    }

    return total;
}

double Equipe::calcularImpactoEngenheiros() const {
    double total = 0.0;

    // Considera apenas os funcionarios da classe Engenheiro.
    for (const auto& funcionario : _funcionarios) {
        const auto* engenheiro =
            dynamic_cast<const Engenheiro*>(
                funcionario.get()
            );

        if (engenheiro != nullptr) {
            total += engenheiro->calcularImpacto();
        }
    }

    return total;
}

double Equipe::calcularImpactoMecanicos() const {
    double total = 0.0;

    // Apenas os mecanicos contribuem para a manutencao.
    for (const auto& funcionario : _funcionarios) {
        const auto* mecanico =
            dynamic_cast<const Mecanico*>(
                funcionario.get()
            );

        if (mecanico != nullptr) {
            total += mecanico->calcularImpacto();
        }
    }

    return total;
}

double Equipe::calcularBonusEstrategia() const {
    double impactoEstrategistas = 0.0;
    double impactoChefia = 0.0;

    for (const auto& funcionario : _funcionarios) {
        if (const auto* estrategista =
                dynamic_cast<const Estrategista*>(
                    funcionario.get()
                )) {

            impactoEstrategistas +=
                estrategista->calcularImpacto();
        }

        if (const auto* chefe =
                dynamic_cast<const ChefeEquipe*>(
                    funcionario.get()
                )) {

            impactoChefia +=
                chefe->calcularImpacto();
        }
    }

    // Bonus de ate tres pontos no indice de ritmo durante o GP.
    return std::clamp(
        0.30 * impactoEstrategistas +
        0.20 * impactoChefia,
        0.0,
        3.0
    );
}

// ==========================================
// ENCERRAMENTO DA TEMPORADA
// ==========================================

void Equipe::encerrarTemporada() {
    // Zera os pontos da temporada sem apagar as estatisticas de carreira.
    for (auto& piloto : _pilotos) {
        piloto->zerarTemporada();
    }
}

// ==========================================
// FORCA DOS PILOTOS
// ==========================================

double Equipe::calcularForcaPilotos() const {
    if (_pilotos.empty()) {
        return 0.0;
    }

    double soma = 0.0;

    for (const auto& piloto : _pilotos) {
        soma +=
            0.50 * piloto->getCarro().calcularDesempenho()
            + 0.30 * piloto->getHabilidade()
            + 0.20 * piloto->getConsistencia();
    }

    return soma / static_cast<double>(
        _pilotos.size()
    );
}
