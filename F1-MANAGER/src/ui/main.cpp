
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

// ============================================================
// F1 MANAGER - Mercedes-AMG Petronas
// Interface modularizada em um unico arquivo.
// Compatibilidade pretendida: SFML 3 + Dear ImGui + ImGui-SFML 3
// ============================================================

namespace app {

constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 800;

struct Driver {
    std::string name;
    std::string number;
    std::string country;
    int points;
    int wins;
    float pace;
    float consistency;
};

struct Race {
    std::string name;
    std::string country;
    std::string date;
    bool completed;
};

struct TeamData {
    std::string name = "Mercedes-AMG Petronas";
    std::string principal = "Team Principal";
    double budget = 128'500'000.0;
    int constructorsPosition = 3;
    int constructorsPoints = 284;
    int season = 2026;
};

enum class Page {
    Dashboard,
    Drivers,
    Car,
    Calendar,
    Races,
    Finance,
    Standings,
    Reports
};

static const char* pageName(Page page) {
    switch (page) {
        case Page::Dashboard: return "Dashboard";
        case Page::Drivers:   return "Pilotos";
        case Page::Car:       return "Carro";
        case Page::Calendar:  return "Calendario";
        case Page::Races:     return "Corridas";
        case Page::Finance:   return "Financeiro";
        case Page::Standings: return "Classificacao";
        case Page::Reports:   return "Relatorios";
    }
    return "Dashboard";
}

static std::string money(double value) {
    std::ostringstream out;
    out << "R$ " << std::fixed << std::setprecision(0) << value;
    return out.str();
}

class AppInterface {
public:
    AppInterface()
    : drivers_{
        Driver{"George Russell", "63", "Reino Unido", 167, 2, 94.0f, 91.0f},
        Driver{"Andrea Kimi Antonelli", "12", "Italia", 117, 1, 89.0f, 86.0f}
      },
      races_{
        {"Australia - Melbourne", "Australia", "08 Mar", true},
        {"China - Shanghai", "China", "15 Mar", true},
        {"Japao - Suzuka", "Japao", "29 Mar", true},
        {"Miami", "Estados Unidos", "03 Mai", false},
        {"Monaco", "Monaco", "24 Mai", false},
        {"Espanha - Barcelona", "Espanha", "07 Jun", false}
      } {}

    void render() {
        renderTopBar();

        ImGui::SetNextWindowPos(ImVec2(0, 66), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(220, static_cast<float>(WINDOW_HEIGHT - 66)), ImGuiCond_Always);
        ImGui::Begin("##sidebar", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);
        renderSidebar();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(220, 66), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(WINDOW_WIDTH - 220),
                                       static_cast<float>(WINDOW_HEIGHT - 66)), ImGuiCond_Always);
        ImGui::Begin("##content", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);
        renderCurrentPage();
        ImGui::End();
    }

