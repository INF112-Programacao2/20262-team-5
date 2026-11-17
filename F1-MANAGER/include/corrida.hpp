#ifndef CORRIDA_H
#define CORRIDA_H

#include <vector>
#include <string>

class Resultado {};
//provisoro : implementar resultado

class Corrida {
    private:
        std::vector<Resultado> _resultados; //posicao de cada piloto
        std::string _data;
        double _tamanhoCircuito;
        int _qtdCurvas;
        int _qtdVoltas;
        int _clima;
        /*
        1 - Sol
        2 - Nublado
        3 - Chuva
        */

    public:
        Corrida();
        ~Corrida();

        Resultado getResultado(int indice);
        void setResultado(Resultado resultado, int indice); 

        std::string getData();
        void setData(std::string data);

        double getTamanho();
        void setTamanho(double tamanho);

        int getCurvas();
        void setCurvas(int curvas);

        int getVoltas();
        void setVoltas(int voltas);

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