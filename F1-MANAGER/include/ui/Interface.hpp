
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML/Graphics.hpp>

// Classe responsável por desenhar e controlar a interface
// principal do F1 Manager.
class Interface {
private:
    // Referência à janela principal do programa.
    sf::RenderWindow& janela;

    // Elementos visuais da tela inicial.
    sf::RectangleShape painel;
    sf::RectangleShape barraSuperior;

    sf::Font fonte;
    bool fonteCarregada;

    sf::Text titulo;
    sf::Text subtitulo;
    sf::Text botaoTexto;
    sf::Text rodape;

    // Botão principal do menu.
    sf::RectangleShape botao;

    // Configura os elementos visuais.
    void configurarElementos();

public:
    // Recebe a janela criada pelo programa principal.
    explicit Interface(sf::RenderWindow& janela);

    // Processa eventos da interface.
    void processarEvento(const sf::Event& evento);

    // Desenha os elementos na janela.
    void desenhar();
};

#endif

