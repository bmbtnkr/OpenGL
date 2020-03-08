#include <glad/glad.h> // include GLAD before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

// resize callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// process input function prototype
void processInput(GLFWwindow* window);

// source code for vertex shader stored in a const C string
const char* vertexShaderSource = "#version 460 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// source code for fragment shader stored in a const C string
const char* fragmentShaderSource = "#verson 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


int main() {
	glfwInit(); // initialize GLFW library
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // sets hint to next glfwCreateWindow call
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // explicity use core-profile
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS compatability / optimization flag

	// create window object - holds all windowing data
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); 
	// required by most of GLFW's other functions
	// requires width and height, optional argument: window name
	
	// this function returns a GLFWwindow object we'll need later for other GLFW operations
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tell GLFW to make the context of the window the main context on the current thread
	glfwMakeContextCurrent(window);
	
	// We register callback functions after creating the window but before the render loop is initiated
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the resize window callback

	/* GLAD manages function pointers for OpenGL so we ant to initialize GLAD before calling any OpenGL functions
		pass GLAD the function to load the address of the OpenGL function pointers
		GLFW gives us glfwGetProcAddress that defines the correct function based on the OS 
	*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setup triangle coordinates using Normalized Device Coordinates (origin=center)
	float verticies[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// generate a buffer using a unique id
	unsigned int VBO;
	glGenBuffers(1, &VBO); // size, *buffer
	// bind the newly created buffer to the array buffer target
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // target, buffer
	/*	Any buffer calls made using array buffer target will be used to configure the 
		previously defined and bound buffer: VBO.
		We can make a call to glBufferData function that copies the previously defined vertex data
		into the buffer's memory
	*/

	// copy user-defined data into the currently defined buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // target, size, data, usage
	// target = type of buffer we want to copy data into: the Vertex Buffer Object currently bound to the GL_ARRAY_BUFFER target
	// size = size in bytes
	// data = actual data we want to send
	// usage = how we want the Graphics Card to manage the given data
	// GL_STREAM_DRAW: data is set once and used by the GPU a few times
	// GL_STATIC_DRAW: data is set once and used many times
	// GL_DYNAMIC_DRAW: data is changed a lot and used many times

	// -----------------------------------------------------------------------------------------------
	// Setup the Vertex / Fragment Shader
	// -----------------------------------------------------------------------------------------------
	// dynamically compile the vertex shader from its source code
	// create a shader object, referenced by an ID
	unsigned int vertexShader;
	// provide the type of shader we want to create as an argument to glCreateShader
	// pass in GL_VERTEX_SHADER
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // shader type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER

	// attach the shader source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // shader, count, string, length
	glCompileShader(vertexShader);

	// check if compilation was successful
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // shader, pname, params

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	// setup the fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// link the compiled shaders to a shader program
	// activate this shader program when rendering objects
	// the activated shader program's shaders will be used when issuing render calls
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// attach the previously compiled shaders to the program object and link them with glLinkProgram
	glAttachShader(shaderProgram, vertexShader); // program, shader
	glAttachShader(shaderProgram, fragmentShader); // program, shader
	glLinkProgram(shaderProgram); // program

	// check if linking the shader program failed




	// render loop - keep drawing images and handling user input until the program has been told to stop
	// check the start of each loop iteration if GLFW has been instructed to close
	while (!glfwWindowShouldClose(window))
	{
		// detect escape key press, close window on event
		processInput(window);

		// rendering commands here
		// at the start of each frame, clear the screen with a color of our choice
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the entire color buffer (we only care about colors) of the current frame buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// swap the color buffer
		// color buffer - a large 2d buffer that contains color values for each pixel
		// in GLFW's window that is used to render during this iteration and output to screen
		glfwSwapBuffers(window);
		// check if any events are triggered (keyboard/mouse input)
		// updates window state, and calls corresponding functions
		glfwPollEvents();
	}

	// Tell OpenGL the size of the rendering window
	// glViewport(0, 0, 800, 600); // x, y, width, height 
	// set the location of the lower left corner of the window
	
	// clean / delete all GLFW's resources that were allocated
	glfwTerminate();
	return 0;
}


// Register a callback function on the window that gets called each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Function to detect ESC key press
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}