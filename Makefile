# Definimos el compilador y las banderas
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Carpeta donde se encuentran los archivos fuente
SRC_DIR = src
# Carpeta donde se almacenarán los archivos objeto
OBJ_DIR = bin

# Archivos fuente
CLIENT_SRC = $(SRC_DIR)/client.cpp
SERVER_SRC = $(SRC_DIR)/server.cpp

# Archivos objeto generados en la carpeta bin
CLIENT_OBJ = $(OBJ_DIR)/client.o
SERVER_OBJ = $(OBJ_DIR)/server.o

# Ejecutables a generar en el directorio raíz
CLIENT_EXEC = client
SERVER_EXEC = server

# Regla por defecto: compila los ejecutables
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Crear la carpeta bin si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regla para compilar el ejecutable client
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar el ejecutable server
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar los archivos .cpp a .o (archivos objeto) en la carpeta bin
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar los archivos generados
clean:
	rm -rf $(OBJ_DIR) $(CLIENT_EXEC) $(SERVER_EXEC)

# Instalar los binarios (opcional)
install: $(CLIENT_EXEC) $(SERVER_EXEC)
	# Añadir los comandos de instalación si es necesario (por ejemplo, copiar a /usr/local/bin)

.PHONY: all clean install
