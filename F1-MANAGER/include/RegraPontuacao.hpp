
#ifndef REGRA_PONTUACAO_HPP
#define REGRA_PONTUACAO_HPP

#include <array>
#include <cstddef>

// Pontuacao didatica dos dez primeiros colocados.
// Mantemos a regra separada da classe Corrida
// para facilitar alteracoes futuras.
class RegraPontuacao {
public:
    static int calcularPontos(int colocacao) {
        static constexpr std::array<int, 10> tabela = {
            25, 18, 15, 12, 10, 8, 6, 4, 2, 1
        };

        if (colocacao < 1 ||
            colocacao > static_cast<int>(tabela.size())) {
            return 0;
        }

        return tabela[
            static_cast<std::size_t>(colocacao - 1)
        ];
    }
};

#endif

