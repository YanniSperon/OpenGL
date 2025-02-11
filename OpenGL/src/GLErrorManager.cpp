#include "GLErrorManager.h"
#include "GL/glew.h"
#include <iostream>

void GLClearError() {
	while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << std::endl << "[OpenGL Error] (" << "0x" << std::hex << error << std::dec
			<< " -- " << error << "):" << std::endl << "        Function: -   " <<
			function << std::endl << "        File:     -   " << file << std::endl
			<< "        Line:     -   " << line << std::endl;
		return false;
	}
	return true;
}