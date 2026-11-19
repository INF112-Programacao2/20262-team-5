#ifndef RELATORIO_H
#define RELATORIO_H

#include "financeiro.hpp"
//#include "equipe.hpp"
//#include "desempenho.hpp"
#include "corrida.hpp"

class Relatorio {
    public:
        void exibirRelatorioFinanceiro(Financeiro financeiro);
        void exibirRelatorioEquipe(/*Equipe equipe*/);
        void exibirRelatorioDesempenho(/*Desempenho desempenho??*/);
        void exibirRelatorioCorrida(Corrida corrida);
};

#endif