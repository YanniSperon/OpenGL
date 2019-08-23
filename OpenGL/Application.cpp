#include "Main.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
	GLFWwindow* window;

	//Initialize the library
	if (!glfwInit()) {
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	if (glewInit() != GLEW_OK) {
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f,
	};

	unsigned int buffer;
	// & is used for the memory address
	glGenBuffers(1, &buffer);
	// create buffers^ then set \/ as active
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// once buffer is bound, we can tell opengl what the layout of our buffer is.
	glEnableVertexAttribArray(0);
	// index we want to   \/   ^ enable
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	// this is the starting index, amount of indexes in each pair, valuetype, normalize?
	// , amount of bytes between each pair of values, and finally offset between each
	// attribute (only needed if you store multiple values like texture data too

	// dynamic is changed values (updated every frame) and drawn often

	// we are storing the cache (buffer) with the variable name buffer to access it.
	// it returns an integer which is used like a key in a dictionary

	// first we create them with glGenBuffers(size, &key) then we bind it to the
	// currently used buffer with glBindBuffers(type(enum), key)

	// next we are creating the bufferdata to give opengl the data we are working
	// with and the size of it. Since we created a buffer, but did not actually put
	// anything in it
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		// issue draw call for created buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// only do that ^ if you dont have an index buffer. We dont
		// have to pass in any values because they are all already
		// selected

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	std::cout << "Program closed!";

	glfwTerminate();
	return 0;
}