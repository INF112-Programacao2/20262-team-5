
#include "EquipeAdversaria.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

EquipeAdversaria::EquipeAdversaria(
    std::string nome,
    int nivelTecnico,
    int nivelEstrategico
)
    : Equipe(std::move(nome)),
      _nivelTecnico(nivelTecnico),
      _nivelEstrategico(nivelEstrategico) {

    // uso uma escala de 0 a 100 para manter os niveis
    // comparaveis aos atributos dos pilotos e dos carros.
    if (_nivelTecnico < 0 || _nivelTecnico > 100 ||
        _nivelEstrategico < 0 || _nivelEstrategico > 100) {
        throw std::invalid_argument(
            "Os niveis da equipe devem estar entre 0 e 100."
        );
    }
}

int EquipeAdversaria::getNivelTecnico() const {
    return _nivelTecnico;
}

int EquipeAdversaria::getNivelEstrategico() const {
    return _nivelEstrategico;
}

double EquipeAdversaria::getBonusEstrategia() const {
    return _bonusEstrategia;
}

void EquipeAdversaria::executarEstrategia() {

    // calculo uma vantagem fixa para representar a
    // preparacao da equipe. Nao altero o carro nem os pontos.
    _bonusEstrategia = 0.05 * _nivelEstrategico;
}

double EquipeAdversaria::calcularForcaEquipe() const {

    //  nao atribuo desempenho a uma equipe sem pilotos.
    if (quantidadePilotos() == 0) {
        return 0.0;
    }

    const double forcaPilotos = calcularForcaPilotos();

    // priorizo os carros e pilotos, mas considero a
    // estrutura tecnica e a preparacao da adversaria.
    const double forca =
        0.80 * forcaPilotos
        + 0.20 * _nivelTecnico
        + _bonusEstrategia;

    return std::clamp(forca, 0.0, 100.0);
}
