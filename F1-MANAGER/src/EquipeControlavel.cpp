
#include "EquipeControlavel.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace {

// Aceita competencias validas no formato AAAA-MM.
void validarCompetencia(
    const std::string& competencia
) {
    if (
        competencia.size() != 7 ||
        competencia[4] != '-'
    ) {
        throw std::invalid_argument(
            "Competencia deve estar no formato AAAA-MM."
        );
    }

    for (
        std::size_t i = 0;
        i < competencia.size();
        ++i
    ) {
        if (i == 4) {
            continue;
        }

        if (
            competencia[i] < '0' ||
            competencia[i] > '9'
        ) {
            throw std::invalid_argument(
                "Competencia deve estar no formato AAAA-MM."
            );
        }
    }

    const int mes =
        (competencia[5] - '0') * 10 +
        (competencia[6] - '0');

    if (
        mes < 1 ||
        mes > 12 ||
        competencia.substr(0, 4) == "0000"
    ) {
        throw std::invalid_argument(
            "Mes ou ano invalido na competencia."
        );
    }
}

} // namespace

// ==========================================
// CONSTRUTOR
// ==========================================

EquipeControlavel::EquipeControlavel(
    std::string nome,
    double orcamento
)
    : Equipe(std::move(nome)),
      _financeiro(orcamento) {
}

// ==========================================
// FINANCEIRO
// ==========================================

Financeiro& EquipeControlavel::getFinanceiro() {
    return _financeiro;
}

const Financeiro&
EquipeControlavel::getFinanceiro() const {
    return _financeiro;
}

// ==========================================
// FORCA DA EQUIPE
// ==========================================

double EquipeControlavel::calcularForcaEquipe() const {
    return calcularForcaPilotos();
}

// ==========================================
// MELHORIAS DO CARRO
// ==========================================

bool EquipeControlavel::melhorarCarro(
    const std::string& idPiloto,
    const std::string& componente,
    double incremento,
    double custo,
    const std::string& data
) {
    if (
        !std::isfinite(incremento) ||
        incremento <= 0.0 ||
        !std::isfinite(custo) ||
        custo <= 0.0
    ) {
        throw std::invalid_argument(
            "Incremento ou custo invalido."
        );
    }

    Piloto* piloto = buscarPiloto(idPiloto);

    if (piloto == nullptr) {
        throw std::invalid_argument(
            "Piloto nao encontrado."
        );
    }

    // Cada ponto de impacto dos engenheiros aumenta o ganho em 2%.
    // O bonus total e limitado a 10% para preservar o balanceamento.
    const double bonus =
        0.02 * std::min(
            5.0,
            calcularImpactoEngenheiros()
        );

    const double incrementoEfetivo =
        incremento * (1.0 + bonus);

    // A melhoria e testada primeiro em uma copia, evitando compras sem efeito.
    Carro atualizado = piloto->getCarro();

    const double forcaAntes =
        atualizado.calcularDesempenho();

    atualizado.melhorarComponente(componente,incrementoEfetivo);

    if (
        atualizado.calcularDesempenho() <= forcaAntes
    ) {
        return false;
    }

    // O carro original e atualizado somente apos a cobranca ser aprovada.
    if (!_financeiro.removerFundos(
        custo,
        data,
        "Melhoria de " + componente + " - " + idPiloto
    )) {
        return false;
    }

    piloto->getCarro() = std::move(atualizado);

    return true;
}

// ==========================================
// CONSULTA DE SALARIOS PAGOS
// ==========================================

bool EquipeControlavel::folhaJaPaga(
    const std::string& competencia
) const {
    validarCompetencia(competencia);

    return _competenciasPagas.count(competencia) != 0;
}

// ==========================================
// PAGAMENTO DA FOLHA SALARIAL
// ==========================================

bool EquipeControlavel::pagarFolhaMensal(
    const std::string& competencia
) {
    validarCompetencia(competencia);

    // Impede a cobranca duplicada do mesmo mes.
    if (folhaJaPaga(competencia)) {
        return false;
    }

    const double total = folhaSalarialMensal();

    if (
        !std::isfinite(total) ||
        total <= 0.0
    ) {
        return false;
    }

    // Registra o mes apenas se a despesa for aceita.
    const auto insercao =
        _competenciasPagas.insert(competencia);

    try {
        if (!_financeiro.removerFundos(
            total,
            competencia,
            "Folha salarial - " + competencia
        )) {
            _competenciasPagas.erase(
                insercao.first
            );

            return false;
        }
    }
    catch (...) {
        _competenciasPagas.erase(
            insercao.first
        );

        throw;
    }

    return true;
}

// ==========================================
// HISTORICO FINANCEIRO
// ==========================================

void EquipeControlavel::gerenciarFinanceiro() const {
    _financeiro.exibirHistoricoDeTransacoes();
}
