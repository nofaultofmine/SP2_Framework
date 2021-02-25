#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "SceneYX.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include <string>


SceneYX::SceneYX()
{
	skybox_translateX = skybox_translateY = skybox_translateZ = 0.0f;
	spire_passive_rotate = 0.0f;
	cam_control = false;
	taxi_rise_transition = 0;
	pause = false;
	cam_control = gamestart = true;

	cash = 0;
	// Spire locations

	// Destination spires
	spire_x1 = -65; spire_y1 = 0; spire_z1 = -35;	// Resident District
	spire_x2 = 30; spire_y2 = 10; spire_z2 = -10;	// Hospital
	spire_x3 = 20; spire_y3 = 0; spire_z3 = 85;	// Taxi Company
	spire_x4 = -194; spire_y4 = 66; spire_z4 = 54;	// Commerical District
	spire_x5 = -170; spire_y5 = 0; spire_z5 = -100;	// Mall

	// Passenger spires
	hospital_spire_x1 = 30; hospital_spire_y1 = 0;  hospital_spire_z1 = -20; // Hospital
	commercial_spire_x2 = -100; commercial_spire_y2 = 0; commercial_spire_z2 = 100; // Commercial
	residential1_spire_x3 = -50; residential1_spire_y3 = 0; residential1_spire_z3 = -35; // Residential 1
	residential2_spire_x4 = -130; residential2_spire_y4 = 0; residential2_spire_z4 = -150; // Residential 2
	mall_spire_x5 = -180; mall_spire_y5 = 0; mall_spire_z5 = -75; // Mall

	value_store = speed_boost = lift_boost = side_hover = false;
	dialogue_1 = dialogue_2 = dialogue_3 = dialogue_4 = dialogue_5 = false;
	end_dialogue_1 = end_dialogue_2 = end_dialogue_3 = end_dialogue_4 = end_dialogue_5 = false;
	passenger_deliver_1 = passenger_deliver_2 = passenger_deliver_3 = passenger_deliver_4 = passenger_deliver_5 = end_game = true;
}

SceneYX::~SceneYX()
{
}

