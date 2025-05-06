CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
INCLUDES = -Isrc -I/usr/include/qt -I/usr/include/qt/QtWidgets -I/usr/include/qt/QtCore -I/usr/include/qt/QtGui -I/usr/include/qt/QtOpenGL

SRC_DIR = src
BUILD_DIR = build
MOC_DIR = $(BUILD_DIR)/moc
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

QT_LIBS = -lQt5Widgets -lQt5Core -lQt5Gui -lQt5OpenGL
GL_LIBS = -lGL -lGLU -lglut

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/view/*.cpp) $(wildcard $(SRC_DIR)/model/*.cpp) $(wildcard $(SRC_DIR)/controller/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/view/*.h) $(wildcard $(SRC_DIR)/model/*.h) $(wildcard $(SRC_DIR)/controller/*.h)
MOC_HEADERS = $(wildcard $(SRC_DIR)/view/*.h)
MOC_SOURCES = $(patsubst $(SRC_DIR)/view/%.h, $(MOC_DIR)/view/moc_%.cpp, $(MOC_HEADERS))
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES)) $(patsubst $(MOC_DIR)/view/moc_%.cpp, $(OBJ_DIR)/view/moc_%.o, $(MOC_SOURCES))
TARGET = $(BIN_DIR)/viewer

all: dirs $(MOC_SOURCES) $(TARGET)

dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(MOC_DIR)/view
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/view
	mkdir -p $(OBJ_DIR)/model
	mkdir -p $(OBJ_DIR)/controller
	mkdir -p $(OBJ_DIR)/view/moc
	mkdir -p $(BIN_DIR)

$(MOC_DIR)/view/moc_%.cpp: $(SRC_DIR)/view/%.h
	moc $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/view/moc_%.o: $(MOC_DIR)/view/moc_%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(QT_LIBS) $(GL_LIBS)

format-check:
	clang-format -style=Google -n $(SOURCES) $(HEADERS)

format:
	clang-format -style=Google -i $(SOURCES) $(HEADERS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all dirs clean format-check format 