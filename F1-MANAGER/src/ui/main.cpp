#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// ============================================================
// F1 MANAGER - Mercedes-AMG Petronas
// Interface SFML 3 + Dear ImGui + ImGui-SFML.
//
// Esta etapa melhora a camada visual do projeto original.
// Os dados exibidos ainda sao demonstrativos e ficam em memoria.
// A simulacao oficial sera conectada ao motor C++ separadamente.
// Nao incluir este arquivo no executavel headless: ambos tem main().
// ============================================================

namespace app {

constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 800;
constexpr float TOP_HEIGHT = 72.0f;
constexpr float SIDEBAR_WIDTH = 236.0f;

const ImVec4 VERDE(0.00f, 0.80f, 0.70f, 1.00f);
const ImVec4 AMARELO(0.95f, 0.74f, 0.32f, 1.00f);
const ImVec4 CINZA(0.55f, 0.62f, 0.65f, 1.00f);

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

struct CarPart {
    const char* name;
    float quality;
    float condition;
};

struct FinanceEntry {
    std::string description;
    double amount;
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

const char* pageName(Page page) {
    switch (page) {
        case Page::Dashboard: return "Dashboard";
        case Page::Drivers: return "Pilotos";
        case Page::Car: return "Carro";
        case Page::Calendar: return "Calendario";
        case Page::Races: return "Corridas";
        case Page::Finance: return "Financeiro";
        case Page::Standings: return "Classificacao";
        case Page::Reports: return "Relatorios";
    }
    return "Dashboard";
}

std::string money(double value) {
    std::ostringstream stream;
    stream << "R$ " << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

class AppInterface {
public:
    AppInterface()
        : drivers_{{
            {"George Russell", "63", "Reino Unido", 167, 2, 94.0f, 91.0f},
            {"Andrea Kimi Antonelli", "12", "Italia", 117, 1, 89.0f, 86.0f}
          }},
          races_{{
            {"Australia - Melbourne", "Australia", "08 Mar", true},
            {"China - Shanghai", "China", "15 Mar", true},
            {"Japao - Suzuka", "Japao", "29 Mar", true},
            {"Miami", "Estados Unidos", "03 Mai", false},
            {"Monaco", "Monaco", "24 Mai", false},
            {"Espanha - Barcelona", "Espanha", "07 Jun", false}
          }} {}

    void initializeTheme() {
        applyTheme();
    }

    void render() {
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        const float width = std::max(display.x, SIDEBAR_WIDTH + 400.0f);
        const float height = std::max(display.y, TOP_HEIGHT + 350.0f);

        renderTopBar(width);

        const ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowPos(ImVec2(0, TOP_HEIGHT), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(SIDEBAR_WIDTH, height - TOP_HEIGHT), ImGuiCond_Always);
        ImGui::Begin("##sidebar", nullptr, flags);
        renderSidebar();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(SIDEBAR_WIDTH, TOP_HEIGHT), ImGuiCond_Always);
        ImGui::SetNextWindowSize(
            ImVec2(width - SIDEBAR_WIDTH, height - TOP_HEIGHT), ImGuiCond_Always
        );
        ImGui::Begin("##content", nullptr, flags);
        renderCurrentPage();
        ImGui::End();
    }

private:
    TeamData team_;
    std::array<Driver, 2> drivers_;
    std::vector<Race> races_;
    std::array<CarPart, 4> parts_{{
        {"Motor", 0.92f, 0.91f},
        {"Freios", 0.88f, 0.84f},
        {"Suspensao", 0.90f, 0.88f},
        {"Chassi", 0.94f, 0.96f}
    }};
    std::vector<FinanceEntry> transactions_;

    Page currentPage_ = Page::Dashboard;
    int selectedDriver_ = 0;
    bool showNotifications_ = false;

    double transactionValue_ = 0.0;
    char transactionDescription_[128] = "";
    std::string financeMessage_;
    std::string raceMessage_;
    std::string reportMessage_;

    // ---------- Identidade visual ----------
    // Centralizamos o tema para nao repetir cores em cada tela.
    void applyTheme() {
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.ChildRounding = 9.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 7.0f;
        style.ScrollbarRounding = 7.0f;
        style.GrabRounding = 5.0f;
        style.WindowPadding = ImVec2(18, 16);
        style.FramePadding = ImVec2(12, 8);
        style.ItemSpacing = ImVec2(10, 10);
        style.WindowBorderSize = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.ScrollbarSize = 10.0f;

        ImVec4* c = style.Colors;
        c[ImGuiCol_WindowBg] = ImVec4(0.035f, 0.047f, 0.055f, 1.0f);
        c[ImGuiCol_ChildBg] = ImVec4(0.075f, 0.090f, 0.105f, 1.0f);
        c[ImGuiCol_PopupBg] = ImVec4(0.075f, 0.090f, 0.105f, 1.0f);
        c[ImGuiCol_Text] = ImVec4(0.94f, 0.96f, 0.97f, 1.0f);
        c[ImGuiCol_TextDisabled] = CINZA;
        c[ImGuiCol_Border] = ImVec4(0.18f, 0.22f, 0.24f, 1.0f);
        c[ImGuiCol_Separator] = ImVec4(0.17f, 0.22f, 0.24f, 1.0f);
        c[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.14f, 0.16f, 1.0f);
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.20f, 0.22f, 1.0f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.24f, 0.25f, 1.0f);
        c[ImGuiCol_Button] = ImVec4(0.00f, 0.34f, 0.30f, 1.0f);
        c[ImGuiCol_ButtonHovered] = VERDE;
        c[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.48f, 0.42f, 1.0f);
        c[ImGuiCol_Header] = ImVec4(0.00f, 0.32f, 0.28f, 0.65f);
        c[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.48f, 0.42f, 0.85f);
        c[ImGuiCol_HeaderActive] = VERDE;
        c[ImGuiCol_CheckMark] = VERDE;
        c[ImGuiCol_SliderGrab] = VERDE;
        c[ImGuiCol_SliderGrabActive] = VERDE;
        c[ImGuiCol_PlotHistogram] = VERDE;
        c[ImGuiCol_PlotLines] = VERDE;
        c[ImGuiCol_TableHeaderBg] = ImVec4(0.11f, 0.14f, 0.16f, 1.0f);
        c[ImGuiCol_TableRowBg] = ImVec4(0.075f, 0.09f, 0.105f, 1.0f);
        c[ImGuiCol_TableRowBgAlt] = ImVec4(0.10f, 0.12f, 0.14f, 1.0f);
    }

