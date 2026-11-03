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

        void adicionarFundos(double valor, std::string data, std::string descricao);
        void removerFundos(double valor, std::string data, std::string descricao);
        void exibirHistoricoDeTransacoes();
};

#endif