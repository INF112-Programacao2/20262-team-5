
#include "corrida.hpp"
#include "piloto.hpp"
#include "RegraPontuacao.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {

// Valida os valores de clima utilizados no projeto.
void validarClima(int clima) {
    if (clima < 1 || clima > 3) {
        throw std::invalid_argument(
            "Clima invalido: use 1, 2 ou 3."
        );
    }
}

// Ordenacao por classificacao:
// 1. Pilotos que completaram a corrida.
// 2. Pilotos que abandonaram (DNF).
// 3. Pilotos desclassificados (DSQ).
//
// Empates sao resolvidos pelo ID do piloto.
bool melhorResultado(const Resultado& a, const Resultado& b) {
    if (a.getDesclassificado() != b.getDesclassificado()) {
        return !a.getDesclassificado();
    }

    if (a.getCompletouCorrida() != b.getCompletouCorrida()) {
        return a.getCompletouCorrida();
    }

    if (a.getVoltasConcluidas() != b.getVoltasConcluidas()) {
        return a.getVoltasConcluidas() > b.getVoltasConcluidas();
    }

    if (a.getTempo() != b.getTempo()) {
        return a.getTempo() < b.getTempo();
    }

    return a.getPiloto().getId() < b.getPiloto().getId();
}

} // namespace

Corrida::Corrida(
    std::string data,
    Circuito circuito,
    int clima,
    int qtdVoltas
)
    : _data(std::move(data)),
      _circuito(std::move(circuito)),
      _clima(clima),
      _qtdVoltas(qtdVoltas) {

    validarClima(_clima);

    if (_data.empty() || _qtdVoltas < 1 || _qtdVoltas > 500) {
        throw std::invalid_argument(
            "Data ou numero de voltas invalido."
        );
    }
}

// ===============================
// GETTERS E SETTERS
// ===============================

Resultado Corrida::getResultado(int indice) const {
    if (indice < 0) {
        throw std::out_of_range(
            "Indice de resultado negativo."
        );
    }

    return _resultados.at(
        static_cast<std::size_t>(indice)
    );
}

void Corrida::setResultado(Resultado resultado, int indice) {
    if (_estado != EstadoCorrida::EM_ANDAMENTO) {
        throw std::logic_error(
            "Resultados so podem ser editados durante a corrida."
        );
    }

    if (indice < 0) {
        throw std::out_of_range(
            "Indice de resultado negativo."
        );
    }

    Resultado& atual = _resultados.at(
        static_cast<std::size_t>(indice)
    );

    if (&atual.getPiloto() != &resultado.getPiloto() ||
        resultado.getColocacao() != 0 ||
        resultado.getPontos() != 0 ||
        resultado.getVoltasConcluidas() > _qtdVoltas ||
        (resultado.getCompletouCorrida() &&
         resultado.getVoltasConcluidas() != _qtdVoltas)) {

        throw std::invalid_argument(
            "Substituicao de resultado invalida."
        );
    }

    atual = std::move(resultado);
}

const std::vector<Resultado>& Corrida::getResultados() const {
    return _resultados;
}

std::string Corrida::getData() const {
    return _data;
}

void Corrida::setData(std::string data) {
    if (_estado != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Nao e possivel alterar data apos o inicio."
        );
    }

    if (data.empty()) {
        throw std::invalid_argument("Data vazia.");
    }

    _data = std::move(data);
}

Circuito Corrida::getCircuito() const {
    return _circuito;
}

void Corrida::setCircuito(Circuito circuito) {
    if (_estado != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Nao e possivel alterar circuito apos o inicio."
        );
    }

    _circuito = std::move(circuito);
}

int Corrida::getClima() const {
    return _clima;
}

void Corrida::setClima(int clima) {
    if (_estado != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Nao e possivel alterar clima apos o inicio."
        );
    }

    validarClima(clima);
    _clima = clima;
}

int Corrida::getQtdVoltas() const {
    return _qtdVoltas;
}

EstadoCorrida Corrida::getEstado() const {
    return _estado;
}

// ===============================
// GERENCIAMENTO DE PILOTOS
// ===============================

