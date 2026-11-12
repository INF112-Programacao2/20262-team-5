#include "../include/contrato.hpp"
#include <stdexcept>

Contrato:: Contrato(int duracao, double salario, double rescisao) {
    setDuracao(duracao);
    setSalario(salario);
    setRescisao(rescisao);
}

double Contrato::custoTotal() const {
    return _salario * _duracao;
}

//getters
int Contrato :: getDuracao() const{
  return _duracao; 
}

double Contrato :: getSalario() const{
    return _salario;
}

double Contrato :: getRescisao() const{
    return _rescisao;
}

//setters
void Contrato :: setDuracao(int duracao){
   if(duracao <= 0){
    throw std::invalid_argument("Erro! o contrato não pode ter duração negativa!");
   }
   else{
    _duracao = duracao;
   }
}

void Contrato :: setSalario(double salario){
    if(salario <= 0){
      throw std::invalid_argument("Erro! o contrato não pode ter um salário negativo");
    }
    else{
      _salario = salario;  
    }
}

void Contrato :: setRescisao(double rescisao){
    if(rescisao < 0){
      throw std::invalid_argument("Erro! o contrato não pode ter uma multa de rescisão negativa!");
    }
    else{
      _rescisao = rescisao;  
    }
}
