#ifndef PILOTO_HPP
#define PILOTO_HPP
#include <string>
#include "pessoa.hpp"
#include "contrato.hpp"
#include "carro.hpp"

class Piloto : public Pessoa{
private:
 Carro _carro;
 Contrato _contrato;
 int _titulos = 0;
 int _vitorias = 0;
 int _pontos = 0;
public:
 Piloto(std::string nome, std::string id, int idade, const Carro& carro, int duracao,double salario, double rescisao);
 //getters
 Carro getCarro() const;
 Contrato getContrato() const;
 int getTitulos() const;
 int getVitorias() const;
 int getPontos() const;
 //Setters
 void setTitulos(int titulos);
 void setVitorias(int vitorias);
 void setPontos(int pontos);
//outras funções
 void registrarResultado(int pontos, bool vitoria);
 void zerarTemporada();

};
#endif