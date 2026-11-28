
#include "Campeonato.hpp"
#include "funcionario.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// ==========================================
// LEITURA E VALIDACAO DE ENTRADAS
// ==========================================

std::string lerTexto(const std::string& mensagem) {
    while (true) {
        std::cout << mensagem;

        std::string texto;

        if (!std::getline(std::cin, texto)) {
            throw std::runtime_error("Entrada encerrada.");
        }

        if (texto.find_first_not_of(" \t\r\n") !=
            std::string::npos) {
            return texto;
        }

        std::cout << "O campo nao pode ficar vazio.\n";
    }
}

int lerInteiro(
    const std::string& mensagem,
    int minimo,
    int maximo
) {
    while (true) {
        const std::string entrada = lerTexto(mensagem);
        std::istringstream fluxo(entrada);

        int valor = 0;
        char extra = '\0';

        if ((fluxo >> valor) &&
            !(fluxo >> extra) &&
            valor >= minimo &&
            valor <= maximo) {
            return valor;
        }

        std::cout
            << "Digite um inteiro entre "
            << minimo << " e " << maximo << ".\n";
    }
}

double lerValor(
    const std::string& mensagem,
    double minimo,
    double maximo
) {
    while (true) {
        const std::string entrada = lerTexto(mensagem);
        std::istringstream fluxo(entrada);

        double valor = 0.0;
        char extra = '\0';

        if ((fluxo >> valor) &&
            !(fluxo >> extra) &&
            std::isfinite(valor) &&
            valor >= minimo &&
            valor <= maximo) {
            return valor;
        }

        std::cout
            << "Valor invalido. Intervalo permitido: "
            << minimo << " a " << maximo << ".\n";
    }
}

// ==========================================
// DADOS DO CAMPEONATO
// ==========================================

const char* nomeClima(int clima) {
    switch (clima) {
        case 1:
            return "Sol";
        case 2:
            return "Nublado";
        case 3:
            return "Chuva";
        default:
            return "Desconhecido";
    }
}

std::string dataReferencia(
    const Campeonato& campeonato
) {
    const Corrida* atual =
        campeonato.getCorridaAtual();

    if (atual != nullptr) {
        return atual->getData();
    }

    return campeonato.getCorrida(
        campeonato.quantidadeEtapas() - 1
    ).getData();
}

// ==========================================
// CALENDARIO
// ==========================================

void exibirCalendario(
    const Campeonato& campeonato
) {
    std::cout
        << "\n========== CALENDARIO ==========\n";

    for (
        std::size_t i = 0;
        i < campeonato.quantidadeEtapas();
        ++i
    ) {
        const Corrida& gp =
            campeonato.getCorrida(i);

        Circuito pista = gp.getCircuito();

        const char* estado =
            gp.getEstado() == EstadoCorrida::FINALIZADA
                ? "CONCLUIDO"
                : (i == campeonato.getEtapaAtual()
                    ? "ATUAL"
                    : "PENDENTE");

        std::cout
            << std::setw(2) << (i + 1) << ". "
            << std::left
            << std::setw(23) << pista.getNome()
            << std::right
            << " | " << gp.getData()
            << " | " << nomeClima(gp.getClima())
            << " | " << estado << '\n';
    }
}

void exibirProximaEtapa(
    const Campeonato& campeonato
) {
    const Corrida* gp =
        campeonato.getCorridaAtual();

    if (!gp) {
        std::cout << "Temporada encerrada.\n";
        return;
    }

    Circuito pista = gp->getCircuito();

    std::cout
        << "Etapa "
        << (campeonato.getEtapaAtual() + 1)
        << "/" << campeonato.quantidadeEtapas()
        << ": " << pista.getNome()
        << " (" << pista.getPais() << ")\n"
        << "Data: " << gp->getData()
        << " | Clima: " << nomeClima(gp->getClima())
        << " | Voltas: " << gp->getQtdVoltas()
        << '\n';

    if (gp->getEstado() ==
        EstadoCorrida::FINALIZADA) {
        std::cout
            << "GP concluido. Use a opcao de avancar etapa.\n";
    }
}

// ==========================================
// RESULTADOS DE CORRIDA
// ==========================================

