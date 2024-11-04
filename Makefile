# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente y objetivos
CLIENT_SRC = $(SRC_DIR)/client_start.cpp
SERVER_SRC = $(SRC_DIR)/server_start.cpp
CLIENT_OBJ = $(OBJ_DIR)/client_start.o
SERVER_OBJ = $(OBJ_DIR)/server_start.o
CLIENT_BIN = $(BIN_DIR)/client
SERVER_BIN = $(BIN_DIR)/server

# Objetivo por defecto
all: $(CLIENT_BIN) $(SERVER_BIN)

# Compilar cliente
$(CLIENT_BIN): $(CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar servidor
$(SERVER_BIN): $(SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar archivos .cpp a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Ejecutar cliente y servidor
run_client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

run_server: $(SERVER_BIN)
	./$(SERVER_BIN)

.PHONY: all clean run_client run_server
