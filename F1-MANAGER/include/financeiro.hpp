
#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#include "transacao.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Financeiro {
private:
    double _saldo;
    double _orcamento;
    double _despesasTotais;
    double _receitasTotais;
    std::vector<Transacao> _transacoes;

public:
    explicit Financeiro(double orcamento);
    ~Financeiro() = default;

    double getSaldo() const;
    void setSaldo(double saldo);

    double getOrcamento() const;
    void setOrcamento(double orcamento);

    double getDespesasTotais() const;
    void setDespesasTotais(double despesasTotais);

    double getReceitasTotais() const;
    void setReceitasTotais(double receitasTotais);

    Transacao getTransacao(int i) const;
    void setTransacao(Transacao transacao, int i);

    std::size_t quantidadeTransacoes() const;

    //Registra uma transacao positiva - aumenta o saldo - registra nas receitas totais
    void adicionarFundos(
        double valor,
        std::string data,
        std::string descricao
    );

    // Verifica se existe saldo suficiente para realizar uma despesa.
    bool podeGastar(double valor) const;

    //Caso o orcamento permita --> reduz o saldo - registra uma transacao negativa - registra nas despesas totais
    // Retorna false quando o saldo disponivel nao cobre o valor solicitado.
    bool removerFundos(
        double valor,
        std::string data,
        std::string descricao
    );

    //Exibe as transacoes a partir da mais recente
    //Por enquanto eh fe de que o usuario vai colocar na ordem certa, mas um sort nao faria mal
    void exibirHistoricoDeTransacoes() const;
};

#endif
