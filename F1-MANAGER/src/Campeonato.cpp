
#include "Campeonato.hpp"
#include "EquipeAdversaria.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

namespace {

struct PilotoDados {
    const char* nome;
    const char* numero;
    int idade;
    double habilidade;
    double consistencia;
};

struct EquipeDados {
    const char* nome;
    int nivelTecnico;
    int nivelEstrategico;
    double qualidadeCarro;
    PilotoDados primeiro;
    PilotoDados segundo;
};

struct EtapaDados {
    const char* data;
    const char* circuito;
    const char* pais;
    double extensaoKm;
    int curvas;
    int clima;
    int voltas;
};

// Nomes e numeros representam um grid de referencia de 2026.
// As notas de desempenho, as datas e o clima sao ficticios.
const std::array<EquipeDados, 11> EQUIPES = {{
    {
        "Mercedes", 91, 90, 91,
        {"George Russell", "63", 28, 93, 91},
        {"Kimi Antonelli", "12", 20, 89, 86}
    },
    {
        "McLaren", 94, 93, 93,
        {"Lando Norris", "1", 27, 94, 92},
        {"Oscar Piastri", "81", 25, 93, 92}
    },
    {
        "Ferrari", 92, 90, 92,
        {"Charles Leclerc", "16", 29, 94, 91},
        {"Lewis Hamilton", "44", 41, 94, 92}
    },
    {
        "Red Bull", 93, 92, 92,
        {"Max Verstappen", "3", 29, 97, 95},
        {"Isack Hadjar", "6", 22, 87, 85}
    },
    {
        "Aston Martin", 83, 84, 84,
        {"Fernando Alonso", "14", 45, 92, 93},
        {"Lance Stroll", "18", 28, 81, 81}
    },
    {
        "Alpine", 79, 80, 79,
        {"Pierre Gasly", "10", 30, 88, 87},
        {"Franco Colapinto", "43", 23, 84, 83}
    },
    {
        "Williams", 84, 83, 85,
        {"Alex Albon", "23", 30, 88, 87},
        {"Carlos Sainz", "55", 32, 91, 90}
    },
    {
        "Haas", 80, 78, 80,
        {"Esteban Ocon", "31", 30, 86, 85},
        {"Oliver Bearman", "87", 21, 85, 82}
    },
    {
        "Racing Bulls", 81, 81, 81,
        {"Liam Lawson", "30", 24, 85, 83},
        {"Arvid Lindblad", "41", 19, 82, 80}
    },
    {
        "Audi", 82, 83, 82,
        {"Nico Hulkenberg", "27", 39, 87, 89},
        {"Gabriel Bortoleto", "5", 22, 85, 85}
    },
    {
        "Cadillac", 75, 76, 76,
        {"Sergio Perez", "11", 36, 85, 85},
        {"Valtteri Bottas", "77", 37, 86, 87}
    }
}};

// Calendario de 24 GPs do jogo; as datas nao sao oficiais.
const std::array<EtapaDados, 24> ETAPAS = {{
    {"2026-03-08", "Albert Park", "Australia", 5.278, 14, 1, 20},
    {"2026-03-22", "Shanghai", "China", 5.451, 16, 2, 20},
    {"2026-04-05", "Suzuka", "Japao", 5.807, 18, 1, 20},
    {"2026-04-19", "Sakhir", "Bahrein", 5.412, 15, 1, 20},
    {"2026-05-03", "Jeddah", "Arabia Saudita", 6.174, 27, 1, 20},
    {"2026-05-17", "Miami", "Estados Unidos", 5.412, 19, 2, 20},
    {"2026-05-31", "Gilles Villeneuve", "Canada", 4.361, 14, 3, 20},
    {"2026-06-14", "Monaco", "Monaco", 3.337, 19, 1, 20},
    {"2026-06-28", "Barcelona", "Espanha", 4.657, 14, 1, 20},
    {"2026-07-05", "Red Bull Ring", "Austria", 4.318, 10, 2, 20},
    {"2026-07-19", "Silverstone", "Reino Unido", 5.891, 18, 3, 20},
    {"2026-08-02", "Spa-Francorchamps", "Belgica", 7.004, 19, 2, 20},
    {"2026-08-16", "Hungaroring", "Hungria", 4.381, 14, 1, 20},
    {"2026-08-30", "Zandvoort", "Paises Baixos", 4.259, 14, 2, 20},
    {"2026-09-06", "Monza", "Italia", 5.793, 11, 1, 20},
    {"2026-09-13", "Baku", "Azerbaijao", 6.003, 20, 1, 20},
    {"2026-09-27", "Marina Bay", "Singapura", 4.940, 19, 3, 20},
    {"2026-10-11", "COTA", "Estados Unidos", 5.513, 20, 1, 20},
    {"2026-10-18", "Hermanos Rodriguez", "Mexico", 4.304, 17, 2, 20},
    {"2026-11-01", "Interlagos", "Brasil", 4.309, 15, 3, 20},
    {"2026-11-08", "Las Vegas", "Estados Unidos", 6.201, 17, 1, 20},
    {"2026-11-15", "Lusail", "Catar", 5.419, 16, 1, 20},
    {"2026-11-22", "Yas Marina", "Emirados Arabes", 5.281, 16, 2, 20},
    {"2026-12-06", "Portimao", "Portugal", 4.653, 15, 1, 20}
}};

// ==========================================
// CRIACAO DOS CARROS E PILOTOS
// ==========================================

Carro criarCarro(
    const std::string& nome,
    int ano,
    double qualidade
) {
    return Carro(
        nome + " 2026",
        ano,
        120.0,
        f1::Motor(std::min(100.0, qualidade + 2.0)),
        f1::Freio(qualidade),
        f1::Suspensao(qualidade),
        f1::Chassi(std::max(0.0, qualidade - 1.0))
    );
}

void cadastrarDupla(
    Equipe& equipe,
    int ano,
    const EquipeDados& dados
) {
    for (const PilotoDados& piloto :
         {dados.primeiro, dados.segundo}) {

        equipe.cadastrarPiloto(
            Piloto(
                piloto.nome,
                piloto.numero,
                piloto.idade,
                criarCarro(
                    dados.nome,
                    ano,
                    dados.qualidadeCarro
                ),
                12,
                10000.0,
                0.0,
                piloto.habilidade,
                piloto.consistencia
            )
        );
    }
}

} // namespace

