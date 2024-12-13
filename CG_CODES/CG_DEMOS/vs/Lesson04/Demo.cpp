#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	Build3DDesks();
	BuildTexturedPlane();
	BuildTexturedWalls();
	Build3DWhiteboard();
	Build3DAlmari();
	Build3DWindows();
	Build3DAC();
	BuildTexturedCeiling();
	Build3DDeskSatu();
	Build3DPainting();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	Draw3DDesks();
	DrawColoredPlane();
	DrawTexturedWalls();
	Draw3DWhiteboard();
	Draw3DAlmari();
	Draw3DWindows();
	Draw3DAC();
	DrawTexturedCeiling();
	Draw3DDeskSatu();
	Draw3DPainting();
	

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildTexturedCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 1, 0,  // 5
		0.5, -0.5, -0.5, 1, 1,  // 6
		0.5, -0.5,  0.5, 0, 1,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 1, 0, // 9
		0.5,   0.5, -0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 1, 0, // 13
		-0.5,  0.5,  0.5, 1, 1, // 14
		-0.5,  0.5, -0.5, 0, 1, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 1, 0,  // 17
		-0.5, 0.5, -0.5, 1, 1,  // 18
		0.5, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 1, 0,  // 21
		0.5, -0.5,  0.5, 1, 1,  // 22
		-0.5, -0.5,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-10.0, -0.5, -10.0,  0,  0,
		 10.0, -0.5, -10.0, 10,  0,
		 10.0, -0.5,  10.0, 10, 10,
		-10.0, -0.5,  10.0,  0, 10,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Demo::BuildTexturedWalls()
{
	// Load and create a texture for the walls
	glGenTextures(1, &textureWalls);
	glBindTexture(GL_TEXTURE_2D, textureWalls);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("wall.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for walls
	GLfloat vertices[] = {
		// Format position, tex coords
		// Right wall
		 10.0, -0.5, -10.0, 0, 0,
		 10.0, -0.5,  10.0, 10, 0,
		 10.0, 10.0,  10.0, 10, 10,
		 10.0, 10.0, -10.0, 0, 10,

		 // Left wall
		 -10.0, -0.5, -10.0, 0, 0,
		 -10.0, -0.5,  10.0, 10, 0,
		 -10.0, 10.0,  10.0, 10, 10,
		 -10.0, 10.0, -10.0, 0, 10,

		 // Front wall
		 -10.0, -0.5, 10.0, 0, 0,
		  10.0, -0.5, 10.0, 10, 0,
		  10.0, 10.0, 10.0, 10, 10,
		 -10.0, 10.0, 10.0, 0, 10,

		 // Back wall
		 -10.0, -0.5, -10.0, 0, 0,
		  10.0, -0.5, -10.0, 10, 0,
		  10.0, 10.0, -10.0, 10, 10,
		 -10.0, 10.0, -10.0, 0, 10,
	};

	GLuint indices[] = {
		// Right wall
		0, 1, 2, 0, 2, 3,
		// Left wall
		4, 5, 6, 4, 6, 7,
		// Front wall
		8, 9, 10, 8, 10, 11,
		// Back wall
		12, 13, 14, 12, 14, 15
	};

	glGenVertexArrays(1, &VAOWalls);
	glGenBuffers(1, &VBOWalls);
	glGenBuffers(1, &EBOWalls);

	glBindVertexArray(VAOWalls);

	glBindBuffer(GL_ARRAY_BUFFER, VBOWalls);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWalls);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedWalls()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureWalls);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOWalls);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Build3DWhiteboard()
{
	// Load and create a texture for the whiteboard
	glGenTextures(1, &textureWhiteboard);
	glBindTexture(GL_TEXTURE_2D, textureWhiteboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("download.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for the whiteboard
	GLfloat vertices[] = {
		// Format position, tex coords
		// Front face of the whiteboard
		-3.8f, 1.6f, 9.99f, 0.0f, 0.0f,  // Bottom-left corner (Lebih lebar dan turun sedikit)
		 3.8f, 1.6f, 9.99f, 1.0f, 0.0f,  // Bottom-right corner (Lebih lebar dan turun sedikit)
		 3.8f, 5.8f, 9.99f, 1.0f, 1.0f,  // Top-right corner (Lebih lebar dan lebih tinggi)
		-3.8f, 5.8f, 9.99f, 0.0f, 1.0f   // Top-left corner (Lebih lebar dan lebih tinggi)
	};


	GLuint indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};

	glGenVertexArrays(1, &VAOWhiteboard);
	glGenBuffers(1, &VBOWhiteboard);
	glGenBuffers(1, &EBOWhiteboard);

	glBindVertexArray(VAOWhiteboard);

	glBindBuffer(GL_ARRAY_BUFFER, VBOWhiteboard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWhiteboard);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DWhiteboard()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureWhiteboard);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 2);

	glBindVertexArray(VAOWhiteboard);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Build3DDesks() {
	// Load and create a texture for the desk
	glGenTextures(1, &textureDesk);
	glBindTexture(GL_TEXTURE_2D, textureDesk);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Geometry for the desk top and legs
	GLfloat vertices[] = {
		// Top of the desk (3D block)
		-0.8f, 1.0f, -0.5f, 0.0f, 0.0f,  // Bottom-left
		 0.8f, 1.0f, -0.5f, 1.0f, 0.0f,  // Bottom-right
		 0.8f, 1.2f, -0.5f, 1.0f, 1.0f,  // Top-right
		-0.8f, 1.2f, -0.5f, 0.0f, 1.0f,  // Top-left
		-0.8f, 1.0f,  0.5f, 0.0f, 0.0f,  // Bottom-left back
		 0.8f, 1.0f,  0.5f, 1.0f, 0.0f,  // Bottom-right back
		 0.8f, 1.2f,  0.5f, 1.0f, 1.0f,  // Top-right back
		-0.8f, 1.2f,  0.5f, 0.0f, 1.0f,  // Top-left back

		// Front-left leg (3D block)
		-0.75f, 0.0f, -0.45f, 0.0f, 0.0f,  // Bottom-front-left
		-0.55f, 0.0f, -0.45f, 1.0f, 0.0f,  // Bottom-front-right
		-0.55f, 1.0f, -0.45f, 1.0f, 1.0f,  // Top-front-right
		-0.75f, 1.0f, -0.45f, 0.0f, 1.0f,  // Top-front-left
		-0.75f, 0.0f, -0.35f, 0.0f, 0.0f,  // Bottom-back-left
		-0.55f, 0.0f, -0.35f, 1.0f, 0.0f,  // Bottom-back-right
		-0.55f, 1.0f, -0.35f, 1.0f, 1.0f,  // Top-back-right
		-0.75f, 1.0f, -0.35f, 0.0f, 1.0f,  // Top-back-left

		// Front-right leg
		 0.55f, 0.0f, -0.45f, 0.0f, 0.0f,
		 0.75f, 0.0f, -0.45f, 1.0f, 0.0f,
		 0.75f, 1.0f, -0.45f, 1.0f, 1.0f,
		 0.55f, 1.0f, -0.45f, 0.0f, 1.0f,
		 0.55f, 0.0f, -0.35f, 0.0f, 0.0f,
		 0.75f, 0.0f, -0.35f, 1.0f, 0.0f,
		 0.75f, 1.0f, -0.35f, 1.0f, 1.0f,
		 0.55f, 1.0f, -0.35f, 0.0f, 1.0f,

		 // Back-left leg
		 -0.75f, 0.0f,  0.35f, 0.0f, 0.0f,
		 -0.55f, 0.0f,  0.35f, 1.0f, 0.0f,
		 -0.55f, 1.0f,  0.35f, 1.0f, 1.0f,
		 -0.75f, 1.0f,  0.35f, 0.0f, 1.0f,
		 -0.75f, 0.0f,  0.45f, 0.0f, 0.0f,
		 -0.55f, 0.0f,  0.45f, 1.0f, 0.0f,
		 -0.55f, 1.0f,  0.45f, 1.0f, 1.0f,
		 -0.75f, 1.0f,  0.45f, 0.0f, 1.0f,

		 // Back-right leg
		  0.55f, 0.0f,  0.35f, 0.0f, 0.0f,
		  0.75f, 0.0f,  0.35f, 1.0f, 0.0f,
		  0.75f, 1.0f,  0.35f, 1.0f, 1.0f,
		  0.55f, 1.0f,  0.35f, 0.0f, 1.0f,
		  0.55f, 0.0f,  0.45f, 0.0f, 0.0f,
		  0.75f, 0.0f,  0.45f, 1.0f, 0.0f,
		  0.75f, 1.0f,  0.45f, 1.0f, 1.0f,
		  0.55f, 1.0f,  0.45f, 0.0f, 1.0f,
	};

	GLuint indices[] = {
		// Desk top
		0, 1, 2, 0, 2, 3, // Front
		4, 5, 6, 4, 6, 7, // Back
		0, 1, 5, 0, 5, 4, // Bottom
		1, 2, 6, 1, 6, 5, // Right
		2, 3, 7, 2, 7, 6, // Top
		3, 0, 4, 3, 4, 7, // Left

		// Front-left leg
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		8, 9, 13, 8, 13, 12,
		9, 10, 14, 9, 14, 13,
		10, 11, 15, 10, 15, 14,
		11, 8, 12, 11, 12, 15,

		// Front-right leg
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
		16, 17, 21, 16, 21, 20,
		17, 18, 22, 17, 22, 21,
		18, 19, 23, 18, 23, 22,
		19, 16, 20, 19, 20, 23,

		// Back-left leg
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		24, 25, 29, 24, 29, 28,
		25, 26, 30, 25, 30, 29,
		26, 27, 31, 26, 31, 30,
		27, 24, 28, 27, 28, 31,

		// Back-right leg
		32, 33, 34, 32, 34, 35,
		36, 37, 38, 36, 38, 39,
		32, 33, 37, 32, 37, 36,
		33, 34, 38, 33, 38, 37,
		34, 35, 39, 34, 39, 38,
		35, 32, 36, 35, 36, 39,
	};

	glGenVertexArrays(1, &VAODesk);
	glGenBuffers(1, &VBODesk);
	glGenBuffers(1, &EBODesk);

	glBindVertexArray(VAODesk);

	glBindBuffer(GL_ARRAY_BUFFER, VBODesk);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBODesk);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DDesks() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureDesk);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 3);

	glBindVertexArray(VAODesk);

	// Position the desk on the floor
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw the desk
	glDrawElements(GL_TRIANGLES, 36 + (4 * 36), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::Build3DDeskSatu() {
	// Load and create a texture for the desk
	glGenTextures(1, &textureDeskSatu);
	glBindTexture(GL_TEXTURE_2D, textureDeskSatu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Geometry for the desk (identical to Build3DDesks)
	GLfloat vertices[] = {
		// Top of the desk (3D block)
		-0.5f, 1.0f, -0.5f, 0.0f, 0.0f,  // Bottom-left
		 0.5f, 1.0f, -0.5f, 1.0f, 0.0f,  // Bottom-right
		 0.5f, 1.2f, -0.5f, 1.0f, 1.0f,  // Top-right
		-0.5f, 1.2f, -0.5f, 0.0f, 1.0f,  // Top-left
		-0.5f, 1.0f,  0.5f, 0.0f, 0.0f,  // Bottom-left back
		 0.5f, 1.0f,  0.5f, 1.0f, 0.0f,  // Bottom-right back
		 0.5f, 1.2f,  0.5f, 1.0f, 1.0f,  // Top-right back
		-0.5f, 1.2f,  0.5f, 0.0f, 1.0f,  // Top-left back

		// Front-left leg (3D block)
		-0.55f, 0.0f, -0.45f, 0.0f, 0.0f,  // Bottom-front-left
		-0.35f, 0.0f, -0.45f, 1.0f, 0.0f,  // Bottom-front-right
		-0.35f, 1.0f, -0.45f, 1.0f, 1.0f,  // Top-front-right
		-0.55f, 1.0f, -0.45f, 0.0f, 1.0f,  // Top-front-left
		-0.55f, 0.0f, -0.35f, 0.0f, 0.0f,  // Bottom-back-left
		-0.35f, 0.0f, -0.35f, 1.0f, 0.0f,  // Bottom-back-right
		-0.35f, 1.0f, -0.35f, 1.0f, 1.0f,  // Top-back-right
		-0.55f, 1.0f, -0.35f, 0.0f, 1.0f,  // Top-back-left

		// Front-right leg
		 0.35f, 0.0f, -0.45f, 0.0f, 0.0f,
		 0.55f, 0.0f, -0.45f, 1.0f, 0.0f,
		 0.55f, 1.0f, -0.45f, 1.0f, 1.0f,
		 0.35f, 1.0f, -0.45f, 0.0f, 1.0f,
		 0.35f, 0.0f, -0.35f, 0.0f, 0.0f,
		 0.55f, 0.0f, -0.35f, 1.0f, 0.0f,
		 0.55f, 1.0f, -0.35f, 1.0f, 1.0f,
		 0.35f, 1.0f, -0.35f, 0.0f, 1.0f,

		 // Back-left leg
		 -0.55f, 0.0f,  0.35f, 0.0f, 0.0f,
		 -0.35f, 0.0f,  0.35f, 1.0f, 0.0f,
		 -0.35f, 1.0f,  0.35f, 1.0f, 1.0f,
		 -0.55f, 1.0f,  0.35f, 0.0f, 1.0f,
		 -0.55f, 0.0f,  0.45f, 0.0f, 0.0f,
		 -0.35f, 0.0f,  0.45f, 1.0f, 0.0f,
		 -0.35f, 1.0f,  0.45f, 1.0f, 1.0f,
		 -0.55f, 1.0f,  0.45f, 0.0f, 1.0f,

		 // Back-right leg
		  0.35f, 0.0f,  0.35f, 0.0f, 0.0f,
		  0.55f, 0.0f,  0.35f, 1.0f, 0.0f,
		  0.55f, 1.0f,  0.35f, 1.0f, 1.0f,
		  0.35f, 1.0f,  0.35f, 0.0f, 1.0f,
		  0.35f, 0.0f,  0.45f, 0.0f, 0.0f,
		  0.55f, 0.0f,  0.45f, 1.0f, 0.0f,
		  0.55f, 1.0f,  0.45f, 1.0f, 1.0f,
		  0.35f, 1.0f,  0.45f, 0.0f, 1.0f,
	};

	GLuint indices[] = {
		// Desk top
		0, 1, 2, 0, 2, 3, // Front
		4, 5, 6, 4, 6, 7, // Back
		0, 1, 5, 0, 5, 4, // Bottom
		1, 2, 6, 1, 6, 5, // Right
		2, 3, 7, 2, 7, 6, // Top
		3, 0, 4, 3, 4, 7, // Left

		// Front-left leg
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		8, 9, 13, 8, 13, 12,
		9, 10, 14, 9, 14, 13,
		10, 11, 15, 10, 15, 14,
		11, 8, 12, 11, 12, 15,

		// Front-right leg
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
		16, 17, 21, 16, 21, 20,
		17, 18, 22, 17, 22, 21,
		18, 19, 23, 18, 23, 22,
		19, 16, 20, 19, 20, 23,

		// Back-left leg
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		24, 25, 29, 24, 29, 28,
		25, 26, 30, 25, 30, 29,
		26, 27, 31, 26, 31, 30,
		27, 24, 28, 27, 28, 31,

		// Back-right leg
		32, 33, 34, 32, 34, 35,
		36, 37, 38, 36, 38, 39,
		32, 33, 37, 32, 37, 36,
		33, 34, 38, 33, 38, 37,
		34, 35, 39, 34, 39, 38,
		35, 32, 36, 35, 36, 39,
	};

	glGenVertexArrays(1, &VAODeskSatu);
	glGenBuffers(1, &VBODeskSatu);
	glGenBuffers(1, &EBODeskSatu);

	glBindVertexArray(VAODeskSatu);

	glBindBuffer(GL_ARRAY_BUFFER, VBODeskSatu);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBODeskSatu);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DDeskSatu() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureDeskSatu);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 3);

	glBindVertexArray(VAODeskSatu);

	// Geser meja kedua ke posisi baru (misalnya ke kanan)
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f)); // Geser 2 unit ke kanan
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Gambar meja kedua
	glDrawElements(GL_TRIANGLES, 36 + (4 * 36), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}



