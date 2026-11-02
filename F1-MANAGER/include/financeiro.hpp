#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#include "transacao.hpp"
#include <vector>

class Financeiro {
    private:
        double _saldo;
        double _orcamento;
        double _despesasTotais;
        _transacoes = new vector<Transacao>
    public:
        Financeiro(double saldo, double orcamento, double despesastotais);
        ~Financeiro();

        void adicionarFundos(double valor);
        void removerFundos(double valor);
        void exibirHistoricoDeTransacoes();
};

#endif