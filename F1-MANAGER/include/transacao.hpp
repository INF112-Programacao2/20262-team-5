#include <string>

class Transacao {
    private:
        double _valor;
        std::string _data;
        std::string _descricao;
    public:
        Transacao(double valor, std::string data, std::string descricao);
        ~Transacao();

        double getValor();
        void setValor(double valor);

        std::string getData();
        void setData(std::string data);

        std::string getDescricao();
        void setDescricao(std::string descricao);
};