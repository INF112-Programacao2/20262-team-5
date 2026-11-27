
#ifndef CAMPEONATO_HPP
#define CAMPEONATO_HPP

#include "EquipeControlavel.hpp"
#include "GestorEtapa.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

struct PosicaoPiloto {
    std::string piloto;
    std::string id;
    std::string equipe;
    int pontos;
    int vitorias;
};

struct PosicaoEquipe {
    std::string equipe;
    int pontos;
    int vitorias;
};

struct ClassificacaoCampeonato {
    std::vector<PosicaoPiloto> pilotos;
    std::vector<PosicaoEquipe> equipes;
};

// Coordena as equipes, os GPs e as classificacoes de uma temporada.
class Campeonato {
private:
    int _temporada;
    std::size_t _etapaAtual = 0;

    // As corridas usam ponteiros para os pilotos pertencentes as equipes.
    // A ordem dos atributos garante que as corridas sejam destruidas primeiro.
    std::vector<std::unique_ptr<Equipe>> _equipes;
    std::vector<std::unique_ptr<Corrida>> _corridas;

public:
    explicit Campeonato(int temporada = 2026);

    // Evita copias de objetos que possuem recursos e ponteiros associados.
    Campeonato(const Campeonato&) = delete;
    Campeonato& operator=(const Campeonato&) = delete;

    void carregarDadosPredefinidos();

    void simularEtapa(
        SimuladorDesempenho& simulador
    );

    void avancarEtapa();

    ClassificacaoCampeonato calcularClassificacao() const;

    int getTemporada() const;
    std::size_t getEtapaAtual() const;
    std::size_t quantidadeEtapas() const;
    std::size_t quantidadeEquipes() const;

    bool temporadaFinalizada() const;

    Equipe* buscarEquipe(
        const std::string& nome
    );

    const Equipe* buscarEquipe(
        const std::string& nome
    ) const;

    Equipe& getEquipe(
        std::size_t indice
    );

    const Equipe& getEquipe(
        std::size_t indice
    ) const;

    EquipeControlavel& getMercedes();
    const EquipeControlavel& getMercedes() const;

    const Corrida& getCorrida(
        std::size_t indice
    ) const;

    const Corrida* getCorridaAtual() const;
};

#endif
