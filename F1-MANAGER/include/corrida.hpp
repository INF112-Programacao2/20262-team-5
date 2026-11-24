
#ifndef CORRIDA_H
#define CORRIDA_H

#include <string>
#include <vector>

#include "resultado.hpp"
#include "circuito.hpp"

class Piloto;

// Estados possiveis de uma corrida.
enum class EstadoCorrida {
    NAO_INICIADA,
    EM_ANDAMENTO,
    FINALIZADA
};

class Corrida {
private:
    std::vector<Resultado> _resultados; //posicao de cada piloto

    // A corrida utiliza os pilotos, mas nao possui esses objetos.
    // Futuramente, a equipe e o grid gerenciarao seu ciclo de vida.
    std::vector<Piloto*> _inscritos;

    std::string _data;
    Circuito _circuito;

    int _clima;
    /*
    1 - Sol
    2 - Nublado
    3 - Chuva
    */

    int _qtdVoltas;

    EstadoCorrida _estado = EstadoCorrida::NAO_INICIADA;

public:
    // 20 voltas por padrao para preservar
    // chamadas antigas do projeto.
    Corrida(
        std::string data,
        Circuito circuito,
        int clima,
        int qtdVoltas = 20
    );

    ~Corrida() = default;

    // Resultados
    Resultado getResultado(int indice) const;
    void setResultado(Resultado resultado, int indice);

    const std::vector<Resultado>& getResultados() const;

    // Dados do GP
    std::string getData() const;
    void setData(std::string data);

    // Retorno por valor para manter compatibilidade
    // com a classe Circuito original.
    Circuito getCircuito() const;
    void setCircuito(Circuito circuito);

    int getClima() const;
    void setClima(int clima);

    int getQtdVoltas() const;
    EstadoCorrida getEstado() const;

    // Inscricoes e registro de resultados
    void adicionarPiloto(Piloto& piloto);

    const std::vector<Piloto*>& getPilotos() const;

    void adicionarResultado(Resultado resultado);

    //metodos usados para saber a relevancia de cada metrica em cada corrida: Tracao, Velocidade, Aceleracao;
    double calcularCoeficienteTracao();
    double calcularCoeficienteVelocidade();
    double calcularCoeficienteAceleracao();

    //Ordena os pilotos - vai ser util no relatorio
    void ordenarPilotosCrescente();
    void ordenarPilotosDecrescente();

    //Vai ser usado no relatorio
    void exibirResultados() const;

    //Se por algum motivo um corredor nao terminar a corrida, deve ser desconsiderado
    //na pontuacao, mas NAO removido do historico de resultados.
    void desconsiderarDesclassificados();

    // Controle do ciclo de vida da corrida
    void iniciar();
    void finalizar();
};

#endif