void exibirResultado(const Corrida& gp) {
    if (gp.getEstado() != EstadoCorrida::FINALIZADA) {
        std::cout
            << "Este GP ainda nao foi disputado.\n";
        return;
    }

    std::cout
        << "\n===== RESULTADO: "
        << gp.getCircuito().getNome()
        << " =====\n";

    for (const Resultado& r : gp.getResultados()) {
        const std::string situacao =
            r.getDesclassificado() ? "DSQ" :
            (r.getCompletouCorrida() ? "OK" : "DNF");

        std::cout
            << std::setw(2) << r.getColocacao()
            << "  "
            << std::left << std::setw(23)
            << r.getPiloto().getNome()
            << std::right
            << " #" << std::setw(2)
            << r.getPiloto().getId()
            << " | " << std::setw(3)
            << r.getPontos() << " pts"
            << " | " << std::setw(2)
            << r.getVoltasConcluidas() << " voltas"
            << " | " << situacao << '\n';
    }
}

void exibirUltimoResultado(
    const Campeonato& campeonato
) {
    if (campeonato.quantidadeEtapas() == 0) {
        return;
    }

    std::size_t indice =
        campeonato.getEtapaAtual();

    if (indice == campeonato.quantidadeEtapas()) {
        --indice;
    }
    else if (
        campeonato.getCorrida(indice).getEstado() !=
        EstadoCorrida::FINALIZADA
    ) {
        if (indice == 0) {
            std::cout
                << "Nenhuma corrida disputada ate agora.\n";
            return;
        }

        --indice;
    }

    exibirResultado(
        campeonato.getCorrida(indice)
    );
}

// ==========================================
// CLASSIFICACAO
// ==========================================

void exibirClassificacao(
    const Campeonato& campeonato,
    bool construtores
) {
    const ClassificacaoCampeonato tabela =
        campeonato.calcularClassificacao();

    if (construtores) {
        std::cout
            << "\n===== CONSTRUTORES =====\n";

        int posicao = 1;

        for (const PosicaoEquipe& item :
             tabela.equipes) {
            std::cout
                << std::setw(2) << posicao++ << ". "
                << std::left << std::setw(18)
                << item.equipe << std::right
                << " | " << std::setw(4)
                << item.pontos << " pts"
                << " | " << item.vitorias
                << " vitorias\n";
        }
    }
    else {
        std::cout
            << "\n===== PILOTOS =====\n";

        int posicao = 1;

        for (const PosicaoPiloto& item :
             tabela.pilotos) {
            std::cout
                << std::setw(2) << posicao++ << ". "
                << std::left << std::setw(23)
                << item.piloto
                << std::setw(16) << item.equipe
                << std::right
                << " | " << std::setw(4)
                << item.pontos << " pts"
                << " | " << item.vitorias
                << " vitorias\n";
        }
    }
}

// ==========================================
// GARAGEM MERCEDES
// ==========================================

void exibirCarros(
    EquipeControlavel& mercedes
) {
    std::cout
        << "\n===== GARAGEM MERCEDES =====\n";

    for (
        const Piloto* piloto :
        mercedes.listarPilotos()
    ) {
        const Carro& carro =
            piloto->getCarro();

        std::cout
            << piloto->getNome()
            << " (#" << piloto->getId() << ")\n"
            << "  Modelo: " << carro.getModelo()
            << " | Desempenho: "
            << std::fixed << std::setprecision(2)
            << carro.calcularDesempenho()
            << " | Condicao: "
            << carro.getCondicao()
            << "%\n";
    }
}

Piloto& escolherPiloto(
    EquipeControlavel& mercedes
) {
    const auto pilotos =
        mercedes.listarPilotos();

    if (pilotos.empty()) {
        throw std::logic_error(
            "Mercedes sem pilotos."
        );
    }

    for (
        std::size_t i = 0;
        i < pilotos.size();
        ++i
    ) {
        std::cout
            << (i + 1) << ". "
            << pilotos[i]->getNome() << '\n';
    }

    const int opcao = lerInteiro(
        "Selecione o piloto: ",
        1,
        static_cast<int>(pilotos.size())
    );

    return *pilotos[
        static_cast<std::size_t>(opcao - 1)
    ];
}

