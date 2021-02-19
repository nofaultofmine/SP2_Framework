#ifndef SceneMH_H
#define SceneMH_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class SceneMH : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_FLOOR,
		GEO_QUAD,
		GEO_QUAD2,
		//Skybox & Lightball
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		//Other Models
		GEO_TABLE,
		GEO_CHAIR,
		GEO_BUILDING,
		GEO_STALL,
		GEO_TILE,
		GEO_TEXT,
		GEO_TEXT1,
		GEO_TEXT2,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		//Second Light
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		//Third Light
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};


private:
	MS modelStack, viewStack, projectionStack;

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	Light light[3];

	float rotateAngle;
	float bodyRotateAngle;
	float bodyRotateAngle2;
	float bodyRotateAngle3;
	float headRotateAngle;
	float swordRotateAngle;
	float swordRotateAngle2;
	float armRotateAngle;
	float armRotateAngle2;
	float armIdleRotateAngle;
	float bombX;
	float bombY;
	float bombZ;
	//Link Position Parameters
	float translateX;
	float translateY;
	float translateZ;
	float scaleAll;
	float eyeScale;
	float LSPEED;

	float swordFlashTimer;
	float blinkTimer;
	float bombTimer;
	float rotateSpeed;
	float bodyRotateSpeed;
	float bodyRotateSpeed2;
	float headRotateSpeed;
	float armRotateSpeed;
	float armRotateSpeed2;
	float armIdleRotateSpeed;
	float swordRotateSpeed;
	float swordRotateSpeed2;
	float translateSpeed;
	float scaleSpeed;
	float eyeScaleSpeed;
	float bombGravity;

	//Assignment 1 Booleans
	bool eyesClosed;
	bool bombFlash;
	bool swordSequence1;
	bool swordSequence2;
	bool throwSequence1;
	bool throwSequence2;
	bool throwSequence3;
	bool anim1Playing;
	bool anim2Playing;
	bool anim3Playing;

	//Assignment2 Booleans
	bool fpsCamera;
	bool chest1Opened;
	bool canInteract;
	bool gotBombs;
	bool setBombInActive;
	bool caveEntranceBroken;
	bool cutSceneOn;
	bool reachedDekuTree;
	bool masterSwordGet;
	bool darkLinkDefeated;
	bool canClick;
	bool instructionTextBoxOn;
	bool insideCave;
	bool atCaveEntrance;

	//Assignment2 Values
	float textTimer;
	float lilyRotateAngle;
	float masterSwordY;
	float darkLinkY;
	int dialogueNumber;
	int goalNumber;

	//float FPS;

	Camera3 camera;
	Camera2 camera2;
	//Camera3 camera2;

	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	//unsigned m_vertexBuffer[NUM_GEOMETRY];
	//unsigned m_colorBuffer[NUM_GEOMETRY];
	//unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

public:
	SceneMH();
	~SceneMH();

	virtual void Init();
	virtual void Reset();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderTableSet(float x, float y, float z);
	void RenderStallSet(float x, float y, float z, std::string text);
};

#endif