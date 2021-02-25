#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "SceneT.h"
#include <sstream>

SceneT::SceneT()
{
}

SceneT::~SceneT()
{
}

void SceneT::Init()
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

	//camera.Init(Vector3(-240, 10, -240), Vector3(0, 10, -240), Vector3(0, 1, 0));
	camera.Init(Vector3(-10, 10, -120), Vector3(0, 10, 0), Vector3(0, 1, 0));

	// Animation Parameters
	LSPEED = 10.0f;
	translateY = 0;
	transYDir = -0.5;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	message = " ";
	keyInstruction = " ";
	showMsg = false;

	talk = false;
	talkCount = 0;
	
	startFollow = false;

	//Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor.tga");
	
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("textBox", Color(0, 0, 0), 1.f);

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

	meshList[GEO_BUILDING1] = MeshBuilder::GenerateOBJMTL("building1", "OBJ//building1.obj", "OBJ//building1.mtl");
	meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJMTL("building2", "OBJ//building2.obj", "OBJ//building2.mtl");
	meshList[GEO_BUILDING3] = MeshBuilder::GenerateOBJMTL("building3", "OBJ//building3.obj", "OBJ//building3.mtl");
	meshList[GEO_BUILDING4] = MeshBuilder::GenerateOBJMTL("building4", "OBJ//building4.obj", "OBJ//building4.mtl");
	meshList[GEO_BUILDING5] = MeshBuilder::GenerateOBJMTL("building5", "OBJ//building5.obj", "OBJ//building5.mtl");
	meshList[GEO_BUILDING6] = MeshBuilder::GenerateOBJMTL("building6", "OBJ//building6.obj", "OBJ//building6.mtl");
	meshList[GEO_BUILDING7] = MeshBuilder::GenerateOBJMTL("building7", "OBJ//building7.obj", "OBJ//building7.mtl");
	meshList[GEO_BUILDING8] = MeshBuilder::GenerateOBJMTL("building8", "OBJ//building8.obj", "OBJ//building8.mtl");

	meshList[STRAIGHT_ROAD] = MeshBuilder::GenerateOBJMTL("straightroad", "OBJ//road.obj", "OBJ//road.mtl");
	meshList[CURVE_ROAD] = MeshBuilder::GenerateOBJMTL("curve", "OBJ//road_curve.obj", "OBJ//road_curve.mtl");
	meshList[SHARP_CURVE] = MeshBuilder::GenerateOBJMTL("sharpcurve", "OBJ//curve.obj", "OBJ//curve.mtl");
	meshList[CURVE_INT] = MeshBuilder::GenerateOBJMTL("curveint", "OBJ//road_curveIntersection.obj", "OBJ//road_curveIntersection.mtl");
	meshList[SPLIT_ROAD] = MeshBuilder::GenerateOBJMTL("roadsplit", "OBJ//road_split.obj", "OBJ//road_split.mtl");
	meshList[ROUNDABOUT] = MeshBuilder::GenerateOBJMTL("roundabout", "OBJ//roundabout.obj", "OBJ//roundabout.mtl");
	meshList[ROAD_INT] = MeshBuilder::GenerateOBJMTL("roadint", "OBJ//road_intersection.obj", "OBJ//road_intersection.mtl");
	meshList[ROAD_END] = MeshBuilder::GenerateOBJMTL("roadend", "OBJ//road_end.obj", "OBJ//road_end.mtl");

	meshList[GEO_BOT] = MeshBuilder::GenerateOBJMTL("bot", "OBJ//boto.obj", "OBJ//boto.mtl");
	meshList[BOT_MOUTH] = MeshBuilder::GenerateOBJMTL("mouth", "OBJ//mouth.obj", "OBJ//mouth.mtl");
	meshList[GEO_SIGN] = MeshBuilder::GenerateOBJMTL("Sign", "OBJ//sign1.obj", "OBJ//sign1.mtl");
	meshList[GEO_SIGN]->textureID = LoadTGA("Image//SignDiffuse.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 18, 18, 1.0f);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1500, 1500, 1500);

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

void SceneT::Reset()
{

}

void SceneT::Update(double dt)
{
	camera.Update(dt);

	light[1].position.x = camera.position.x;
	light[1].position.z = camera.position.z;

	static bool buttonState = false;
	static bool bbuttonState = false;

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

	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);

	//up
	if ((camera.position.x < 0 && camera.position.x > -20) && (camera.position.z < 250 && camera.position.z > 200))
	{
		showMsg = true;
		message = "Visit the transport area?";
		keyInstruction = "(Press 'F')";
		if (Application::IsKeyPressed('F'))
		{
			Application::ChangeScene(1, true);
		}
	}
	//right
	else if ((camera.position.x < -200 && camera.position.x > -250) && (camera.position.z < 0 && camera.position.z > -20))
	{
		showMsg = true;
		message = "   Visit the hospital?";
		keyInstruction = "(Press 'F')";
	}
	//left
	else if ((camera.position.x < 250 && camera.position.x > 200) && (camera.position.z < 10 && camera.position.z > -10))
	{
		showMsg = true;
		message = "    Visit the school?";
		keyInstruction = "(Press 'F')";
	}
	//down
	else if ((camera.position.x < 10 && camera.position.x > -10) && (camera.position.z < -200 && camera.position.z > -250))
	{
		showMsg = true;
		message = "   Visit the Kopitiam?";
		keyInstruction = "(Press 'F')";
	}
	else if ((camera.position.x < 4 && camera.position.x > -4) && (camera.position.z < -65 && camera.position.z > -75) && startFollow == false)
	{
		showMsg = true;
		message = "          Talk?";
		keyInstruction = "(Press 'E')";
		if (!bbuttonState && Application::IsKeyPressed('E'))
		{
			bbuttonState = true;
			talk = true;
		}
		else if (bbuttonState && !Application::IsKeyPressed('E'))
		{
			bbuttonState = false;
		}
	}
	else
		showMsg = false;

	if (talk == true)
	{
		showMsg = false;
		if (!buttonState && Application::IsKeyPressed('E'))
		{
			buttonState = true;
			talkCount += 1;
		}
		else if (buttonState && !Application::IsKeyPressed('E'))
		{
			buttonState = false;
		}


		translateY += (float)(transYDir * dt);
		if (translateY > 0)
		{
			transYDir = -0.5;
		}
		if (translateY < -0.5)
		{
			transYDir = 0.5;
		}
	}
	else
		translateY = 0;
}

