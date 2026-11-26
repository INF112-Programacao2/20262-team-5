
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

    // usei esse metodo para consultar o tamanho do historico.
    std::size_t quantidadeTransacoes() const;

    //Registra uma transacao positiva - aumenta o saldo - registra nas receitas totais
    // verifico se o valor eh positivo antes de registrar a receita.
    void adicionarFundos(
        double valor,
        std::string data,
        std::string descricao
    );

    // verifico o saldo antes de autorizar qualquer despesa.
    bool podeGastar(double valor) const;

    //Caso o orcamento permita --> reduz o saldo - registra uma transacao negativa - registra nas despesas totais
    // retorno true quando consigo pagar e false quando o saldo eh insuficiente.
    bool removerFundos(
        double valor,
        std::string data,
        std::string descricao
    );

    //Exibe as transacoes a partir da mais recente
    //Por enquanto eh fe de que o usuario vai colocar na ordem certa, mas um sort nao faria mal
    // Eu percorro o vetor de tras para frente para mostrar a ultima transacao primeiro.
    void exibirHistoricoDeTransacoes() const;
};

#endif
