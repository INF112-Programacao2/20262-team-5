#include "resultado.hpp"

Resultado::Resultado(Piloto piloto, int colocacao, int tempo, bool completouCorrida, int pontos):
    _piloto(piloto), _colocacao(colocacao), _tempo(tempo), _completouCorrida(completouCorrida), _pontos(pontos) {}
Resultado::~Resultado() {}

Piloto Resultado::getPiloto(){
    return _piloto;
}
void Resultado::setPiloto(Piloto piloto){
    _piloto = piloto;
}

int Resultado::getColocacao() const{
    return _colocacao;
}
void Resultado::setColocacao(int colocacao){
    _colocacao = colocacao;
}

double Resultado::getTempo(){
    return _tempo;
}
void Resultado::setTempo(double tempo){
    _tempo = tempo;
}

bool Resultado::getCompletouCorrida(){
    return _completouCorrida;
}
void Resultado::setCompletouCorrida(bool completou){
    _completouCorrida = completou;
}

int Resultado::getPontos(){
    return _pontos;
}
void Resultado::setPontos(int pontos){
    _pontos = pontos;
}