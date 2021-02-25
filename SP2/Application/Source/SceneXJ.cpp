#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "SceneXJ.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include <string>

//hi XinJie
SceneXJ::SceneXJ()
{
}

SceneXJ::~SceneXJ()
{
}

void SceneXJ::Init()
{
	// Init VBO here
	// Set background color to dark blue
	glClearColor(0.147f, 0.0f, 0.255f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(-240, 10, -240), Vector3(0, 10, -240), Vector3(0, 1, 0));


	// Animation Parameters
	LSPEED = 10.0f;
	rotateAngle = 0.0f;
	translateX = 0.0f;
	translateY = 0.0f;
	translateZ = -100.0f;
	translatedoor = 0;

	dooropen = false;
	doorclose = true;
	textbox = false;
	interact = false;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	//floor
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//tiles.tga");
	
	//walls
	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL2] = MeshBuilder::GenerateQuad("wall2", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL2]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL3] = MeshBuilder::GenerateQuad("wall3", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL3]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL4] = MeshBuilder::GenerateQuad("wall4", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL4]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL5] = MeshBuilder::GenerateQuad("wall5", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL5]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL6] = MeshBuilder::GenerateQuad("wall6", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL6]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL7] = MeshBuilder::GenerateQuad("wall7", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL7]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL8] = MeshBuilder::GenerateQuad("wall8", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL8]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL9] = MeshBuilder::GenerateQuad("wall9", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL9]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL10] = MeshBuilder::GenerateQuad("wall10", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL10]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL11] = MeshBuilder::GenerateQuad("wall11", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL11]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_WALL12] = MeshBuilder::GenerateQuad("wall12", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL12]->textureID = LoadTGA("Image/wall.tga");

	meshList[GEO_ROOF] = MeshBuilder::GenerateQuad("roof", Color(1, 1, 1), 1.f);
	meshList[GEO_ROOF]->textureID = LoadTGA("Image/wall.tga");

	//Skybox
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	//text

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_TEXT1] = MeshBuilder::GenerateText("text1", 16, 16);
	meshList[GEO_TEXT1]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Image//ComicSans.tga");

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("textbox", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//Text_box1.tga");





	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 18, 18, 1.0f);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1500, 1500, 1500);

	//objects

	meshList[GEO_ZOOM] = MeshBuilder::GenerateQuad("zoom", Color(1, 1, 1), 1.f);
	meshList[GEO_ZOOM]->textureID = LoadTGA("Image//zoom1.tga");

	meshList[GEO_ZOOM2] = MeshBuilder::GenerateQuad("zoom", Color(1, 1, 1), 1.f);
	meshList[GEO_ZOOM2]->textureID = LoadTGA("Image//zoom2.tga");

	meshList[GEO_ZOOM3] = MeshBuilder::GenerateQuad("zoom", Color(1, 1, 1), 1.f);
	meshList[GEO_ZOOM3]->textureID = LoadTGA("Image//zoom3.tga");

	meshList[GEO_ZOOM4] = MeshBuilder::GenerateQuad("zoom", Color(1, 1, 1), 1.f);
	meshList[GEO_ZOOM4]->textureID = LoadTGA("Image//zoom4.tga");

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJMTL("model", "OBJ//Door.obj", "OBJ//Door.mtl");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//Door.TGA");

	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJMTL("model", "OBJ//computerScreen.obj", "OBJ//computerScreen.mtl");
	meshList[GEO_MODEL2]->textureID = LoadTGA("Image/water.tga");

	meshList[GEO_MODEL3] = MeshBuilder::GenerateOBJMTL("model", "OBJ//deskCorner.obj", "OBJ//deskCorner.mtl");
	meshList[GEO_MODEL3]->textureID = LoadTGA("Image/water.tga");

	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJMTL("model", "OBJ//Cyborg.obj", "OBJ//Cyborg.mtl");
	meshList[GEO_MODEL4]->textureID = LoadTGA("Image/Cyborg.tga");

	


	//Load vertex and fragment shaders

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	//Second Light Parameters
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	//Third Light parameters
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 80, 50);
	light[0].color.Set(0.4f, 0.4f, 1.f);
	light[0].power = 0.4f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(camera.position.x, 10, camera.position.z);
	light[1].color.Set(1, 1, 1);
	light[1].power = 2.0f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(0, 60, 50);
	light[2].color.Set(1, 1, 0.5f);
	light[2].power = 2.0f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
}

void SceneXJ::Reset()
{

}

