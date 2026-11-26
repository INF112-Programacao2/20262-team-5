
#ifndef EQUIPE_ADVERSARIA_HPP
#define EQUIPE_ADVERSARIA_HPP

#include "equipe.hpp"

#include <string>

// Eu represento aqui as equipes controladas pelo computador.
// Cada equipe adversaria tem seu nivel tecnico e estrategico.
class EquipeAdversaria : public Equipe {
private:
    int _nivelTecnico;
    int _nivelEstrategico;
    double _bonusEstrategia = 0.0;

public:
    EquipeAdversaria(
        std::string nome,
        int nivelTecnico,
        int nivelEstrategico
    );

    int getNivelTecnico() const;
    int getNivelEstrategico() const;
    double getBonusEstrategia() const;

    // Eu preparo um bonus estrategico para a proxima etapa.
    // Se chamar novamente, nao acumulo o bonus varias vezes.
    void executarEstrategia();

    // Eu combino o desempenho dos pilotos com o nivel
    // tecnico da equipe e a preparacao estrategica.
    double calcularForcaEquipe() const override;
};

#endif
