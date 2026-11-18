#include "circuito.hpp"

Circuito::Circuito(std::string nome, std::string pais, double extensao, int qtdCurvas):
    _nome(nome), _pais(pais), _extensao(extensao), _qtdCurvas(qtdCurvas) {};
Circuito::~Circuito() {}

std::string Circuito::getNome(){
    return _nome;
}
void Circuito::setNome(std::string nome){
    _nome = nome;
}

std::string Circuito::getPais(){
    return _pais;
}
void Circuito::setPais(std::string pais){
    _pais = pais;
}

double Circuito::getExtensao(){
    return _extensao;
}
void Circuito::setExtensao(double extensao){
    _extensao = extensao;
}

int Circuito::getCurvas(){
    return _qtdCurvas;
}
void Circuito::setCurvas(int curvas){
    _qtdCurvas = curvas;
}