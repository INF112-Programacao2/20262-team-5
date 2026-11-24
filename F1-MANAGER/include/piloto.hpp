
#ifndef PILOTO_HPP
#define PILOTO_HPP

#include <string>

#include "pessoa.hpp"
#include "contrato.hpp"
#include "carro.hpp"

class Piloto : public Pessoa {
private:
    Carro _carro;
    Contrato _contrato;

    int _titulos = 0;
    int _vitorias = 0;
    int _pontos = 0;

    double _habilidade;
    double _consistencia;

public:
    // Mantem o construtor antigo (7 argumentos)
    // e permite definir os atributos do simulador.
    Piloto(
        std::string nome,
        std::string id,
        int idade,
        const Carro& carro,
        int duracao,
        double salario,
        double rescisao,
        double habilidade = 80.0,
        double consistencia = 80.0
    );

    std::string getTipo() const override {
        return "Piloto";
    }

    //getters
    const Carro& getCarro() const;
    Carro& getCarro();

    const Contrato& getContrato() const;

    int getTitulos() const;
    int getVitorias() const;
    int getPontos() const;

    double getHabilidade() const {
        return _habilidade;
    }

    double getConsistencia() const {
        return _consistencia;
    }

    //Setters
    void setTitulos(int titulos);
    void setVitorias(int vitorias);
    void setPontos(int pontos);

    //outras funcoes
    void registrarResultado(int pontos, bool vitoria);
    void zerarTemporada();
};

#endif
