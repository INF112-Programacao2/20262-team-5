#include <iostream>
#include <iomanip>
#include <string>
#include "../include/relatorio.hpp"

void Relatorio::exibirRelatorioFinanceiro(Financeiro financeiro){
    std::cout << std::fixed << std::setprecision(2) << "Saldo atual: R$" << financeiro.getSaldo() << ".\n";
    std::cout << std::fixed << std::setprecision(2) << "Orcamento: R$" << financeiro.getOrcamento() << ".\n";
    std::cout << std::fixed << std::setprecision(2) << "Despesas Totais: R$" << financeiro.getDespesasTotais() << ".\n";
    std::cout << std::fixed << std::setprecision(2) << "Receita total: R$" << financeiro.getReceitasTotais() << ".\n";
    std::cout << "Historico de transacoes:\n";
    financeiro.exibirHistoricoDeTransacoes();
    std::cout << "\n";
}

void Relatorio::exibirRelatorioEquipe(/*Equipe equipe*/){
    //esperar pela implementacao
}

void Relatorio::exibirRelatorioDesempenho(/*Desempenho desempenho??*/){
    //esperar pela implementacao
}

void Relatorio::exibirRelatorioCorrida(Corrida corrida){
    std::string clima;
    if(corrida.getClima()==1){
        clima = "Sol";
    }else if(corrida.getClima()==2){
        clima = "Nublado";
    }else{
        clima = "Chuva";
    }

    corrida.ordenarPilotosCrescente();
    std::cout << "Data: " << corrida.getData() << ".\n";
    std::cout << "Pais: " << corrida.getCircuito().getPais() << ".\n";
    std::cout << "Circuito: " << corrida.getCircuito().getNome() << ".\n";
    std::cout << std::fixed << std::setprecision(1) << "Extensao: " << corrida.getCircuito().getExtensao() << "Km.\n";
    std::cout << "Quantidade de curvas: " << corrida.getCircuito().getCurvas() << ".\n";
    std::cout << "Clima: " << clima << ".\n";
    std::cout << "Resultados:\n";
    corrida.exibirResultados();
    std::cout << "\n";
}