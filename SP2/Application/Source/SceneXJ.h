#ifndef SceneXJ_H
#define SceneXJ_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class SceneXJ : public Scene
{

	enum scenes
	{

	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_FLOOR,
		GEO_QUAD,
		GEO_QUAD2,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_RING1,
		GEO_RING2,
		GEO_SPHERE,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_CYLINDER,
		GEO_CYLINDER2,
		GEO_CYLINDER3,
		GEO_CYLINDER4,
		GEO_CYLINDER5,
		GEO_CYLINDER6,
		GEO_CONE,
		GEO_CONE2,
		GEO_HEMISPHERE,

		GEO_ZOOM,
		GEO_ZOOM2,
		GEO_ZOOM3,
		GEO_ZOOM4,

		GEO_WALL,
		GEO_WALL2,
		GEO_WALL3,
		GEO_WALL4,
		GEO_WALL5,
		GEO_WALL6,
		GEO_WALL7,
		GEO_WALL8,
		GEO_WALL9,
		GEO_WALL10, 
		GEO_WALL11,
		GEO_WALL12,
		GEO_ROOF,


		GEO_TEXTBOX,

		//Skybox & Lightball
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		//Other Models
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_MODEL7,
		GEO_MODEL8,
		GEO_MODEL9,
		GEO_MODEL10,
		GEO_MODEL11,
		GEO_MODEL12,
		GEO_MODEL13,
		GEO_MODEL14,
		GEO_MODEL15,
		GEO_MODEL16,
		GEO_MODEL17,
		GEO_MODEL18,
		GEO_MODEL19,
		GEO_MODEL20,
		GEO_MODEL21,
		GEO_MODEL22,
		GEO_MODEL23,
		GEO_MODEL24,
		GEO_MODEL25,
		GEO_MODEL26,
		GEO_MODEL27,
		GEO_MODEL28,
		GEO_MODEL29,
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

	//Link Position Parameters
	float translateX;
	float translateY;
	float translateZ;
	float scaleAll;
	float LSPEED;

	float translatedoor;
	float time;



	//Assignment2 Values
	float textTimer;
	int dialogueNumber;
	int goalNumber;

	//SP2
	bool dooropen;
	bool doorclose;
	bool textbox;
	bool interact;


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
	SceneXJ();
	~SceneXJ();

	virtual void Init();
	virtual void Reset();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif