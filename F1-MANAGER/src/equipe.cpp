
#include "equipe.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

Equipe::Equipe(std::string nome)
    : _nome(std::move(nome)) {

    if (_nome.empty()) {
        throw std::invalid_argument("Nome da equipe vazio.");
    }
}

const std::string& Equipe::getNome() const {
    return _nome;
}

std::size_t Equipe::quantidadePilotos() const {
    return _pilotos.size();
}

void Equipe::cadastrarPiloto(Piloto piloto) {
    if (buscarPiloto(piloto.getId()) != nullptr) {
        throw std::invalid_argument(
            "Piloto ja cadastrado na equipe."
        );
    }

    // Eu crio o piloto no heap para preservar seu endereco
    // mesmo quando o vector precisar crescer.
    _pilotos.push_back(
        std::make_unique<Piloto>(std::move(piloto))
    );
}

Piloto* Equipe::buscarPiloto(const std::string& id) {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    return it == _pilotos.end() ? nullptr : it->get();
}

const Piloto* Equipe::buscarPiloto(
    const std::string& id
) const {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    return it == _pilotos.end() ? nullptr : it->get();
}

void Equipe::removerPiloto(const std::string& id) {
    const auto it = std::find_if(
        _pilotos.begin(),
        _pilotos.end(),
        [&id](const std::unique_ptr<Piloto>& piloto) {
            return piloto->getId() == id;
        }
    );

    if (it == _pilotos.end()) {
        throw std::invalid_argument(
            "Piloto nao encontrado."
        );
    }

    _pilotos.erase(it);
}

double Equipe::folhaSalarialMensal() const {
    double total = 0.0;

    // Eu somo os salarios dos pilotos cadastrados.
    // Os salarios dos funcionarios entram nesta conta depois.
    for (const auto& piloto : _pilotos) {
        total += piloto->getContrato().getSalario();
    }

    return total;
}

void Equipe::encerrarTemporada() {
    // Eu zero a pontuacao, mas preservo as estatisticas
    // de carreira que Piloto::zerarTemporada() ja mantem.
    for (auto& piloto : _pilotos) {
        piloto->zerarTemporada();
    }
}

double Equipe::calcularForcaPilotos() const {
    if (_pilotos.empty()) {
        return 0.0;
    }

    double soma = 0.0;

    for (const auto& piloto : _pilotos) {
        soma +=
            0.50 * piloto->getCarro().calcularDesempenho()
            + 0.30 * piloto->getHabilidade()
            + 0.20 * piloto->getConsistencia();
    }

    return soma / static_cast<double>(_pilotos.size());
}
