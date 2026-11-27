
#ifndef EQUIPE_CONTROLAVEL_HPP
#define EQUIPE_CONTROLAVEL_HPP

#include "equipe.hpp"
#include "financeiro.hpp"

#include <set>
#include <string>

// Equipe administrada pelo jogador, com controle financeiro e melhorias.
class EquipeControlavel : public Equipe {
private:
    Financeiro _financeiro;

    // Armazena os meses que ja tiveram seus salarios pagos.
    std::set<std::string> _competenciasPagas;

public:
    EquipeControlavel(
        std::string nome,
        double orcamento
    );

    Financeiro& getFinanceiro();

    const Financeiro& getFinanceiro() const;

    double calcularForcaEquipe() const override;

    // Aplica a melhoria apenas se o componente puder evoluir e houver saldo.
    bool melhorarCarro(
        const std::string& idPiloto,
        const std::string& componente,
        double incremento,
        double custo,
        const std::string& data
    );

    // Registra um unico pagamento por competencia (formato AAAA-MM).
    // Retorna false se ja foi pago ou se o caixa for insuficiente.
    bool pagarFolhaMensal(
        const std::string& competencia
    );

    bool folhaJaPaga(
        const std::string& competencia
    ) const;

    void gerenciarFinanceiro() const;
};

#endif
