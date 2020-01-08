EXEC = 3DCube
COMP = g++
LIB = -lglfw -lGLEW -lGL
FLAGS = -Wall
DIRS = -I./ -Ivendor/
IMGUI = imgui_impl_opengl3.o imgui_impl_glfw.o imgui.o imgui_demo.o imgui_draw.o imgui_widgets.o
OBJS = GLCall.o VertexBuffer.o VertexArray.o BufferLayoutData.o IndexBuffer.o Shader.o Texture.o stb_image.o Renderer.o $(IMGUI)

$(EXEC): source.cpp $(OBJS)
	$(COMP) $^ -o $(EXEC) $(LIB) $(FLAGS) $(DIRS)

GLCall.o: GLCall.cpp GLCall.h
	$(COMP) $< -c $(DIRS) $(FLAGS)

VertexArray.o: VertexArray.cpp VertexArray.h BufferLayoutData.o VertexBuffer.o GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

VertexBuffer.o: VertexBuffer.cpp VertexBuffer.h GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

IndexBuffer.o: IndexBuffer.cpp IndexBuffer.h GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

BufferLayoutData.o: BufferLayoutData.cpp BufferLayoutData.h GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

Shader.o: Shader.cpp Shader.h GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

Texture.o: Texture.cpp Texture.h GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

Renderer.o: Renderer.cpp Renderer.h VertexArray.o Shader.o IndexBuffer.o GLCall.o
	$(COMP) $< -c $(DIRS) $(FLAGS)

stb_image.o: vendor/stb_image/stb_image.cpp vendor/stb_image/stb_image.h
	$(COMP) $< -c $(DIRS) $(FLAGS)

imgui.o: vendor/imgui/imgui.cpp vendor/imgui/imgui.h
	$(COMP) $< -c $(INCLUDE)

imgui_demo.o: vendor/imgui/imgui_demo.cpp
	$(COMP) $< -c $(INCLUDE)

imgui_draw.o: vendor/imgui/imgui_draw.cpp
	$(COMP) $< -c $(INCLUDE)

imgui_impl_glfw.o: vendor/imgui/imgui_impl_glfw.cpp vendor/imgui/imgui_impl_glfw.h
	$(COMP) $< -c $(INCLUDE)

imgui_impl_opengl3.o: vendor/imgui/imgui_impl_opengl3.cpp vendor/imgui/imgui_impl_opengl3.h
	$(COMP) $< -c $(INCLUDE)

imgui_widgets.o: vendor/imgui/imgui_widgets.cpp
	$(COMP) $< -c $(INCLUDE)

# Remove .o Files
clean:
	rm *.o
