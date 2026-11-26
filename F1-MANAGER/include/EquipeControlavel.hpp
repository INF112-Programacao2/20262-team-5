
#ifndef EQUIPE_CONTROLAVEL_HPP
#define EQUIPE_CONTROLAVEL_HPP

#include "equipe.hpp"
#include "financeiro.hpp"

#include <string>

// Eu separo a equipe controlada das adversarias para
// deixar as compras e decisoes do jogador nesta classe.
class EquipeControlavel : public Equipe {
private:
    Financeiro _financeiro;

public:
    EquipeControlavel(
        std::string nome,
        double orcamento
    );

    // Eu disponibilizo o financeiro para consultar
    // o saldo, as receitas e as despesas da equipe.
    Financeiro& getFinanceiro();
    const Financeiro& getFinanceiro() const;

    // Eu sobrescrevo o metodo abstrato da classe Equipe.
    double calcularForcaEquipe() const override;

    // Eu so aplico a melhoria se a equipe puder pagar.
    // O id e string porque Pessoa::getId() retorna string.
    bool melhorarCarro(
        const std::string& idPiloto,
        const std::string& componente,
        double incremento,
        double custo,
        const std::string& data
    );

    // Eu uso este metodo para consultar as movimentacoes.
    void gerenciarFinanceiro() const;
};

#endif
