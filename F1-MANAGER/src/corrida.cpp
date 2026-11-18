#include "corrida.hpp"
#include <algorithm>

Corrida::Corrida(std::string data, Circuito circuito, int clima):
    _data(data), _circuito(circuito), _clima(clima) {}
Corrida::~Corrida() {} //se atentar aos resultados pq talvez tenham que ser destruidos (aniquilados)

Resultado Corrida::getResultado(int indice){
    return _resultados[indice];
}
void Corrida::setResultado(Resultado resultado, int indice){
    _resultados[indice] = resultado;
}

std::string Corrida::getData(){
    return _data;
}
void Corrida::setData(std::string data){
    _data = data;
}

Circuito Corrida::getCircuito(){
    return _circuito;
}
void Corrida::setCircuito(Circuito circuito){
    _circuito = circuito;
}

int Corrida::getClima(){
    return _clima;
}
void Corrida::setClima(int clima){
    _clima = clima;
}


//Todos os calculos estao sujeitos a alteracoes, isso eh um teste de principios
double Corrida::calcularCoeficienteTracao(){
    double coeficiente = 1.0;
    if(_clima == 3){ //clima chuvoso
        coeficiente += 0.5;
    }
    coeficiente += 0.01 * _circuito.getCurvas();
    return coeficiente;
}

double Corrida::calcularCoeficienteVelocidade(){
    double coeficiente = 1.0;
    if(_clima == 2){ //clima nublado
        coeficiente -= 0.25;
    }
    coeficiente += 0.05 * _circuito.getExtensao();
    coeficiente -= 0.01 * _circuito.getCurvas();
    return coeficiente;
}

double Corrida::calcularCoeficienteAceleracao(){
    double coeficiente = 1.0;
    coeficiente -= 0.001 * _circuito.getExtensao();
    coeficiente += 0.03 * _circuito.getCurvas();
    return coeficiente;
}

void Corrida::ordenarPilotosCrescente(){
    std::sort(_resultados.begin(), _resultados.end(), [](const Resultado& a, const Resultado& b) {
        return a.getColocacao() < b.getColocacao();
    });
}
void Corrida::ordenarPilotosDecrescente(){ 
    std::sort(_resultados.begin(), _resultados.end(), [](const Resultado& a, const Resultado& b) {
        return a.getColocacao() > b.getColocacao();
    });
}

void Corrida::desconsiderarDesclassificados(){
    for(int i=0; i<_resultados.size(); i++){
        if(!_resultados[i].getCompletouCorrida()){
            _resultados.erase(_resultados.begin() + i);
            i--;
        }
    }
}

//nn entendi ainda o que esses vao fazer
void Corrida::iniciar() {}
void Corrida::finalizar() {}