void Corrida::adicionarPiloto(Piloto& piloto) {
    if (_estado != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Inscricoes encerradas."
        );
    }

    // Impede cadastrar o mesmo piloto duas vezes.
    const auto jaExiste = std::any_of(
        _inscritos.begin(),
        _inscritos.end(),

        [&piloto](const Piloto* p) {
            return p->getId() == piloto.getId();
        }
    );

    if (jaExiste) {
        throw std::invalid_argument(
            "Piloto ja inscrito."
        );
    }

    _inscritos.push_back(&piloto);
}

const std::vector<Piloto*>& Corrida::getPilotos() const {
    return _inscritos;
}

void Corrida::adicionarResultado(Resultado resultado) {
    if (_estado != EstadoCorrida::EM_ANDAMENTO) {
        throw std::logic_error(
            "Corrida nao esta em andamento."
        );
    }

    const std::string& id = resultado.getPiloto().getId();

    // Confirma que o resultado pertence a um piloto inscrito.
    const bool inscrito = std::any_of(
        _inscritos.begin(),
        _inscritos.end(),

        [&resultado](const Piloto* p) {
            return p == &resultado.getPiloto();
        }
    );

    const bool repetido = std::any_of(
        _resultados.begin(),
        _resultados.end(),

        [&id](const Resultado& r) {
            return r.getPiloto().getId() == id;
        }
    );

    if (!inscrito || repetido ||
        resultado.getPontos() != 0 ||
        resultado.getColocacao() != 0 ||
        resultado.getVoltasConcluidas() > _qtdVoltas ||
        (resultado.getCompletouCorrida() &&
         resultado.getVoltasConcluidas() != _qtdVoltas)) {

        throw std::invalid_argument(
            "Resultado invalido, repetido ou nao inscrito."
        );
    }

    _resultados.push_back(std::move(resultado));
}

// ===============================
// COEFICIENTES DE DESEMPENHO
// ===============================

//Todos os calculos estao sujeitos a alteracoes, isso eh um teste de principios
double Corrida::calcularCoeficienteTracao() {
    double coeficiente = 1.0;

    if (_clima == 3) { //clima chuvoso
        coeficiente += 0.5;
    }

    coeficiente += 0.01 * _circuito.getCurvas();

    return coeficiente;
}

double Corrida::calcularCoeficienteVelocidade() {
    double coeficiente = 1.0;

    if (_clima == 2) { //clima nublado
        coeficiente -= 0.25;
    }

    coeficiente += 0.05 * _circuito.getExtensao();
    coeficiente -= 0.01 * _circuito.getCurvas();

    return coeficiente;
}

double Corrida::calcularCoeficienteAceleracao() {
    double coeficiente = 1.0;

    coeficiente -= 0.001 * _circuito.getExtensao();
    coeficiente += 0.03 * _circuito.getCurvas();

    return coeficiente;
}

// ===============================
// ORDENACAO DOS RESULTADOS
// ===============================

//Ordena os pilotos - vai ser util no relatorio
void Corrida::ordenarPilotosCrescente() {
    if (_estado != EstadoCorrida::FINALIZADA) {
        throw std::logic_error(
            "Classificacao ainda nao definida."
        );
    }

    std::stable_sort(
        _resultados.begin(),
        _resultados.end(),

        [](const Resultado& a, const Resultado& b) {
            const int pa = a.getColocacao();
            const int pb = b.getColocacao();

            // DSQ (posicao 0) fica no final.
            if (pa == 0 || pb == 0) {
                return pb == 0 && pa != 0;
            }

            return pa < pb;
        }
    );
}

void Corrida::ordenarPilotosDecrescente() {
    if (_estado != EstadoCorrida::FINALIZADA) {
        throw std::logic_error(
            "Classificacao ainda nao definida."
        );
    }

    std::stable_sort(
        _resultados.begin(),
        _resultados.end(),

        [](const Resultado& a, const Resultado& b) {
            const int pa = a.getColocacao();
            const int pb = b.getColocacao();

            // DSQ continua no final.
            if (pa == 0 || pb == 0) {
                return pb == 0 && pa != 0;
            }

            return pa > pb;
        }
    );
}

