main.exe: folder build/glad.o main.o build/stb.o
	gcc -std=c11 "build/glad.o" "build/main.o" "build/stb.o" -o "build/main.exe" -L "lib/" -lopengl32 -lglfw3 -lgdi32 

folder:
	if not exist "build/" (mkdir build)

build/glad.o: src/glad.c
	gcc -std=c11 -c -I "include/" "src/glad.c" -o "build/glad.o" -lopengl32 -lgdi32 -lglfw3

main.o: src/main.c
	gcc -std=c11 -c -I "include/" "src/main.c" -o "build/main.o" -L "lib/" -lopengl32 -lglfw3 -lgdi32

build/stb.o: include/stb/stb.c
	gcc -std=c11 -c -I "include/" "include/stb/stb.c" -o "build/stb.o" -L "lib/" -lopengl32 -lglfw3 -lgdi32