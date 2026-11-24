
#include "SimuladorDesempenho.hpp"
#include "resultado.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// Representa o estado temporario de um piloto na pista.
// Os dados sao usados durante a simulacao de um GP.
struct PilotoNaPista {
    Piloto* piloto;

    // Trabalhamos com uma copia do carro.
    // Assim o combustivel do carro cadastrado nao e alterado.
    Carro carro;

    double tempo = 0.0;
    int voltas = 0;
    bool ativo = true;

    std::string ocorrencia = "Finalizou";

    explicit PilotoNaPista(Piloto& p)
        : piloto(&p),
          carro(p.getCarro()) {}
};

} // namespace

// ========================================
// CONSTRUTOR
// ========================================

SimuladorDesempenho::SimuladorDesempenho(
    std::uint32_t semente,
    ConfiguracaoSimulacao config
)
    : _gerador(semente),
      _config(config) {

    // Validacao das configuracoes de simulacao.
    if (!std::isfinite(_config.variacaoVolta) ||
        _config.variacaoVolta < 0.0 ||
        _config.variacaoVolta > 0.5 ||

        !std::isfinite(_config.probabilidadeFalhaBase) ||
        _config.probabilidadeFalhaBase < 0.0 ||
        _config.probabilidadeFalhaBase > 1.0 ||

        !std::isfinite(_config.consumoPorKm) ||
        _config.consumoPorKm < 0.0) {

        throw std::invalid_argument(
            "Configuracao de simulacao invalida."
        );
    }
}

// ========================================
// CALCULO DO DESEMPENHO
// ========================================

double SimuladorDesempenho::calcularDesempenho(
    const Piloto& piloto,
    const Carro& carro,
    Clima clima
) const {

    // Em condicoes de chuva, a habilidade do piloto
    // possui maior influencia no desempenho.
    //
    // A consistencia sera utilizada na variacao
    // dos tempos entre as voltas.

    const double pesoPiloto =
        (clima == Clima::CHUVA) ? 0.55 : 0.40;

    const double indice =
        pesoPiloto * piloto.getHabilidade() +
        (1.0 - pesoPiloto) * carro.calcularDesempenho();

    return std::clamp(indice, 0.0, 100.0);
}

// ========================================
// SIMULACAO DE CORRIDA
// ========================================

