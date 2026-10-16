#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <memory>
#include <optional>

// ============================================================
// ESTRUTURA BASE DA INTERFACE (SFML 3 + DEAR IMGUI)
// ============================================================

class AppInterface {
private:
    // Alocação dinâmica de recursos da aplicação
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::Texture> heroTexture;
    
    sf::Clock deltaClock;
    int paginaAtual = 0;
    bool temTextura = false;

    // Configuração de temas e estilos do ImGui
    void configurarEstilo() {
        ImGuiStyle& style = ImGui::GetStyle();
        
        // Espaçamentos e arredondamentos
        style.WindowPadding = ImVec2(18, 18);
        style.FramePadding = ImVec2(11, 8);
        style.WindowRounding = 12;
        style.ChildRounding = 10;
        style.FrameRounding = 6;
        style.WindowBorderSize = 0;

        // Paleta de cores (Tema Dark/Teal)
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]       = ImVec4(0.035f, 0.045f, 0.055f, 1.0f);
        colors[ImGuiCol_ChildBg]        = ImVec4(0.070f, 0.085f, 0.100f, 1.0f);
        colors[ImGuiCol_Border]         = ImVec4(0.170f, 0.210f, 0.230f, 0.75f);
        colors[ImGuiCol_Button]         = ImVec4(0.000f, 0.300f, 0.270f, 1.0f);
        colors[ImGuiCol_ButtonHovered]  = ImVec4(0.000f, 0.470f, 0.400f, 1.0f);
        colors[ImGuiCol_ButtonActive]   = ImVec4(0.000f, 0.370f, 0.320f, 1.0f);
        colors[ImGuiCol_Text]           = ImVec4(0.930f, 0.960f, 0.970f, 1.0f);
    }

    // Renderização do menu lateral (Sidebar)
    void renderizarMenuLateral() {
        ImGui::BeginChild("sidebar", ImVec2(220, 0), true);

        ImGui::TextColored(ImVec4(0.00f, 0.88f, 0.73f, 1.0f), "MERCEDES F1");
        ImGui::TextDisabled("TEAM MANAGER");
        ImGui::Separator();
        ImGui::Spacing();

        const char* paginas[] = { "01  Dashboard", "02  Pilotos", "03  Corridas", "04  Sobre" };
        
        for (int i = 0; i < 4; ++i) {
            if (paginaAtual == i) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.30f, 0.27f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            }

            if (ImGui::Button(paginas[i], ImVec2(-1, 40))) {
                paginaAtual = i;
            }

            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        ImGui::EndChild();
    }

    // Renderização do conteúdo principal conforme a página selecionada
    void renderizarConteudo() {
        ImGui::BeginChild("content_area", ImVec2(0, 0), false);

        switch (paginaAtual) {
            case 0:
                ImGui::Text("DASHBOARD PRINCIPAL");
                ImGui::Separator();
                ImGui::Text("Visao geral do sistema.");
                break;
            case 1:
                ImGui::Text("GESTAO DE PILOTOS");
                ImGui::Separator();
                ImGui::Text("Painel de cadastro de pilotos.");
                break;
            case 2:
                ImGui::Text("REGISTO DE CORRIDAS");
                ImGui::Separator();
                ImGui::Text("Historico e resultados de GP.");
                break;
            case 3:
                ImGui::Text("SOBRE O SISTEMA");
                ImGui::Separator();
                ImGui::Text("Desenvolvido em C++ com SFML 3 e Dear ImGui.");
                break;
            default:
                paginaAtual = 0;
                break;
        }

        ImGui::EndChild();
    }

public:
    AppInterface() {
        // Alocação dinâmica da janela SFML
        window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode({1440u, 900u}),
            "Mercedes F1 Manager - Base Interface"
        );
        window->setFramerateLimit(60);

        // Inicialização do ImGui-SFML
        if (!ImGui::SFML::Init(*window)) {
            throw std::runtime_error("Erro ao inicializar ImGui-SFML");
        }

        // Alocação dinâmica e carregamento de texturas
        heroTexture = std::make_unique<sf::Texture>();
        temTextura = heroTexture->loadFromFile("assets/mercedes_hero.jpg");

        configurarEstilo();
    }

    ~AppInterface() {
        // Encerramento limpo do ImGui
        ImGui::SFML::Shutdown();
    }

    // Loop principal da aplicação
    void executar() {
        while (window->isOpen()) {
            // Processamento de eventos da janela (SFML 3 syntax)
            while (const std::optional event = window->pollEvent()) {
                ImGui::SFML::ProcessEvent(*window, *event);

                if (event->is<sf::Event::Closed>()) {
                    window->close();
                }
            }

            // Atualização do ImGui
            ImGui::SFML::Update(*window, deltaClock.restart());

            // Janela do ImGui ocupando 100% da viewport
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::Begin("RootWindow", nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse
            );

            // Renderização dos componentes visuais
            renderizarMenuLateral();
            ImGui::SameLine();
            renderizarConteudo();

            ImGui::End();

            // Renderização final na janela SFML
            window->clear(sf::Color(9, 12, 15));
            ImGui::SFML::Render(*window);
            window->display();
        }
    }
};

// ============================================================
// PONTO DE ENTRADA DO PROGRAMA
// ============================================================

int main() {
    try {
        // Alocação dinâmica do gestor da interface
        auto app = std::make_unique<AppInterface>();
        app->executar();
    } 
    catch (const std::exception& e) {
        return 1;
    }

    return 0;
}