void Demo::Build3DAlmari() {
	// Load and create a texture for the almari
	glGenTextures(1, &textureAlmari);
	glBindTexture(GL_TEXTURE_2D, textureAlmari);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("almari.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for the almari
	GLfloat vertices[] = {
		// Format position, tex coords
		// Front face
		-8.0f, -1.0f, 9.5f, 0.0f, 0.0f,  // Bottom-left
		-6.0f, -1.0f, 9.5f, 1.0f, 0.0f,  // Bottom-right
		-6.0f,  4.0f, 9.5f, 1.0f, 1.0f,  // Top-right
		-8.0f,  4.0f, 9.5f, 0.0f, 1.0f,  // Top-left

		// Back face
		-8.0f, -1.0f, 8.5f, 0.0f, 0.0f,  // Bottom-left
		-6.0f, -1.0f, 8.5f, 1.0f, 0.0f,  // Bottom-right
		-6.0f,  4.0f, 8.5f, 1.0f, 1.0f,  // Top-right
		-8.0f,  4.0f, 8.5f, 0.0f, 1.0f,  // Top-left

		// Left face
		-8.0f, -1.0f, 8.5f, 0.0f, 0.0f,  // Bottom-back
		-8.0f, -1.0f, 9.5f, 1.0f, 0.0f,  // Bottom-front
		-8.0f,  4.0f, 9.5f, 1.0f, 1.0f,  // Top-front
		-8.0f,  4.0f, 8.5f, 0.0f, 1.0f,  // Top-back

		// Right face
		-6.0f, -1.0f, 8.5f, 0.0f, 0.0f,  // Bottom-back
		-6.0f, -1.0f, 9.5f, 1.0f, 0.0f,  // Bottom-front
		-6.0f,  4.0f, 9.5f, 1.0f, 1.0f,  // Top-front
		-6.0f,  4.0f, 8.5f, 0.0f, 1.0f,  // Top-back

		// Top face
		-8.0f,  4.0f, 8.5f, 0.0f, 0.0f,  // Back-left
		-6.0f,  4.0f, 8.5f, 1.0f, 0.0f,  // Back-right
		-6.0f,  4.0f, 9.5f, 1.0f, 1.0f,  // Front-right
		-8.0f,  4.0f, 9.5f, 0.0f, 1.0f,  // Front-left

		// Bottom face
		-6.0f, -1.0f, 8.5f, 0.0f, 0.0f,  // Back-left
		-4.0f, -1.0f, 8.5f, 1.0f, 0.0f,  // Back-right
		-4.0f, -1.0f, 9.5f, 1.0f, 1.0f,  // Front-right
		-6.0f, -1.0f, 9.5f, 0.0f, 1.0f   // Front-left
	};

	GLuint indices[] = {
		0,  1,  2,  0,  2,  3,   // Front face
		4,  5,  6,  4,  6,  7,   // Back face
		8,  9,  10, 8,  10, 11,  // Left face
		12, 13, 14, 12, 14, 15,  // Right face
		16, 17, 18, 16, 18, 19,  // Top face
		20, 21, 22, 20, 22, 23   // Bottom face
	};

	glGenVertexArrays(1, &VAOAlmari);
	glGenBuffers(1, &VBOAlmari);
	glGenBuffers(1, &EBOAlmari);

	glBindVertexArray(VAOAlmari);

	glBindBuffer(GL_ARRAY_BUFFER, VBOAlmari);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOAlmari);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}


