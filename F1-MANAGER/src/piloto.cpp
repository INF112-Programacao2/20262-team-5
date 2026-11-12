#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/piloto.hpp"

Piloto::Piloto(std::string nome, std::string cpf, int idade, const Carro& carro, int duracao,double salario, double rescisao)
     : Pessoa(nome, id, idade), _carro(carro), _contrato(duracao, salario, rescisao){
        if(idade < 18){
            throw std::invalid_argument("O piloto precisa ter pelo menos 18 anos");
        }
}

//getters
Carro Piloto::getCarro() const{
    return _carro;  
}
Contrato Piloto::getContrato() const{
    return _contrato;
}
int Piloto:: getPontos() const{
    return _pontos;
}
int Piloto:: getTitulos() const{
    return _titulos;
}
int Piloto:: getVitorias() const{
    return _vitorias;
}

//setters
void Piloto::setTitulos(int titulos){
  if(titulos < 0){
    throw std::invalid_argument("Erro! O piloto não pode ter títulos negativos");
  }
  else{
    _titulos = titulos;
  }
}
void Piloto::setVitorias(int vitorias){
    if(vitorias < 0){
        throw std::invalid_argument("Erro! O piloto não pode ter vitórias negativas");
    }
    else{
        _vitorias = vitorias;
    }
}
void Piloto::setPontos(int pontos){
    if(pontos < 0){
        throw std::invalid_argument("Erro! O piloto não pode ter pontos negativos");
    }
    else{
        _pontos = pontos;
    }
}
//outras funções
void Piloto::registrarResultado(int pontos, bool vitoria{
    if(pontos < 0){
       throw std::invalid_argument("Erro! O piloto não pode ter pontos negativos");
    }
    else{
        _pontos += pontos;
        if(vitoria){
            _vitorias++;
        }
    }
}
void Piloto::zerarTemporada(){
    _pontos = 0;
    _vitorias = 0;
}