
#include "EquipeControlavel.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

//  inicializo a classe Equipe e o financeiro
// com o orcamento informado no cadastro.
EquipeControlavel::EquipeControlavel(
    std::string nome,
    double orcamento
)
    : Equipe(std::move(nome)),
      _financeiro(orcamento) {
}

// ==========================================
// ACESSO AO FINANCEIRO
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

    // Eu aproveito o calculo da classe base para
    // considerar os carros e atributos dos pilotos.
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
    // Eu verifico se os valores informados sao validos
    // antes de iniciar a compra da melhoria.
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

    //  procurar o piloto pelo identificador.
    Piloto* piloto = buscarPiloto(idPiloto);

    if (piloto == nullptr) {
        throw std::invalid_argument(
            "Piloto nao encontrado."
        );
    }

    // testar primeiro a melhoria em uma copia, para nao
    // cobrar por componente desconhecido ou ja no limite.
    Carro atualizado = piloto->getCarro();

    double forcaAntes =
        atualizado.calcularDesempenho();

    // Aplicar a melhoria primeiro na copia do carro.
    atualizado.melhorarComponente(
        componente,
        incremento
    );

    double forcaDepois =
        atualizado.calcularDesempenho();

    // cancelar a compra caso a melhoria nao
    // tenha aumentado o desempenho do carro.
    if (forcaDepois <= forcaAntes) {
        return false;
    }

    // cobro apenas depois de validar que a melhoria funciona.
    // Se o dinheiro for insuficiente, o Financeiro retorna false.
    if (!_financeiro.removerFundos(
        custo,
        data,
        "Melhoria de " + componente + " - " + idPiloto
    )) {
        return false;
    }

    // atualizo o carro original somente depois
    // que a compra foi autorizada.
    piloto->getCarro() = std::move(atualizado);

    return true;
}

// ==========================================
// GERENCIAMENTO FINANCEIRO
// ==========================================

void EquipeControlavel::gerenciarFinanceiro() const {

    // utilizo o historico do Financeiro para
    // consultar as movimentacoes realizadas.
    _financeiro.exibirHistoricoDeTransacoes();
}
