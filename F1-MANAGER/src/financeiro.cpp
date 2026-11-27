
#include "financeiro.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

void validarNaoNegativo(double valor) {
    if (!std::isfinite(valor) || valor < 0.0) {
        throw std::invalid_argument(
            "Valor financeiro invalido."
        );
    }
}

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

    validarNaoNegativo(orcamento);
}

// ==========================================
// GETTERS E SETTERS
// ==========================================

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

void Financeiro::setDespesasTotais(
    double despesasTotais
) {
    validarNaoNegativo(despesasTotais);
    _despesasTotais = despesasTotais;
}

double Financeiro::getReceitasTotais() const {
    return _receitasTotais;
}

void Financeiro::setReceitasTotais(
    double receitasTotais
) {
    validarNaoNegativo(receitasTotais);
    _receitasTotais = receitasTotais;
}

// ==========================================
// TRANSACOES
// ==========================================

Transacao Financeiro::getTransacao(int i) const {
    return _transacoes.at(
        static_cast<std::size_t>(i)
    );
}

void Financeiro::setTransacao(
    Transacao transacao,
    int i
) {
    _transacoes.at(
        static_cast<std::size_t>(i)
    ) = std::move(transacao);
}

std::size_t Financeiro::quantidadeTransacoes() const {
    return _transacoes.size();
}

// ==========================================
// RECEITAS
// ==========================================

void Financeiro::adicionarFundos(
    double valor,
    std::string data,
    std::string descricao
) {
    validarPositivo(valor);

    _transacoes.emplace_back(
        valor,
        std::move(data),
        std::move(descricao)
    );

    _receitasTotais += valor;
    _saldo += valor;
}

// ==========================================
// DESPESAS
// ==========================================

bool Financeiro::podeGastar(double valor) const {
    validarPositivo(valor);

    return valor <= _saldo;
}

bool Financeiro::removerFundos(
    double valor,
    std::string data,
    std::string descricao
) {
    if (!podeGastar(valor)) {
        return false;
    }

    // Despesas sao registradas com valores negativos.
    _transacoes.emplace_back(
        -valor,
        std::move(data),
        std::move(descricao)
    );

    _despesasTotais += valor;
    _saldo -= valor;

    return true;
}

// ==========================================
// HISTORICO
// ==========================================

void Financeiro::exibirHistoricoDeTransacoes() const {
    for (
        auto it = _transacoes.rbegin();
        it != _transacoes.rend();
        ++it
    ) {
        // Os getters originais de Transacao nao sao const.
        Transacao transacao = *it;

        std::cout
            << std::fixed
            << std::setprecision(2)
            << "Valor: R$ "
            << transacao.getValor()
            << '\n';

        std::cout
            << "Data: "
            << transacao.getData()
            << '\n';

        std::cout
            << "Descricao: "
            << transacao.getDescricao()
            << "\n\n";
    }
}