void SceneXJ::Update(double dt)
{
	/*camera.Update(dt);*/



	//FPS = 1.f / dt;


	//Interactions
	//Player opens chest





	if (Application::IsKeyPressed(0x31))
	{
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32))
	{
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if (Application::IsKeyPressed('B'))
	{
		light[0].power = 0;
	}
	if (Application::IsKeyPressed('V'))
	{
		light[0].power = 2;
	}

	if ( camera.position.x > 20 && camera.position.x < 340 && camera.position.z > -20 && camera.position.z < 300)
	{
		dooropen = true;
		doorclose = false;
	}

	if (dooropen == true)
	{

			if (translatedoor <= 300)
			{
				translatedoor += 50 * dt;
				interact = true;
			}

			else
			{

				dooropen = false;
				doorclose = true;
				interact = false;

			}

	}


	if (doorclose == true)
	{

		if (time <= 500)

		{
			time += 50 * dt;
		}


		else
		{
			if (translatedoor >= 0.0f)
			{
				translatedoor -= 50 * dt;

			}

			else
			{
				translatedoor = 0;
				time = 0;

			}
		}

	}

	if (interact == false)
	{
		camera.Update(dt);
		textbox = false;
	}
	else
	{
		textbox = true;
	}


	if (Application::IsKeyPressed('E') && camera.position.x > 20 && camera.position.x < 340 && camera.position.z > -300 && camera.position.z < 300)



	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);



}

void SceneXJ::Render()
{
	// Render VBO here
	//Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//##############################

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);
	RenderSkybox();

	//floor
	modelStack.PushMatrix();
	modelStack.Translate(0, -60, -235);
	modelStack.Scale(600, 400, 600);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	//Classroom
	modelStack.PushMatrix();
	modelStack.Translate(-200, 40, -535);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 40, -535);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200, 40, -535);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_WALL3], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300, 40, -435);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL4], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300, 40, -235);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL5], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300, 40, -35);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL6], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-translatedoor,0,0);
	modelStack.Translate(200, 40, 65);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL7], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 40,65);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL8], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, 40, 65);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL9], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-300, 40, -435);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-270, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL10], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-300,40, -235);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-270, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL11], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-300, 40, -35);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-270, 0, 1, 0);
	RenderMesh(meshList[GEO_WALL12], false);
	modelStack.PopMatrix();

	//models
	modelStack.PushMatrix();
	modelStack.Translate(-translatedoor, 0, 0);
	modelStack.Translate(200, -60, 65);
	modelStack.Scale(7, 7, 7);
	modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(170, 0, -390);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300, -80, -340);
	modelStack.Scale(200, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(220, -70, -390);
	modelStack.Scale(11, 11, 11);
	RenderMesh(meshList[GEO_MODEL4], false);
	modelStack.PopMatrix();


	//objects

	modelStack.PushMatrix();
	modelStack.Translate(-200, 20, -35);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-270, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[GEO_ZOOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 20, -65);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-270, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[GEO_ZOOM2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, 20, -405);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-270, 0, 1, 0);
	RenderMesh(meshList[GEO_ZOOM3], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 20, -355);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-270, 0, 1, 0);
	RenderMesh(meshList[GEO_ZOOM4], false);
	modelStack.PopMatrix();

	if (textbox == true) 
	{
		RenderMeshOnScreen(meshList[GEO_TEXTBOX], 40, 13, 80, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], "School sux", Color(0, 0, 0), 3, 10, 16);
	}


}

void SceneXJ::Exit()
{
	// Cleanup VBO here
	//glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	//glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneXJ::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	//enableLight = false;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneXJ::RenderSkybox()
{
    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(0, 0.f, 0.5);
    modelStack.Rotate(180, 1, 0, 0);
    modelStack.Rotate(180, 0, 0, 1);
    RenderMesh(meshList[GEO_FRONT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(-0.5, 0.f, 0);
    modelStack.Rotate(90, 0, 1, 0);
    RenderMesh(meshList[GEO_RIGHT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(0.5, 0.f, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    RenderMesh(meshList[GEO_LEFT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(0, 0.5f, 0);
    modelStack.Rotate(90, 1, 0, 0);
    modelStack.Rotate(-90, 0, 0, 1);
    RenderMesh(meshList[GEO_TOP], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(0, -0.5f, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    modelStack.Rotate(90, 0, 0, 1);
    RenderMesh(meshList[GEO_BOTTOM], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Scale(3000, 3000, 3000);
    modelStack.Translate(0, 0.f, -0.5);
    modelStack.Rotate(180, 0, 0, 1);
    modelStack.Rotate(180, 0, 0, 1);
    RenderMesh(meshList[GEO_BACK], false);
    modelStack.PopMatrix();
}

void SceneXJ::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneXJ::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneXJ::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 0);
	//to do: scale and translate accordingly
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
