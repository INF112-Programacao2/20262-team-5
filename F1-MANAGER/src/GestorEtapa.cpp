
#include "GestorEtapa.hpp"
#include "EquipeAdversaria.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

void GestorEtapa::executar(
    Corrida& corrida,
    SimuladorDesempenho& simulador,
    const std::vector<Equipe*>& equipes
) {
    if (corrida.getEstado() != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Esta etapa ja foi iniciada ou finalizada."
        );
    }

    // Valida as equipes antes de iniciar a simulacao.
    std::unordered_set<Equipe*> equipesUnicas;

    for (Equipe* equipe : equipes) {
        if (equipe == nullptr ||
            !equipesUnicas.insert(equipe).second) {

            throw std::invalid_argument(
                "Equipe nula ou repetida na etapa."
            );
        }
    }

    const std::vector<Piloto*>& inscritos =
        corrida.getPilotos();

    if (inscritos.size() < 2) {
        throw std::logic_error(
            "A etapa precisa de pelo menos dois pilotos."
        );
    }

    // Associa cada piloto inscrito ao objeto original de sua equipe.
    std::unordered_map<const Piloto*, Piloto*> pilotosOriginais;

    std::unordered_map<const Piloto*, Equipe*> equipePorPiloto;

    for (Piloto* piloto : inscritos) {
        if (piloto == nullptr ||
            pilotosOriginais.count(piloto) != 0) {

            throw std::invalid_argument(
                "Piloto nulo ou repetido na etapa."
            );
        }

        Equipe* proprietaria = nullptr;

        for (Equipe* equipe : equipes) {
            if (equipe->buscarPiloto(piloto->getId()) == piloto) {

                if (proprietaria != nullptr) {
                    throw std::invalid_argument(
                        "Piloto associado a mais de uma equipe."
                    );
                }

                proprietaria = equipe;
            }
        }

        if (proprietaria == nullptr) {
            throw std::invalid_argument(
                "Piloto inscrito sem equipe correspondente."
            );
        }

        pilotosOriginais.emplace(piloto, piloto);
        equipePorPiloto.emplace(piloto, proprietaria);
    }

    // Prepara cada adversaria uma unica vez antes de calcular os bonus.
    for (Equipe* equipe : equipes) {
        if (auto* adversaria =
                dynamic_cast<EquipeAdversaria*>(equipe)) {

            adversaria->executarEstrategia();
        }
    }

    // ==========================================
    // BONUS ESTRATEGICOS
    // ==========================================

    std::unordered_map<const Piloto*, double> bonusPorPiloto;

    for (Piloto* piloto : inscritos) {
        Equipe* equipe = equipePorPiloto.at(piloto);

        double bonus = equipe->calcularBonusEstrategia();

        if (auto* adversaria =
                dynamic_cast<EquipeAdversaria*>(equipe)) {

            bonus +=
                0.60 * adversaria->getBonusEstrategia();
        }

        bonusPorPiloto.emplace(
            piloto,
            std::clamp(bonus, 0.0, 3.0)
        );
    }

    // ==========================================
    // EXECUCAO DA CORRIDA
    // ==========================================

    // O motor registra os resultados e encerra a corrida.
    simulador.simularCorrida(
        corrida,
        bonusPorPiloto
    );

    // ==========================================
    // CALCULO DO DESGASTE
    // ==========================================

    // O desgaste fica no carro cadastrado para afetar os proximos GPs.
    const double extensaoKm =
        corrida.getCircuito().getExtensao();

    const int clima = corrida.getClima();

    const double fatorClima =
        (clima == 3) ? 1.25 :
        (clima == 2) ? 1.05 : 1.0;

    // Prepara as alteracoes antes de modificar os carros originais.
    std::vector<std::pair<Piloto*, double>> desgastes;

    desgastes.reserve(
        corrida.getResultados().size()
    );

    for (const Resultado& resultado : corrida.getResultados()) {

        // Localiza o piloto original correspondente ao resultado.
        const Piloto* ponteiroResultado =
            &resultado.getPiloto();

        const auto encontrado =
            pilotosOriginais.find(ponteiroResultado);

        if (encontrado == pilotosOriginais.end()) {
            throw std::logic_error(
                "Resultado associado a piloto nao inscrito."
            );
        }

        const int voltas =
            resultado.getVoltasConcluidas();

        if (voltas <= 0) {
            continue;
        }

        const double desgaste =
            extensaoKm *
            voltas *
            0.07 *
            fatorClima;

        if (!std::isfinite(desgaste) ||
            desgaste <= 0.0) {

            throw std::logic_error(
                "Desgaste calculado com valor invalido."
            );
        }

        desgastes.emplace_back(
            encontrado->second,
            desgaste
        );
    }

    // ==========================================
    // APLICACAO DO DESGASTE
    // ==========================================

    for (const auto& desgaste : desgastes) {
        desgaste.first->getCarro().desgastar(
            desgaste.second
        );
    }
}
