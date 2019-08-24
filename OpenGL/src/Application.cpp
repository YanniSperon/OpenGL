#include "Main.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
const char* get_platform_name() {
	return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}

static void GLClearError() {
	while (glGetError());
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << std::endl << "[OpenGL Error] (" << "0x" << std::hex << error << std::dec 
			<< " -- "<< error << "):" << std::endl << "        Function: -   " << 
			function << std::endl << "        File:     -   " << file << std::endl 
			<< "        Line:     -   " << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
				// set mode to vertex
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	// Need error handling for syntax and stuff in the shader source code

	return id;
}

// take in two shaders to compile them. The string is the shader source code, you can take it in from a file or something.
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	//Initialize the library
	if (!glfwInit()) {
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
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

	std::cout << "Driver Version:    " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Operating System:  " << get_platform_name() << std::endl;

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

	unsigned int buffer;
	// & is used for the memory address
	GLCall(glGenBuffers(1, &buffer));
	// create buffers^ then set \/ as active
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	// next we are creating the bufferdata to give opengl the data we are working
	// with and the size of it. Since we created a buffer, but did not actually put
	// anything in it
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	// once buffer is bound, we can tell opengl what the layout of our buffer is.
	GLCall(glEnableVertexAttribArray(0));
	// index we want to   \/   ^ enable
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	// this is the starting index, amount of indexes in each pair, valuetype, normalize?
	// , amount of bytes between each pair of values (stride), and finally offset between each
	// attribute (only needed if you store multiple values like texture data too

	// dynamic is changed values (updated every frame) and drawn often

	// we are storing the cache (buffer) with the variable name buffer to access it.
	// it returns an integer which is used like a key in a dictionary

	// first we create them with glGenBuffers(size, &key) then we bind it to the
	// currently used buffer with glBindBuffers(type(enum), key)

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	
	ShaderProgramSource source = parseShader("res/shaders/Basic.shader");

	unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	// var name in shader of uniform
	int location = glGetUniformLocation(shader, "u_Color");
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));




	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));




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
		GLCall(glClear(GL_COLOR_BUFFER_BIT));


		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, g, b, a));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		// issue draw call for created buffer
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		// only do that ^ if you dont have an index buffer. We dont
		// have to pass in any values because they are all already
		// selected
		/* Swap front and back buffers */


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
		


		
		glfwSwapBuffers(window);

		/* Poll for and process events */

		glfwPollEvents();
	}
	std::cout << "Program closed!";
	//glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}