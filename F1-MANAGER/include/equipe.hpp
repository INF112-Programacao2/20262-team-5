
#ifndef EQUIPE_HPP
#define EQUIPE_HPP

#include "piloto.hpp"
#include "funcionario.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// Classe abstrata: cada tipo de equipe define o proprio calculo de forca.
class Equipe {
private:
    std::string _nome;

    // unique_ptr preserva o endereco dos objetos ao expandir os vetores.
    // Corridas podem manter ponteiros para pilotos inscritos.
    std::vector<std::unique_ptr<Piloto>> _pilotos;
    std::vector<std::unique_ptr<Funcionario>> _funcionarios;

protected:
    // Base de desempenho compartilhada pelas equipes derivadas.
    double calcularForcaPilotos() const;

    // Somente engenheiros contribuem para o bonus das melhorias.
    double calcularImpactoEngenheiros() const;

public:
    explicit Equipe(std::string nome);
    virtual ~Equipe() = default;

    // A equipe possui os pilotos e funcionarios; copias sao proibidas.
    Equipe(const Equipe&) = delete;
    Equipe& operator=(const Equipe&) = delete;

    const std::string& getNome() const;

    std::size_t quantidadePilotos() const;
    std::size_t quantidadeFuncionarios() const;

    // Gerenciamento de pilotos.
    void cadastrarPiloto(Piloto piloto);

    Piloto* buscarPiloto(const std::string& id);

    const Piloto* buscarPiloto(
        const std::string& id
    ) const;

    // A remocao deve ocorrer apenas quando nao houver referencias externas.
    void removerPiloto(const std::string& id);

    // A equipe assume a propriedade do funcionario contratado.
    void cadastrarFuncionario(
        std::unique_ptr<Funcionario> funcionario
    );

    Funcionario* buscarFuncionario(
        const std::string& id
    );

    const Funcionario* buscarFuncionario(
        const std::string& id
    ) const;

    void removerFuncionario(
        const std::string& id
    );

    // Inclui os salarios de pilotos e funcionarios.
    double folhaSalarialMensal() const;

    // Soma os impactos individuais; o efeito nas corridas sera integrado depois.
    double calcularImpactoFuncionarios() const;

    void encerrarTemporada();

    virtual double calcularForcaEquipe() const = 0;
};

#endif
