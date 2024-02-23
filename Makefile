CXX=g++
CXXFLAGS=-lglfw

engineTemplate: src/engine_core/main.cpp src/engine_core/TextureManager.cpp src/engine_core/ShaderManager.cpp src/engine_core/GLCore.cpp src/utils/stb_image.cpp src/utils/glad.c
	$(CXX) -o engineTemplate src/engine_core/main.cpp src/engine_core/TextureManager.cpp src/engine_core/ShaderManager.cpp src/engine_core/GLCore.cpp src/utils/stb_image.cpp src/utils/glad.c $(CXXFLAGS) 

TextureManager.o: src/engine_core/TextureManager.cpp
	$(CXX) -o TextureManager.o src/engine_core/TextureManager.cpp src/utils/glad.c src/utils/stb_image.cpp $(CXXFLAGS) 
	
ShaderManager.o: src/engine_core/ShaderManager.cpp
	$(CXX) -o ShaderManager.o src/engine_core/ShaderManager.cpp src/utils/glad.c $(CXXFLAGS) 
	
GLCore.o: src/engine_core/GLCore.cpp
	$(CXX) -o GLCore.o src/engine_core/GLCore.cpp src/utils/glad.c $(CXXFLAGS) 

stb_image.o: src/utils/stb_image.cpp
	$(CXX) -o stb_image.o src/utils/stb_image.cpp src/utils/glad.c $(CXXFLAGS) 

.PHONY: clean

clean:
	rm engineTemplate