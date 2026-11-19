#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#include "transacao.hpp"
#include <vector>

class Financeiro {
    private:
        double _saldo;
        double _orcamento;
        double _despesasTotais;
        double _receitasTotais;
        std::vector<Transacao> _transacoes;
    public:
        Financeiro(double orcamento);
        ~Financeiro();

        double getSaldo();
        void setSaldo(double saldo);

        double getOrcamento();
        void setOrcamento(double orcamento);

        double getDespesasTotais();
        void setDespesasTotais(double despesasTotais);

        double getReceitasTotais();
        void setReceitasTotais(double receitasTotais);

        Transacao getTransacao(int i);
        void setTransacao(Transacao transacao, int i);
        
        //Registra uma transacao positiva - aumenta o saldo - registra nas receitas totais
        void adicionarFundos(double valor, std::string data, std::string descricao);

        //Caso o orcamento permita --> reduz o saldo - registra uma transacao negativa - registra nas despesas totais
        void removerFundos(double valor, std::string data, std::string descricao);

        //Exibe as transacoes a partir da mais recente
        //Por enquanto eh fe de que o usuario vai colocar na ordem certa, mas um sort nao faria mal
        void exibirHistoricoDeTransacoes();
};

#endif