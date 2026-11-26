
#ifndef EQUIPE_HPP
#define EQUIPE_HPP

#include "piloto.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// Eu deixo Equipe abstrata porque cada tipo de equipe
// vai calcular sua forca de acordo com suas proprias regras.
class Equipe {
private:
    std::string _nome;

    // Eu mantenho cada piloto em um endereco estavel.
    // A Corrida guarda ponteiros para os pilotos inscritos.
    std::vector<std::unique_ptr<Piloto>> _pilotos;

protected:
    // Eu aproveito o desempenho dos pilotos como base
    // para os calculos das classes derivadas.
    double calcularForcaPilotos() const;

public:
    explicit Equipe(std::string nome);
    virtual ~Equipe() = default;

    // Eu evito copiar equipes para nao duplicar os pilotos.
    Equipe(const Equipe&) = delete;
    Equipe& operator=(const Equipe&) = delete;

    const std::string& getNome() const;
    std::size_t quantidadePilotos() const;

    void cadastrarPiloto(Piloto piloto);

    Piloto* buscarPiloto(const std::string& id);
    const Piloto* buscarPiloto(const std::string& id) const;

    // Eu so removo pilotos entre corridas, quando nao
    // existem inscricoes ou resultados apontando para eles.
    void removerPiloto(const std::string& id);

    double folhaSalarialMensal() const;
    void encerrarTemporada();

    virtual double calcularForcaEquipe() const = 0;
};

#endif