    void renderTopBar(float width) {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, TOP_HEIGHT), ImGuiCond_Always);
        ImGui::Begin("##topbar", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::TextColored(VERDE, "MERCEDES-AMG PETRONAS");
        ImGui::SameLine();
        ImGui::TextDisabled("  /  F1 MANAGER");
        ImGui::SameLine(0, 28);
        ImGui::Text("TEMPORADA %d", team_.season);

        ImGui::SameLine();
        ImGui::SetCursorPosX(std::max(10.0f, width - 215.0f));
        if (ImGui::SmallButton("AVISOS")) {
            showNotifications_ = !showNotifications_;
        }
        ImGui::SameLine();
        ImGui::TextColored(VERDE, "SISTEMA ONLINE");

        if (showNotifications_) {
            ImGui::SetNextWindowPos(ImVec2(width - 340.0f, TOP_HEIGHT - 4.0f), ImGuiCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(315, 115), ImGuiCond_Appearing);
            ImGui::Begin("Notificacoes", &showNotifications_);
            ImGui::TextWrapped("Interface em modo demonstracao. Os dados reais do campeonato ainda serao integrados.");
            ImGui::End();
        }
        ImGui::End();
    }

    void renderSidebar() {
        ImGui::TextDisabled("MENU PRINCIPAL");
        ImGui::Spacing();
        navButton("PAINEL GERAL", Page::Dashboard);
        navButton("PILOTOS", Page::Drivers);
        navButton("DESENVOLVIMENTO", Page::Car);
        navButton("CALENDARIO", Page::Calendar);
        navButton("CENTRAL DE GPs", Page::Races);
        navButton("FINANCEIRO", Page::Finance);
        navButton("CLASSIFICACAO", Page::Standings);
        navButton("RELATORIOS", Page::Reports);

        ImGui::SetCursorPosY(std::max(ImGui::GetCursorPosY() + 10.0f,
                                     ImGui::GetWindowHeight() - 95.0f));
        ImGui::Separator();
        ImGui::TextColored(VERDE, "MERCEDES 2026");
        ImGui::TextDisabled("Protótipo visual / INF112");
    }

