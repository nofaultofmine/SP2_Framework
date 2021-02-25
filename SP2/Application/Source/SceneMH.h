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
#include "EntityMGR.h"
#include "Player.h"
#include <vector>
#include <string>

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
		GEO_SOUP,
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

	//float FPS;

	EntityMGR EnMGR;
	Player* PlayerEntity;
	Entity* seat = nullptr;
	//Camera3 camera2;

	int currEvent = 0;
	float EventLength = 0;

	bool inMenus = false;

	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	//unsigned m_vertexBuffer[NUM_GEOMETRY];
	//unsigned m_colorBuffer[NUM_GEOMETRY];
	//unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;

	std::vector<Entity* [5]> tablesets;
	int traySize = 0;

	void InitTableSet(float x, float y, float z, int tablenum);

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderWorld();
	void RenderMenus();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void AddToTray(int UUID, int size);
	void RenderTray();
	void RenderFoodAtTable(Entity* entity, bool isForward);

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
	void RenderFood(double dt);

};

#endif