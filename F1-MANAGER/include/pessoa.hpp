
#pragma once

#include <stdexcept>
#include <string>
#include <utility>

// Classe abstrata que representa uma pessoa do sistema.
// Evita repetir nome, identificador e idade nas classes derivadas.
class Pessoa {
private:
    std::string _nome;
    std::string _id;
    int _idade;

public:
    Pessoa(std::string nome, std::string id, int idade)
        : _nome(std::move(nome)),
          _id(std::move(id)),
          _idade(idade) {

        if (_nome.empty() || _id.empty() || _idade <= 0) {
            throw std::invalid_argument(
                "Dados da pessoa invalidos."
            );
        }
    }

    virtual ~Pessoa() = default;

    const std::string& getNome() const {
        return _nome;
    }

    const std::string& getId() const {
        return _id;
    }

    int getIdade() const {
        return _idade;
    }

    // Polimorfismo: cada classe derivada informa sua funcao no sistema.
    virtual std::string getTipo() const = 0;
};
