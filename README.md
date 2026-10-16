## Compilação

O projeto utiliza C++ com SFML 3, Dear ImGui e ImGui-SFML. As dependências precisam estar instaladas e configuradas antes da compilação.

### Windows

Recomendamos utilizar o MSYS2 UCRT64, com compilador C++ e as bibliotecas necessárias. Configure os caminhos de include e linkagem para SFML 3, Dear ImGui e ImGui-SFML.

### Ubuntu / Linux

Instale as ferramentas básicas de desenvolvimento:

```bash
sudo apt update
sudo apt install build-essential cmake git pkg-config
```

Instale ou compile uma versão compatível da SFML 3, do Dear ImGui e do ImGui-SFML. Os pacotes disponíveis nos repositórios da distribuição podem ter versões diferentes das utilizadas no projeto.

Depois, configure os caminhos das bibliotecas e compile o código conforme a configuração de build do projeto.

### Observações

- As versões de SFML, Dear ImGui e ImGui-SFML precisam ser compatíveis entre si.
- As bibliotecas externas não estão incluídas neste repositório.
- O comando exato de compilação será documentado após a configuração do sistema de build.