private:
    TeamData team_;
    std::array<Driver, 2> drivers_;
    std::vector<Race> races_;
    Page currentPage_ = Page::Dashboard;
    int selectedDriver_ = 0;
    bool showNotifications_ = false;

    // ---------- Identidade visual ----------
    void applyTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 8.0f;
        style.GrabRounding = 5.0f;
        style.WindowPadding = ImVec2(18, 16);
        style.FramePadding = ImVec2(10, 7);
        style.ItemSpacing = ImVec2(10, 10);

        ImVec4* c = style.Colors;
        c[ImGuiCol_WindowBg]        = ImVec4(0.027f, 0.055f, 0.067f, 1.00f);
        c[ImGuiCol_ChildBg]         = ImVec4(0.055f, 0.090f, 0.105f, 1.00f);
        c[ImGuiCol_PopupBg]         = ImVec4(0.055f, 0.090f, 0.105f, 0.98f);
        c[ImGuiCol_Border]           = ImVec4(0.12f, 0.20f, 0.22f, 0.75f);
        c[ImGuiCol_Text]             = ImVec4(0.90f, 0.94f, 0.95f, 1.00f);
        c[ImGuiCol_TextDisabled]     = ImVec4(0.48f, 0.57f, 0.60f, 1.00f);
        c[ImGuiCol_FrameBg]          = ImVec4(0.09f, 0.15f, 0.17f, 1.00f);
        c[ImGuiCol_FrameBgHovered]   = ImVec4(0.12f, 0.23f, 0.24f, 1.00f);
        c[ImGuiCol_FrameBgActive]    = ImVec4(0.10f, 0.29f, 0.28f, 1.00f);
        c[ImGuiCol_Button]           = ImVec4(0.00f, 0.42f, 0.39f, 1.00f);
        c[ImGuiCol_ButtonHovered]    = ImVec4(0.00f, 0.58f, 0.52f, 1.00f);
        c[ImGuiCol_ButtonActive]     = ImVec4(0.00f, 0.34f, 0.32f, 1.00f);
        c[ImGuiCol_Header]           = ImVec4(0.00f, 0.30f, 0.28f, 0.65f);
        c[ImGuiCol_HeaderHovered]    = ImVec4(0.00f, 0.43f, 0.40f, 0.80f);
        c[ImGuiCol_HeaderActive]     = ImVec4(0.00f, 0.48f, 0.44f, 1.00f);
        c[ImGuiCol_Separator]        = ImVec4(0.14f, 0.24f, 0.25f, 1.00f);
        c[ImGuiCol_CheckMark]        = ImVec4(0.00f, 0.82f, 0.73f, 1.00f);
        c[ImGuiCol_SliderGrab]       = ImVec4(0.00f, 0.65f, 0.59f, 1.00f);
        c[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.85f, 0.76f, 1.00f);
    }

    void renderTopBar() {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(WINDOW_WIDTH), 66), ImGuiCond_Always);
        ImGui::Begin("##topbar", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);
        ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "MERCEDES");
        ImGui::SameLine();
        ImGui::TextDisabled("AMG PETRONAS FORMULA ONE TEAM");
        ImGui::SameLine(0, 40);
        ImGui::Text("|  TEMPORADA %d", team_.season);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 190);
        ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "●");
        ImGui::SameLine();
        ImGui::Text("Sistema operacional");
        ImGui::SameLine();
        if (ImGui::SmallButton("Notificacoes")) {
            showNotifications_ = !showNotifications_;
        }
        if (showNotifications_) {
            ImGui::SetNextWindowPos(ImVec2(940, 62), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);
            ImGui::Begin("Avisos", &showNotifications_);
            ImGui::TextWrapped("Voce nao possui alertas importantes no momento.");
            ImGui::End();
        }
        ImGui::End();
    }

    void renderSidebar() {
        ImGui::TextDisabled("MENU PRINCIPAL");
        ImGui::Spacing();
        navButton("Dashboard", Page::Dashboard);
        navButton("Pilotos", Page::Drivers);
        navButton("Carro", Page::Car);
        navButton("Calendario", Page::Calendar);
        navButton("Corridas", Page::Races);
        navButton("Financeiro", Page::Finance);
        navButton("Classificacao", Page::Standings);
        navButton("Relatorios", Page::Reports);

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 82);
        ImGui::Separator();
        ImGui::TextDisabled("MERCEDES-AMG PETRONAS");
        ImGui::TextDisabled("F1 MANAGER 2026");
    }

    void navButton(const char* label, Page page) {
        const bool active = currentPage_ == page;
        if (active) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.38f, 0.35f, 1.0f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        if (ImGui::Button(label, ImVec2(-1, 39))) {
            currentPage_ = page;
        }
        ImGui::PopStyleColor();
    }

    // ---------- Componentes reutilizaveis ----------
    void sectionTitle(const char* title, const char* subtitle = nullptr) {
        ImGui::Text("%s", title);
        if (subtitle) {
            ImGui::TextDisabled("%s", subtitle);
        }
        ImGui::Spacing();
    }

    void metricCard(const char* title, const std::string& value, const char* footer, float width) {
        ImGui::BeginChild(title, ImVec2(width, 105), true);
        ImGui::TextDisabled("%s", title);
        ImGui::Spacing();
        ImGui::Text("%s", value.c_str());
        ImGui::TextColored(ImVec4(0.00f, 0.78f, 0.69f, 1.0f), "%s", footer);
        ImGui::EndChild();
    }

    void renderCurrentPage() {
        switch (currentPage_) {
            case Page::Dashboard: renderDashboard(); break;
            case Page::Drivers:   renderDrivers(); break;
            case Page::Car:       renderCar(); break;
            case Page::Calendar:  renderCalendar(); break;
            case Page::Races:     renderRaces(); break;
            case Page::Finance:   renderFinance(); break;
            case Page::Standings: renderStandings(); break;
            case Page::Reports:   renderReports(); break;
        }
    }

    // ---------- Paginas ----------
    void renderDashboard() {
        sectionTitle("Dashboard", "Visao geral da equipe e da temporada");
        const float cardWidth = (ImGui::GetContentRegionAvail().x - 30.0f) / 4.0f;
        metricCard("ORCAMENTO DISPONIVEL", money(team_.budget), "Situacao financeira", cardWidth);
        ImGui::SameLine();
        metricCard("CONSTRUTORES", "#" + std::to_string(team_.constructorsPosition), "Posicao atual", cardWidth);
        ImGui::SameLine();
        metricCard("PONTOS DA EQUIPE", std::to_string(team_.constructorsPoints), "Temporada 2026", cardWidth);
        ImGui::SameLine();
        metricCard("PILOTOS", "02", "Dupla titular", cardWidth);

        ImGui::Spacing();
        ImGui::BeginChild("next_race", ImVec2(ImGui::GetContentRegionAvail().x * 0.58f, 245), true);
        ImGui::TextDisabled("PROXIMA CORRIDA");
        ImGui::Spacing();
        ImGui::Text("MIAMI GRAND PRIX");
        ImGui::TextDisabled("Miami International Autodrome - Estados Unidos");
        ImGui::Spacing();
        ImGui::Text("03 MAI 2026");
        ImGui::Separator();
        ImGui::Text("Preparacao da equipe");
        ImGui::BulletText("Revisar configuracao aerodinamica");
        ImGui::BulletText("Conferir desgaste dos componentes");
        ImGui::BulletText("Definir estrategia de pneus");
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("team_status", ImVec2(0, 245), true);
        ImGui::TextDisabled("STATUS DA EQUIPE");
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "OPERACIONAL");
        ImGui::Text("Carro: pronto para a proxima etapa");
        ImGui::Text("Pilotos: 2/2 ativos");
        ImGui::Text("Funcionarios: equipe completa");
        ImGui::Separator();
        ImGui::TextDisabled("Indicadores ilustrativos da interface.");
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::BeginChild("driver_summary", ImVec2(0, 170), true);
        ImGui::Text("DESTAQUES DOS PILOTOS");
        ImGui::Separator();
        for (const auto& driver : drivers_) {
            ImGui::Text("#%s  %s", driver.number.c_str(), driver.name.c_str());
            ImGui::SameLine(450);
            ImGui::Text("%d pts", driver.points);
        }
        ImGui::EndChild();
    }

    void renderDrivers() {
        sectionTitle("Pilotos", "Gerencie os pilotos titulares da Mercedes");
        for (int i = 0; i < static_cast<int>(drivers_.size()); ++i) {
            const Driver& d = drivers_[i];
            ImGui::PushID(i);
            ImGui::BeginChild("driver_card", ImVec2(0, 145), true);
            ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "#%s", d.number.c_str());
            ImGui::SameLine();
            ImGui::Text("%s", d.name.c_str());
            ImGui::TextDisabled("%s", d.country.c_str());
            ImGui::Text("Pontos: %d   |   Vitorias: %d", d.points, d.wins);
            ImGui::ProgressBar(d.pace / 100.0f, ImVec2(250, 12), "Ritmo");
            ImGui::SameLine();
            ImGui::ProgressBar(d.consistency / 100.0f, ImVec2(250, 12), "Consistencia");
            if (ImGui::Button("Ver perfil")) selectedDriver_ = i;
            ImGui::EndChild();
            ImGui::PopID();
        }
        const Driver& selected = drivers_[selectedDriver_];
        ImGui::Text("Perfil selecionado: %s - carro #%s", selected.name.c_str(), selected.number.c_str());
    }

    void renderCar() {
        sectionTitle("Carro", "Estado geral e componentes do monoposto");
        ImGui::BeginChild("car_overview", ImVec2(0, 145), true);
        ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "MERCEDES W17");
        ImGui::Text("Monoposto da temporada 2026");
        ImGui::TextDisabled("A representacao visual do carro pode ser adicionada em assets/images/cars.");
        ImGui::EndChild();

        const std::array<const char*, 4> parts = {"Motor", "Freios", "Suspensao", "Chassi"};
        const std::array<float, 4> condition = {0.91f, 0.84f, 0.88f, 0.96f};
        for (std::size_t i = 0; i < parts.size(); ++i) {
            ImGui::BeginChild(parts[i], ImVec2(0, 72), true);
            ImGui::Text("%s", parts[i]);
            ImGui::SameLine(170);
            ImGui::ProgressBar(condition[i], ImVec2(-1, 18));
            ImGui::EndChild();
        }
    }

    void renderCalendar() {
        sectionTitle("Calendario", "Etapas previstas para a temporada");
        if (ImGui::BeginTable("calendar_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("Etapa");
            ImGui::TableSetupColumn("Local");
            ImGui::TableSetupColumn("Data");
            ImGui::TableSetupColumn("Situacao");
            ImGui::TableHeadersRow();
            for (const auto& race : races_) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", race.name.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%s", race.country.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%s", race.date.c_str());
                ImGui::TableNextColumn();
                ImGui::TextColored(race.completed ? ImVec4(0.0f, 0.78f, 0.69f, 1.0f) : ImVec4(0.9f, 0.7f, 0.25f, 1.0f),
                                   "%s", race.completed ? "Concluida" : "Pendente");
            }
            ImGui::EndTable();
        }
    }

    void renderRaces() {
        sectionTitle("Corridas", "Acompanhe os resultados e simule etapas");
        ImGui::BeginChild("race_actions", ImVec2(0, 105), true);
        ImGui::Text("CENTRAL DE CORRIDAS");
        ImGui::TextDisabled("A simulacao real deve ser conectada ao modulo SimuladorDesempenho.");
        if (ImGui::Button("Simular proxima corrida")) {
            raceMessage_ = "Acao demonstrativa: conecte este botao ao motor de simulacao.";
        }
        if (!raceMessage_.empty()) ImGui::TextWrapped("%s", raceMessage_.c_str());
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Text("Ultimas etapas");
        for (const auto& race : races_) {
            if (race.completed) ImGui::BulletText("%s - concluida", race.name.c_str());
        }
    }

    void renderFinance() {
        sectionTitle("Financeiro", "Controle de orcamento e movimentacoes");
        metricCard("SALDO DISPONIVEL", money(team_.budget), "Saldo ilustrativo", 320);
        ImGui::Spacing();
        ImGui::BeginChild("finance_actions", ImVec2(0, 160), true);
        ImGui::Text("REGISTRAR MOVIMENTACAO");
        ImGui::InputDouble("Valor (R$)", &transactionValue_, 1000.0, 10000.0, "%.2f");
        ImGui::InputText("Descricao", transactionDescription_, sizeof(transactionDescription_));
        if (ImGui::Button("Registrar despesa") && transactionValue_ > 0) {
            team_.budget -= transactionValue_;
            financeMessage_ = "Despesa registrada na demonstracao da interface.";
            transactionValue_ = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("Registrar receita") && transactionValue_ > 0) {
            team_.budget += transactionValue_;
            financeMessage_ = "Receita registrada na demonstracao da interface.";
            transactionValue_ = 0;
        }
        if (!financeMessage_.empty()) ImGui::TextWrapped("%s", financeMessage_.c_str());
        ImGui::EndChild();
    }

    void renderStandings() {
        sectionTitle("Classificacao", "Posicao da equipe no campeonato");
        ImGui::BeginChild("standing_card", ImVec2(0, 150), true);
        ImGui::Text("MUNDIAL DE CONSTRUTORES");
        ImGui::TextColored(ImVec4(0.00f, 0.82f, 0.73f, 1.0f), "3o lugar - Mercedes");
        ImGui::Text("284 pontos");
        ImGui::TextDisabled("Dados demonstrativos. Substitua pelos resultados do Campeonato.");
        ImGui::EndChild();
        ImGui::Spacing();
        ImGui::Text("PILOTOS DA EQUIPE");
        for (const auto& d : drivers_) {
            ImGui::BulletText("%s (#%s): %d pontos", d.name.c_str(), d.number.c_str(), d.points);
        }
    }

    void renderReports() {
        sectionTitle("Relatorios", "Resumo gerencial da temporada");
        ImGui::BeginChild("report_card", ImVec2(0, 190), true);
        ImGui::Text("RELATORIO GERAL");
        ImGui::Separator();
        ImGui::BulletText("Equipe: %s", team_.name.c_str());
        ImGui::BulletText("Temporada: %d", team_.season);
        ImGui::BulletText("Pilotos ativos: %zu", drivers_.size());
        ImGui::BulletText("Orcamento atual: %s", money(team_.budget).c_str());
        ImGui::BulletText("Pontos de construtores: %d", team_.constructorsPoints);
        if (ImGui::Button("Exportar relatorio (demonstracao)")) {
            reportMessage_ = "A exportacao deve ser implementada no modulo Relatorio.";
        }
        if (!reportMessage_.empty()) ImGui::TextWrapped("%s", reportMessage_.c_str());
        ImGui::EndChild();
    }

    double transactionValue_ = 0.0;
    char transactionDescription_[128] = "";
    std::string financeMessage_;
    std::string raceMessage_;
    std::string reportMessage_;

public:
    void initializeTheme() { applyTheme(); }
};

} // namespace app

int main() {
    sf::RenderWindow window(
        sf::VideoMode({app::WINDOW_WIDTH, app::WINDOW_HEIGHT}),
        "F1 MANAGER | Mercedes-AMG Petronas",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        return 1;
    }

    app::AppInterface interface;
    interface.initializeTheme();

    sf::Clock deltaClock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                window.setView(sf::View(sf::FloatRect(
                    {0.f, 0.f},
                    {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)}
                )));
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color(7, 14, 17));
        interface.render();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