    void navButton(const char* label, Page page) {
        const bool selected = currentPage_ == page;
        ImGui::PushStyleColor(ImGuiCol_Button,
            selected ? ImVec4(0.00f, 0.34f, 0.30f, 1.0f)
                     : ImVec4(0.09f, 0.11f, 0.13f, 1.0f));
        if (ImGui::Button(label, ImVec2(-1, 40))) {
            currentPage_ = page;
        }
        ImGui::PopStyleColor();
    }

    // ---------- Componentes reutilizaveis ----------
    void sectionTitle(const char* title, const char* subtitle = nullptr) {
        ImGui::TextColored(VERDE, "MERCEDES / %s", pageName(currentPage_));
        ImGui::Text("%s", title);
        if (subtitle) ImGui::TextDisabled("%s", subtitle);
        ImGui::Separator();
        ImGui::Spacing();
    }

    void metricCard(const char* id, const char* title,
                    const std::string& value, const char* footer, float width) {
        ImGui::BeginChild(id, ImVec2(width, 112), true);
        ImGui::TextDisabled("%s", title);
        ImGui::Spacing();
        ImGui::Text("%s", value.c_str());
        ImGui::TextColored(VERDE, "%s", footer);
        ImGui::EndChild();
    }

    void progressRow(const char* title, float value) {
        ImGui::Text("%s", title);
        ImGui::SameLine(180);
        ImGui::ProgressBar(std::clamp(value, 0.0f, 1.0f), ImVec2(-1, 16));
    }

    const Race* nextRace() const {
        const auto it = std::find_if(races_.begin(), races_.end(),
            [](const Race& race) { return !race.completed; });
        return it == races_.end() ? nullptr : &*it;
    }

    void renderCurrentPage() {
        switch (currentPage_) {
            case Page::Dashboard: renderDashboard(); break;
            case Page::Drivers: renderDrivers(); break;
            case Page::Car: renderCar(); break;
            case Page::Calendar: renderCalendar(); break;
            case Page::Races: renderRaces(); break;
            case Page::Finance: renderFinance(); break;
            case Page::Standings: renderStandings(); break;
            case Page::Reports: renderReports(); break;
        }
    }

