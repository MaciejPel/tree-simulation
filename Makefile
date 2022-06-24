LIBS=-lGL -lglfw -lGLEW
HEADERS=allmodels.h  constants.h  cube.h  lodepng.h  model.h  shaderprogram.h cone.h leaf.h
FILES=cube.cpp  lodepng.cpp  main_file.cpp  model.cpp  shaderprogram.cpp cone.cpp leaf.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
	./main_file