#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2,
		VAOWalls, VBOWalls, EBOWalls, textureWalls,
		VAOWhiteboard, VBOWhiteboard, EBOWhiteboard, textureWhiteboard,
		VAODesk, VBODesk, EBODesk, textureDesk,
		VAOAlmari, VBOAlmari, EBOAlmari, textureAlmari,
		VAOWindows, VBOWindows, EBOWindows, textureWindows,
		VAOAC, VBOAC, EBOAC, textureAC,
		VAOCeiling, VBOCeiling, EBOCeiling, textureCeiling,
		VAODeskSatu, VBODeskSatu, EBODeskSatu, textureDeskSatu,
		VAOPainting, VBOPainting, EBOPainting, texturePainting,
		VAOPaintingDua, VBOPaintingDua, EBOPaintingDua, texturePaintingDua
		;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildTexturedCube();
	void BuildTexturedPlane();
	void DrawTexturedCube();
	void DrawColoredPlane();
	void Build3DDesks();
	void BuildTexturedWalls();
	void Build3DWhiteboard();
	void Build3DAlmari();
	void Build3DWindows();
	void Draw3DAlmari();
	void Draw3DDesks();
	void DrawTexturedWalls();
	void Draw3DWhiteboard();
	void Draw3DWindows();
	void Build3DAC();
	void Draw3DAC();
	void BuildTexturedCeiling();
	void DrawTexturedCeiling();
	void Build3DDeskSatu();
	void Draw3DDeskSatu();
	void Build3DPainting();
	void Draw3DPainting(); 
	void Build3DPaintingDua();
	void Draw3DPaintingDua();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

