CC = g++
LANG_STD = -std=c++17
FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I/opt/homebrew/include
SRC_FILES = ./src/*.cpp ./src/Logger/*.cpp ./src/Game/*.cpp
LIB_PATH = -L/opt/homebrew/lib
LIBS = -lSDL2 -lSDL2_image -lglm
PROJECT = engine

build:
	${CC} ${FLAGS} ${LANG_STD} ${INCLUDE_PATH} ${SRC_FILES} ${LIB_PATH} ${LIBS} -o ${PROJECT}.exe
	
run:
	./${PROJECT}.exe
