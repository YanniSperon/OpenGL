#include <sstream>
#include <iostream>
#include <fstream>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GLErrorManager.h"

#include "Renderer.h"

const char* get_platform_name() {
	return (PLATFORM_NAME == nullptr) ? "" : PLATFORM_NAME;
}

int main(void)
{
	GLFWwindow* window;

	//Initialize the library
	if (!glfwInit()) {
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	// ---------------------------------------------------
	// quick simple run down of how opengl pipeline works:
	// ---------------------------------------------------
	// -we basically supply our gpu with data. we store memory on gpu that contains
	// data of drawing like verticies and stuff.
	// -we use a shader executed on gpu to read data and put it on screen
	// this uses a vertex buffer which is stored on gpu which is read
	// by shader that knows layout and it knows all the data
	// without vertex buffer, we dont know its anything but bytes.
	// since it tells opengl what it is and what is in it
	// ---------------------------------------------------

	// a vertex is not a position, it can contain much more than that.
	// it is just mostly used to mean that.
	// so he will call it a vertex position


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core/compat profile

	std::cout << "Driver Version:    " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Operating System:  " << get_platform_name() << std::endl;
	{
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		};

		// Uniforms are a way to get cpu data to our shader
		// Like a way to pass our c++ code info to our shader to like color a triangle
		// attributes are per vertex and uniforms are per draw
		// must bound shader program before using glUniform

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};


		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		va.AddBuffer(vb, 2);

		

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");


		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;


		float r = 0.0f;
		float rInc = 0.05f;
		float g = 1.0f;
		float gInc = 0.04f;
		float b = 0.5f;
		float bInc = 0.025f;

		float a = 1.0f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();





			shader.SetUniform4f("u_Color", r, g, b, a);

			renderer.Draw(va, ib, shader);





			if (r >= 1.0f) {
				rInc = -0.05f;
			}
			else if (r <= 0.0f) {
				rInc = 0.05f;
			}

			if (g >= 1.0f) {
				gInc = -0.04f;
			}
			else if (g <= 0.0f) {
				gInc = 0.04f;
			}

			if (b <= 1.0f) {
				bInc = -0.025f;
			}
			else if (b >= 0.0f) {
				bInc = 0.025f;
			}

			r += rInc;
			g += gInc;
			b += bInc;




			/* Swap front and back buffers */

			glfwSwapBuffers(window);


			/* Poll for and process events */

			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}