
#pragma once

#include "IFpeca.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace f1 {

class PecaBase : public IFPeca {
public:
    explicit PecaBase(double qualidade)
        : qualidade_(qualidade) {

        if (!std::isfinite(qualidade) ||
            qualidade < 0.0 || qualidade > 100.0) {
            throw std::invalid_argument(
                "Qualidade deve estar entre 0 e 100."
            );
        }
    }

    double calcularDesempenho() const override {
        return qualidade_;
    }

    void melhorar(double incremento) override {
        if (!std::isfinite(incremento) ||
            incremento < 0.0) {
            throw std::invalid_argument(
                "Incremento invalido."
            );
        }

        qualidade_ = std::min(
            100.0, qualidade_ + incremento
        );
    }

private:
    double qualidade_;
};

class Motor final : public PecaBase {
public:
    using PecaBase::PecaBase;

    std::string obterNome() const override {
        return "Motor";
    }
};

class Freio final : public PecaBase {
public:
    using PecaBase::PecaBase;

    std::string obterNome() const override {
        return "Freio";
    }
};

class Suspensao final : public PecaBase {
public:
    using PecaBase::PecaBase;

    std::string obterNome() const override {
        return "Suspensao";
    }
};

class Chassi final : public PecaBase {
public:
    using PecaBase::PecaBase;

    std::string obterNome() const override {
        return "Chassi";
    }
};

} // namespace f1