void SceneYX::Init()
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

	camera.Init(Vector3(5, 1, 87.5), Vector3(7.5, 1, 85), Vector3(0, 1, 0));

	// Animation Parameters
	

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor.tga");
	meshList[GEO_FLOOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_FLOOR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR]->material.kShininess = 0.2f;
	
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Purple Button", Color(1, 0, 1), 1.f);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//textBox.tga");
	meshList[GEO_QUAD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_QUAD]->material.kShininess = 0.2f;

	meshList[GEO_BOX] = MeshBuilder::GenerateCube("reference", Color(0, 1, 0), 1);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 18, 18, 1.0f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("Destination text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT1] = MeshBuilder::GenerateText("TITLE", 16, 16);
	meshList[GEO_TEXT1]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("START", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1500, 1500, 1500);

	meshList[GEO_NAV_SPIRE] = MeshBuilder::GenerateCube("SatNAv Spire", Color(0,1,0), 1);
	meshList[GEO_NAV_TAXI] = MeshBuilder::GenerateCube("taxi company spire", Color(1, 1, 0), 1);

	// Road
	meshList[GEO_ROAD] = MeshBuilder::GenerateOBJMTL("Taxi", "OBJ//road_straight.obj", "OBJ//road_straight.mtl");

	// Hover Taxi
	meshList[GEO_TAXI] = MeshBuilder::GenerateOBJMTL("Taxi", "OBJ//taxi.obj", "OBJ//taxi.mtl");
	//meshList[GEO_TAXI] = MeshBuilder::GenerateOBJ("Taxi", "OBJ//mall.obj");
	meshList[GEO_HOVER] = MeshBuilder::GenerateCube("Hoverpad", Color(0.5, 0.5, 0.5), 1.0f);
	meshList[GEO_HOVER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HOVER]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_HOVER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_HOVER]->material.kShininess = 0.5f;

	// Taxi Company
	meshList[GEO_TAXI_COMPANY] = MeshBuilder::GenerateOBJMTL("TAXI COMPANY", "OBJ//taxicompany.obj", "OBJ//taxicompany.mtl");
	meshList[GEO_TAXI_PAD] = MeshBuilder::GenerateCube("Taxi Parking lot", Color(1, 1, 0.75), 1.0f);
	meshList[GEO_TAXI_PAD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TAXI_PAD]->material.kDiffuse.Set(1, 1, 1);
	meshList[GEO_TAXI_PAD]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TAXI_PAD]->material.kShininess = 1.0f;

	// Taxi helper bot
	meshList[GEO_ROBOT] = MeshBuilder::GenerateOBJMTL("Helper Bot", "OBJ//CyborgTaxi.obj", "OBJ//CyborgTaxi.mtl");
	meshList[GEO_ROBOT]->textureID = LoadTGA("Image//CyborgTaxi.tga");

	// Passenger Pads and Spires
	meshList[GEO_PASSENGERS_PAD_1] = MeshBuilder::GenerateCube("GEO_PASSENGERS_PAD_1", Color(1, 0.75, 0.75), 1.0f);
	meshList[GEO_PASSENGERS_SPIRE_1] = MeshBuilder::GenerateCube("GEO_PASSENGERS_SPIRE_1", Color(1, 0, 0), 1.0f);

	meshList[GEO_PASSENGERS_PAD_2] = MeshBuilder::GenerateCube("GEO_PASSENGERS_PAD_2", Color(1, 0.75, 0.75), 1.0f);
	meshList[GEO_PASSENGERS_SPIRE_2] = MeshBuilder::GenerateCube("GEO_PASSENGERS_SPIRE_2", Color(1, 0, 0), 1.0f);

	meshList[GEO_PASSENGERS_PAD_3] = MeshBuilder::GenerateCube("GEO_PASSENGERS_PAD_3", Color(1, 0.75, 0.75), 1.0f);
	meshList[GEO_PASSENGERS_SPIRE_3] = MeshBuilder::GenerateCube("GEO_PASSENGERS_SPIRE_3", Color(1, 0, 0), 1.0f);

	meshList[GEO_PASSENGERS_PAD_4] = MeshBuilder::GenerateCube("GEO_PASSENGERS_PAD_4", Color(1, 0.75, 0.75), 1.0f);
	meshList[GEO_PASSENGERS_SPIRE_4] = MeshBuilder::GenerateCube("GEO_PASSENGERS_SPIRE_4", Color(1, 0, 0), 1.0f);

	meshList[GEO_PASSENGERS_PAD_5] = MeshBuilder::GenerateCube("GEO_PASSENGERS_PAD_5", Color(1, 0.75, 0.75), 1.0f);
	meshList[GEO_PASSENGERS_SPIRE_5] = MeshBuilder::GenerateCube("GEO_PASSENGERS_SPIRE_5", Color(1, 0, 0), 1.0f);


	// Passenger Models
	meshList[GEO_PASSENGERS_MALE_1] = MeshBuilder::GenerateOBJMTL("Male Passenger", "OBJ//Male_Standing.obj", "OBJ//Male_Standing.mtl");
	meshList[GEO_PASSENGERS_MALE_2] = MeshBuilder::GenerateOBJMTL("Male Passenger", "OBJ//Male_Standing_CoveringEyes.obj", "OBJ//Male_Standing_CoveringEyes.mtl");
	meshList[GEO_PASSENGERS_MALE_3] = MeshBuilder::GenerateOBJMTL("Male Passenger", "OBJ//Male_Standing_Hips.obj", "OBJ//Male_Standing_Hips.mtl");
	meshList[GEO_PASSENGERS_MALE_4] = MeshBuilder::GenerateOBJMTL("Male Passenger", "OBJ//Male_Standing_Waving.obj", "OBJ//Male_Standing_Waving.mtl");
	meshList[GEO_PASSENGERS_MALE_5] = MeshBuilder::GenerateOBJMTL("Male Passenger", "OBJ//Male_Walking.obj", "OBJ//Male_Walking.mtl");

	meshList[GEO_PASSENGERS_MALE_HAIR_1] = MeshBuilder::GenerateOBJMTL("Male Passenger Hair", "OBJ//Male_Hairstyle_1.obj", "OBJ//Male_Hairstyle_1.mtl");
	meshList[GEO_PASSENGERS_MALE_HAIR_2] = MeshBuilder::GenerateOBJMTL("Male Passenger Hair", "OBJ//Male_Hairstyle_2.obj", "OBJ//Male_Hairstyle_2.mtl");
	meshList[GEO_PASSENGERS_MALE_HAIR_3] = MeshBuilder::GenerateOBJMTL("Male Passenger Hair", "OBJ//Male_Hairstyle_3.obj", "OBJ//Male_Hairstyle_3.mtl");
	meshList[GEO_PASSENGERS_MALE_HAIR_4] = MeshBuilder::GenerateOBJMTL("Male Passenger Hair", "OBJ//Male_Hairstyle_4.obj", "OBJ//Male_Hairstyle_4.mtl");

	meshList[GEO_PASSENGERS_FEMALE_1] = MeshBuilder::GenerateOBJMTL("Female Passenger", "OBJ//Female_Standing.obj", "OBJ//Female_Standing.mtl");
	meshList[GEO_PASSENGERS_FEMALE_2] = MeshBuilder::GenerateOBJMTL("Female Passenger", "OBJ//Female_Standing_CoveringEyes.obj", "OBJ//Female_Standing_CoveringEyes.mtl");
	meshList[GEO_PASSENGERS_FEMALE_3] = MeshBuilder::GenerateOBJMTL("Female Passenger", "OBJ//Female_Standing_Hips.obj", "OBJ//Female_Standing_Hips.mtl");
	meshList[GEO_PASSENGERS_FEMALE_4] = MeshBuilder::GenerateOBJMTL("Female Passenger", "OBJ//Woman_Standing_Waving.obj", "OBJ//Woman_Standing_Waving.mtl");
	meshList[GEO_PASSENGERS_FEMALE_5] = MeshBuilder::GenerateOBJMTL("Female Passenger", "OBJ//Female_Walking.obj", "OBJ//Female_Walking.mtl");

	meshList[GEO_PASSENGERS_FEMALE_HAIR_1] = MeshBuilder::GenerateOBJMTL("Female Passenger Hair", "OBJ//Female_Hairstyle_1.obj", "OBJ//Female_Hairstyle_1.mtl");
	meshList[GEO_PASSENGERS_FEMALE_HAIR_2] = MeshBuilder::GenerateOBJMTL("Female Passenger Hair", "OBJ//Female_Hairstyle_2.obj", "OBJ//Female_Hairstyle_2.mtl");
	meshList[GEO_PASSENGERS_FEMALE_HAIR_3] = MeshBuilder::GenerateOBJMTL("Female Passenger Hair", "OBJ//Female_Hairstyle_3.obj", "OBJ//Female_Hairstyle_3.mtl");
	meshList[GEO_PASSENGERS_FEMALE_HAIR_4] = MeshBuilder::GenerateOBJMTL("Female Passenger Hair", "OBJ//Female_Hairstyle_4.obj", "OBJ//Female_Hairstyle_4.mtl");


	// Residential Buildings
	meshList[GEO_FLAT1] = MeshBuilder::GenerateOBJMTL("Flat_1", "OBJ//Residential Buildings 006.obj", "OBJ//Residential Buildings 006.mtl");

	meshList[GEO_FLAT_PAD] = MeshBuilder::GenerateCube("Flat pad", Color(0.75, 1, 0.75), 1.0f);
	meshList[GEO_FLAT_PAD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLAT_PAD]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_FLAT_PAD]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLAT_PAD]->material.kShininess = 1.f;

	// Mall

	//meshList[GEO_MALL] = MeshBuilder::GenerateOBJMTL("MAll", "OBJ//plaza.obj", "OBJ//plaza.mtl");
	meshList[GEO_MALL] = MeshBuilder::GenerateCube("Placeholder Mall", Color(0.9, 0.9, 0.9), 1.0f);
	meshList[GEO_MALL]->material.kAmbient.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_MALL]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_MALL]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_MALL]->material.kShininess = 1.f;

	meshList[GEO_MALL_SIGN] = MeshBuilder::GenerateCube("Placeholder Mall Sign", Color(0,1,0), 1.0f);
	meshList[GEO_MALL_SIGN]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_MALL_SIGN]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_MALL_SIGN]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MALL_SIGN]->material.kShininess = 1.f;

	meshList[GEO_MALL_PAD] = MeshBuilder::GenerateCube("Placeholder Mall Pad", Color(0.75, 1, 0.75), 1.0f);

	// Park
	meshList[GEO_TREE1] = MeshBuilder::GenerateOBJMTL("Tree 1", "OBJ//tree_pineTallA_detailed.obj", "OBJ//tree_pineTallA_detailed.mtl");
	meshList[GEO_TREE2] = MeshBuilder::GenerateOBJMTL("Tree 2", "OBJ//tree_pineTallB_detailed.obj", "OBJ//tree_pineTallB_detailed.mtl");
	meshList[GEO_TREE3] = MeshBuilder::GenerateOBJMTL("Tree 3", "OBJ//tree_pineTallC_detailed.obj", "OBJ//tree_pineTallC_detailed.mtl");
	meshList[GEO_TREE4] = MeshBuilder::GenerateOBJMTL("Tree 4", "OBJ//tree_pineTallD_detailed.obj", "OBJ//tree_pineTallD_detailed.mtl");

	meshList[GEO_PARK_PAD] = MeshBuilder::GenerateCube("Park Landing platform", Color(0, 0.25f, 0), 1.0f);

	// Business scifi buildings
	meshList[GEO_SCIFI_1] = MeshBuilder::GenerateOBJMTL("Scifi_1", "OBJ//building1.obj", "OBJ//building1.mtl");
	meshList[GEO_SCIFI_PLATFORM] = MeshBuilder::GenerateCube("Hover Platform", Color(0.75, 1, 0.75), 1);
	meshList[GEO_SCIFI_PLATFORM]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SCIFI_PLATFORM]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_SCIFI_PLATFORM]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SCIFI_PLATFORM]->material.kShininess = 1.f;

	meshList[GEO_SCIFI_2] = MeshBuilder::GenerateOBJMTL("Scifi_2", "OBJ//building2.obj", "OBJ//building2.mtl");
	meshList[GEO_SCIFI_3] = MeshBuilder::GenerateOBJMTL("Scifi_3", "OBJ//building3.obj", "OBJ//building3.mtl");
	meshList[GEO_SCIFI_4] = MeshBuilder::GenerateOBJMTL("Scifi_4", "OBJ//building4.obj", "OBJ//building4.mtl");
	meshList[GEO_SCIFI_5] = MeshBuilder::GenerateOBJMTL("Scifi_5", "OBJ//building5.obj", "OBJ//building5.mtl");
	meshList[GEO_SCIFI_6] = MeshBuilder::GenerateOBJMTL("Scifi_6", "OBJ//building6.obj", "OBJ//building6.mtl");
	meshList[GEO_SCIFI_7] = MeshBuilder::GenerateOBJMTL("Scifi_7", "OBJ//building7.obj", "OBJ//building7.mtl");
	// Hospital
	meshList[GEO_HOSPITAL] = MeshBuilder::GenerateCube("Placeholder Hospital", Color(1, 1, 1), 1.0f);
	meshList[GEO_HOSPITAL]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_HOSPITAL]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_HOSPITAL]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HOSPITAL]->material.kShininess = 1.f;

	meshList[GEO_RED_CROSS] = MeshBuilder::GenerateCube("Placeholder Cross", Color(1, 0, 0), 1.0f);
	meshList[GEO_RED_CROSS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RED_CROSS]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_RED_CROSS]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_RED_CROSS]->material.kShininess = 1.f;

	meshList[GEO_HOSPITAL_PAD] = MeshBuilder::GenerateCube("Landing pad hospital", Color(0.75, 1, 0.75), 1.0f);
	meshList[GEO_HOSPITAL_PAD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_HOSPITAL_PAD]->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	meshList[GEO_HOSPITAL_PAD]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HOSPITAL_PAD]->material.kShininess = 1.f;

	// Skybox
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

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

	/*//Second Light Parameters
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
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");*/

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(-250, 250, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	/*light[1].type = Light::LIGHT_POINT;
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
	light[2].exponent = 3.f;*/

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

	/*glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
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
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);*/

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
}