void SceneT::Render()
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

	RenderMesh(meshList[GEO_AXES], false);
	RenderSkybox();


	modelStack.PushMatrix();
	modelStack.Translate(-15, 0, 24);
	modelStack.Rotate(80, 0, 1, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_SIGN], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(185, 0, 125);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_SIGN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19, 11, 24.5);
	modelStack.Rotate(-10, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderText(meshList[GEO_TEXT], "Press 'E'", Color(0, 0, 0));
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-18, 10, 24.5);
	modelStack.Rotate(-10, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderText(meshList[GEO_TEXT], "to read", Color(0, 0, 0));
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(184, 11, 121);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderText(meshList[GEO_TEXT], "Press 'E'", Color(0, 0, 0));
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(184, 10, 121);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderText(meshList[GEO_TEXT], "to read", Color(0, 0, 0));
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(30, 0, -85);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-200, 0, 80);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(1.8, 1.8, 1.8);
	RenderMesh(meshList[GEO_BUILDING1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-100, 2, -90);
	modelStack.Rotate(200, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(115, 2, -65);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_BUILDING2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 145);
	RenderMesh(meshList[GEO_BUILDING3], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, -90);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING4], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, 50);
	modelStack.Rotate(-60, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_BUILDING4], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 10);
	modelStack.Scale(1.2, 1.2, 1.2);
	RenderMesh(meshList[GEO_BUILDING5], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(210, 0, 100);
	RenderMesh(meshList[GEO_BUILDING5], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(30, 0, -240);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_BUILDING5], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(125, 0, 95);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(125, 0, 65);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(160, 0, 95);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(160, 0, 60);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(125, 0, 35);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(195, 0, 95);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(195, 0, 60);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, 80);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, 50);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, 20);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, -10);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, -40);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, -70);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(560, 0, -100);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING6], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, -120);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING7], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-300, 0, 80);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BUILDING8], true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -175);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(150, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, -50);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[SHARP_CURVE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, 50);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(180, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, 150);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[SHARP_CURVE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-65, 0, 150);
	modelStack.Scale(10, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -90);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[SPLIT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(10, 0, -70);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[SHARP_CURVE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(40, 0, -60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 230);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_INT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 150);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[ROUNDABOUT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 215);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(70, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-175, 0, 140);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-140, 0, 150);
	modelStack.Scale(30, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-185, 0, 70);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-195, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-232.5, 0, -10);
	modelStack.Scale(35, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, -60);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(110, 0, 0);
	modelStack.Scale(100, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(215, 0, 0);
	modelStack.Scale(70, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, -25);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(30, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[ROAD_INT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(80, 0, 220);
	modelStack.Scale(120, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(160, 0, 210);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(170, 0, 100);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(180, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(170, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[ROAD_INT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 120);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(5, 20, 20);
	RenderMesh(meshList[ROAD_END], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 140);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(140, 20, 20);
	RenderMesh(meshList[STRAIGHT_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 50);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(40, 0, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[CURVE_ROAD], true);
	modelStack.PopMatrix();

	if (startFollow == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x - 5, 0, camera.position.z + -40);
		modelStack.Rotate(180, 0, 1, 0);
	}

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -60);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_BOT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, translateY, -60);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[BOT_MOUTH], true);
	modelStack.PopMatrix();

	if (startFollow == true)
	{
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Scale(500, 500, 500);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	ss << "cam posX: " << camera.position.x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 0, 3);
	std::ostringstream ss1;
	ss1 << "cam posZ: " << camera.position.z;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(1, 0, 0), 3, 0, 0);

	if (showMsg == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], message, Color(1, 0, 0), 3, 4, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], keyInstruction, Color(1, 0, 0), 3, 25, 25);
	}
	if (talk == true)
	{
		switch (talkCount)
		{
		case 1:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Hello!", Color(1, 1, 1), 3, 34, 4);
			break;
		case 2:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Welcome to the future", Color(1, 1, 1), 3, 10, 5);
			RenderTextOnScreen(meshList[GEO_TEXT], "of Singapore!", Color(1, 1, 1), 3, 22, 2);
			break;
		case 3:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Yeah, we left all the other", Color(1, 1, 1), 3, 0, 5);
			RenderTextOnScreen(meshList[GEO_TEXT], /*idiot*/ "countries back on Earth.", Color(1, 1, 1), 3, 5, 2);
			break;
		case 4:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "They were all busy fighting.", Color(1, 1, 1), 2.9, 0, 4);
			break;
		case 5:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Anyway, I'm not here to", Color(1, 1, 1), 3, 6, 5);
			RenderTextOnScreen(meshList[GEO_TEXT], "talk about them,", Color(1, 1, 1), 3, 17, 2);
			break;
		case 6:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "I'm here to show", Color(1, 1, 1), 3, 18, 5);
			RenderTextOnScreen(meshList[GEO_TEXT], "you around! :)", Color(1, 1, 1), 3, 20, 2);
			break;
		case 7:
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 5, 80, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "So lets go!", Color(1, 1, 1), 3, 25, 4);
			break;
		case 8:
			talkCount = 0;
			talk = false;
			startFollow = true;
			break;
		}
	}
}

void SceneT::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneT::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(0, 0.f, 0.5);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(-0.5, 0.f, 0);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(0.5, 0.f, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(0, 0.5f, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(0, -0.5f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(600, 600, 600);
	modelStack.Translate(0, 0.f, -0.5);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void SceneT::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
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
}

void SceneT::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneT::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

void SceneT::Exit()
{
	// Cleanup VBO here
	//glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	//glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}