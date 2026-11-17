#ifndef CORRIDA_H
#define CORRIDA_H

#include <vector>
#include <string>

#include "resultado.hpp"
#include "circuito.hpp"

class Corrida {
    private:
        std::vector<Resultado> _resultados; //posicao de cada piloto
        std::string _data;
        Circuito _circuito;
        int _clima;
        /*
        1 - Sol
        2 - Nublado
        3 - Chuva
        */

    public:
        Corrida(std::string data, Circuito circuito, int clima);
        ~Corrida();

        Resultado getResultado(int indice);
        void setResultado(Resultado resultado, int indice); 

        std::string getData();
        void setData(std::string data);

        Circuito getCircuito();
        void setCircuito(Circuito circuito);

        int getClima();
        void setClima(int clima);

        //metodos usados para saber a relevancia de cada metrica em cada corrida: Tracao, Velocidade, Aceleracao;
        double calcularCoeficienteTracao();
        double calcularCoeficienteVelocidade();
        double calcularCoeficienteAceleracao();

        void ordenarPilotosCrescente();
        void ordenarPilotosDecrescente();

        //Se por algum motivo um corredor nao terminar a corrida, deve ser desconsiderado
        void desconsiderarDesclassificados();

        void iniciar();
        void finalizar();
};

#endif