#include <iostream>
#include <iomanip>
#include <string>
#include "../include/financeiro.hpp"
#include "../include/transacao.hpp"

Financeiro::Financeiro(double orcamento):
_saldo(0), _orcamento(orcamento), _despesasTotais(0), _receitasTotais(0) {}

Financeiro::~Financeiro() {}

double Financeiro::getSaldo(){
    return _saldo;
}
void Financeiro::setSaldo(double saldo){
    _saldo = saldo;
}

double Financeiro::getOrcamento(){
    return _orcamento;
}
void Financeiro::setOrcamento(double orcamento){
    _orcamento = orcamento;
}

double Financeiro::getDespesasTotais(){
    return _despesasTotais;
}
void Financeiro::setDespesasTotais(double despesasTotais){
    _despesasTotais = despesasTotais;
}

double Financeiro::getReceitasTotais(){
    return _receitasTotais;
}
void Financeiro::setReceitasTotais(double receitasTotais){
    _receitasTotais = receitasTotais;
}

Transacao Financeiro::getTransacao(int i){
    return _transacoes[i];
}
void Financeiro::setTransacao(Transacao transacao, int i){
    _transacoes[i].setValor(transacao.getValor());
    _transacoes[i].setData(transacao.getData());
    _transacoes[i].setDescricao(transacao.getDescricao());
}

void Financeiro::adicionarFundos(double valor, std::string data, std::string descricao){
    _receitasTotais += valor;
    _saldo += valor;
    Transacao transacao(valor, data, descricao);
    _transacoes.push_back(transacao);
}
void Financeiro::removerFundos(double valor, std::string data, std::string descricao){
    if((_despesasTotais + valor) - _receitasTotais <= _orcamento){
        _despesasTotais += valor;
        valor *= -1;
        _saldo += valor;
        Transacao transacao(valor, data, descricao);
        _transacoes.push_back(transacao);
    }
}
void Financeiro::exibirHistoricoDeTransacoes(){
    for(int i=_transacoes.size()-1; i>=0; i--){
        std::cout << std::fixed << std::setprecision(2) << "Valor: " << _transacoes[i].getValor() << "\n";
        std::cout << "Data: " << _transacoes[i].getData() << "\n";
        std::cout << "Descricao: " << _transacoes[i].getDescricao() << "\n\n";
    }
}