void Demo::Draw3DAlmari() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureAlmari);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 4);

	glBindVertexArray(VAOAlmari);

	// Draw the almari
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, 0.0f, -4.5f)); // Adjusted position to a corner
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Build3DWindows() {
	// Load and create a texture for the window frame
	glGenTextures(1, &textureWindows);
	glBindTexture(GL_TEXTURE_2D, textureWindows);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Geometry for window frame
	GLfloat vertices[] = {
		// Format: x, y, z, u, v
		// Front face
		4.5f, 2.0f, 9.9f, 0.0f, 0.0f,   // Bottom-left
		8.5f, 2.0f, 9.9f, 1.0f, 0.0f,   // Bottom-right
		8.5f, 5.0f, 9.9f, 1.0f, 1.0f,   // Top-right
		4.5f, 5.0f, 9.9f, 0.0f, 1.0f,   // Top-left

		// Back face
		3.5f, 2.0f, 7.8f, 0.0f, 0.0f,   // Bottom-left
		7.5f, 2.0f, 7.8f, 1.0f, 0.0f,   // Bottom-right
		7.5f, 5.0f, 7.8f, 1.0f, 1.0f,   // Top-right
		3.5f, 5.0f, 7.8f, 0.0f, 1.0f,   // Top-left

		// Left face
		3.5f, 2.0f, 7.8f, 0.0f, 0.0f,   // Bottom-back
		3.5f, 2.0f, 9.2f, 1.0f, 0.0f,   // Bottom-front
		3.5f, 5.0f, 9.2f, 1.0f, 1.0f,   // Top-front
		3.5f, 5.0f, 7.8f, 0.0f, 1.0f,   // Top-back

		// Right face
		7.5f, 2.0f, 7.8f, 0.0f, 0.0f,   // Bottom-back
		7.5f, 2.0f, 9.2f, 1.0f, 0.0f,   // Bottom-front
		7.5f, 5.0f, 9.2f, 1.0f, 1.0f,   // Top-front
		7.5f, 5.0f, 7.8f, 0.0f, 1.0f,   // Top-back

		// Top face
		3.5f, 5.0f, 7.8f, 0.0f, 0.0f,   // Back-left
		7.5f, 5.0f, 7.8f, 1.0f, 0.0f,   // Back-right
		7.5f, 5.0f, 9.2f, 1.0f, 1.0f,   // Front-right
		3.5f, 5.0f, 9.2f, 0.0f, 1.0f,   // Front-left

		// Bottom face
		3.5f, 2.0f, 7.8f, 0.0f, 0.0f,   // Back-left
		7.5f, 2.0f, 7.8f, 1.0f, 0.0f,   // Back-right
		7.5f, 2.0f, 9.2f, 1.0f, 1.0f,   // Front-right
		3.5f, 2.0f, 9.2f, 0.0f, 1.0f    // Front-left
	};


	GLuint indices[] = {
		0, 1, 2, 0, 2, 3, // Front face
	};

	glGenVertexArrays(1, &VAOWindows);
	glGenBuffers(1, &VBOWindows);
	glGenBuffers(1, &EBOWindows);

	glBindVertexArray(VAOWindows);

	glBindBuffer(GL_ARRAY_BUFFER, VBOWindows);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWindows);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DWindows() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textureWindows);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 5);

	glBindVertexArray(VAOWindows);

	// Model transformation to place the window in the desired position
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 1.0f, 9.5f)); // Adjust position
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildTexturedCeiling()
{
	// Load and create a texture for the ceiling
	glGenTextures(1, &textureCeiling);
	glBindTexture(GL_TEXTURE_2D, textureCeiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("ceiling.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for the ceiling
	GLfloat vertices[] = {
		// Format position, tex coords
		// Ceiling (roof)
		-10.0f, 10.0f, -10.0f, 0.0f, 0.0f,  // Back-left
		 10.0f, 10.0f, -10.0f, 1.0f, 0.0f,  // Back-right
		 10.0f, 10.0f,  10.0f, 1.0f, 1.0f,  // Front-right
		-10.0f, 10.0f,  10.0f, 0.0f, 1.0f   // Front-left
	};

	GLuint indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};

	glGenVertexArrays(1, &VAOCeiling);
	glGenBuffers(1, &VBOCeiling);
	glGenBuffers(1, &EBOCeiling);

	glBindVertexArray(VAOCeiling);

	glBindBuffer(GL_ARRAY_BUFFER, VBOCeiling);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCeiling);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTexturedCeiling()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE2); // Use texture unit 2 for the ceiling
	glBindTexture(GL_TEXTURE_2D, textureCeiling);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 2);

	glBindVertexArray(VAOCeiling);

	// Draw the ceiling
	glm::mat4 model = glm::mat4(1.0f); // No translation or scaling required for the ceiling
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::Build3DAC() {
	// Load and create a texture for the AC
	glGenTextures(1, &textureAC);
	glBindTexture(GL_TEXTURE_2D, textureAC);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("ac.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Geometry for AC
	GLfloat vertices[] = {
		// Format: x, y, z, u, v
		// Front face
		-2.5f, 6.0f, 9.9f, 0.0f, 0.0f,  // Bottom-left
		 2.5f, 6.0f, 9.9f, 1.0f, 0.0f,  // Bottom-right
		 2.5f, 8.0f, 9.9f, 1.0f, 1.0f,  // Top-right
		-2.5f, 8.0f, 9.9f, 0.0f, 1.0f   // Top-left
	};

	GLuint indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};

	glGenVertexArrays(1, &VAOAC);
	glGenBuffers(1, &VBOAC);
	glGenBuffers(1, &EBOAC);

	glBindVertexArray(VAOAC);

	glBindBuffer(GL_ARRAY_BUFFER, VBOAC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOAC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DAC() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, textureAC);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 6);

	glBindVertexArray(VAOAC);

	// Transformasi untuk menempatkan AC di lokasi yang sesuai
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, -7.5f)); // Sesuaikan posisi
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::Build3DPainting() {
	// Load and create a texture for the painting
	glGenTextures(1, &texturePainting);
	glBindTexture(GL_TEXTURE_2D, texturePainting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("painting.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for the painting
	GLfloat vertices[] = {
		// Format: x, y, z, u, v
		-9.5f, 2.0f, -5.0f, 0.0f, 0.0f,  // Bottom-left (lebih rendah & lebih lebar)
		-9.5f, 2.0f,  2.0f, 1.0f, 0.0f,  // Bottom-right (lebih rendah & lebih lebar)
		-9.5f, 8.0f,  2.0f, 1.0f, 1.0f,  // Top-right (lebih tinggi & lebih lebar)
		-9.5f, 8.0f, -5.0f, 0.0f, 1.0f   // Top-left (lebih tinggi & lebih lebar)
	};
	GLuint indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};

	glGenVertexArrays(1, &VAOPainting);
	glGenBuffers(1, &VBOPainting);
	glGenBuffers(1, &EBOPainting);

	glBindVertexArray(VAOPainting);

	glBindBuffer(GL_ARRAY_BUFFER, VBOPainting);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPainting);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DPainting() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE7); // Use texture unit 7 for the painting
	glBindTexture(GL_TEXTURE_2D, texturePainting);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 7);

	glBindVertexArray(VAOPainting);

	// Transformasi model untuk menempatkan painting di left wall
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Posisi tetap karena sudah sesuai koordinat left wall
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw the painting
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::Build3DPaintingDua() {
	// Load and create a texture for the painting
	glGenTextures(1, &texturePaintingDua);
	glBindTexture(GL_TEXTURE_2D, texturePaintingDua);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("painting.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry for the painting
	GLfloat vertices[] = {
		// Format: position x, y, z, tex coord u, v
		// Front face of the painting
		9.8f, 4.0f, -2.0f, 0.0f, 0.0f,   // Bottom-left
		9.8f, 4.0f,  2.0f, 1.0f, 0.0f,   // Bottom-right
		9.8f, 7.0f,  2.0f, 1.0f, 1.0f,   // Top-right
		9.8f, 7.0f, -2.0f, 0.0f, 1.0f    // Top-left
	};

	GLuint indices[] = {
		0, 1, 2,   // First triangle
		0, 2, 3    // Second triangle
	};

	glGenVertexArrays(1, &VAOPaintingDua);
	glGenBuffers(1, &VBOPaintingDua);
	glGenBuffers(1, &EBOPaintingDua);

	glBindVertexArray(VAOPainting);

	glBindBuffer(GL_ARRAY_BUFFER, VBOPaintingDua);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPaintingDua);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::Draw3DPaintingDua() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE8); // Texture unit untuk lukisan
	glBindTexture(GL_TEXTURE_2D, texturePainting);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 8);

	glBindVertexArray(VAOPainting);

	// Transformasi: Sesuaikan posisi pada right wall
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Tambahkan translasi jika diperlukan
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Gambar lukisan
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Camera: Free Camera Implementation", 800, 600, false, true);
}