    // ---------- Paginas ----------
    void renderDashboard() {
        sectionTitle("CENTRO DE COMANDO", "Mercedes-AMG Petronas | Visao geral da temporada demonstrativa");
        const float available = ImGui::GetContentRegionAvail().x;
        const float gap = ImGui::GetStyle().ItemSpacing.x;
        const float cardWidth = (available - 3.0f * gap) / 4.0f;

        metricCard("##budget", "ORCAMENTO", money(team_.budget), "Disponivel", cardWidth);
        ImGui::SameLine();
        metricCard("##constructors", "CONSTRUTORES", "#" + std::to_string(team_.constructorsPosition),
                   "Posicao ilustrativa", cardWidth);
        ImGui::SameLine();
        metricCard("##points", "PONTOS DA EQUIPE", std::to_string(team_.constructorsPoints),
                   "Temporada", cardWidth);
        ImGui::SameLine();
        metricCard("##pilots", "PILOTOS", "02", "Titulares", cardWidth);

        ImGui::Spacing();
        const float leftWidth = (available - gap) * 0.57f;
        ImGui::BeginChild("##next_gp", ImVec2(leftWidth, 260), true);
        ImGui::TextColored(VERDE, "PROXIMO GRANDE PREMIO");
        ImGui::Separator();
        if (const Race* race = nextRace()) {
            ImGui::Text("%s", race->name.c_str());
            ImGui::TextDisabled("%s  |  %s", race->country.c_str(), race->date.c_str());
            ImGui::Spacing();
            ImGui::BulletText("Analisar ritmo e consistencia dos pilotos");
            ImGui::BulletText("Verificar qualidade do carro");
            ImGui::BulletText("Preparar estrategia para o GP");
            ImGui::Spacing();
            if (ImGui::Button("ABRIR CENTRAL DE CORRIDAS", ImVec2(-1, 38))) {
                currentPage_ = Page::Races;
            }
        } else {
            ImGui::Text("Nao ha GPs pendentes no calendario cadastrado.");
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("##drivers_summary", ImVec2(0, 260), true);
        ImGui::TextColored(VERDE, "PILOTOS MERCEDES");
        ImGui::Separator();
        for (const Driver& driver : drivers_) {
            ImGui::Text("#%s  %s", driver.number.c_str(), driver.name.c_str());
            ImGui::TextDisabled("%d pts  |  %d vitorias", driver.points, driver.wins);
            ImGui::ProgressBar(driver.pace / 100.0f, ImVec2(-1, 10), "");
            ImGui::Spacing();
        }
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::BeginChild("##operations", ImVec2(0, 151), true);
        ImGui::TextColored(VERDE, "CENTRO DE OPERACOES");
        ImGui::TextDisabled("Acesso rapido as principais areas de gerenciamento.");
        if (ImGui::Button("DESENVOLVIMENTO DO CARRO", ImVec2(255, 38))) {
            currentPage_ = Page::Car;
        }
        ImGui::SameLine();
        if (ImGui::Button("CONTROLE FINANCEIRO", ImVec2(240, 38))) {
            currentPage_ = Page::Finance;
        }
        ImGui::TextDisabled("Os indicadores ainda nao estao ligados ao nucleo de simulacao.");
        ImGui::EndChild();
    }

    void renderDrivers() {
        sectionTitle("PILOTOS", "Russell e Antonelli | Perfil da dupla titular Mercedes");
        for (int i = 0; i < static_cast<int>(drivers_.size()); ++i) {
            const Driver& d = drivers_[i];
            ImGui::PushID(i);
            ImGui::BeginChild("##driver_card", ImVec2(0, 172), true);
            ImGui::TextColored(VERDE, "#%s", d.number.c_str());
            ImGui::SameLine();
            ImGui::Text("%s", d.name.c_str());
            ImGui::TextDisabled("%s", d.country.c_str());
            ImGui::Text("%d pontos  |  %d vitorias", d.points, d.wins);
            ImGui::TextDisabled("Ritmo");
            ImGui::ProgressBar(d.pace / 100.0f, ImVec2(-1, 11), "");
            ImGui::TextDisabled("Consistencia");
            ImGui::ProgressBar(d.consistency / 100.0f, ImVec2(-1, 11), "");
            if (ImGui::SmallButton("SELECIONAR PERFIL")) selectedDriver_ = i;
            ImGui::EndChild();
            ImGui::PopID();
        }
        const Driver& selected = drivers_[selectedDriver_];
        ImGui::TextColored(VERDE, "PERFIL ATIVO: %s / #%s", selected.name.c_str(), selected.number.c_str());
        ImGui::TextDisabled("Notas demonstrativas; os dados oficiais virao do GerenciadorMercedes.");
    }

    void renderCar() {
        sectionTitle("DESENVOLVIMENTO DO W17", "Visao tecnica dos componentes do monoposto");
        ImGui::BeginChild("##car_overview", ImVec2(0, 143), true);
        ImGui::TextColored(VERDE, "MERCEDES W17 | TEMPORADA 2026");
        ImGui::Separator();
        ImGui::Text("Foco: desempenho, confiabilidade e manutencao");
        ImGui::TextDisabled("Indicadores demonstrativos: melhorias reais serao ligadas a Carro, Estoque e Financeiro.");
        ImGui::EndChild();

        ImGui::Spacing();
        for (std::size_t i = 0; i < parts_.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));
            ImGui::BeginChild("##part", ImVec2(0, 94), true);
            ImGui::TextColored(VERDE, "%s", parts_[i].name);
            progressRow("Qualidade", parts_[i].quality);
            progressRow("Condicao", parts_[i].condition);
            ImGui::EndChild();
            ImGui::PopID();
        }
    }

    void renderCalendar() {
        sectionTitle("CALENDARIO", "Etapas cadastradas na demonstracao da interface");
        if (ImGui::BeginTable("##calendar", 4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("GP");
            ImGui::TableSetupColumn("Pais");
            ImGui::TableSetupColumn("Data");
            ImGui::TableSetupColumn("Situacao");
            ImGui::TableHeadersRow();
            for (const Race& race : races_) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", race.name.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%s", race.country.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%s", race.date.c_str());
                ImGui::TableNextColumn();
                ImGui::TextColored(race.completed ? VERDE : AMARELO,
                                   "%s", race.completed ? "Concluida" : "Pendente");
            }
            ImGui::EndTable();
        }
        ImGui::Spacing();
        ImGui::TextDisabled("Calendario parcial ilustrativo, ainda nao sincronizado com os GPs reais.");
    }

    void renderRaces() {
        sectionTitle("CENTRAL DE CORRIDAS", "Acompanhe a etapa e prepare o simulador de GP");
        ImGui::BeginChild("##race_header", ImVec2(0, 165), true);
        ImGui::TextColored(VERDE, "RACE CONTROL  /  MERCEDES");
        ImGui::Separator();
        if (const Race* race = nextRace()) {
            ImGui::Text("PROXIMO GP: %s", race->name.c_str());
            ImGui::TextDisabled("%s  |  %s", race->country.c_str(), race->date.c_str());
        } else {
            ImGui::Text("Nenhum GP pendente no calendario demonstrativo.");
        }
        if (ImGui::Button("SIMULAR PROXIMO GP", ImVec2(220, 37))) {
            // Integracao futura: chamar o SimuladorDesempenho via camada de servico.
            // Nao criamos resultados ficticios no botao para evitar duplicar o motor.
            raceMessage_ = "Motor headless ainda nao conectado a interface. Nenhum GP foi alterado.";
        }
        ImGui::EndChild();

        if (!raceMessage_.empty()) {
            ImGui::TextColored(AMARELO, "%s", raceMessage_.c_str());
        }
        ImGui::Spacing();
        ImGui::BeginChild("##race_drivers", ImVec2(0, 190), true);
        ImGui::TextColored(VERDE, "ESTADO DOS CARROS MERCEDES");
        ImGui::Separator();
        for (const Driver& d : drivers_) {
            ImGui::Text("#%s  %s", d.number.c_str(), d.name.c_str());
            ImGui::TextDisabled("Ritmo: %.0f  |  Consistencia: %.0f", d.pace, d.consistency);
            ImGui::Separator();
        }
        ImGui::EndChild();
        ImGui::Spacing();
        ImGui::Text("ETAPAS JA CADASTRADAS COMO CONCLUIDAS");
        for (const Race& race : races_) {
            if (race.completed) ImGui::BulletText("%s", race.name.c_str());
        }
    }

    bool validTransaction() {
        if (!std::isfinite(transactionValue_) || transactionValue_ <= 0.0) {
            financeMessage_ = "Informe um valor positivo e valido.";
            return false;
        }
        if (transactionDescription_[0] == '\0') {
            financeMessage_ = "Informe uma descricao para a movimentacao.";
            return false;
        }
        return true;
    }

    void recordTransaction(bool expense) {
        if (!validTransaction()) return;
        if (expense && transactionValue_ > team_.budget) {
            financeMessage_ = "Saldo demonstrativo insuficiente.";
            return;
        }
        const double amount = expense ? -transactionValue_ : transactionValue_;
        if (!std::isfinite(team_.budget + amount)) {
            financeMessage_ = "Valor financeiro excessivo.";
            return;
        }
        transactions_.push_back({transactionDescription_, amount});
        team_.budget += amount;
        financeMessage_ = "Movimentacao registrada apenas nesta sessao da interface.";
        transactionValue_ = 0.0;
        transactionDescription_[0] = '\0';
    }

    void renderFinance() {
        sectionTitle("CENTRO FINANCEIRO", "Controle demonstrativo de orcamento e movimentacoes");
        const float cardWidth = (ImGui::GetContentRegionAvail().x - 10.0f) / 2.0f;
        metricCard("##saldo", "SALDO DISPONIVEL", money(team_.budget), "Situacao atual", cardWidth);
        ImGui::SameLine();
        metricCard("##mov", "MOVIMENTACOES", std::to_string(transactions_.size()), "Nesta sessao", cardWidth);
        ImGui::Spacing();

        ImGui::BeginChild("##finance_actions", ImVec2(0, 195), true);
        ImGui::TextColored(VERDE, "REGISTRAR MOVIMENTACAO");
        ImGui::InputDouble("Valor (R$)", &transactionValue_, 1000.0, 10000.0, "%.2f");
        ImGui::InputText("Descricao", transactionDescription_, sizeof(transactionDescription_));
        if (ImGui::Button("REGISTRAR DESPESA", ImVec2(210, 37))) recordTransaction(true);
        ImGui::SameLine();
        if (ImGui::Button("REGISTRAR RECEITA", ImVec2(210, 37))) recordTransaction(false);
        if (!financeMessage_.empty()) ImGui::TextWrapped("%s", financeMessage_.c_str());
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Text("HISTORICO DE MOVIMENTACOES");
        if (transactions_.empty()) {
            ImGui::TextDisabled("Nenhuma movimentacao registrada nesta sessao.");
        } else if (ImGui::BeginTable("##finance_history", 2,
                   ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("Descricao");
            ImGui::TableSetupColumn("Valor");
            ImGui::TableHeadersRow();
            for (auto it = transactions_.rbegin(); it != transactions_.rend(); ++it) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", it->description.c_str());
                ImGui::TableNextColumn();
                ImGui::TextColored(it->amount >= 0 ? VERDE : AMARELO,
                                   "%s", money(it->amount).c_str());
            }
            ImGui::EndTable();
        }
        ImGui::TextDisabled("A integracao com Financeiro e a persistencia serao feitas depois.");
    }

    void renderStandings() {
        sectionTitle("CLASSIFICACAO", "Desempenho da Mercedes na temporada demonstrativa");
        ImGui::BeginChild("##constructors_table", ImVec2(0, 165), true);
        ImGui::TextColored(VERDE, "CAMPEONATO DE CONSTRUTORES");
        ImGui::Separator();
        ImGui::Text("MERCEDES  /  POSICAO #%d", team_.constructorsPosition);
        ImGui::Text("%d pontos", team_.constructorsPoints);
        ImGui::TextDisabled("Posicao ilustrativa; a classificacao oficial depende do Campeonato.");
        ImGui::EndChild();
        ImGui::Spacing();
        ImGui::Text("PILOTOS DA MERCEDES");
        for (const Driver& d : drivers_) {
            ImGui::BeginChild(d.number.c_str(), ImVec2(0, 85), true);
            ImGui::Text("#%s  %s", d.number.c_str(), d.name.c_str());
            ImGui::TextColored(VERDE, "%d pts  |  %d vitorias", d.points, d.wins);
            ImGui::EndChild();
        }
    }

    void exportReport() {
        std::ofstream out("relatorio_mercedes_demo.txt");
        if (!out) {
            reportMessage_ = "Nao foi possivel criar o arquivo de relatorio.";
            return;
        }
        out << "F1 MANAGER / MERCEDES (RELATORIO DEMONSTRATIVO)\n";
        out << "Temporada: " << team_.season << '\n';
        out << "Orcamento: " << money(team_.budget) << '\n';
        out << "Pontos de construtores: " << team_.constructorsPoints << '\n';
        for (const Driver& d : drivers_) {
            out << d.name << " (#" << d.number << "): " << d.points << " pontos\n";
        }
        reportMessage_ = out ? "Relatorio gerado: relatorio_mercedes_demo.txt"
                             : "Ocorreu um erro ao salvar o relatorio.";
    }

    void renderReports() {
        sectionTitle("RELATORIOS", "Resumo administrativo da Mercedes");
        ImGui::BeginChild("##report_card", ImVec2(0, 260), true);
        ImGui::TextColored(VERDE, "RELATORIO GERAL DA TEMPORADA");
        ImGui::Separator();
        ImGui::BulletText("Equipe: %s", team_.name.c_str());
        ImGui::BulletText("Temporada: %d", team_.season);
        ImGui::BulletText("Pilotos titulares: %zu", drivers_.size());
        ImGui::BulletText("Orcamento: %s", money(team_.budget).c_str());
        ImGui::BulletText("Pontos de construtores: %d", team_.constructorsPoints);
        ImGui::Spacing();
        if (ImGui::Button("EXPORTAR RELATORIO DEMO", ImVec2(255, 37))) {
            exportReport();
        }
        if (!reportMessage_.empty()) ImGui::TextWrapped("%s", reportMessage_.c_str());
        ImGui::EndChild();
        ImGui::TextDisabled("Relatorio ilustrativo. Na versao final sera alimentado pelo nucleo do projeto.");
    }
};

} // namespace app

// ========================================
// PONTO DE ENTRADA DA INTERFACE GRAFICA
// ========================================
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