// ==========================================
// MELHORIAS DE COMPONENTES
// ==========================================
void melhorarCarro(Campeonato& campeonato) {
    EquipeControlavel& mercedes =
        campeonato.getMercedes();

    Piloto& piloto =
        escolherPiloto(mercedes);

    const std::string componentes[] = {
        "motor",
        "freio",
        "suspensao",
        "chassi"
    };

    const double custos[] = {
        500000.0,
        250000.0,
        300000.0,
        400000.0
    };

    while (true) {
        std::cout
            << "\n1. Motor      (+2 pontos, custo 500000)\n"
            << "2. Freio      (+2 pontos, custo 250000)\n"
            << "3. Suspensao  (+2 pontos, custo 300000)\n"
            << "4. Chassi     (+2 pontos, custo 400000)\n"
            << "0. Voltar\n";

        const int opcao = lerInteiro(
            "Componente: ", 0, 4
        );

        if (opcao == 0) {
            break;
        }

        const std::size_t indice =
            static_cast<std::size_t>(opcao - 1);

        const bool realizada =
            mercedes.melhorarCarro(
                piloto.getId(),
                componentes[indice],
                2.0,
                custos[indice],
                dataReferencia(campeonato)
            );

        std::cout
            << (realizada
                ? "Melhoria concluida!\n"
                : "Compra recusada: saldo insuficiente ou peca no limite.\n");

        exibirCarros(mercedes);
    }
}
// ==========================================
// MANUTENCAO DOS CARROS
// ==========================================

void realizarManutencao(
    Campeonato& campeonato
) {
    EquipeControlavel& mercedes =
        campeonato.getMercedes();

    Piloto& piloto =
        escolherPiloto(mercedes);

    std::cout
        << "Condicao atual: "
        << piloto.getCarro().getCondicao()
        << "%\n"
        << "1. Revisao leve  (+10 pontos, custo 100000)\n"
        << "2. Revisao media (+25 pontos, custo 250000)\n"
        << "3. Revisao total (+50 pontos, custo 500000)\n";

    const int opcao = lerInteiro(
        "Manutencao: ", 1, 3
    );

    const double pontos[] = {
        10.0,
        25.0,
        50.0
    };

    const double custos[] = {
        100000.0,
        250000.0,
        500000.0
    };

    const std::size_t indice =
        static_cast<std::size_t>(opcao - 1);

    const bool realizada =
        mercedes.realizarManutencao(
            piloto.getId(),
            pontos[indice],
            custos[indice],
            dataReferencia(campeonato)
        );

    std::cout
        << (realizada
            ? "Manutencao concluida!\n"
            : "Manutencao recusada: saldo insuficiente ou carro conservado.\n");

    exibirCarros(mercedes);
}

// ==========================================
// CONTRATACAO DE FUNCIONARIOS
// ==========================================

void contratarFuncionario(
    EquipeControlavel& mercedes,
    std::vector<std::string>& contratados,
    int& proximoId
) {
    std::cout
        << "\n1. Engenheiro\n"
        << "2. Mecanico\n"
        << "3. Estrategista\n"
        << "4. Chefe de equipe\n";

    const int cargo = lerInteiro(
        "Cargo: ", 1, 4
    );

    const std::string nome =
        lerTexto("Nome: ");

    const int idade = lerInteiro(
        "Idade (18 a 100): ", 18, 100
    );

    const double salario = lerValor(
        "Salario mensal (1000 a 1000000): ",
        1000.0,
        1000000.0
    );

 // Todo funcionario contratado comeca sem tempo de servico.
const int tempo = 0;
    const Contrato contrato(
        12, salario, 0.0
    );

    const std::string id =
        "F" + std::to_string(proximoId);

    std::unique_ptr<Funcionario> funcionario;

    if (cargo == 1) {
        const std::string especialidade =
            lerTexto("Especialidade: ");

        funcionario =
            std::make_unique<Engenheiro>(
                nome,
                id,
                idade,
                contrato,
                tempo,
                especialidade
            );
    }
    else if (cargo == 2) {
        const int habilidade = lerInteiro(
            "Habilidade (0 a 100): ",
            0, 100
        );

        funcionario =
            std::make_unique<Mecanico>(
                nome,
                id,
                idade,
                contrato,
                tempo,
                habilidade
            );
    }
    else if (cargo == 3) {
        const int experiencia = lerInteiro(
            "Experiencia (0 a 50): ",
            0, 50
        );

        funcionario =
            std::make_unique<Estrategista>(
                nome,
                id,
                idade,
                contrato,
                tempo,
                experiencia
            );
    }
    else {
        const int experiencia = lerInteiro(
            "Experiencia (0 a 50): ",
            0, 50
        );

        funcionario =
            std::make_unique<ChefeEquipe>(
                nome,
                id,
                idade,
                contrato,
                tempo,
                experiencia
            );
    }

    // A propriedade do funcionario passa para a Mercedes.
    mercedes.cadastrarFuncionario(
        std::move(funcionario)
    );

    contratados.push_back(id);
    ++proximoId;

    std::cout
        << "Funcionario contratado! ID: "
        << id
        << ". O salario entrara na folha mensal.\n";
}

