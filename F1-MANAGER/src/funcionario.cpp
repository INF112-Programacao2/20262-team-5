
#include "funcionario.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

// ===========================================
// CLASSE BASE FUNCIONARIO
// ===========================================

Funcionario::Funcionario(
    std::string nome,
    std::string id,
    int idade,
    std::string cargo,
    const Contrato& contrato,
    int tempoDeServico
)
    : Pessoa(std::move(nome), std::move(id), idade),
      _cargo(std::move(cargo)),
      _contrato(contrato),
      _tempoDeServico(tempoDeServico) {

    if (_cargo.empty() || _tempoDeServico < 0) {
        throw std::invalid_argument(
            "Dados do funcionario invalidos."
        );
    }
}

const std::string& Funcionario::getCargo() const {
    return _cargo;
}

const Contrato& Funcionario::getContrato() const {
    return _contrato;
}

int Funcionario::getTempoDeServico() const {
    return _tempoDeServico;
}

std::string Funcionario::getTipo() const {
    return _cargo;
}

// ===========================================
// ENGENHEIRO
// ===========================================

Engenheiro::Engenheiro(
    std::string nome,
    std::string id,
    int idade,
    const Contrato& contrato,
    int tempoDeServico,
    std::string especialidade
)
    : Funcionario(
        std::move(nome),
        std::move(id),
        idade,
        "Engenheiro",
        contrato,
        tempoDeServico
    ),
      _especialidade(std::move(especialidade)) {

    if (_especialidade.empty()) {
        throw std::invalid_argument(
            "Especialidade invalida."
        );
    }
}

const std::string& Engenheiro::getEspecialidade() const {
    return _especialidade;
}

double Engenheiro::calcularImpacto() const {
    return std::min(
        5.0,
        1.0 + 0.1 * getTempoDeServico()
    );
}

// ===========================================
// MECANICO
// ===========================================

Mecanico::Mecanico(
    std::string nome,
    std::string id,
    int idade,
    const Contrato& contrato,
    int tempoDeServico,
    int nivelHabilidade
)
    : Funcionario(
        std::move(nome),
        std::move(id),
        idade,
        "Mecanico",
        contrato,
        tempoDeServico
    ),
      _nivelHabilidade(nivelHabilidade) {

    if (_nivelHabilidade < 0 || _nivelHabilidade > 100) {
        throw std::invalid_argument(
            "Habilidade fora da escala 0 a 100."
        );
    }
}

int Mecanico::getNivelHabilidade() const {
    return _nivelHabilidade;
}

double Mecanico::calcularImpacto() const {
    return std::min(
        5.0,
        0.04 * _nivelHabilidade +
        0.1 * getTempoDeServico()
    );
}

// ===========================================
// ESTRATEGISTA
// ===========================================

Estrategista::Estrategista(
    std::string nome,
    std::string id,
    int idade,
    const Contrato& contrato,
    int tempoDeServico,
    int experiencia
)
    : Funcionario(
        std::move(nome),
        std::move(id),
        idade,
        "Estrategista",
        contrato,
        tempoDeServico
    ),
      _experiencia(experiencia) {

    if (_experiencia < 0) {
        throw std::invalid_argument(
            "Experiencia nao pode ser negativa."
        );
    }
}

int Estrategista::getExperiencia() const {
    return _experiencia;
}

double Estrategista::calcularImpacto() const {
    return std::min(
        5.0,
        0.3 * _experiencia +
        0.05 * getTempoDeServico()
    );
}

// ===========================================
// CHEFE DE EQUIPE
// ===========================================

ChefeEquipe::ChefeEquipe(
    std::string nome,
    std::string id,
    int idade,
    const Contrato& contrato,
    int tempoDeServico,
    int experiencia
)
    : Funcionario(
        std::move(nome),
        std::move(id),
        idade,
        "ChefeEquipe",
        contrato,
        tempoDeServico
    ),
      _experiencia(experiencia) {

    if (_experiencia < 0) {
        throw std::invalid_argument(
            "Experiencia nao pode ser negativa."
        );
    }
}

int ChefeEquipe::getExperiencia() const {
    return _experiencia;
}

double ChefeEquipe::calcularImpacto() const {
    return std::min(
        5.0,
        0.25 * _experiencia +
        0.1 * getTempoDeServico()
    );
}
