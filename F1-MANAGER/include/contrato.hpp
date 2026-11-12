#ifndef CONTRATO_HPP
#define CONTRATO_HPP

class Contrato{
private:
 int _duracao;
 double _salario;
 double _rescisao;
public:
 Contrato (int duracao, double salario, double rescisao);
 //getters
 int getDuracao() const;
 double getSalario() const;
 double getRescisao() const;
 //setters
 void setDuracao(int duracao);
 void setSalario(double salario);
 void setRescisao(double rescisao);

 //Função custo total (para o financeiro)
  double custoTotal() const;
};




#endif