// ==========================================
// CONSULTA DE FUNCIONARIOS
// ==========================================

void exibirFuncionarios(
    const EquipeControlavel& mercedes,
    const std::vector<std::string>& contratados
) {
    std::cout
        << "\n===== FUNCIONARIOS MERCEDES =====\n";

    if (contratados.empty()) {
        std::cout
            << "Nenhum funcionario contratado.\n";
    }

    for (const std::string& id : contratados) {
        const Funcionario* funcionario =
            mercedes.buscarFuncionario(id);

        if (funcionario != nullptr) {
            std::cout
                << funcionario->getNome()
                << " (" << funcionario->getCargo()
                << ") | ID " << id
                << " | salario "
                << funcionario->getContrato().getSalario()
                << " | impacto "
                << funcionario->calcularImpacto()
                << '\n';
        }
    }

    std::cout
        << "Total de funcionarios: "
        << mercedes.quantidadeFuncionarios()
        << '\n'
        << "Folha salarial total (pilotos + funcionarios): "
        << mercedes.folhaSalarialMensal()
        << '\n'
        << "Bonus estrategico: "
        << mercedes.calcularBonusEstrategia()
        << '\n';
}

// ==========================================
// PAGAMENTO DE SALARIOS
// ==========================================

void pagarSalarios(
    Campeonato& campeonato
) {
    EquipeControlavel& mercedes =
        campeonato.getMercedes();

    const std::string competencia =
        dataReferencia(campeonato).substr(0, 7);

    std::cout
        << "Competencia: " << competencia
        << " | Folha: "
        << mercedes.folhaSalarialMensal()
        << '\n';

    if (mercedes.folhaJaPaga(competencia)) {
        std::cout
            << "Esta competencia ja foi paga.\n";
        return;
    }

    if (mercedes.pagarFolhaMensal(competencia)) {
        std::cout
            << "Folha paga! Novo saldo: "
            << mercedes.getFinanceiro().getSaldo()
            << '\n';
    }
    else {
        std::cout
            << "Pagamento nao realizado: saldo insuficiente ou folha vazia.\n";
    }
}

// ==========================================
// FINANCEIRO
// ==========================================

void exibirFinanceiro(
    const EquipeControlavel& mercedes
) {
    const Financeiro& financeiro =
        mercedes.getFinanceiro();

    std::cout
        << "\n===== FINANCEIRO MERCEDES =====\n"
        << "Orcamento inicial: "
        << financeiro.getOrcamento() << '\n'
        << "Saldo atual: "
        << financeiro.getSaldo() << '\n'
        << "Receitas: "
        << financeiro.getReceitasTotais() << '\n'
        << "Despesas: "
        << financeiro.getDespesasTotais() << '\n';

    if (financeiro.quantidadeTransacoes() == 0) {
        std::cout
            << "Sem transacoes registradas.\n";
    }
    else {
        mercedes.gerenciarFinanceiro();
    }
}

// ==========================================
// MENU PRINCIPAL
// ==========================================

