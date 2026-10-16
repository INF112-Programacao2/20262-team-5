# 🏎️ Sistema de Gerenciamento de Fórmula 1 — F1 Manager

Sistema desenvolvido em C++ com o objetivo de gerenciar informações relacionadas à equipe Mercedes-AMG PETRONAS Formula One Team, tendo como principal funcionalidade um **Simulador de Desempenho**. O projeto também contempla funcionalidades de gerenciamento financeiro, organização de informações e geração de relatórios.

O trabalho faz parte de um projeto acadêmico da disciplina de Programação 2, do curso de Ciência da Computação da Universidade Federal de Viçosa (UFV).

**Status:** Em desenvolvimento.

---

## 📑 Índice

- [Sobre o projeto](#-sobre-o-projeto)
- [Objetivos](#-objetivos)
- [Simulador de desempenho](#-simulador-de-desempenho)
- [Funcionalidades](#-funcionalidades)
- [Tecnologias utilizadas](#-tecnologias-utilizadas)
- [Estrutura do projeto](#-estrutura-do-projeto)
- [Armazenamento de dados](#-armazenamento-de-dados)
- [Equipe de desenvolvimento](#-equipe-de-desenvolvimento)
- [Referências](#-referências)

---

## 🎯 Sobre o projeto

O F1 Manager é um sistema de gerenciamento inspirado no contexto da Fórmula 1, com foco na equipe Mercedes-AMG PETRONAS.

A proposta é desenvolver uma aplicação com interface gráfica que permita organizar informações da equipe e utilizar dados do sistema para simular e apresentar indicadores de desempenho.

O projeto é desenvolvido com foco na aplicação prática dos conceitos de programação orientada a objetos, organização de código, manipulação de arquivos e desenvolvimento de interfaces gráficas em C++.

## 🎯 Objetivos

- Desenvolver um sistema de gerenciamento relacionado à Fórmula 1.
- Implementar um simulador de desempenho como funcionalidade central.
- Organizar informações sobre pilotos, corridas e resultados.
- Gerenciar informações financeiras da equipe.
- Apresentar dados e resultados por meio de relatórios.
- Aplicar conceitos de programação orientada a objetos.
- Desenvolver uma interface gráfica interativa.

## 🏁 Simulador de desempenho

O **Simulador de Desempenho** é o principal componente do sistema.

Seu propósito é utilizar os dados disponíveis para representar e analisar o desempenho da equipe em cenários relacionados às corridas de Fórmula 1.

A proposta contempla:

- Análise de indicadores de desempenho.
- Organização dos dados relacionados às corridas.
- Apresentação visual dos resultados.
- Comparação de informações para auxiliar a análise de desempenho.

As funcionalidades específicas serão implementadas e refinadas durante o desenvolvimento do projeto.

## ⚙️ Funcionalidades

O sistema está sendo desenvolvido com as seguintes funcionalidades previstas:

- **Simulador de desempenho:** análise e apresentação de indicadores relacionados às corridas.
- **Gerenciamento de pilotos:** organização das informações dos pilotos.
- **Gerenciamento de corridas:** armazenamento e consulta de dados relacionados às corridas.
- **Gerenciamento financeiro:** organização de receitas, despesas e demais informações financeiras.
- **Relatórios:** apresentação organizada dos dados e resultados do sistema.
- **Interface gráfica:** interação com as funcionalidades por meio de uma interface visual.

> Observação: a lista apresenta as funcionalidades previstas para o projeto. Nem todas necessariamente estão implementadas na versão atual.

## 💻 Tecnologias utilizadas

| Tecnologia | Aplicação |
|---|---|
| C++ | Linguagem principal de desenvolvimento |
| SFML 3 | Janela gráfica e recursos multimídia |
| Dear ImGui | Construção dos componentes da interface |
| ImGui-SFML | Integração entre Dear ImGui e SFML |
| Git | Controle de versão |
| GitHub | Hospedagem e colaboração no código |

## 📂 Estrutura do projeto

A estrutura principal do projeto está organizada da seguinte maneira:

```text
F1-MANAGER/
├── data/
│   ├── corridas.txt
│   ├── financeiro.txt
│   └── pilotos.txt
├── include/
│   ├── arquivos.hpp
│   └── ui/
│       └── Interface.hpp
└── src/
    ├── main.cpp
    └── ui/
        └── main.cpp
```

- **`data/`**: arquivos utilizados para armazenar dados do sistema.
- **`include/`**: arquivos de cabeçalho e declarações.
- **`include/ui/`**: declarações relacionadas à interface gráfica.
- **`src/`**: arquivos de implementação.
- **`src/ui/`**: implementação da interface gráfica.

A estrutura poderá ser modificada conforme a evolução do projeto.

## 💾 Armazenamento de dados

O sistema utiliza arquivos de texto para armazenar informações, sem depender de um banco de dados SQL.

| Arquivo | Finalidade |
|---|---|
| `corridas.txt` | Armazenamento de informações relacionadas às corridas |
| `financeiro.txt` | Armazenamento de informações financeiras |
| `pilotos.txt` | Armazenamento de informações dos pilotos |

A leitura e a escrita desses arquivos permitem persistir informações entre as execuções do programa, conforme as funcionalidades implementadas.

## 👥 Equipe de desenvolvimento

| Integrante | Matricula |
|---|---|
| Kelvyn Caetano | 124691 |
| Pedro | 124670 |
| Pedro Henrique | 124667 |
| Gabriel | 124671 |

**Instituição:** Universidade Federal de Viçosa (UFV)  
**Curso:** Ciência da Computação  
**Disciplina:** Programação II

## 📚 Referências

As fontes abaixo servem como referências para o desenvolvimento, a consulta de documentação técnica e a obtenção de informações sobre a Fórmula 1.

1. **Fórmula 1 — Site oficial:** https://www.formula1.com/
2. **Mercedes-AMG PETRONAS Formula One Team — Site oficial:** https://www.mercedesamgf1.com/
3. **SFML — Documentação oficial:** https://www.sfml-dev.org/
4. **Dear ImGui — Repositório oficial:** https://github.com/ocornut/imgui
5. **ImGui-SFML — Repositório oficial:** https://github.com/SFML/imgui-sfml

---

*Projeto acadêmico desenvolvido para fins educacionais.*
