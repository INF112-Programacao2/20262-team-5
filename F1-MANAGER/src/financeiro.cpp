
#include "financeiro.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

// rejeitair valores negativos, infinitos e NaN para manter o caixa valido.
void validarNaoNegativo(double valor) {
    if (!std::isfinite(valor) || valor < 0.0) {
        throw std::invalid_argument(
            "Valor financeiro invalido."
        );
    }
}

//  esta verificacao nas movimentacoes, que devem ter valor maior que zero.
void validarPositivo(double valor) {
    if (!std::isfinite(valor) || valor <= 0.0) {
        throw std::invalid_argument(
            "O valor deve ser positivo."
        );
    }
}

} // namespace

Financeiro::Financeiro(double orcamento)
    : _saldo(orcamento),
      _orcamento(orcamento),
      _despesasTotais(0.0),
      _receitasTotais(0.0) {

    //  considero o orcamento inicial como o dinheiro disponivel em caixa.
    validarNaoNegativo(orcamento);
}

double Financeiro::getSaldo() const {
    return _saldo;
}

void Financeiro::setSaldo(double saldo) {
    validarNaoNegativo(saldo);
    _saldo = saldo;
}

double Financeiro::getOrcamento() const {
    return _orcamento;
}

void Financeiro::setOrcamento(double orcamento) {
    validarNaoNegativo(orcamento);
    _orcamento = orcamento;
}

double Financeiro::getDespesasTotais() const {
    return _despesasTotais;
}

void Financeiro::setDespesasTotais(double despesasTotais) {
    validarNaoNegativo(despesasTotais);
    _despesasTotais = despesasTotais;
}

double Financeiro::getReceitasTotais() const {
    return _receitasTotais;
}

void Financeiro::setReceitasTotais(double receitasTotais) {
    validarNaoNegativo(receitasTotais);
    _receitasTotais = receitasTotais;
}

Transacao Financeiro::getTransacao(int i) const {
    // usei o at() para identificar acessos fora dos limites do vetor.
    return _transacoes.at(static_cast<std::size_t>(i));
}

void Financeiro::setTransacao(Transacao transacao, int i) {
    // mantenho este metodo para compatibilidade com o projeto original.
    // Nas operacoes normais, eu prefiro registrar receitas e despesas pelos metodos proprios.
    _transacoes.at(static_cast<std::size_t>(i)) =
        std::move(transacao);
}

std::size_t Financeiro::quantidadeTransacoes() const {
    return _transacoes.size();
}

void Financeiro::adicionarFundos(
    double valor,
    std::string data,
    std::string descricao
) {
    validarPositivo(valor);

    // registrei a receita antes de atualizar o caixa e os totais.
    _transacoes.emplace_back(
        valor,
        std::move(data),
        std::move(descricao)
    );

    _receitasTotais += valor;
    _saldo += valor;
}

bool Financeiro::podeGastar(double valor) const {
    validarPositivo(valor);

    return valor <= _saldo;
}

bool Financeiro::removerFundos(
    double valor,
    std::string data,
    std::string descricao
) {
    // Eu bloqueio a compra quando nao ha saldo suficiente.
    if (!podeGastar(valor)) {
        return false;
    }

    // Eu registro uma despesa com sinal negativo no historico.
    _transacoes.emplace_back(
        -valor,
        std::move(data),
        std::move(descricao)
    );

    _despesasTotais += valor;
    _saldo -= valor;

    return true;
}

void Financeiro::exibirHistoricoDeTransacoes() const {
    for (
        auto it = _transacoes.rbegin();
        it != _transacoes.rend();
        ++it
    ) {
        // Eu copio a transacao porque os getters originais ainda nao sao const.
        Transacao transacao = *it;

        std::cout
            << std::fixed
            << std::setprecision(2)
            << "Valor: R$ "
            << transacao.getValor()
            << '\n'
            << "Data: "
            << transacao.getData()
            << '\n'
            << "Descricao: "
            << transacao.getDescricao()
            << "\n\n";
    }
}