void exibirMenu(
    const Campeonato& campeonato
) {
    std::cout
        << "\n==========================================\n"
        << "       F1 MANAGER - MERCEDES 2026\n"
        << "==========================================\n";

    exibirProximaEtapa(campeonato);

    std::cout
        << "------------------------------------------\n"
        << " 1. Calendario completo\n"
        << " 2. Simular GP atual\n"
        << " 3. Avancar para a proxima etapa\n"
        << " 4. Resultado do ultimo GP\n"
        << " 5. Classificacao de pilotos\n"
        << " 6. Classificacao de construtores\n"
        << " 7. Ver carros da Mercedes\n"
        << " 8. Melhorar componente\n"
        << " 9. Realizar manutencao\n"
        << "10. Contratar funcionario\n"
        << "11. Consultar funcionarios e salarios\n"
        << "12. Pagar folha do mes atual\n"
        << "13. Financeiro e transacoes\n"
        << " 0. Sair\n";
}

} // namespace

// ==========================================
// EXECUCAO PRINCIPAL
// ==========================================

int main() {
    try {
        Campeonato campeonato(2026);

        campeonato.carregarDadosPredefinidos();

        SimuladorDesempenho simulador;

        // IDs usados para apresentar as contratacoes realizadas no menu.
        std::vector<std::string> contratados;

        int proximoIdFuncionario = 1;

        std::cout
            << "Bem-vindo ao F1 Manager!\n"
            << "Mercedes e a unica equipe controlavel.\n"
            << "Os atributos e os resultados do campeonato sao ficticios.\n";

        while (true) {
            exibirMenu(campeonato);

            const int opcao = lerInteiro(
                "Escolha: ", 0, 13
            );

            if (opcao == 0) {
                break;
            }

            try {
                switch (opcao) {

                    case 1:
                        exibirCalendario(campeonato);
                        break;

                    case 2: {
                        const Corrida* atual =
                            campeonato.getCorridaAtual();

                        if (atual == nullptr) {
                            std::cout
                                << "A temporada ja terminou.\n";
                        }
                        else if (
                            atual->getEstado() ==
                            EstadoCorrida::FINALIZADA
                        ) {
                            std::cout
                                << "GP ja disputado. Avance para a proxima etapa.\n";
                        }
                        else {
                            campeonato.simularEtapa(simulador);

                            exibirResultado(
                                campeonato.getCorrida(
                                    campeonato.getEtapaAtual()
                                )
                            );
                        }

                        break;
                    }

                    case 3:
                        campeonato.avancarEtapa();

                        if (campeonato.temporadaFinalizada()) {
                            std::cout
                                << "\nTEMPORADA ENCERRADA!\n";

                            exibirClassificacao(
                                campeonato, false
                            );

                            exibirClassificacao(
                                campeonato, true
                            );
                        }
                        else {
                            std::cout
                                << "Etapa avancada com sucesso.\n";
                        }

                        break;

                    case 4:
                        exibirUltimoResultado(campeonato);
                        break;

                    case 5:
                        exibirClassificacao(
                            campeonato, false
                        );
                        break;

                    case 6:
                        exibirClassificacao(
                            campeonato, true
                        );
                        break;

                    case 7:
                        exibirCarros(
                            campeonato.getMercedes()
                        );
                        break;

                    case 8:
                        melhorarCarro(campeonato);
                        break;

                    case 9:
                        realizarManutencao(campeonato);
                        break;

                    case 10:
                        contratarFuncionario(
                            campeonato.getMercedes(),
                            contratados,
                            proximoIdFuncionario
                        );
                        break;

                    case 11:
                        exibirFuncionarios(
                            campeonato.getMercedes(),
                            contratados
                        );
                        break;

                    case 12:
                        pagarSalarios(campeonato);
                        break;

                    case 13:
                        exibirFinanceiro(
                            campeonato.getMercedes()
                        );
                        break;

                    default:
                        break;
                }
            }
            catch (const std::exception& erro) {
                std::cout
                    << "Operacao nao realizada: "
                    << erro.what() << '\n';
            }
        }

        std::cout
            << "Programa encerrado. "
            << "O salvamento de partidas ainda nao foi implementado.\n";
    }
    catch (const std::exception& erro) {
        std::cerr
            << "Falha ao iniciar/executar o jogo: "
            << erro.what() << '\n';

        return 1;
    }

    return 0;
}