// ==========================================
// CONSTRUTOR
// ==========================================

Campeonato::Campeonato(int temporada)
    : _temporada(temporada) {

    if (temporada < 1950 || temporada > 2100) {
        throw std::invalid_argument(
            "Temporada invalida."
        );
    }
}

// ==========================================
// CARREGAR DADOS PREDEFINIDOS
// ==========================================

void Campeonato::carregarDadosPredefinidos() {
    if (!_equipes.empty() || !_corridas.empty()) {
        throw std::logic_error(
            "Os dados do campeonato ja foram carregados."
        );
    }

    std::vector<std::unique_ptr<Equipe>> equipes;
    std::vector<std::unique_ptr<Corrida>> corridas;

    equipes.reserve(EQUIPES.size());
    corridas.reserve(ETAPAS.size());

    // Cadastra a Mercedes como unica equipe controlavel.
    for (const EquipeDados& dados : EQUIPES) {
        std::unique_ptr<Equipe> equipe;

        if (std::string(dados.nome) == "Mercedes") {
            equipe = std::make_unique<EquipeControlavel>(
                dados.nome,
                100000000.0
            );
        }
        else {
            equipe = std::make_unique<EquipeAdversaria>(
                dados.nome,
                dados.nivelTecnico,
                dados.nivelEstrategico
            );
        }

        cadastrarDupla(
            *equipe,
            _temporada,
            dados
        );

        equipes.push_back(std::move(equipe));
    }

    // Cria as etapas antes de iniciar o campeonato.
    for (const EtapaDados& dados : ETAPAS) {
        corridas.push_back(
            std::make_unique<Corrida>(
                dados.data,
                Circuito(
                    dados.circuito,
                    dados.pais,
                    dados.extensaoKm,
                    dados.curvas
                ),
                dados.clima,
                dados.voltas
            )
        );
    }

    // A troca e realizada somente quando todos os objetos foram criados.
    _equipes.swap(equipes);
    _corridas.swap(corridas);

    _etapaAtual = 0;
}

// ==========================================
// SIMULACAO DA ETAPA ATUAL
// ==========================================

void Campeonato::simularEtapa(
    SimuladorDesempenho& simulador
) {
    if (_corridas.empty() || temporadaFinalizada()) {
        throw std::logic_error(
            "Nao existe etapa disponivel para simular."
        );
    }

    Corrida& gp = *_corridas.at(_etapaAtual);

    if (gp.getEstado() != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "A etapa atual ja foi disputada."
        );
    }

    std::unordered_set<std::string> ids;
    std::vector<Equipe*> participantes;

    participantes.reserve(_equipes.size());

    // Confere todo o grid antes de inscrever qualquer piloto.
    for (const auto& equipe : _equipes) {
        if (equipe->quantidadePilotos() != 2) {
            throw std::logic_error(
                "Cada equipe precisa de dois pilotos."
            );
        }

        participantes.push_back(equipe.get());
    }

    // Valida o grid atual sem depender de numeros fixos.
    std::vector<Piloto*> pilotos;

    pilotos.reserve(participantes.size() * 2);

    for (Equipe* equipe : participantes) {
        for (Piloto* piloto : equipe->listarPilotos()) {
            if (
                piloto == nullptr ||
                !ids.insert(piloto->getId()).second
            ) {
                throw std::logic_error(
                    "Grid com piloto nulo ou ID repetido."
                );
            }

            pilotos.push_back(piloto);
        }
    }

    if (pilotos.size() != _equipes.size() * 2) {
        throw std::logic_error(
            "Grid incompleto."
        );
    }

    // Inscreve os pilotos originais na corrida.
    for (Piloto* piloto : pilotos) {
        gp.adicionarPiloto(*piloto);
    }

    // Executa estrategia, GP e desgaste permanente.
    GestorEtapa::executar(
        gp,
        simulador,
        participantes
    );
}

