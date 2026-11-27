
# F1 Manager - Mercedes | GNU Make | C++17
# Executar na pasta F1-MANAGER (Ubuntu/WSL ou MSYS2).

CXX := g++
CPPFLAGS := -Iinclude
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic
LDFLAGS :=
LDLIBS :=

ifeq ($(STRICT),1)
CXXFLAGS += -Werror
endif

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

SRC_DIR := src
TEST_DIR := tests
TOOLS_DIR := tools
BUILD_DIR := build
BIN_DIR := bin

# Programa principal headless
APP_SRC := $(SRC_DIR)/main_headless.cpp
APP_OBJ := $(BUILD_DIR)/src/main_headless.o
APP := $(BIN_DIR)/f1_manager$(EXE)

# Impede que os testes recebam mais de uma funcao main().
MAIN_SRCS := $(wildcard $(SRC_DIR)/main*.cpp)
CORE_SRCS := $(filter-out $(MAIN_SRCS),$(wildcard $(SRC_DIR)/*.cpp))
CORE_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/src/%.o,$(CORE_SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/teste_*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%$(EXE),$(TEST_SRCS))

TOOLS_SRCS := $(wildcard $(TOOLS_DIR)/*.cpp)
TOOLS_BINS := $(patsubst $(TOOLS_DIR)/%.cpp,$(BIN_DIR)/%$(EXE),$(TOOLS_SRCS))

.DEFAULT_GOAL := all
.PHONY: all run test tools clean help

all: $(APP)

$(APP): $(APP_OBJ) $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR)/teste_%$(EXE): $(TEST_DIR)/teste_%.cpp $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR)/%$(EXE): $(TOOLS_DIR)/%.cpp $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

run: $(APP)
	@./$(APP)

test: $(TEST_BINS)
	@set -e; $(foreach t,$(TEST_BINS),echo "Executando $(t)"; ./$(t);) echo "Todos os testes passaram."

tools: $(TOOLS_BINS)
	@echo "Ferramentas compiladas."

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(APP) $(TEST_BINS) $(TOOLS_BINS)
	@echo "Objetos e executaveis gerados pelo Makefile removidos."

help:
	@echo "make           Compila o sistema da Mercedes"
	@echo "make run       Executa o menu headless"
	@echo "make test      Compila e executa os testes"
	@echo "make tools     Compila ferramentas de tools/"
	@echo "make clean     Remove os objetos e executaveis gerados"
	@echo "make STRICT=1  Ativa -Werror"

-include $(CORE_OBJS:.o=.d) $(APP_OBJ:.o=.d)
