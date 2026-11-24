
#pragma once

#include "pecas.hpp"

#include <string>

// Carro e composto de quatro pecas;
// cada uma implementa a interface IFPeca.
class Carro {
private:
    std::string _modelo;
    int _ano;
    double _combustivel;

    f1::Motor _motor;
    f1::Freio _freio;
    f1::Suspensao _suspensao;
    f1::Chassi _chassi;

public:
    Carro(
        std::string modelo,
        int ano,
        double combustivel,
        f1::Motor motor,
        f1::Freio freio,
        f1::Suspensao suspensao,
        f1::Chassi chassi
    );

    // Getters
    const std::string& getModelo() const {
        return _modelo;
    }

    int getAno() const {
        return _ano;
    }

    double getCombustivel() const {
        return _combustivel;
    }

    // Indice didatico de desempenho, de 0 a 100.
    double calcularDesempenho() const;

    // Retorna false quando nao ha combustivel suficiente.
    bool consumirCombustivel(double quantidade);

    // Melhoria de uma peca: a qualidade permanece entre 0 e 100.
    void melhorarComponente(
        const std::string& nome,
        double incremento = 5.0
    );
};

