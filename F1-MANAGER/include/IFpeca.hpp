
#pragma once
#include <string>

namespace f1 {

class IFPeca {
public:
    virtual ~IFPeca() = default;

    virtual double calcularDesempenho() const = 0;

    virtual std::string obterNome() const = 0;

    virtual void melhorar(double incremento) = 0;
};

} // namespace f1
