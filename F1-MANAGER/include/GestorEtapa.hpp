
#ifndef GESTOR_ETAPA_HPP
#define GESTOR_ETAPA_HPP

#include "SimuladorDesempenho.hpp"
#include "equipe.hpp"

#include <vector>

// Faz a ligacao entre equipes, simulacao e desgaste apos o GP.
class GestorEtapa {
public:
    static void executar(
        Corrida& corrida,
        SimuladorDesempenho& simulador,
        const std::vector<Equipe*>& equipes
    );
};

#endif
