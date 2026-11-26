
#ifndef FUNCIONARIO_HPP
#define FUNCIONARIO_HPP

#include "pessoa.hpp"
#include "contrato.hpp"

#include <string>

// Classe abstrata que concentra os dados comuns aos funcionarios.
class Funcionario : public Pessoa {
private:
    std::string _cargo;
    Contrato _contrato;
    int _tempoDeServico;

public:
    Funcionario(
        std::string nome,
        std::string id,
        int idade,
        std::string cargo,
        const Contrato& contrato,
        int tempoDeServico
    );

    ~Funcionario() override = default;

    const std::string& getCargo() const;
    const Contrato& getContrato() const;
    int getTempoDeServico() const;

    // O cargo identifica o tipo de funcionario no sistema.
    std::string getTipo() const override;

    // Cada funcao calcula seu proprio impacto (escala de 0 a 5).
    virtual double calcularImpacto() const = 0;
};

// ===========================================
// ENGENHEIRO
// ===========================================

class Engenheiro : public Funcionario {
private:
    std::string _especialidade;

public:
    Engenheiro(
        std::string nome,
        std::string id,
        int idade,
        const Contrato& contrato,
        int tempoDeServico,
        std::string especialidade
    );

    const std::string& getEspecialidade() const;

    double calcularImpacto() const override;
};

// ===========================================
// MECANICO
// ===========================================

class Mecanico : public Funcionario {
private:
    int _nivelHabilidade;

public:
    Mecanico(
        std::string nome,
        std::string id,
        int idade,
        const Contrato& contrato,
        int tempoDeServico,
        int nivelHabilidade
    );

    int getNivelHabilidade() const;

    double calcularImpacto() const override;
};

// ===========================================
// ESTRATEGISTA
// ===========================================

class Estrategista : public Funcionario {
private:
    int _experiencia;

public:
    Estrategista(
        std::string nome,
        std::string id,
        int idade,
        const Contrato& contrato,
        int tempoDeServico,
        int experiencia
    );

    int getExperiencia() const;

    double calcularImpacto() const override;
};

// ===========================================
// CHEFE DE EQUIPE
// ===========================================

class ChefeEquipe : public Funcionario {
private:
    int _experiencia;

public:
    ChefeEquipe(
        std::string nome,
        std::string id,
        int idade,
        const Contrato& contrato,
        int tempoDeServico,
        int experiencia
    );

    int getExperiencia() const;

    double calcularImpacto() const override;
};

#endif