void SceneYX::Reset()
{

}

void SceneYX::Update(double dt)
{
	if (cam_control)
	{
		camera.Update(dt, speed_boost, lift_boost, side_hover);
	}

	// Culling debug
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

	// Light toggle
	if (Application::IsKeyPressed('B'))
	{
		light[0].power = 0.5;
	}
	if (Application::IsKeyPressed('V'))
	{
		light[0].power = 1;
	}

	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	/*glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);*/

	// Move lightball
	static const float LSPEED = 20.f;

	/*
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);
		*/
	//std::cout << light[0].position.x << ", "<< light[0].position.y << ", " << light[0].position.z << std::endl;

	/*// Skybox translate
	if (Application::IsKeyPressed('W')) // z-
		if (skybox_translateZ <= -250) { skybox_translateZ += (float)(5 * dt); }
		else { skybox_translateZ -= (float)(25 * dt); }
	if (Application::IsKeyPressed('S'))// z+
		if (skybox_translateZ >= 250) { skybox_translateZ -= (float)(5 * dt); }
		else { skybox_translateZ += (float)(25 * dt); }
	if (Application::IsKeyPressed('A'))// x-
		if (skybox_translateX <= -250) { skybox_translateX += (float)(5 * dt); }
		else { skybox_translateX -= (float)(25 * dt); }
	if (Application::IsKeyPressed('D'))// x+
		if (skybox_translateX >= 250) { skybox_translateX -= (float)(5 * dt); }
		else { skybox_translateX += (float)(25 * dt); }
	if (Application::IsKeyPressed('Q'))// y-
		if (skybox_translateY <= -250) { skybox_translateY += (float)(5 * dt); }
		else { skybox_translateY -= (float)(25 * dt); }
	if (Application::IsKeyPressed('E'))// y+
		if (skybox_translateY >= 250) { skybox_translateY -= (float)(5 * dt); }
		else { skybox_translateY += (float)(25 * dt); }*/

	static bool spacestate = false;
	if (Application::IsKeyPressed(VK_SPACE))
	{
		spacestate = true;
	}

	else
	{
		spacestate = false;
	}
	// Start Menu
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0) && !gamestart)
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned UI_WIDTH = w / 10;
		unsigned h = Application::GetWindowHeight();
		unsigned UI_HEIGHT = h / 10;
		float posX = x / w * UI_WIDTH; //convert (0,800) to (0,80)
		float posY = (1.f - y / h) * UI_HEIGHT; //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
		if (posX > 32.5 && posX < 46 && posY > 5 && posY < 8 && !cam_control && !gamestart)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
			cam_control = true;
			gamestart = true;

			camera.position.x = 5;
			camera.position.y = 1;
			camera.position.z = 85;

			camera.target.x = 4;
			camera.target.y = 1;
			camera.target.z = 85;
		}
		else
		{
			std::cout << "Miss!" << std::endl;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0) && !gamestart)
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	//camera.position.y = 10;

	// Passive SatNav Spire rotation
	if (!pause)
	{
		spire_passive_rotate += (float)(10 * dt);
	}

	// Pause Menu
	if (Application::IsKeyPressed('P') && gamestart && !pause && !upgrade)
	{
		pause = true;
		cam_control = false;
	}

		if (!bLButtonState && Application::IsMousePressed(0) && pause && !upgrade)
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;

			//Converting Viewport space to UI space
			double x, y;
			Application::GetCursorPos(&x, &y);
			unsigned w = Application::GetWindowWidth();
			unsigned UI_WIDTH = w / 10;
			unsigned h = Application::GetWindowHeight();
			unsigned UI_HEIGHT = h / 10;
			float posX = x / w * UI_WIDTH; //convert (0,800) to (0,80)
			float posY = (1.f - y / h) * UI_HEIGHT; //convert (600,0) to (0,60)
			std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
			if (posX > 0 && posX < 18.5 && posY > 4.5f && posY < 7.5f && !cam_control)
			{
				std::cout << "Hit!" << std::endl;
				//trigger user action or function
				pause = false;
				cam_control = true;
			}
			else
			{
				std::cout << "Miss!" << std::endl;
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0) && pause)
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}


	// Upgrade menu
		if (camera.position.x >= spire_x3 - 5 && camera.position.x <= spire_x3 + 5 &&
			camera.position.y >= spire_y3 && camera.position.y <= spire_y3 + 2 &&
			camera.position.z >= spire_z3 - 5 && camera.position.z <= spire_z3 + 5 &&
			Application::IsKeyPressed('U') && gamestart && !upgrade && !pause)
		{
			upgrade = true;
			cam_control = false;
			
			// Store original cam position
			if (!value_store)
			{
				tempxyz_position[0] = camera.position.x;
				tempxyz_position[1] = camera.position.y;
				tempxyz_position[2] = camera.position.z;

				tempxyz_target[0] = camera.target.x;
				tempxyz_target[1] = camera.target.y;
				tempxyz_target[2] = camera.target.z;

				tempxyz_up[0] = camera.up.x;
				tempxyz_up[1] = camera.up.y;
				tempxyz_up[2] = camera.up.z;
				value_store = true;
			}

			// Look at taxi
			camera.position.x = 16;
			camera.position.y = 1;
			camera.position.z = 89;

			camera.target.x = 17;
			camera.target.y = 1;
			camera.target.z = 88;

			camera.up.x = 0;
			camera.up.y = 1;
			camera.up.z = 0;

		}

		if (!bLButtonState && Application::IsMousePressed(0) && upgrade && !pause)
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;

			//Converting Viewport space to UI space
			double x, y;
			Application::GetCursorPos(&x, &y);
			unsigned w = Application::GetWindowWidth();
			unsigned UI_WIDTH = w / 10;
			unsigned h = Application::GetWindowHeight();
			unsigned UI_HEIGHT = h / 10;
			float posX = x / w * UI_WIDTH; //convert (0,800) to (0,80)
			float posY = (1.f - y / h) * UI_HEIGHT; //convert (600,0) to (0,60)
			std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
			if (posX > 1.5 && posX < 4.5 && posY > 9.5 && posY < 12.5 && !cam_control && cash >= 10)
			{
				// Get Side Thrusters
				std::cout << "Hit!" << std::endl;
				//trigger user action or function
				side_hover = true;
				cash -= 10;
			}

			if (posX > 1.5 && posX < 4.5 && posY > 12.5 && posY < 15.5 && !cam_control && cash >= 10)
			{
				// Upgrade Hover
				std::cout << "Hit!" << std::endl;
				//trigger user action or function
				lift_boost = true;
				cash -= 10;
			}

			if (posX > 1.5 && posX < 4.5 && posY > 15.5 && posY < 18.5 && !cam_control && cash >= 20)
			{
				// Upgrade Speed
				std::cout << "Hit!" << std::endl;
				//trigger user action or function
				speed_boost = true;
				cash -= 20;
			}


			if (posX > 0 && posX < 18.5 && posY > 4.5f && posY < 7.5f && !cam_control)
			{
				// Exit Upgrade
				std::cout << "Hit!" << std::endl;
				//trigger user action or function
				upgrade = false;
				cam_control = true;

				camera.position.x = tempxyz_position[0]; // Revert to original position
				camera.position.y = tempxyz_position[1];
				camera.position.z = tempxyz_position[2];

				camera.target.x = tempxyz_target[0];
				camera.target.y = tempxyz_target[1];
				camera.target.z = tempxyz_target[2];

				camera.up.x = tempxyz_up[0];
				camera.up.y = tempxyz_up[1];
				camera.up.z = tempxyz_up[2];
			}
			else
			{
				std::cout << "Miss!" << std::endl;
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0) && upgrade)
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}


	// Passenger Pick up & Drop off


	if (camera.position.x >= hospital_spire_x1 - 5 && camera.position.x <= hospital_spire_x1 + 5 &&
		camera.position.y >= hospital_spire_y1 && camera.position.y <= hospital_spire_y1 + 2 &&
		camera.position.z >= hospital_spire_z1 - 5 && camera.position.z <= hospital_spire_z1 + 5
		&& !passenger_deliver_1 && passengerID == 0) // Pick up at hospital
	{
		dialogue_1 = true;
		if (spacestate)
		{
			passengerID = 1;
			destination_1 = true;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		dialogue_1 = false;
	}

	if (camera.position.x >= spire_x5 - 5 && camera.position.x <= spire_x5 + 5 &&
		camera.position.y >= spire_y5 && camera.position.y <= spire_y5 + 2 &&
		camera.position.z >= spire_z5 - 5 && camera.position.z <= spire_z5 + 5
		&& !passenger_deliver_1 && passengerID == 1) // Drop off at Mall
	{
		end_dialogue_1 = true;
		if (spacestate)
		{
			passengerID = 0;
			destination_1 = false;
			passenger_deliver_1 = true;

			cash += 10;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		end_dialogue_1 = false;
	}


	if (camera.position.x >= commercial_spire_x2 - 5 && camera.position.x <= commercial_spire_x2 + 5 &&
		camera.position.y >= commercial_spire_y2 && camera.position.y <= commercial_spire_y2 + 2 &&
		camera.position.z >= commercial_spire_z2 - 5 && camera.position.z <= commercial_spire_z2 + 5
		&& !passenger_deliver_2 && passengerID == 0) // Pick up at commercial
	{
		dialogue_2 = true;
		if (spacestate)
		{
			passengerID = 2;
			destination_2 = true;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		dialogue_2 = false;
	}

	if (camera.position.x >= spire_x1 - 5 && camera.position.x <= spire_x1 + 5 &&
		camera.position.y >= spire_y1 && camera.position.y <= spire_y1 + 2 &&
		camera.position.z >= spire_z1 - 5 && camera.position.z <= spire_z1 + 5
		&& !passenger_deliver_2 && passengerID == 2) // Drop off at Resident
	{
		end_dialogue_2 = true;
		if (spacestate)
		{
			passengerID = 0;
			destination_2= false;
			passenger_deliver_2 = true;

			cash += 10;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		end_dialogue_2 = false;
	}


	if (camera.position.x >= residential1_spire_x3 - 5 && camera.position.x <= residential1_spire_x3 + 5 &&
		camera.position.y >= residential1_spire_y3 && camera.position.y <= residential1_spire_y3 + 2 &&
		camera.position.z >= residential1_spire_z3 - 5 && camera.position.z <= residential1_spire_z3 + 5
		&& !passenger_deliver_3 && passengerID == 0) // Pick up at Resident 1
	{
		dialogue_3 = true;
		if (spacestate)
		{
			passengerID = 3;
			destination_3 = true;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		dialogue_3 = false;
	}

	if (camera.position.x >= spire_x4 - 5 && camera.position.x <= spire_x4 + 5 &&
		camera.position.y >= spire_y4 && camera.position.y <= spire_y4 + 2 &&
		camera.position.z >= spire_z4 - 5 && camera.position.z <= spire_z4 + 5
		&& !passenger_deliver_3 && passengerID == 3) // Drop off at Commercial
	{
		end_dialogue_3 = true;
		if (spacestate)
		{
			passengerID = 0;
			destination_3 = false;
			passenger_deliver_3 = true;

			cash += 10;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		end_dialogue_3 = false;
	}


	if (camera.position.x >= residential2_spire_x4 - 5 && camera.position.x <= residential2_spire_x4 + 5 &&
		camera.position.y >= residential2_spire_y4 && camera.position.y <= residential2_spire_y4 + 2 &&
		camera.position.z >= residential2_spire_z4 - 5 && camera.position.z <= residential2_spire_z4 + 5
		&& !passenger_deliver_4 && passengerID == 0) // Pick up at Resident 2
	{
		dialogue_4 = true;
		if (spacestate)
		{
			passengerID = 4;
			destination_4 = true;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		dialogue_4 = false;
	}

	if (camera.position.x >= spire_x2 - 5 && camera.position.x <= spire_x2 + 5 &&
		camera.position.y >= spire_y2 && camera.position.y <= spire_y2 + 2 &&
		camera.position.z >= spire_z2 - 5 && camera.position.z <= spire_z2 + 5
		&& !passenger_deliver_4 && passengerID == 4) // Drop off at hospital
	{
		end_dialogue_4 = true;
		if (spacestate)
		{
			passengerID = 0;
			destination_4 = false;
			passenger_deliver_4 = true;

			cash += 10;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		end_dialogue_4 = false;
	}


	if (camera.position.x >= mall_spire_x5 - 5 && camera.position.x <= mall_spire_x5 + 5 &&
		camera.position.y >= mall_spire_y5 && camera.position.y <= mall_spire_y5 + 2 &&
		camera.position.z >= mall_spire_z5 - 5 && camera.position.z <= mall_spire_z5 + 5
		&& !passenger_deliver_5 && passengerID == 0) // Pick up at Mall
	{
		dialogue_5 = true;
		if (spacestate)
		{
			passengerID = 5;
			destination_5 = true;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		dialogue_5 = false;
	}

	if (camera.position.x >= spire_x1 - 5 && camera.position.x <= spire_x1 + 5 &&
		camera.position.y >= spire_y1 && camera.position.y <= spire_y1 + 2 &&
		camera.position.z >= spire_z1 - 5 && camera.position.z <= spire_z1 + 5
		&& !passenger_deliver_5 && passengerID == 5) // Drop off at resident
	{
		end_dialogue_5 = true;
		if (spacestate)
		{
			passengerID = 0;
			destination_5 = false;
			passenger_deliver_5 = true;

			cash += 10;
			std::cout << passengerID << std::endl;
		}
	}
	else
	{
		end_dialogue_5 = false;
	}

	// Restart
	bool r_key = false;
	if (Application::IsKeyPressed('R'))
	{
		r_key = true;
	}

	else
	{
		r_key = false;
	}

	if (passenger_deliver_1 && passenger_deliver_2 && passenger_deliver_3 && passenger_deliver_4 && passenger_deliver_5 && r_key)
	{
		// Reset scene
		skybox_translateX = skybox_translateY = skybox_translateZ = 0.0f;
		spire_passive_rotate = 0.0f;
		cam_control = false;
		taxi_rise_transition = 0;
		pause = false;
		cam_control = gamestart = true;

		cash = 0;
		// Spire locations

		// Destination spires
		spire_x1 = -65; spire_y1 = 0; spire_z1 = -35;	// Resident District
		spire_x2 = 30; spire_y2 = 10; spire_z2 = -10;	// Hospital
		spire_x3 = 20; spire_y3 = 0; spire_z3 = 85;	// Taxi Company
		spire_x4 = -194; spire_y4 = 66; spire_z4 = 54;	// Commerical District
		spire_x5 = -170; spire_y5 = 0; spire_z5 = -100;	// Mall

		// Passenger spires
		hospital_spire_x1 = 30; hospital_spire_y1 = 0;  hospital_spire_z1 = -20; // Hospital
		commercial_spire_x2 = -100; commercial_spire_y2 = 0; commercial_spire_z2 = 100; // Commercial
		residential1_spire_x3 = -50; residential1_spire_y3 = 0; residential1_spire_z3 = -35; // Residential 1
		residential2_spire_x4 = -130; residential2_spire_y4 = 0; residential2_spire_z4 = -150; // Residential 2
		mall_spire_x5 = -180; mall_spire_y5 = 0; mall_spire_z5 = -75; // Mall

		value_store = speed_boost = lift_boost = side_hover = false;
		dialogue_1 = dialogue_2 = dialogue_3 = dialogue_4 = dialogue_5 = false;
		end_dialogue_1 = end_dialogue_2 = end_dialogue_3 = end_dialogue_4 = end_dialogue_5 = false;
		passenger_deliver_1 = passenger_deliver_2 = passenger_deliver_3 = passenger_deliver_4 = passenger_deliver_5 = end_game = false;

		// Reset camera
		camera.position.x = 5;
		camera.position.y = 1;
		camera.position.z = 87.5;

		camera.target.x = 7.5;
		camera.target.y = 1;
		camera.target.z = 85;

		camera.up.x = 0;
		camera.up.y = 1;
		camera.up.z = 0;
	}
}

void SceneYX::Render()
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

	/*if (light[1].type == Light::LIGHT_DIRECTIONAL)
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
	}*/

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);
	RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y,
		light[0].position.z);
	//xRenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	// Ground mesh
	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(300,500,500);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	// Park

	/*
	modelStack.PushMatrix();
	modelStack.Translate(-170, 0, -225);
	//modelStack.Rotate(90, 0, 1, 0);
	//modelStack.Scale(500, 1, 5);
	//RenderMesh(meshList[GEO_TREE1], true);

	for (int a = 0; a < 50; a++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 20);
		RenderMesh(meshList[GEO_TREE2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 10);
		RenderMesh(meshList[GEO_TREE4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 0);
		RenderMesh(meshList[GEO_TREE3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, -10);
		RenderMesh(meshList[GEO_TREE1], true);
		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-170, 0, -200);
	//modelStack.Rotate(90, 0, 1, 0);
	//modelStack.Scale(500, 1, 5);
	//RenderMesh(meshList[GEO_TREE1], true);

	for (int a = 0; a < 50; a++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 20);
		RenderMesh(meshList[GEO_TREE2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 10);
		RenderMesh(meshList[GEO_TREE4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, 0);
		RenderMesh(meshList[GEO_TREE3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(a * 2, 0, -10);
		RenderMesh(meshList[GEO_TREE1], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(120, 0, 0);
	modelStack.Scale(7, 0.5, 7);
	RenderMesh(meshList[GEO_PARK_PAD], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	*/

	// Roads
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.1f, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(500, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0.1f, 0);
	//modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(300, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0.1f, 150);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(285, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-95, 0.1f, 125);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(270, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-190, 0.1f, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(500, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	// Taxi company
	modelStack.PushMatrix();
	//modelStack.Scale(3, 6, 3);
	//modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(20, 0, 75);
	RenderMesh(meshList[GEO_TAXI_COMPANY], true); // Tier 0

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 10);
	modelStack.Scale(13, 0.5, 13);
	RenderMesh(meshList[GEO_TAXI_PAD], false); // Tier 1
	for (int a = 0; a < 10; a++)
	{
		modelStack.PushMatrix();
		modelStack.Scale(0.0769230769230, 2, 0.0769230769230);
		modelStack.Translate(5.5f, 0.25, 6 - (1 * a));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_TAXI], true); // Tier 2
		modelStack.PopMatrix();
	}
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Scale(0.0769230769230, 2, 0.0769230769230);
	//modelStack.Translate(6, 0.25, -6);
	//modelStack.Rotate(-45, 0, 1, 0);
	//modelStack.Scale(0.1, 0.1, 0.1);
	//modelStack.Scale(13, 0.5, 13);
	//RenderMesh(meshList[GEO_ROBOT], true); // Tier 2

	//modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10, 0.1f, 85);
	modelStack.Scale(25, 1, 5);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	// Passengers //

	modelStack.PushMatrix(); // Hospital Pickup
	modelStack.Translate(hospital_spire_x1, hospital_spire_y1, hospital_spire_z1);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_PASSENGERS_PAD_1], false);

	if (!destination_1 && passengerID == 0 && !passenger_deliver_1)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(10, 0.1f, 85);
		modelStack.Scale(0.1, 1000, 0.1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_PASSENGERS_SPIRE_1], false);

		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix(); // Commercial Pickup
	modelStack.Translate(commercial_spire_x2, commercial_spire_y2, commercial_spire_z2);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_PASSENGERS_PAD_2], false);

	if (!destination_2 && passengerID == 0 && !passenger_deliver_2)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(10, 0.1f, 85);
		modelStack.Scale(0.1, 1000, 0.1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_PASSENGERS_SPIRE_2], false);

		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix(); // Resident Pickup 1
	modelStack.Translate(residential1_spire_x3, residential1_spire_y3, residential1_spire_z3);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_PASSENGERS_PAD_3], false);

	if (!destination_3 && passengerID == 0 && !passenger_deliver_3)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(10, 0.1f, 85);
		modelStack.Scale(0.1, 1000, 0.1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_PASSENGERS_SPIRE_3], false);

		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix(); // Resident Pickup 2
	modelStack.Translate(residential2_spire_x4, residential2_spire_y4, residential2_spire_z4);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_PASSENGERS_PAD_4], false);

	if (!destination_4 && passengerID == 0 && !passenger_deliver_4)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(10, 0.1f, 85);
		modelStack.Scale(0.1, 1000, 0.1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_PASSENGERS_SPIRE_4], false);

		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix(); // Mall Pickup
	modelStack.Translate(mall_spire_x5, mall_spire_y5, mall_spire_z5);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_PASSENGERS_PAD_5], false);

	if (!destination_5 && passengerID == 0 && !passenger_deliver_5)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(10, 0.1f, 85);
		modelStack.Scale(0.1, 1000, 0.1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_PASSENGERS_SPIRE_5], false);

		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// taxi
	if (!gamestart || upgrade)
	{
		modelStack.PushMatrix();
		if (upgrade)
		{
			modelStack.Translate(20, 0.25, 85);
		}

		else
		{
			modelStack.Translate(10, 0, 85);
		}
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_TAXI], true);

		modelStack.PushMatrix();
		modelStack.Scale(1, 0.4f, 1.75);
		modelStack.Translate(0, 0.5, 0);
		RenderMesh(meshList[GEO_HOVER], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	// SatNav Spires //

	// Residential District Spire
	if (destination_2 && passengerID == 2 && !passenger_deliver_2 || destination_5 && passengerID == 5 && !passenger_deliver_5)
	{
		modelStack.PushMatrix();
		modelStack.Translate(spire_x1, spire_y1, spire_z1);
		modelStack.Scale(1, 1000, 1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_NAV_SPIRE], false);
		modelStack.PopMatrix();
	}

	// Hospital Spire
	if (destination_4 && passengerID == 4 && !passenger_deliver_4)
	{
		modelStack.PushMatrix();
		modelStack.Translate(spire_x2, spire_y2, spire_z2);
		modelStack.Scale(1, 1000, 1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_NAV_SPIRE], false);
		modelStack.PopMatrix();
	}

	// Taxi Company Spire (Upgrade interaction only)
	if (!upgrade)
	{
		modelStack.PushMatrix();
		modelStack.Translate(spire_x3, spire_y3, spire_z3);
		modelStack.Scale(1, 1000, 1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_NAV_TAXI], false);
		modelStack.PopMatrix();
	}

	// Commercial District Spire
	if (destination_3 && passengerID == 3 && !passenger_deliver_3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(spire_x4, spire_y4, spire_z4);
		modelStack.Scale(1, 1000, 1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_NAV_SPIRE], false);
		modelStack.PopMatrix();
	}

	// Mall District Spire
	if (destination_1 && passengerID == 1 && !passenger_deliver_1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(spire_x5, spire_y5, spire_z5);
		modelStack.Scale(1, 1000, 1);
		modelStack.Rotate(spire_passive_rotate, 0, 1, 0);
		RenderMesh(meshList[GEO_NAV_SPIRE], false);
		modelStack.PopMatrix();
	}
	

	// Mall
	modelStack.PushMatrix();
	modelStack.Translate(-220, 0, -100);
	modelStack.Scale(50,20,100);
	RenderMesh(meshList[GEO_MALL], true);

	modelStack.PushMatrix();
	modelStack.Scale(0.02, 0.05, 0.01);
	modelStack.Translate(25, 8, 0);
	modelStack.Scale(1,2,10);
	RenderMesh(meshList[GEO_MALL_SIGN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(0.02, 0.05, 0.01);
	modelStack.Translate(50, 0, 0);
	modelStack.Scale(30, 0.5, 30);
	RenderMesh(meshList[GEO_MALL_PAD], false);

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Hospital

	modelStack.PushMatrix();
	modelStack.Scale(10,10,10);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(-1, 0.5, -3);
	RenderMesh(meshList[GEO_HOSPITAL], true);

	modelStack.PushMatrix();
	modelStack.Scale(0.1,0.35,0.1);
	modelStack.Translate(-5, 0.9, 0);
	RenderMesh(meshList[GEO_RED_CROSS], true);

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(0.1, 0.1, 0.35);
	modelStack.Translate(-5, 3.1, 0);
	//modelStack.Rotate(45, 0, 0, 1);
	RenderMesh(meshList[GEO_RED_CROSS], true);

	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);
	modelStack.Scale(0.5, 0.1, 0.5);
	//modelStack.Rotate(45, 0, 0, 1);
	RenderMesh(meshList[GEO_HOSPITAL_PAD], false);

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Business District

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(-20, 0, 200);
	RenderMesh(meshList[GEO_SCIFI_1], true);
	modelStack.PushMatrix();
	modelStack.Translate(74, 66, -6);
	modelStack.Scale(8, 1, 8);
	modelStack.Rotate(-7, 0, 1, 0);
	RenderMesh(meshList[GEO_SCIFI_PLATFORM], false);
	//modelStack.PushMatrix();
	//modelStack.Scale(0.08, 1000, 0.08);
	//RenderMesh(meshList[GEO_NAV_SPIRE], false);
	//modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 30);
	RenderMesh(meshList[GEO_SCIFI_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 230);
	RenderMesh(meshList[GEO_SCIFI_3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, 0, 250);
	RenderMesh(meshList[GEO_SCIFI_4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-75, 0, 30);
	RenderMesh(meshList[GEO_SCIFI_5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-25, 0, 30);
	RenderMesh(meshList[GEO_SCIFI_6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 100);
	RenderMesh(meshList[GEO_SCIFI_7], true);
	modelStack.PopMatrix();

	// Residential District
	for (int a = 0; a < 10; a++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-11 * (a + 1), 0, -15);
		RenderMesh(meshList[GEO_FLAT1], true);
		modelStack.PopMatrix();
	}

	for (int b = 0; b < 10; b++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-11 * (b + 1), 0, -60);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_FLAT1], true);
		//modelStack.PopMatrix();

		if (b == 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0,0,-25);
			modelStack.Scale(7,1,7);
			RenderMesh(meshList[GEO_FLAT_PAD], false);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	for (int c = 0; c < 10; c++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-11 * (c + 1), 0, -105);
		RenderMesh(meshList[GEO_FLAT1], true);
		modelStack.PopMatrix();
	}

	for (int d = 0; d < 10; d++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-11 * (d + 1), 0, -150);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_FLAT1], true);
		modelStack.PopMatrix();
	}


	// Onscreen text (UI)

	// Passenger pcik up & drop off
	if (camera.position.x >= hospital_spire_x1 - 5 && camera.position.x <= hospital_spire_x1 + 5 &&
		camera.position.y >= hospital_spire_y1 && camera.position.y <= hospital_spire_y1 + 2 &&
		camera.position.z >= hospital_spire_z1 - 5 && camera.position.z <= hospital_spire_z1 + 5
		&& !passenger_deliver_1 && !destination_1 && passengerID == 0) // Pick up at hospital
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick Up Passenger Nurse?", Color(0, 1, 0), 2, 0, 10);
	}

	if (!pause || !upgrade)
	{
		if (destination_1)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Mall", Color(0, 1, 0), 3, 38, 57);
		}
	}

	if (camera.position.x >= spire_x5 - 5 && camera.position.x <= spire_x5 + 5 &&
		camera.position.y >= spire_y5 && camera.position.y <= spire_y5 + 2 &&
		camera.position.z >= spire_z5 - 5 && camera.position.z <= spire_z5 + 5
		&& !passenger_deliver_1 && passengerID == 1) // Drop off at mall
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Drop off Passenger Nurse?", Color(0, 1, 0), 2, 0, 10);
	}

	//

	if (camera.position.x >= commercial_spire_x2 - 5 && camera.position.x <= commercial_spire_x2 + 5 &&
		camera.position.y >= commercial_spire_y2 && camera.position.y <= commercial_spire_y2 + 2 &&
		camera.position.z >= commercial_spire_z2 - 5 && camera.position.z <= commercial_spire_z2 + 5
		&& !passenger_deliver_2 && !destination_2 && passengerID == 0) // Pick up at commercial
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick Up Passenger Office Worker?", Color(0, 1, 0), 2, 0, 10);
	}

	if (!pause || !upgrade)
	{
		if (destination_2)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Flats", Color(0, 1, 0), 3, 38, 57);
		}
	}

	if (camera.position.x >= spire_x1 - 5 && camera.position.x <= spire_x1 + 5 &&
		camera.position.y >= spire_y1 && camera.position.y <= spire_y1 + 2 &&
		camera.position.z >= spire_z1 - 5 && camera.position.z <= spire_z1 + 5
		&& !passenger_deliver_2 && passengerID == 2) // Drop off at resident
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Drop off Passenger Office Worker?", Color(0, 1, 0), 2, 0, 10);
	}

	//

	if (camera.position.x >= residential1_spire_x3 - 5 && camera.position.x <= residential1_spire_x3 + 5 &&
		camera.position.y >= residential1_spire_y3 && camera.position.y <= residential1_spire_y3 + 2 &&
		camera.position.z >= residential1_spire_z3 - 5 && camera.position.z <= residential1_spire_z3 + 5
		&& !passenger_deliver_3 && !destination_3 && passengerID == 0) // Pick up at resident 1
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick Up Passenger Secretary?", Color(0, 1, 0), 2, 0, 10);
	}

	if (!pause || !upgrade)
	{
		if (destination_3)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "CEO Tower", Color(0, 1, 0), 3, 38, 57);
		}
	}

	if (camera.position.x >= spire_x4 - 5 && camera.position.x <= spire_x4 + 5 &&
		camera.position.y >= spire_y4 && camera.position.y <= spire_y4 + 2 &&
		camera.position.z >= spire_z4 - 5 && camera.position.z <= spire_z4 + 5
		&& !passenger_deliver_3 && passengerID == 3) // Drop off at commercial
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Drop off Passenger Secretary?", Color(0, 1, 0), 2, 0, 10);
	}

	//

	if (camera.position.x >= residential2_spire_x4 - 5 && camera.position.x <= residential2_spire_x4 + 5 &&
		camera.position.y >= residential2_spire_y4 && camera.position.y <= residential2_spire_y4 + 2 &&
		camera.position.z >= residential2_spire_z4 - 5 && camera.position.z <= residential2_spire_z4 + 5
		&& !passenger_deliver_4 && !destination_4 && passengerID == 0) // Pick up at resident 2
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick Up Passenger Granny?", Color(0, 1, 0), 2, 0, 10);
	}

	if (!pause || !upgrade)
	{
		if (destination_4)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Hospital", Color(0, 1, 0), 3, 38, 57);
		}
	}

	if (camera.position.x >= spire_x2 - 5 && camera.position.x <= spire_x2 + 5 &&
		camera.position.y >= spire_y2 && camera.position.y <= spire_y2 + 2 &&
		camera.position.z >= spire_z2 - 5 && camera.position.z <= spire_z2 + 5
		&& !passenger_deliver_4 && passengerID == 4) // Drop off at hospital
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Drop off Passenger Granny?", Color(0, 1, 0), 2, 0, 10);
	}


	if (camera.position.x >= mall_spire_x5 - 5 && camera.position.x <= mall_spire_x5 + 5 &&
		camera.position.y >= mall_spire_y5 && camera.position.y <= mall_spire_y5 + 2 &&
		camera.position.z >= mall_spire_z5 - 5 && camera.position.z <= mall_spire_z5 + 5
		&& !passenger_deliver_5 && !destination_5 && passengerID == 0) // Pick up at mall
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick Up Passenger Mother?", Color(0, 1, 0), 2, 0, 10);
	}

	if (!pause || !upgrade)
	{
		if (destination_5)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Flats", Color(0, 1, 0), 3, 38, 57);
		}
	}

	if (camera.position.x >= spire_x1 - 5 && camera.position.x <= spire_x1 + 5 &&
		camera.position.y >= spire_y1 && camera.position.y <= spire_y1 + 2 &&
		camera.position.z >= spire_z1 - 5 && camera.position.z <= spire_z1 + 5
		&& !passenger_deliver_5 && passengerID == 5) // Drop off at resident
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Drop off Passenger Mother?", Color(0, 1, 0), 2, 0, 10);
	}




	if (camera.position.x >= spire_x3 - 5 && camera.position.x <= spire_x3 + 5 &&
		camera.position.y >= spire_y3 && camera.position.y <= spire_y3 + 2 &&
		camera.position.z >= spire_z3 - 5 && camera.position.z <= spire_z3 + 5 &&
		gamestart && !upgrade && !pause)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press U to upgrade", Color(0, 1, 0), 2, 0, 10);
	}
	// Taxi Company


	// Other UIs
	if (!cam_control && !gamestart)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "HoverTaxi Simulator", Color(0, 1, 0), 4, 2.5, 10);
		RenderMeshOnScreen(meshList[GEO_QUAD], 39.75, 6.6f, 15, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], "Start", Color(0, 1, 0), 3, 32.5, 5);
	}

	else if (cam_control)
	{
		/*if (passengerID == 1)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Destination: CEO Tower", Color(0, 1, 0), 3, 0, 0);
		}
		*/
		RenderTextOnScreen(meshList[GEO_TEXT], "Destination: ", Color(0, 1, 0), 3, 0, 57);
		RenderTextOnScreen(meshList[GEO_TEXT], "Cash: $" + std::to_string(cash), Color(0, 1, 0), 3, 0, 54);
	}

	if (pause && gamestart)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Paused", Color(0, 1, 0), 4, 2, 10);
		RenderMeshOnScreen(meshList[GEO_QUAD], 1.5, 6.6f, 35, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], "Resume", Color(0, 1, 0), 3, 1, 5);
	}

	if (upgrade && gamestart)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Upgrades", Color(0, 1, 0), 3, 0, 57);
		RenderTextOnScreen(meshList[GEO_TEXT], "Cash: $" + std::to_string(cash), Color(0, 1, 0), 3, 0, 54);
		
		RenderMeshOnScreen(meshList[GEO_QUAD], 3.5, 11.5, 3, 3);
		RenderMeshOnScreen(meshList[GEO_QUAD], 3.5, 14.5, 3, 3);
		RenderMeshOnScreen(meshList[GEO_QUAD], 3.5, 17.5, 3, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], "  Speed boost: $20", Color(0, 1, 0), 3, 2, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "  Hover boost: $10", Color(0, 1, 0), 3, 2, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], "  Side thrusters: $10", Color(0, 1, 0), 3, 2, 16);

		if (side_hover)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "X", Color(0, 1, 0), 3, 2, 10);
		}

		if (lift_boost)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "X", Color(0, 1, 0), 3, 2, 13);
		}

		if (speed_boost)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "X", Color(0, 1, 0), 3, 2, 16);

		}
		
		
		RenderMeshOnScreen(meshList[GEO_QUAD], 1.5, 6.6f, 35, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], "Exit", Color(0, 1, 0), 3, 1, 5);
	}

	if (passenger_deliver_1 && passenger_deliver_2 && passenger_deliver_3 && passenger_deliver_4 && passenger_deliver_5)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "All passengers delivered", Color(0, 1, 0), 2, 2.5, 14);
		RenderTextOnScreen(meshList[GEO_TEXT], "You may head to either sides of the map", Color(0, 1, 0), 2, 2.5, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "to visit other scenes", Color(0, 1, 0), 2, 2.5, 8);
		RenderTextOnScreen(meshList[GEO_TEXT], "Or press R to restart", Color(0, 1, 0), 2, 2.5, 4);
	}
}

void SceneYX::Exit()
{
	// Cleanup VBO here
	//glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	//glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneYX::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneYX::RenderSkybox()
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

void SceneYX::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneYX::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneYX::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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