void SimuladorDesempenho::simularCorrida(
    Corrida& corrida
) {

    // Uma corrida finalizada nao pode ser simulada novamente.
    if (corrida.getEstado() != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Esta corrida ja foi iniciada ou finalizada."
        );
    }

    if (corrida.getPilotos().size() < 2) {
        throw std::logic_error(
            "A corrida exige pelo menos dois pilotos."
        );
    }

    // ========================================
    // DADOS DO CIRCUITO
    // ========================================

    Circuito pista = corrida.getCircuito();

    const double extensaoKm = pista.getExtensao();
    const int curvas = pista.getCurvas();

    if (!std::isfinite(extensaoKm) ||
        extensaoKm <= 0.0 ||
        curvas < 0) {

        throw std::invalid_argument(
            "Dados de circuito invalidos."
        );
    }

    const Clima clima = converterClima(
        corrida.getClima()
    );

    // Velocidade media teorica de referencia.
    // Mais curvas reduzem a velocidade media.
    const double velocidadeBase = std::clamp(
        245.0 - 0.7 * static_cast<double>(curvas),
        130.0,
        245.0
    );

    // Converte distancia/velocidade para segundos.
    const double tempoBaseSeg =
        extensaoKm * 3600.0 / velocidadeBase;

    // Condicoes climaticas influenciam o ritmo.
    const double fatorClima =
        (clima == Clima::CHUVA) ? 1.12 :
        (clima == Clima::NUBLADO) ? 1.025 :
        1.0;

    // Consumo por volta.
    const double consumoPorVolta =
        extensaoKm * _config.consumoPorKm *
        ((clima == Clima::CHUVA) ? 1.03 : 1.0);

    // ========================================
    // PREPARACAO DOS PILOTOS
    // ========================================

    std::vector<PilotoNaPista> participantes;

    participantes.reserve(
        corrida.getPilotos().size()
    );

    for (Piloto* piloto : corrida.getPilotos()) {

        if (piloto == nullptr) {
            throw std::logic_error(
                "Piloto nulo na corrida."
            );
        }

        participantes.emplace_back(*piloto);
    }

    // ========================================
    // DISTRIBUICOES ALEATORIAS
    // ========================================

    // CORRECAO:
    // A distribuicao normal exige desvio padrao > 0.
    //
    // Utilizamos desvio padrao 1 e multiplicamos
    // o resultado por _config.variacaoVolta.
    //
    // Dessa forma, configuracoes com variacaoVolta = 0
    // tambem funcionam corretamente.
    std::normal_distribution<double> variacao(
        0.0,
        1.0
    );

    // Distribuicao uniforme para verificar falhas.
    std::uniform_real_distribution<double> sorteio(
        0.0,
        1.0
    );

    // ========================================
    // SIMULACAO VOLTA A VOLTA
    // ========================================

    for (int volta = 0;
         volta < corrida.getQtdVoltas();
         ++volta) {

        for (PilotoNaPista& estado : participantes) {

            // Pilotos que abandonaram nao continuam.
            if (!estado.ativo) {
                continue;
            }

            // ========================================
            // CONFIABILIDADE MECANICA
            // ========================================

            const double qualidade =
                estado.carro.calcularDesempenho();

            // Carros com menor qualidade possuem
            // maior probabilidade de falha.
            const double chanceFalha = std::clamp(
                _config.probabilidadeFalhaBase +
                (100.0 - qualidade) * 0.000015 +
                ((clima == Clima::CHUVA) ? 0.0005 : 0.0),
                0.0,
                1.0
            );

            if (sorteio(_gerador) < chanceFalha) {
                estado.ativo = false;
                estado.ocorrencia = "Falha mecanica";
                continue;
            }

            // ========================================
            // COMBUSTIVEL
            // ========================================

            if (!estado.carro.consumirCombustivel(
                    consumoPorVolta)) {

                estado.ativo = false;

                estado.ocorrencia =
                    "Combustivel insuficiente";

                continue;
            }

            // ========================================
            // DESEMPENHO DO PILOTO E DO CARRO
            // ========================================

            const double indice = calcularDesempenho(
                *estado.piloto,
                estado.carro,
                clima
            );

            // Pilotos e carros melhores tendem a completar
            // uma volta em menos tempo.
            const double fatorRitmo =
                1.0 + (100.0 - indice) * 0.0014;

            // ========================================
            // VARIACAO DOS TEMPOS
            // ========================================

            // CORRECAO:
            // Quando variacaoVolta = 0, o ruido e zero.
            // Nao precisamos realizar um sorteio desnecessario.
            double ruido = 0.0;

            if (_config.variacaoVolta > 0.0) {

                // Maior consistencia reduz a variacao
                // do tempo entre as voltas.
                ruido =
                    variacao(_gerador) *
                    _config.variacaoVolta *
                    (1.0 +
                        (100.0 -
                         estado.piloto->getConsistencia())
                        / 150.0);
            }

            const double fatorVariacao = std::clamp(
                1.0 + ruido,
                0.80,
                1.20
            );

            // ========================================
            // PESO DO COMBUSTIVEL
            // ========================================

            // Mais combustivel significa mais peso.
            // Simplificacao da fisica de corrida.
            const double fatorPeso =
                1.0 + std::clamp(
                    estado.carro.getCombustivel() * 0.00025,
                    0.0,
                    0.04
                );

            // ========================================
            // TEMPO FINAL DA VOLTA
            // ========================================

            const double tempoVolta =
                tempoBaseSeg *
                fatorClima *
                fatorRitmo *
                fatorVariacao *
                fatorPeso;

            estado.tempo += tempoVolta;
            ++estado.voltas;
        }
    }

    // ========================================
    // REGISTRO DOS RESULTADOS
    // ========================================

    // Inicia o registro oficial somente depois
    // de calcular todas as voltas.
    corrida.iniciar();

    for (PilotoNaPista& estado : participantes) {

        const bool terminou =
            estado.ativo &&
            estado.voltas == corrida.getQtdVoltas();

        corrida.adicionarResultado(
            Resultado(
                *estado.piloto,
                0,
                estado.tempo,
                terminou,
                0,
                estado.voltas,
                estado.ocorrencia
            )
        );
    }

    // ========================================
    // FINALIZACAO DA CORRIDA
    // ========================================

    // Classifica os pilotos, atribui pontos
    // e encerra oficialmente o GP.
    corrida.finalizar();
}
