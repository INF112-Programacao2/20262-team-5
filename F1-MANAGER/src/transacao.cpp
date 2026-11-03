#include <string>
#include "../include/transacao.hpp"

Transacao::Transacao(double valor, std::string data, std::string descricao):
    _valor(valor), _data(data), _descricao(descricao) {}

Transacao::~Transacao() {};

double Transacao::getValor(){
    return _valor; 
}
void Transacao::setValor(double valor){
    _valor = valor;
}

std::string Transacao::getData(){
    return _data; 
}
void Transacao::setData(std::string data){
    _data = data;
}

std::string Transacao::getDescricao(){
    return _descricao; 
}
void Transacao::setDescricao(std::string descricao){
    _descricao = descricao;
}