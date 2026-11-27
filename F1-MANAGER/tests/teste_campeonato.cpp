
#include "Campeonato.hpp"
#include "EquipeAdversaria.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

int main() {
    Campeonato campeonato(2026);

    campeonato.carregarDadosPredefinidos();

    // ==========================================
    // VERIFICACAO DO CADASTRO
    // ==========================================

    assert(campeonato.getTemporada() == 2026);
    assert(campeonato.quantidadeEquipes() == 11);
    assert(campeonato.quantidadeEtapas() == 24);
    assert(campeonato.getEtapaAtual() == 0);

    assert(campeonato.getCorridaAtual() != nullptr);

    assert(
        campeonato.getMercedes().quantidadePilotos() == 2
    );

    assert(
        campeonato.getMercedes().buscarPiloto("63")
        != nullptr
    );

    // Verifica as dez equipes adversarias.
    int adversarias = 0;

    for (
        std::size_t i = 0;
        i < campeonato.quantidadeEquipes();
        ++i
    ) {
        const Equipe& equipe = campeonato.getEquipe(i);

        assert(equipe.quantidadePilotos() == 2);

        if (
            dynamic_cast<const EquipeAdversaria*>(
                &equipe
            ) != nullptr
        ) {
            ++adversarias;
        }
    }

    assert(adversarias == 10);

    // ==========================================
    // CLASSIFICACAO INICIAL
    // ==========================================

    auto classificacao =
        campeonato.calcularClassificacao();

    assert(classificacao.pilotos.size() == 22);
    assert(classificacao.equipes.size() == 11);

    std::unordered_set<std::string> ids;

    for (const auto& piloto : classificacao.pilotos) {
        assert(ids.insert(piloto.id).second);
        assert(piloto.pontos == 0);
    }

    // ==========================================
    // PROTECAO DE ETAPAS
    // ==========================================

    bool rejeitou = false;

    try {
        campeonato.avancarEtapa();
    }
    catch (const std::logic_error&) {
        rejeitou = true;
    }

    assert(rejeitou);

    // ==========================================
    // CONFIGURACAO DO SIMULADOR
    // ==========================================

    ConfiguracaoSimulacao config;

    config.variacaoVolta = 0.0;
    config.probabilidadeFalhaBase = 0.0;

    SimuladorDesempenho simulador(
        42,
        config
    );

    // ==========================================
    // SIMULACAO DA TEMPORADA COMPLETA
    // ==========================================

    for (
        std::size_t etapa = 0;
        etapa < campeonato.quantidadeEtapas();
        ++etapa
    ) {
        campeonato.simularEtapa(simulador);

        const Corrida& gp =
            campeonato.getCorrida(etapa);

        assert(
            gp.getEstado() == EstadoCorrida::FINALIZADA
        );

        assert(gp.getResultados().size() == 22);

        // Uma etapa nao pode ser disputada duas vezes.
        if (etapa == 0) {
            rejeitou = false;

            try {
                campeonato.simularEtapa(simulador);
            }
            catch (const std::logic_error&) {
                rejeitou = true;
            }

            assert(rejeitou);
        }

        // ==========================================
        // VERIFICACAO DAS CLASSIFICACOES
        // ==========================================

        classificacao =
            campeonato.calcularClassificacao();

        int somaPilotos = 0;
        int somaEquipes = 0;

        for (const auto& piloto : classificacao.pilotos) {
            somaPilotos += piloto.pontos;
        }

        for (const auto& equipe : classificacao.equipes) {
            somaEquipes += equipe.pontos;
        }

        // Os pontos das equipes devem ser a soma
        // dos pontos de todos os seus pilotos.
        assert(somaPilotos == somaEquipes);

        campeonato.avancarEtapa();
    }

    // ==========================================
    // ENCERRAMENTO DO CAMPEONATO
    // ==========================================

    assert(campeonato.temporadaFinalizada());

    assert(campeonato.getCorridaAtual() == nullptr);

    assert(
        campeonato.getEtapaAtual() ==
        campeonato.quantidadeEtapas()
    );

    assert(
        campeonato.calcularClassificacao().pilotos.size()
        == 22
    );

    // Nao permite simular alem do calendario.
    rejeitou = false;

    try {
        campeonato.simularEtapa(simulador);
    }
    catch (const std::logic_error&) {
        rejeitou = true;
    }

    assert(rejeitou);

    // ==========================================
    // RESULTADO FINAL
    // ==========================================

    std::cout
        << "CAMPEAO DOS PILOTOS: "
        << classificacao.pilotos.front().piloto
        << " ("
        << classificacao.pilotos.front().pontos
        << " pontos)\n";

    std::cout
        << "CAMPEA DE CONSTRUTORES: "
        << classificacao.equipes.front().equipe
        << " ("
        << classificacao.equipes.front().pontos
        << " pontos)\n";

    std::cout
        << "TESTE CAMPEONATO: OK\n";

    return 0;
}
