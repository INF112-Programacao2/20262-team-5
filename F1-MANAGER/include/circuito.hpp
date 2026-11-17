#ifndef CIRCUITO_H
#define CIRCUITO_H

#include <string>

class Circuito {
    private:
        std::string _nome;
        std::string _pais;
        double _extensao;
        int _qtdCurvas;

    public:
        Circuito(std::string nome, std::string pais, double extensao, int qtdCurvas);
        ~Circuito();

        std::string getNome();
        void setNome(std::string nome);

        std::string getPais();
        void setPais(std::string pais);

        double getExtensao();
        void setExtensao(double extensao);

        int getCurvas();
        void setCurvas(int curvas);
};

#endif