// ==========================================
// AVANCAR ETAPA
// ==========================================

void Campeonato::avancarEtapa() {
    if (_corridas.empty() || temporadaFinalizada()) {
        throw std::logic_error(
            "Nao existe proxima etapa."
        );
    }

    if (
        _corridas.at(_etapaAtual)->getEstado()
        != EstadoCorrida::FINALIZADA
    ) {
        throw std::logic_error(
            "Simule a etapa atual antes de avancar."
        );
    }

    ++_etapaAtual;
}

// ==========================================
// CLASSIFICACAO DO CAMPEONATO
// ==========================================

ClassificacaoCampeonato
Campeonato::calcularClassificacao() const {
    ClassificacaoCampeonato classificacao;

    for (const auto& equipe : _equipes) {
        int pontosEquipe = 0;
        int vitoriasEquipe = 0;

        // Corrida::finalizar() ja atualiza os pontos dos pilotos.
        for (const Piloto* piloto : equipe->listarPilotos()) {
            classificacao.pilotos.push_back({
                piloto->getNome(),
                piloto->getId(),
                equipe->getNome(),
                piloto->getPontos(),
                piloto->getVitorias()
            });

            pontosEquipe += piloto->getPontos();
            vitoriasEquipe += piloto->getVitorias();
        }

        classificacao.equipes.push_back({
            equipe->getNome(),
            pontosEquipe,
            vitoriasEquipe
        });
    }

    // Ordena pilotos por pontos, vitorias e nome.
    std::sort(
        classificacao.pilotos.begin(),
        classificacao.pilotos.end(),
        [](const PosicaoPiloto& a, const PosicaoPiloto& b) {
            if (a.pontos != b.pontos) {
                return a.pontos > b.pontos;
            }

            if (a.vitorias != b.vitorias) {
                return a.vitorias > b.vitorias;
            }

            return a.piloto < b.piloto;
        }
    );

    // Ordena as equipes pelos mesmos criterios.
    std::sort(
        classificacao.equipes.begin(),
        classificacao.equipes.end(),
        [](const PosicaoEquipe& a, const PosicaoEquipe& b) {
            if (a.pontos != b.pontos) {
                return a.pontos > b.pontos;
            }

            if (a.vitorias != b.vitorias) {
                return a.vitorias > b.vitorias;
            }

            return a.equipe < b.equipe;
        }
    );

    return classificacao;
}

// ==========================================
// GETTERS
// ==========================================

int Campeonato::getTemporada() const {
    return _temporada;
}

std::size_t Campeonato::getEtapaAtual() const {
    return _etapaAtual;
}

std::size_t Campeonato::quantidadeEtapas() const {
    return _corridas.size();
}

std::size_t Campeonato::quantidadeEquipes() const {
    return _equipes.size();
}

bool Campeonato::temporadaFinalizada() const {
    return !_corridas.empty() &&
           _etapaAtual >= _corridas.size();
}

// ==========================================
// BUSCA DE EQUIPES
// ==========================================

Equipe* Campeonato::buscarEquipe(
    const std::string& nome
) {
    for (auto& equipe : _equipes) {
        if (equipe->getNome() == nome) {
            return equipe.get();
        }
    }

    return nullptr;
}

const Equipe* Campeonato::buscarEquipe(
    const std::string& nome
) const {
    for (const auto& equipe : _equipes) {
        if (equipe->getNome() == nome) {
            return equipe.get();
        }
    }

    return nullptr;
}

Equipe& Campeonato::getEquipe(
    std::size_t indice
) {
    return *_equipes.at(indice);
}

const Equipe& Campeonato::getEquipe(
    std::size_t indice
) const {
    return *_equipes.at(indice);
}

// ==========================================
// ACESSO A MERCEDES
// ==========================================

EquipeControlavel& Campeonato::getMercedes() {
    auto* mercedes = dynamic_cast<EquipeControlavel*>(
        buscarEquipe("Mercedes")
    );

    if (mercedes == nullptr) {
        throw std::logic_error(
            "Mercedes nao cadastrada."
        );
    }

    return *mercedes;
}

const EquipeControlavel&
Campeonato::getMercedes() const {
    auto* mercedes =
        dynamic_cast<const EquipeControlavel*>(
            buscarEquipe("Mercedes")
        );

    if (mercedes == nullptr) {
        throw std::logic_error(
            "Mercedes nao cadastrada."
        );
    }

    return *mercedes;
}

// ==========================================
// CONSULTA DE CORRIDAS
// ==========================================

const Corrida& Campeonato::getCorrida(
    std::size_t indice
) const {
    return *_corridas.at(indice);
}

const Corrida* Campeonato::getCorridaAtual() const {
    if (temporadaFinalizada() || _corridas.empty()) {
        return nullptr;
    }

    return _corridas.at(_etapaAtual).get();
}
