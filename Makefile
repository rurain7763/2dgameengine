build:
	g++ -Wall -std=c++17 -I/opt/homebrew/include ./src/*.cpp -L/opt/homebrew/lib -lSDL2 -o engine.exe
	
run:
	./engine.exe