// ===============================
// DESCLASSIFICACOES
// ===============================

//Se por algum motivo um corredor nao terminar a corrida, deve ser desconsiderado
void Corrida::desconsiderarDesclassificados() {
    // Nao apagamos registros: DNF e DSQ permanecem
    // para consultas e relatorios futuros.
    //
    // O metodo finalizar() atribui 0 ponto a quem
    // nao completou a prova ou foi desclassificado.
}

// ===============================
// EXIBICAO DOS RESULTADOS
// ===============================

//Vai ser usado no relatorio
void Corrida::exibirResultados() const {
    for (const Resultado& r : _resultados) {
        std::cout
            << "Piloto: " << r.getPiloto().getNome() << '\n'
            << "Posicao: "
            << (r.getDesclassificado()
                ? 0 : r.getColocacao()) << '\n'

            << std::fixed << std::setprecision(2)
            << "Tempo: " << r.getTempo() << " s\n"

            << "Voltas: "
            << r.getVoltasConcluidas()
            << '/' << _qtdVoltas << '\n'

            << "Pontos: " << r.getPontos() << '\n'

            << "Status: "
            << (r.getDesclassificado() ? "DSQ" :
                (r.getCompletouCorrida()
                    ? "Finalizou" : "DNF"))

            << " - " << r.getOcorrencia()
            << "\n\n";
    }
}

// ===============================
// INICIAR CORRIDA
// ===============================

void Corrida::iniciar() {
    if (_estado != EstadoCorrida::NAO_INICIADA) {
        throw std::logic_error(
            "Corrida ja iniciada ou encerrada."
        );
    }

    if (_inscritos.size() < 2) {
        throw std::logic_error(
            "Inscreva ao menos dois pilotos."
        );
    }

    _resultados.clear();

    _estado = EstadoCorrida::EM_ANDAMENTO;
}

// ===============================
// FINALIZAR CORRIDA
// ===============================

void Corrida::finalizar() {
    if (_estado != EstadoCorrida::EM_ANDAMENTO) {
        throw std::logic_error(
            "Corrida nao esta em andamento."
        );
    }

    if (_resultados.size() != _inscritos.size()) {
        throw std::logic_error(
            "Ainda faltam resultados de pilotos."
        );
    }

    // Calcula a classificacao em uma copia.
    // Assim, falhas de validacao nao alteram o historico.
    auto classificados = _resultados;

    std::stable_sort(
        classificados.begin(),
        classificados.end(),
        melhorResultado
    );

    int posicao = 1;

    for (Resultado& r : classificados) {
        const bool dsq = r.getDesclassificado();

        r.setColocacao(
            dsq ? 0 : posicao++
        );

        const int pontos =
            (r.getCompletouCorrida() && !dsq)
            ? RegraPontuacao::calcularPontos(
                r.getColocacao()
              )
            : 0;

        r.setPontos(pontos);

        // Verifica overflow antes de atualizar estatisticas.
        const Piloto& piloto = r.getPiloto();

        if (pontos >
            std::numeric_limits<int>::max() -
            piloto.getPontos()) {

            throw std::overflow_error(
                "Pontos do piloto excedem o limite."
            );
        }

        if (r.getColocacao() == 1 &&
            r.getCompletouCorrida() &&
            piloto.getVitorias() ==
            std::numeric_limits<int>::max()) {

            throw std::overflow_error(
                "Vitorias do piloto excedem o limite."
            );
        }
    }

    // Atualiza os pontos e as vitorias somente
    // depois que todos os resultados foram validados.
    for (const Resultado& r : classificados) {
        for (Piloto* piloto : _inscritos) {
            if (piloto == &r.getPiloto()) {

                piloto->registrarResultado(
                    r.getPontos(),
                    r.getCompletouCorrida() &&
                    r.getColocacao() == 1 &&
                    !r.getDesclassificado()
                );

                break;
            }
        }
    }

    _resultados = std::move(classificados);

    // Impede finalizar e pontuar novamente.
    _estado = EstadoCorrida::FINALIZADA;
}
