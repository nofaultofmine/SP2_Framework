#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Assignment2.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include <string>

//hi XinJie
Assignment2::Assignment2()
{
}

Assignment2::~Assignment2()
{
}

void Assignment2::Init()
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
	bodyRotateAngle = 0.0f;
	bodyRotateAngle2 = 0.0f;
	bodyRotateAngle3 = 0.0f;
	headRotateAngle = 0.0f;
	armRotateAngle = -60.0f;
	armRotateAngle2 = 0.0f;
	armIdleRotateAngle -50.0f;
	swordRotateAngle = 0.0f;
	swordRotateAngle2 = 0.0f;
	translateX = 0.0f;
	translateY = 0.0f;
	translateZ = -100.0f;
	masterSwordY = -8.0f;
	bombX = 0.0f;
	bombY = 0.0f;

	scaleAll = 0.2f;
	eyeScale = 0.2f;
	rotateSpeed = 360.0f;
	bodyRotateSpeed = 24.0f;
	bodyRotateSpeed2 = 90.0f;
	headRotateSpeed = 15.0f;
	armRotateSpeed = 60.0f;
	armRotateSpeed2 = 10.0f;
	armIdleRotateSpeed = 60.0f;
	swordRotateSpeed = 0.0f;
	swordRotateSpeed2 = 0.0f;
	translateSpeed = 1.6f;
	scaleSpeed = 1.0f;
	eyeScaleSpeed = 1.0f;
	blinkTimer = 0.0f;
	swordFlashTimer = 0.5f;
	bombTimer = 0.5f;
	bombGravity = 5.0f;

	eyesClosed = false;
	chest1Opened = false;
	canInteract = false;
	setBombInActive = false;
	instructionTextBoxOn = true;
	bombFlash = false;
	swordSequence1 = false;
	swordSequence2 = false;

	throwSequence1 = false;
	throwSequence2 = false;
	throwSequence3 = false;

	anim1Playing = false;
	anim2Playing = false;
	anim3Playing = false;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 0, 0), 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//bottom1.tga");
	
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("textBox", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//textBox.tga");

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJMTL("model1", "OBJ//chest2.obj", "OBJ//chest2.mtl");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chest2.tga");

	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJ("BentTree", "OBJ//BentTree.obj");
	meshList[GEO_MODEL2]->textureID = LoadTGA("Image//BentTree.tga");

	meshList[GEO_MODEL3] = MeshBuilder::GenerateOBJMTL("LilyLarge", "OBJ//lily_large.obj", "OBJ//lily_large.mtl");

	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJ("leaves", "OBJ//Bush_1.obj"); 
	meshList[GEO_MODEL4]->textureID = LoadTGA("Image//Green.tga");

	meshList[GEO_MODEL5] = MeshBuilder::GenerateOBJMTL("DekuTree", "OBJ//dekutree2.obj", "OBJ//dekutree2.mtl");
	meshList[GEO_MODEL5]->textureID = LoadTGA("Image//2.tga");

	meshList[GEO_MODEL6] = MeshBuilder::GenerateOBJMTL("Block", "OBJ//cliff_block_rock.obj", "OBJ//cliff_block_rock.mtl");

	meshList[GEO_MODEL7] = MeshBuilder::GenerateOBJMTL("BlockCave", "OBJ//cliff_blockCave_rock.obj", "OBJ//cliff_blockCave_rock.mtl");

	meshList[GEO_MODEL8] = MeshBuilder::GenerateOBJMTL("CaveOpening", "OBJ//cliff_cave_rock.obj", "OBJ//cliff_cave_rock.mtl");

	meshList[GEO_MODEL9] = MeshBuilder::GenerateOBJMTL("RockCorner", "OBJ//cliff_blockDiagonal_rock.obj", "OBJ//cliff_blockDiagonal_rock.mtl");

	meshList[GEO_MODEL10] = MeshBuilder::GenerateOBJMTL("Tree", "OBJ//TreeLeaf.obj", "OBJ//TreeLeaf.mtl");
	meshList[GEO_MODEL10]->textureID = LoadTGA("Image//TreeLeaf.tga");

	meshList[GEO_MODEL11] = MeshBuilder::GenerateOBJMTL("Bridge", "OBJ//bridge_woodRound.obj", "OBJ//bridge_woodRound.mtl");

	meshList[GEO_MODEL12] = MeshBuilder::GenerateOBJMTL("RiverRocks", "OBJ//ground_riverRocks.obj", "OBJ//ground_riverRocks.mtl");

	meshList[GEO_MODEL13] = MeshBuilder::GenerateOBJMTL("RiverBend", "OBJ//ground_riverBend.obj", "OBJ//ground_riverBend.mtl");

	meshList[GEO_MODEL14] = MeshBuilder::GenerateOBJMTL("Waterfall", "OBJ//cliff_waterfall_rock.obj", "OBJ//cliff_waterfall_rock.mtl");

	meshList[GEO_MODEL15] = MeshBuilder::GenerateOBJMTL("RiverCross", "OBJ//ground_riverCross.obj", "OBJ//ground_riverCross.mtl");

	meshList[GEO_MODEL16] = MeshBuilder::GenerateOBJMTL("RiverStraight", "OBJ//ground_riverStraight.obj", "OBJ//ground_riverStraight.mtl");

	meshList[GEO_MODEL17] = MeshBuilder::GenerateOBJMTL("RiverEnd", "OBJ//ground_riverEndClosed.obj", "OBJ//ground_riverEndClosed.mtl");

	meshList[GEO_MODEL18] = MeshBuilder::GenerateOBJMTL("StatueColumn", "OBJ//statue_column.obj", "OBJ//statue_column.mtl");

	meshList[GEO_MODEL19] = MeshBuilder::GenerateOBJMTL("StatueColumn", "OBJ//statue_columnDamaged.obj", "OBJ//statue_columnDamaged.mtl");

	meshList[GEO_MODEL20] = MeshBuilder::GenerateOBJMTL("RiverOpen", "OBJ//ground_riverOpen.obj", "OBJ//ground_riverOpen.mtl");

	meshList[GEO_MODEL21] = MeshBuilder::GenerateOBJMTL("RiverSide", "OBJ//ground_riverSide.obj", "OBJ//ground_riverSide.mtl");

	meshList[GEO_MODEL22] = MeshBuilder::GenerateOBJMTL("RiverCorner", "OBJ//ground_riverCorner.obj", "OBJ//ground_riverCorner.mtl");

	meshList[GEO_MODEL23] = MeshBuilder::GenerateOBJMTL("StoneA", "OBJ//stone_tallA.obj", "OBJ//stone_tallA.mtl");

	meshList[GEO_MODEL24] = MeshBuilder::GenerateOBJMTL("StoneD", "OBJ//stone_tallD.obj", "OBJ//stone_tallA.mtl");

	meshList[GEO_MODEL25] = MeshBuilder::GenerateOBJMTL("StatueHead", "OBJ//statue_head.obj", "OBJ//statue_head.mtl");

	meshList[GEO_MODEL26] = MeshBuilder::GenerateOBJMTL("MasterSword", "OBJ//mastersword.obj", "OBJ//mastersword.mtl");
	meshList[GEO_MODEL26]->textureID = LoadTGA("Image//mastersword.tga");

	meshList[GEO_MODEL27] = MeshBuilder::GenerateOBJMTL("darkLink", "OBJ//darkLink.obj", "OBJ//darkLink.mtl");
	meshList[GEO_MODEL27]->textureID = LoadTGA("Image//darkLink.tga");

	meshList[GEO_MODEL28] = MeshBuilder::GenerateOBJMTL("Altar", "OBJ//stone_largeA.obj", "OBJ//stone_largeA.mtl");

	meshList[GEO_MODEL29] = MeshBuilder::GenerateOBJMTL("Sign", "OBJ//sign.obj", "OBJ//sign.mtl");

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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_TEXT1] = MeshBuilder::GenerateText("text1", 16, 16);
	meshList[GEO_TEXT1]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text2", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Image//ComicSans.tga");

	//Link Meshes
	{
		meshList[GEO_CUBE] = MeshBuilder::GenerateCube("BeltBuckle", 1, 1, 1, Color(1, 1, 0));
		meshList[GEO_CUBE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CUBE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CUBE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CUBE]->material.kShininess = 5.f;

		meshList[GEO_RING] = MeshBuilder::GenerateRing("mouth", Color(0, 0, 0), 36);
		meshList[GEO_RING]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_RING]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_RING]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_RING]->material.kShininess = 5.f;

		meshList[GEO_SWORDBASE] = MeshBuilder::GenerateCube("Sword Base", 1, 1, 1, Color(0, 0, 1));
		meshList[GEO_SWORDBASE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SWORDBASE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SWORDBASE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SWORDBASE]->material.kShininess = 5.f;

		meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("ArmJoint", Color(0.8f, 0.4f, 0), 36, 36, 1.0f);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE]->material.kShininess = 5.f;

		meshList[GEO_HAIR] = MeshBuilder::GenerateSphere("Hair", Color(1, 0.647f, 0), 36, 36, 1.0f);
		meshList[GEO_HAIR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_HAIR]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_HAIR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_HAIR]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Torso", Color(0, 0.6f, 0), 36, 36, 1.5f, 2);
		meshList[GEO_CYLINDER]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER2] = MeshBuilder::GenerateCylinder("Belt", Color(0.7f, 0.35f, 0), 36, 36, 1.55f, 0.5f);
		meshList[GEO_CYLINDER2]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER2]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER3] = MeshBuilder::GenerateCylinder("arms", Color(0.8f, 0.4f, 0), 36, 36, 1.0f, 0.5f);
		meshList[GEO_CYLINDER3]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER3]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER3]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER4] = MeshBuilder::GenerateCylinder("Legs", Color(1, 1, 1), 36, 36, 0.5f, 1);
		meshList[GEO_CYLINDER4]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER4]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER4]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER4]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER5] = MeshBuilder::GenerateCylinder("Hat base", Color(0, 0.8f, 0), 36, 36, 1.5f, 0.3f);
		meshList[GEO_CYLINDER5]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER5]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER5]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER5]->material.kShininess = 5.f;

		meshList[GEO_CYLINDER6] = MeshBuilder::GenerateCylinder("Hat base", Color(0, 0.4f, 0), 36, 36, 1.5f, 1);
		meshList[GEO_CYLINDER6]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CYLINDER6]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CYLINDER6]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CYLINDER6]->material.kShininess = 5.f;

		meshList[GEO_SWORDBLADE] = MeshBuilder::GenerateCylinder("Sword Blade", Color(0.753f, 0.753f, 0.753f), 36, 36, 0.5f, 5.5f);
		meshList[GEO_SWORDBLADE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_SWORDBLADE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SWORDBLADE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SWORDBLADE]->material.kShininess = 10.f;

		meshList[GEO_SWORDHILT] = MeshBuilder::GenerateCylinder("Sword hilt", Color(0, 0, 1), 36, 36, 0.5f, 2.5f);
		meshList[GEO_SWORDHILT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SWORDHILT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SWORDHILT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SWORDHILT]->material.kShininess = 5.f;

		meshList[GEO_SWORDTIP] = MeshBuilder::GenerateCone("Hat tip", Color(0.753f, 0.753f, 0.753f), 2, 0.5f);
		meshList[GEO_SWORDTIP]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_SWORDTIP]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SWORDTIP]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_SWORDTIP]->material.kShininess = 10.f;

		meshList[GEO_EARS] = MeshBuilder::GenerateHemisphere("Ears", Color(1, 0.855f, 0.725f), 36, 36, 1.0f);
		meshList[GEO_EARS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_EARS]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_EARS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_EARS]->material.kShininess = 5.f;

		meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("BodyTop", Color(0, 0.6f, 0), 36, 36, 1.5f);
		meshList[GEO_SPHERE2]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPHERE2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SPHERE2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE2]->material.kShininess = 5.f;

		meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("Head & Hands", Color(1, 0.855f, 0.725f), 36, 36, 1.0f);
		meshList[GEO_SPHERE3]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SPHERE3]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SPHERE3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SPHERE3]->material.kShininess = 5.f;

		meshList[GEO_EYES] = MeshBuilder::GenerateSphere("eyes", Color(0, 0, 0), 36, 36, 1.0f);
		meshList[GEO_EYES]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_EYES]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_EYES]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_EYES]->material.kShininess = 5.f;

		meshList[GEO_BOMBCASE] = MeshBuilder::GenerateSphere("Bomb case", Color(0, 0, 1), 36, 36, 3.0f);
		meshList[GEO_BOMBCASE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_BOMBCASE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_BOMBCASE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_BOMBCASE]->material.kShininess = 10.f;

		meshList[GEO_LEGJOINT] = MeshBuilder::GenerateSphere("LegJoint", Color(1, 1, 1), 36, 36, 0.5f);
		meshList[GEO_LEGJOINT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_LEGJOINT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LEGJOINT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LEGJOINT]->material.kShininess = 5.f;

		meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("Eyebrows", Color(0.8f, 0.4f, 0), 36, 36, 1.0f);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_HEMISPHERE]->material.kShininess = 5.f;

		meshList[GEO_EXPLOSION] = MeshBuilder::GenerateHemisphere("hemisphere", Color(1.0f, 0.7f, 0), 36, 36, 1.0f);
		meshList[GEO_EXPLOSION]->material.kAmbient.Set(0.65f, 0.65f, 0.65f);
		meshList[GEO_EXPLOSION]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_EXPLOSION]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_EXPLOSION]->material.kShininess = 10.f;

		meshList[GEO_BOMBFUSE] = MeshBuilder::GenerateCylinder("Bombfuse", Color(1.0f, 0.7f, 0), 36, 36, 1.0f, 1);
		meshList[GEO_BOMBFUSE]->material.kAmbient.Set(0.65f, 0.65f, 0.65f);
		meshList[GEO_BOMBFUSE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_BOMBFUSE]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
		meshList[GEO_BOMBFUSE]->material.kShininess = 10.f;

		meshList[GEO_SIDEBURNS] = MeshBuilder::GenerateHemisphere("Sideburns", Color(1, 0.647f, 0), 36, 36, 1.0f);
		meshList[GEO_SIDEBURNS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SIDEBURNS]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SIDEBURNS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SIDEBURNS]->material.kShininess = 5.f;

		meshList[GEO_POMMEL] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0, 0, 1), 36, 36, 1.0f);
		meshList[GEO_POMMEL]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_POMMEL]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_POMMEL]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_POMMEL]->material.kShininess = 5.f;

		meshList[GEO_SWORDGEM] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0.8f, 0, 0), 36, 36, 1.0f);
		meshList[GEO_SWORDGEM]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_SWORDGEM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SWORDGEM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SWORDGEM]->material.kShininess = 5.f;

		meshList[GEO_CONE] = MeshBuilder::GenerateCone("left ear", Color(1, 0.855f, 0.725f), 2, 1);
		meshList[GEO_CONE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CONE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CONE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CONE]->material.kShininess = 5.f;

		meshList[GEO_CONE2] = MeshBuilder::GenerateCone("Hat tip", Color(0, 0.4f, 0), 2, 1.5);
		meshList[GEO_CONE2]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_CONE2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_CONE2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CONE2]->material.kShininess = 5.f;
	}

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

void Assignment2::Reset()
{

}

void Assignment2::Update(double dt)
{
	camera.Update(dt);

	if (camera.position.z > -220.0f && camera.position.z < -180.0f && camera.position.x < -120.0f && camera.position.x > -150.0f)
	{
		if (Application::IsKeyPressed('E'))
		{
			instructionTextBoxOn = true;
		}
	}

	light[1].position.x = camera.position.x;
	light[1].position.z = camera.position.z;
	//Camera bounds

	if (camera.position.z > -172.0f && camera.position.z < 160.0f && camera.position.x < 11.0f && camera.position.x > -11.0f)
		atCaveEntrance = true;
	else
	{
		atCaveEntrance = false;
	}

	if (camera.position.z > -125.0f)
	{
		insideCave = true;
	}

	if (!caveEntranceBroken)
	{
		if (camera.position.x > 240.0f)
		{
			camera.position.x -= 1.5f;
			camera.target.x -= 1.5f;
		}
		if (camera.position.x < -240.0f)
		{
			camera.position.x += 1.5f;
			camera.target.x += 1.5f;
		}
	}
	else
	{
		if (camera.position.z > -169.0f && camera.position.z < 160.0f)
		{	
			if (!insideCave)
			{
				if (camera.position.x > 10.0f)
				{
					camera.position.x -= 1.5f;
					camera.target.x -= 1.5f;
				}
				if (camera.position.x < -10.0f)
				{
					camera.position.x += 1.5f;
					camera.target.x += 1.5f;
				}
			}
			else
			{
				if (camera.position.z < 0.0f)
				{
					if (camera.position.x > 15.0f)
					{
						camera.position.x -= 1.5f;
						camera.target.x -= 1.5f;
					}
					if (camera.position.x < -15.0f)
					{
						camera.position.x += 1.5f;
						camera.target.x += 1.5f;
					}
				}
				if (camera.position.z < 160.0f && camera.position.z > 0.0f)
				{
					if (camera.position.x > 30.0f)
					{
						camera.position.x -= 1.5f;
						camera.target.x -= 1.5f;
					}
					if (camera.position.x < -30.0f)
					{
						camera.position.x += 1.5f;
						camera.target.x += 1.5f;
					}
				}
			}
		}
	}

	if (!caveEntranceBroken)
	{
		if (camera.position.z > -170.0f)
		{
			camera.position.z -= 1.5f;
			camera.target.z -= 1.5f;
		}
		if (camera.position.z < -240.0f)
		{
			camera.position.z += 1.5f;
			camera.target.z += 1.5f;
		}
	}
	else
	{
		if (!atCaveEntrance)
		{
			if (!insideCave)
			{
				if (camera.position.z > -170.0f)
				{
					camera.position.z -= 1.5f;
					camera.target.z -= 1.5f;
				}
			}
		}
		if (insideCave)
		{
			if (camera.position.z > 160.0f)
			{
				camera.position.z -= 1.5f;
				camera.target.z -= 1.5f;
			}
			if (camera.position.z < -125.0f)
			{
				camera.position.z += 1.5f;
				camera.target.z += 1.5f;
			}
		}
	}

	//FPS = 1.f / dt;
	blinkTimer += (float)(1 * dt);
	if (blinkTimer >= 3.0f)
		eyeScale += (float)(eyeScaleSpeed * dt);

	lilyRotateAngle += (float)(10 * dt);

	if (textTimer > 0)
	{
		textTimer -= (float)(1 * dt);
	}

	if (anim1Playing || anim2Playing || anim3Playing)
	{
		//rotateAngle += (float)(rotateSpeed * dt);
		//bodyRotateAngle += (float)(bodyRotateSpeed * dt);
		if (anim1Playing)
		{
			rotateAngle += (float)(rotateSpeed * dt);
			translateY += (float)(translateSpeed * dt);
		}
		if (anim2Playing)
		{
			swordRotateAngle2 += (float)(swordRotateSpeed2 * dt);
			bodyRotateAngle2 += (float)(bodyRotateSpeed2 * dt);
			headRotateAngle += (float)(headRotateSpeed * dt);
			swordRotateAngle += (float)(swordRotateSpeed * dt);

			if (swordSequence1 == false)
			{
				armRotateAngle += (float)(armRotateSpeed * dt);
				armRotateAngle2 += (float)(armRotateSpeed2 * dt);
			}
		}
		if (anim3Playing)
		{
			headRotateAngle += (float)(headRotateSpeed * dt);
			armRotateAngle += (float)(armRotateSpeed * dt);
			armRotateAngle2 += (float)(armRotateSpeed2 * dt);
		}
	}
	else
	{
		armIdleRotateAngle += (float)(armIdleRotateSpeed * dt);
		headRotateSpeed += (float)(5 * dt);
		headRotateAngle += (float)(headRotateSpeed * dt);
		if (headRotateAngle >= 25.0f)
		{
			headRotateSpeed = -headRotateSpeed;
		}
		else if (headRotateAngle <= -25.0f)
		{
			headRotateSpeed = -headRotateSpeed;
		}
		if (armIdleRotateAngle >= -45.0f)
		{
			armIdleRotateSpeed = -30.0f;
		}
		else if (armIdleRotateAngle <= -60.0f)
		{
			armIdleRotateSpeed = 30.0f;
		}
	}

	//Interactions
	//Player opens chest
	if (chest1Opened == false)
	{
		cutSceneOn = true;
		bombY = 0.5f;
		bombX = 230;
		bombZ = -215;
		if (camera.position.x > 210 && camera.position.x < 240 && camera.position.z > -230 && camera.position.z < -210)
		{
			canInteract = true;
			if (Application::IsKeyPressed('E') && canInteract == true)
			{
				chest1Opened = true;
				textTimer = 5.0f;
			}
		}
		else
		{
			canInteract = false;
		}
	}
	else
	{
		if (bombY < 20.0f && !gotBombs)
		{
			bombY += 10 * dt;
		}
		else
		{
			gotBombs = true;
			goalNumber = 1;
			setBombInActive = true;
		}
	}
	//Link Walks to Deku Tree
	if (caveEntranceBroken)
	{
		anim3Playing = false;
		if (translateZ < 70 && cutSceneOn != false && !reachedDekuTree)
		{
			//cutSceneOn = true;
			anim1Playing = true;
			translateZ += 10 * dt;
			if (translateZ > 10 && translateZ < 20)
			{
				translateX += 10 * dt;
				bodyRotateAngle = 45;
			}
			if (translateZ > 20 && translateX > 0.0f)
			{
				bodyRotateAngle = -45;
				translateX -= 10 * dt;
			}
			if (translateX < 0.0f)
			{
				bodyRotateAngle = 0.0f;
			}
		}
		else
		{
			translateX = 0.0f;
			anim1Playing = false;
			if (!masterSwordGet)
			{
				cutSceneOn = false;
			}
			else
				canClick = false;
			reachedDekuTree = true;
			goalNumber = 2;
		}
	}

	//Link throws Bomb
	if (gotBombs && !caveEntranceBroken && camera.position.x > -25 && camera.position.x < 25 && camera.position.z > -230 && camera.position.z < -160)
	{
		if (Application::IsKeyPressed('E'))
		{
			anim1Playing = false;
			anim2Playing = false;
			anim3Playing = true;
		}
	}

	//Master Sword Interaction
	if (!masterSwordGet && translateZ < 30 && reachedDekuTree)
	{
		cutSceneOn = true;
		if (masterSwordY < 0.0f)
		{
			masterSwordY += 3 * dt;
		}
		if(masterSwordY > 0.0f)
		{
			masterSwordGet = true;
			goalNumber = 3;
		}
	}
	if (masterSwordGet && !darkLinkDefeated)
	{
		goalNumber = 3;
		if (dialogueNumber == 5)
		{
			if (translateZ < 55.0f)
			{
				bodyRotateAngle = 0.0f;
				translateZ += 10 * dt;
				anim1Playing = true;
			}
			else
			{
				anim1Playing = false;
				if (!anim2Playing)
				{
					armRotateAngle = -60.0f;
					armRotateAngle2 = 0.0f;
					headRotateAngle = 0.0f;
				}
				anim2Playing = true;
			}
		}
	}

	//Dark Link defeated
	if(darkLinkDefeated)
	{
		headRotateAngle = 0.0f;
		anim1Playing = false;
		anim2Playing = false;
		anim3Playing = false;
		goalNumber = 4;
	}

	if (darkLinkDefeated && camera.position.x > -15 && camera.position.x < 15 && camera.position.z > 90 && camera.position.z < 135)
	{
		if (Application::IsKeyPressed('E'))
		{
			goalNumber = 5;
		}
	}
	
	if (Application::IsKeyPressed('T') || cutSceneOn == true)
	{
		//anim2Playing = false;
		if (cutSceneOn == false)
		{
			if (Application::IsKeyPressed('T'))
			{
				translateZ -= 10 * dt;
				bodyRotateAngle = 180;
				anim1Playing = true;
			}
		}
	}
	else
	{
		anim1Playing = false;
	}

	if (eyeScale >= 0.2f)
		eyeScaleSpeed = -1.0f;
	else if (eyeScale <= 0.01f)
	{
		eyeScaleSpeed = 1.0f;
		eyesClosed = true;
	}
	if (eyeScale >= 0.2f && eyesClosed == true)
	{
		blinkTimer = 0.0f;
		eyesClosed = false;
	}

	//Animation 1 variables (Walking)
	if (anim1Playing == true)
	{
		if (rotateAngle > 45)
			rotateSpeed = -360;
		if (rotateAngle < -45)
			rotateSpeed = 360;

		if (translateY > 0.2f)
			translateSpeed = -1.6f;
		else if(translateY < 0)
			translateSpeed = 1.6f;
	}

	//Animation 2 variables (Sword swinging)
	if (anim2Playing == true)
	{
		if (swordSequence1 == false)
		{
			swordFlashTimer = 0.5f;
			if (bodyRotateAngle3 <= 10.0f)
			{
				bodyRotateAngle3 += (float)(45 * dt);
			}

			if (bodyRotateAngle2 >= 45)
			{
				armRotateSpeed = 0.0f;
				bodyRotateSpeed2 = -360.0f;
				headRotateSpeed = 360.0f;
				armRotateSpeed2 = 270.0f;
				swordRotateSpeed2 = 270.0f;
			}
			if (armRotateAngle2 > 90)
			{
				armRotateSpeed2 = 0.0f;
				swordRotateSpeed2 = 0.0f;
			}
			if (bodyRotateAngle2 <= -75)
			{
				swordSequence1 = true;
			}
		}
		if (swordSequence2 == false && swordSequence1 == true)
		{
			swordRotateSpeed = -1440.0f;

			if (swordRotateAngle <= -180.0f)
			{
				swordRotateSpeed = 0.0f;
			}
			if (swordRotateAngle2 >= -45.0f)
			{
				swordRotateSpeed2 = -1080.0f;
			}
			else
			{
				swordRotateSpeed2 = 0.0f;
			}
			if (bodyRotateAngle2 <= -85.0f)
			{
				swordFlashTimer -= (float)(2 * dt);
				if (swordFlashTimer >= 0.0f)
				{
					bodyRotateSpeed2 = 0.0f;
					headRotateSpeed = 0.0f;
				}
				else
				{
					bodyRotateSpeed2 = 2520.0f;
					headRotateSpeed = -2520.0f;
				}
			}
			if (bodyRotateAngle2 >= 0.0f)
			{
				headRotateSpeed = 0.0f;
			}
			if (bodyRotateAngle2 >= 2160.0f)
			{
				bodyRotateSpeed2 = 0.0f;
				headRotateSpeed = 0.0f;
				swordSequence2 = true;
				darkLinkDefeated = true;
			}
		}
	}

	//Animation 3 variables (Bomb throw)
	if (anim3Playing == true)
	{
		if (throwSequence1 == false)
		{
			swordFlashTimer = 2.0f;
			bombX = 0;
			bombY = 16.0f;
			bombZ = -200;
			headRotateSpeed = -90.0f;
			armRotateSpeed = 270.0f;
			if (headRotateAngle <= -15.0f)
			{
				headRotateSpeed = 0.0f;
			}
			if (armRotateAngle >= 100.0f)
			{
				armRotateSpeed = 0.0f;
				throwSequence1 = true;
			}
		}
		if (throwSequence2 == false && throwSequence1 == true)
		{
			if (armRotateAngle >= 165.0f)
				armRotateSpeed = 0.0f;
			else
				armRotateSpeed = 270.0f;

			if (armRotateAngle2 >= 180.0f)
				armRotateSpeed2 = 0.0f;
			else
				armRotateSpeed2 = 270.0f;

			if (headRotateAngle <= 0.0f)
				headRotateSpeed = 90.0f;
			else
				headRotateSpeed = 0.0f;

			bombGravity += (float)(30 * dt);
			if (bombY >= -2.0f)
			{
				bombY -= bombGravity * dt;
				bombZ += 40 * dt;
			}
			bombTimer -= (float)(1 * dt);
			if (bombTimer <= 0.0f)
			{
				bombFlash = !bombFlash;
				if (bombFlash == true)
				{
					meshList[GEO_BOMBCASE] = MeshBuilder::GenerateSphere("Bomb case", Color(1, 0, 0), 36, 36, 3.0f);
				}
				else
				{
					meshList[GEO_BOMBCASE] = MeshBuilder::GenerateSphere("Bomb case", Color(0, 0, 1), 36, 36, 3.0f);
				}
				bombTimer = 0.3f;
			}

			swordFlashTimer -= (float)(1 * dt);
			if(swordFlashTimer <= 0.0f)
			{
				throwSequence2 = true;
			}
		}
		if (throwSequence3 == false && throwSequence2 == true)
		{
			scaleSpeed += (float)(10 * dt);
			scaleAll += (float)(scaleSpeed * dt);
			if (scaleAll >= 25.0f)
			{
				scaleSpeed = 0.0f;
				throwSequence3 = true;
			}
		}
		else if (throwSequence3 == true)
		{
			caveEntranceBroken = true;
		}
	}

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


	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		if (reachedDekuTree)
		{
			if (dialogueNumber < 4)
			{
				dialogueNumber += 1;
			}
			else if (dialogueNumber == 4 && masterSwordGet)
			{
				dialogueNumber += 1;
			}
		}
		if (instructionTextBoxOn)
		{
			instructionTextBoxOn = false;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		//std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		//std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		//std::cout << "RBUTTON UP" << std::endl;
	}
	//camera.position.y = 10;
}

void Assignment2::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(0, -6, -235);
	modelStack.Scale(600, 200, 200);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(230, -5, -215);
	modelStack.Scale(0.15f, 0.15f, 0.15f);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL1], true);
	modelStack.PopMatrix();

	//Cliffs & Rocks
	//Floor rock
	modelStack.PushMatrix();
	modelStack.Translate(0, -12, 50);
	modelStack.Scale(190, 6.f, 130.f);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	/*
	//Ceiling Rock
	modelStack.PushMatrix();
	modelStack.Translate(0, 110, 60);
	modelStack.Scale(500, 6.f, 420.f);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();
	*/

	//First Wall
	modelStack.PushMatrix();
	modelStack.Translate(30, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(60, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(90, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(180, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-30, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-90, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-180, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 24, -150);
	modelStack.Scale(30, 90, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -6, -150);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(180, 0, 1, 0);
	if (caveEntranceBroken)
	{
		RenderMesh(meshList[GEO_MODEL8], true);
	}
	else
	{
		RenderMesh(meshList[GEO_MODEL7], true);
	}
	//RenderMesh(meshList[GEO_MODEL7], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL9], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, -150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL9], true);
	modelStack.PopMatrix();

	//Second Wall
	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, -120);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, -90);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, -60);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, -30);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 0);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 30);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 60);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 90);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 120);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 180);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 210);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 240);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL9], true);
	modelStack.PopMatrix();

	//Third Wall
	modelStack.PushMatrix();
	modelStack.Translate(210, -6, -120);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, -90);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, -60);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, -30);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 0);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 30);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 60);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 90);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 120);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 150);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 180);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 210);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 240);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL9], true);
	modelStack.PopMatrix();

	//Final Wall
	modelStack.PushMatrix();
	modelStack.Translate(30, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(60, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(90, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(180, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-30, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-90, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-180, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -6, 270);
	modelStack.Scale(30, 120, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	//Cave Floor
	//Entrance Side
	modelStack.PushMatrix();
	modelStack.Translate(0, -66, -120);
	modelStack.Scale(60, 60, 30);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	//End Side
	modelStack.PushMatrix();
	modelStack.Translate(0, -66, 185);
	modelStack.Scale(120, 60, 140);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	//Trees & Nature
	modelStack.PushMatrix();
	modelStack.Translate(0, -5, 200);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-260, -7.f, -200);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-260, -7.f, -170);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-260, -7.f, -150);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-230, -7.f, -150);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-260, -7.f, -230);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-260, -7.f, -260);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-240, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-210, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-180, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-90, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-30, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(30, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(60, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(90, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(180, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(210, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(270, -7.f, -270);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(270, -7.f, -240);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(270, -7.f, -210);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(270, -7.f, -180);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(265, -7.f, -150);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(235, -7.f, -150);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -66, -60);
	modelStack.Scale(60, 60, 90);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();

	//Rivers
	//Left Side
	modelStack.PushMatrix();
	modelStack.Translate(60, -6.f, -105);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -6.f, -105);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, -105);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL22], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, -45);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, 15);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, 75);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, 135);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, 195);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(172.5f, -6.f, 240);
	modelStack.Scale(45, 60, 30);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL22], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -6.f, 240);
	modelStack.Scale(60, 60, 30);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(75, -6.f, 240);
	modelStack.Scale(30, 60, 30);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	//Right Side
	modelStack.PushMatrix();
	modelStack.Translate(-60, -6.f, -105);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, -6.f, -105);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, -105);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL22], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, -45);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, 15);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, 75);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, 135);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, 195);
	modelStack.Scale(45, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-172.5f, -6.f, 240);
	modelStack.Scale(45, 60, 30);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL22], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-120, -6.f, 240);
	modelStack.Scale(60, 60, 30);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-75, -6.f, 240);
	modelStack.Scale(30, 60, 30);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL21], true);
	modelStack.PopMatrix();

	//Large Water Patch
	modelStack.PushMatrix();
	modelStack.Translate(0, -3.f, 75);
	modelStack.Scale(300, 120, 300);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL20], true);
	modelStack.PopMatrix();

	//Cave Decor
	//Columns
	modelStack.PushMatrix();
	modelStack.Translate(60, -6.f, 90);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL18], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(60, -6.f, 45);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL19], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(60, -6.f, 0);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL18], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -6.f, 90);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL18], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -6.f, 45);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL19], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, -6.f, 0);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL18], true);
	modelStack.PopMatrix();

	//Rocks in water
	modelStack.PushMatrix();
	modelStack.Translate(-135, -9.f, 45);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL23], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(135, -9.f, 135);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL23], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(135, -9.f, -45);
	modelStack.Scale(60, 60, 60);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL23], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -9.f, -90);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL24], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-90, -9.f, 135);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL24], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -9.f, 135);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL24], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, -9.f, -90);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL24], true);
	modelStack.PopMatrix();

	//Lilies
	modelStack.PushMatrix();
	modelStack.Translate(-90, -9.f, -80);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-135, -9.f, -10);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-115, -9.f, -25);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(90, -9.f, -80);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -9.f, 0);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-115, -9.f, 120);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(105, -9.f, 70);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -9.f, -50);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(135, -9.f, -30);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(lilyRotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();

	//Sunken objects
	modelStack.PushMatrix();
	modelStack.Translate(-120, -10.f, 180);
	modelStack.Scale(30, 30, 30);
	modelStack.Rotate(70, 1, 0, -1);
	RenderMesh(meshList[GEO_MODEL25], true);
	modelStack.PopMatrix();

	//Trees in water
	modelStack.PushMatrix();
	modelStack.Translate(-150, -10.f, 180);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(-130, 0, 1, 0);
	modelStack.Rotate(20, 0, 0, 1);
	RenderMesh(meshList[GEO_MODEL2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(140, -10.f, 190);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(-30, 0, 1, 0);
	modelStack.Rotate(20, 0, 0, 1);
	RenderMesh(meshList[GEO_MODEL2], true);
	modelStack.PopMatrix();

	//THE Master Sword
	if (!masterSwordGet)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, masterSwordY, 45);
		modelStack.Scale(0.2f, 0.2f, 0.2f);
		modelStack.Rotate(0, 0, 1, 0);
		RenderMesh(meshList[GEO_MODEL26], true);
		modelStack.PopMatrix();
	}

	//Sword Altar
	modelStack.PushMatrix();
	modelStack.Translate(0, -9.f, 45);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(0, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL28], true);
	modelStack.PopMatrix();

	//Dark Link
	if (masterSwordGet && !darkLinkDefeated)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -3, 120);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_MODEL27], true);
		modelStack.PopMatrix();
	}

	//Instructions Sign
	modelStack.PushMatrix();
	modelStack.Translate(-135, -9.f, -190);
	modelStack.Scale(50, 50, 50);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MODEL29], true);
	modelStack.PopMatrix();


	//Link Model
	{
		//Body(Cylinder)
		modelStack.PushMatrix();
		modelStack.Scale(2.0f, 2.0f, 2.0f);
		modelStack.Translate(translateX, 0, translateZ);
		if (anim1Playing == true)
		{
			modelStack.Translate(0, translateY, 0);
			//modelStack.Translate(translateZ, 0, translateX);
			modelStack.Rotate(bodyRotateAngle, 0, 1, 0);
		}
		else if (anim2Playing)
		{
			modelStack.Rotate(bodyRotateAngle2, 0, 1, 0);
			modelStack.Rotate(bodyRotateAngle3, 1, 0, 0);
		}
		/*
		else if (anim3Playing)
		{
			modelStack.Translate(0, 0, translateX);
			modelStack.Rotate(0, 0, 1, 0);
		}
		*/
		else
		{
			modelStack.Translate(0, 0, 0);
			modelStack.Rotate(bodyRotateAngle, 0, 1, 0);
		}
		RenderMesh(meshList[GEO_CYLINDER], true);

		//Top of body (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Translate(0, -1, 0);
		RenderMesh(meshList[GEO_SPHERE2], true);
		modelStack.PopMatrix();

		//Head (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(1.8f, 1.8f, 1.8f);
		if (anim1Playing)
		{
			modelStack.Rotate(0, 0, 1, 0);
		}
		else if (anim2Playing)
		{
			modelStack.Rotate(headRotateAngle, 0, 1, 0);
		}
		else if (anim3Playing)
		{
			modelStack.Rotate(headRotateAngle, 1, 0, 0);
		}
		else
			modelStack.Rotate(headRotateAngle, 0, 1, 0);
		modelStack.Translate(0, 2.2f, 0);
		RenderMesh(meshList[GEO_SPHERE3], true);
		//modelStack.PopMatrix();

		//Nose (Cone)
		modelStack.PushMatrix();
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		modelStack.Translate(0, -2, 10.5f);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_CONE], true);
		modelStack.PopMatrix();

		//Left eye (Sphere)
		modelStack.PushMatrix();
		modelStack.Translate(0.3f, 0.2f, 0.9f);
		modelStack.Rotate(0, 1, 0, 0);
		modelStack.Scale(0.1f, eyeScale, 0.1f);
		RenderMesh(meshList[GEO_EYES], true);
		modelStack.PopMatrix();

		//Right eye (Sphere)
		modelStack.PushMatrix();
		modelStack.Translate(-0.3f, 0.2f, 0.9f);
		modelStack.Rotate(0, 1, 0, 0);
		modelStack.Scale(0.1f, eyeScale, 0.1f);
		RenderMesh(meshList[GEO_EYES], true);
		modelStack.PopMatrix();

		//Hat base (Cylinder)
		modelStack.PushMatrix();
		modelStack.Scale(0.7f, 0.7f, 0.7f);
		modelStack.Translate(0, 0.4f, -0.5f);
		modelStack.Rotate(-70, 1, 0, 0);
		RenderMesh(meshList[GEO_CYLINDER5], true);

		//Hat extension
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Translate(0, 0.65f, 0);
		modelStack.Rotate(0, 1, 0, 0);
		RenderMesh(meshList[GEO_CYLINDER6], true);

		//Hat tip
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Translate(0, 1.5, 0);
		modelStack.Rotate(0, 1, 0, 0);
		RenderMesh(meshList[GEO_CONE2], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		//Hair
		//Top Hair
		modelStack.PushMatrix();
		modelStack.Translate(-0.5f, 0.75f, 0.4f);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(30, 1, 0, 0);
		modelStack.Scale(0.6f, 0.3f, 0.9f);
		RenderMesh(meshList[GEO_HAIR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.6f, 0.8f, 0.4f);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(-30, 1, 0, 0);
		modelStack.Scale(0.5f, 0.25f, 0.7f);
		RenderMesh(meshList[GEO_HAIR], true);
		modelStack.PopMatrix();

		//Sideburn
		modelStack.PushMatrix();
		modelStack.Scale(0.5f, 0.7f, 0.2f);
		modelStack.Translate(-1.35f, 0.1f, 0.0f);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(75, 0, 0, -1);
		RenderMesh(meshList[GEO_SIDEBURNS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.5f, 0.7f, 0.2f);
		modelStack.Translate(1.35f, 0.1f, 0.0f);
		modelStack.Rotate(-180, 1, 0, 0);
		modelStack.Rotate(-75, 0, 0, -1);
		RenderMesh(meshList[GEO_SIDEBURNS], true);
		modelStack.PopMatrix();

		//Ears
		modelStack.PushMatrix();
		modelStack.Translate(1.0f, 0.0f, -0.2f);
		modelStack.Rotate(45, 0, -1, 0);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(0.25f, 0.25f, 0.5f);
		RenderMesh(meshList[GEO_EARS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1.0f, 0.0f, -0.2f);
		modelStack.Rotate(-45, 0, -1, 0);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Scale(0.25f, 0.25f, 0.5f);
		RenderMesh(meshList[GEO_EARS], true);
		modelStack.PopMatrix();

		//Eyebrows
		modelStack.PushMatrix();
		modelStack.Scale(0.2f, 0.1f, 0.1f);
		modelStack.Translate(2, 5, 8.5f);
		modelStack.Rotate(-180, 1, 0, 0);
		modelStack.Rotate(-30, 0, 0, 1);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.2f, 0.1f, 0.1f);
		modelStack.Translate(-2, 5, 8.5f);
		modelStack.Rotate(-180, 1, 0, 0);
		modelStack.Rotate(30, 0, 0, 1);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();

		//Mouth
		modelStack.PushMatrix();
		modelStack.Scale(0.2f, 0.05f, 0.1f);
		modelStack.Translate(0, -7.6f, 9.2f);
		modelStack.Rotate(-245, 1, 0, 0);
		RenderMesh(meshList[GEO_RING], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		//Belt (Cylinder)
		modelStack.PushMatrix();
		modelStack.Scale(1, 0.8f, 1);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Translate(0, -0.2f, 0);
		RenderMesh(meshList[GEO_CYLINDER2], true);
		modelStack.PopMatrix();

		//Belt Buckle (Cube)
		modelStack.PushMatrix();
		modelStack.Scale(0.8f, 0.5f, 0.2f);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Translate(0, -0.3f, 7.1f);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();

		//Left leg joint
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		if (anim1Playing)
		{
			modelStack.Translate(0.75f, -0.625f, 0);
			modelStack.Rotate(rotateAngle, 1, 0, 0);
		}
		else if (anim2Playing)
		{
			modelStack.Translate(0.75f, -0.625f, -0.5f);
			modelStack.Rotate(30, 1, 0, 0);
		}
		else
		{
			modelStack.Translate(0.75f, -0.625f, 0);
			modelStack.Rotate(0, 1, 0, 0);
		}
		RenderMesh(meshList[GEO_LEGJOINT], true);

		//Left Leg
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(0, -0.625f, 0);
		RenderMesh(meshList[GEO_CYLINDER4], true);

		//Left Boot
		modelStack.PushMatrix();
		modelStack.Scale(0.55f, 2, 0.55f);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(0, -0.4f, 0);
		RenderMesh(meshList[GEO_CYLINDER3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.55f, 0.55f, 0.95f);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Translate(0, 2.39f, -0.5f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		//Right leg joint
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Translate(-0.75f, -0.625f, 0);
		if (anim1Playing)
			modelStack.Rotate(-rotateAngle, 1, 0, 0);
		else if (anim2Playing)
		{
			modelStack.Rotate(10, -1, 0, 0);
		}
		else
			modelStack.Rotate(0, 0, 1, 0);
		RenderMesh(meshList[GEO_LEGJOINT], true);

		//Right leg
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(0, -0.625f, 0);
		RenderMesh(meshList[GEO_CYLINDER4], true);

		//Right Boot
		modelStack.PushMatrix();
		modelStack.Scale(0.55f, 2, 0.55f);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(0, -0.4f, 0);
		RenderMesh(meshList[GEO_CYLINDER3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.55f, 0.55f, 0.95f);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Translate(0, 2.39f, -0.5f);
		RenderMesh(meshList[GEO_HEMISPHERE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		//Left arm joint (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(2.5, 3.25, 0);
		if (anim2Playing)
		{
			modelStack.Rotate(armRotateAngle / 2, 0, 0, 1);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(-armRotateAngle2 / 0.9, 1, 0, 0);
			modelStack.Rotate(armRotateAngle2 / 2, 0, 1, 0);
		}
		else if (anim3Playing)
		{
			modelStack.Rotate(-45, 0, 0, 1);
			modelStack.Rotate(armRotateAngle, 0, 0, 1);
			modelStack.Rotate(-armRotateAngle2, 0, 1, 0);
		}
		else
			modelStack.Rotate(armIdleRotateAngle, 0, 0, 1);
		RenderMesh(meshList[GEO_SPHERE], true);

		//Left arm (Cylinder)
		modelStack.PushMatrix();
		modelStack.Scale(6, 1, 1);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Translate(0, -0.25, 0);
		RenderMesh(meshList[GEO_CYLINDER3], true);
		modelStack.PopMatrix();

		//Left hand (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(3.5f, 0, 0);
		RenderMesh(meshList[GEO_SPHERE3], true);
		modelStack.PopMatrix();
		if (anim3Playing)
			modelStack.PopMatrix();

		if (!masterSwordGet)
		{
			//Sword hilt
			modelStack.PushMatrix();
			if (anim3Playing)
			{
				modelStack.Translate(1.0f, 1.5f, -1.5f);
				modelStack.Scale(0.5f, 0.5f, 0.5f);
				modelStack.Rotate(135, 0, 0, 1);
				modelStack.Rotate(90, 0, 1, 0);
			}
			else
			{
				modelStack.Scale(1, 1, 1);
				modelStack.Translate(3.5f, 0, 0);
				modelStack.Rotate(90, 1, 0, 0);
				modelStack.Rotate(45, 0, 1, 0);
			}
			RenderMesh(meshList[GEO_SWORDHILT], true);

			//Sword pommel
			modelStack.PushMatrix();
			modelStack.Scale(1, 1, 1);
			modelStack.Translate(0, -2, 0);
			modelStack.Rotate(180, 1, 0, 0);
			RenderMesh(meshList[GEO_POMMEL], true);
			modelStack.PopMatrix();

			//Sword Gem
			modelStack.PushMatrix();
			modelStack.Scale(0.75f, 0.75f, 0.75f);
			modelStack.Translate(0, -2.5f, 0);
			modelStack.Rotate(0, 1, 0, 0);
			RenderMesh(meshList[GEO_SWORDGEM], true);
			modelStack.PopMatrix();

			//Sword Base
			modelStack.PushMatrix();
			modelStack.Scale(1.25f, 0.75f, 3);
			modelStack.Translate(0, 2, 0);
			modelStack.Rotate(0, 1, 0, 0);
			RenderMesh(meshList[GEO_SWORDBASE], true);
			modelStack.PopMatrix();

			//Sword Blade
			modelStack.PushMatrix();
			modelStack.Scale(0.75f, 1, 1.75f);
			modelStack.Translate(0, 4.5f, 0);
			modelStack.Rotate(0, 1, 0, 0);
			RenderMesh(meshList[GEO_SWORDBLADE], true);
			modelStack.PopMatrix();

			//Sword tip
			modelStack.PushMatrix();
			modelStack.Scale(0.75f, 1, 1.75f);
			modelStack.Translate(0, 8.25f, 0);
			modelStack.Rotate(0, 1, 0, 0);
			RenderMesh(meshList[GEO_SWORDTIP], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			if (!anim3Playing)
				modelStack.PopMatrix();
		}
		if(masterSwordGet)
		{
			//Sword hilt
			modelStack.PushMatrix();
			modelStack.Scale(0.2f, 0.2f, 0.2f);
			if (anim2Playing)
			{
				if (swordSequence1 == false)
				{
					modelStack.Translate(17, 0, 73);
					modelStack.Rotate(-90, 1, 0, 0);
				}
				else
				{
					modelStack.Translate(17, 0, -73);
					modelStack.Rotate(-90, 1, 0, 0);
					modelStack.Rotate(180, 0, 0, 1);
				}
			}
			else
			{
				modelStack.Translate(17, 0, 73);
				modelStack.Rotate(-90, 1, 0, 0);
			}
			RenderMesh(meshList[GEO_MODEL26], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}

		//Right arm joint (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(-2.5, 3.25, 0);
		if (anim1Playing)
		{
			modelStack.Rotate(45, 0, 0, 1);
			modelStack.Rotate(-rotateAngle, 0, 1, 0);
		}
		else if (anim2Playing)
		{
			modelStack.Rotate(45, 0, 0, 1);
			modelStack.Rotate(-20, 1, 0, 0);
		}
		else if (anim3Playing)
		{
			modelStack.Rotate(45, 0, 0, 1);
			modelStack.Rotate(-armRotateAngle, 0, 0, 1);
			modelStack.Rotate(armRotateAngle2, 0, 1, 0);
		}
		else
			modelStack.Rotate(-armIdleRotateAngle, 0, 0, 1);
		RenderMesh(meshList[GEO_SPHERE], true);

		//Right arm (Cylinder)
		modelStack.PushMatrix();
		modelStack.Scale(6, 1, 1);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Translate(0, 0.25, 0);
		RenderMesh(meshList[GEO_CYLINDER3], true);
		modelStack.PopMatrix();

		//Right hand (Sphere)
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Translate(-3.5f, 0, 0);
		RenderMesh(meshList[GEO_SPHERE3], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	//Items
	if (!setBombInActive)
	{
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Translate(bombX, bombY, bombZ);
		modelStack.Rotate(0, 1, 0, 0);
		if (throwSequence2 == false)
			RenderMesh(meshList[GEO_BOMBCASE], true);

		modelStack.PushMatrix();
		modelStack.Scale(3.5f, 0.2f, 3.5f);
		modelStack.Translate(0, 15.0f, 0);
		modelStack.Rotate(0, 1, 0, 0);
		if (throwSequence2 == false)
			RenderMesh(meshList[GEO_SWORDBLADE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.7f, 3, 0.7f);
		modelStack.Translate(0, 1.25f, 0);
		modelStack.Rotate(0, 0, 0, 1);
		if (throwSequence2 == false)
			RenderMesh(meshList[GEO_BOMBFUSE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	if (anim3Playing)
	{
		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		modelStack.Translate(bombX, bombY, bombZ);
		modelStack.Rotate(0, 1, 0, 0);
		if(throwSequence2 == false)
			RenderMesh(meshList[GEO_BOMBCASE], true);

		modelStack.PushMatrix();
		modelStack.Scale(3.5f, 0.2f, 3.5f);
		modelStack.Translate(0, 15.0f, 0);
		modelStack.Rotate(0, 1, 0, 0);
		if (throwSequence2 == false)
			RenderMesh(meshList[GEO_SWORDBLADE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.7f, 3, 0.7f);
		modelStack.Translate(0, 1.25f, 0);
		modelStack.Rotate(0, 0, 0, 1);
		if (throwSequence2 == false)
			RenderMesh(meshList[GEO_BOMBFUSE], true);
		modelStack.PopMatrix();

		if (throwSequence2 == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, -4, 0);
			modelStack.Scale(scaleAll, scaleAll, scaleAll);
			modelStack.Rotate(180, 1, 0, 0);
			if (throwSequence3 == false)
				RenderMesh(meshList[GEO_EXPLOSION], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}


	if(goalNumber == 0)
		RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Find some bombs", Color(0, 1, 0), 3, 0, 55);
	if (goalNumber == 1)
	{
		if(!caveEntranceBroken)
			RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Give bombs to Link.", Color(0, 1, 0), 3, 0, 55);
		if (caveEntranceBroken)
			RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Follow Link.", Color(0, 1, 0), 3, 0, 55);
	}
	if (goalNumber == 2 && dialogueNumber == 4)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Move Link to the Sword", Color(0, 1, 0), 2, 0, 55);
		RenderTextOnScreen(meshList[GEO_TEXT], "Hold T to move Link", Color(0, 1, 0), 2, 0, 50);
	}
	if (goalNumber == 3)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Watch Link.", Color(0, 1, 0), 3, 0, 55);
		if (dialogueNumber < 5)
		{
			RenderMeshOnScreen(meshList[GEO_QUAD], 40, 15, -80, 60);
			RenderTextOnScreen(meshList[GEO_TEXT], "Watch out Link!", Color(1, 1, 0), 3, 10, 16);
		}
	}
	if (goalNumber == 4)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Goal: Talk to Link.", Color(0, 1, 0), 3, 0, 55);
	}
	if (goalNumber == 5)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Thank you for playing", Color(0, 1, 0), 3, 10, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "The End!", Color(0, 1, 0), 3, 25, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press ESC to Exit", Color(0, 1, 0), 3, 15, 20);
	}

	if (reachedDekuTree && dialogueNumber < 4)
	{
		RenderMeshOnScreen(meshList[GEO_QUAD], 40, 15, -80, 60);
		RenderTextOnScreen(meshList[GEO_TEXT], "Click to continue", Color(1, 1, 0), 2, 10, 3);
		if (dialogueNumber == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Link...listen very carefully", Color(1, 1, 0), 2, 10, 16);
			RenderTextOnScreen(meshList[GEO_TEXT], "to what I'm about to say.", Color(1, 1, 0), 2, 10, 13);
		}
		else if(dialogueNumber == 1)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "An ancient evil has awoken", Color(1, 1, 0), 2, 10, 16);
			RenderTextOnScreen(meshList[GEO_TEXT], "and threatens to plunge Hyrule", Color(1, 1, 0), 2, 10, 13);
			RenderTextOnScreen(meshList[GEO_TEXT], "into darkness..", Color(1, 1, 0), 2, 10, 10);
		}
		else if (dialogueNumber == 2)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "The sword you see before you", Color(1, 1, 0), 2, 10, 16);
			RenderTextOnScreen(meshList[GEO_TEXT], "is the Blade of Evil's Bane..", Color(1, 1, 0), 2, 10, 13);
			RenderTextOnScreen(meshList[GEO_TEXT], "The Master Sword.", Color(1, 1, 0), 2, 10, 10);
		}
		else if (dialogueNumber == 3)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You have been chosen as the", Color(1, 1, 0), 2, 10, 16);
			RenderTextOnScreen(meshList[GEO_TEXT], "Hero of this Era..\n", Color(1, 1, 0), 2, 10, 13);
			RenderTextOnScreen(meshList[GEO_TEXT], "Take the blade with you,", Color(1, 1, 0), 2, 10, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], "and save us.", Color(1, 1, 0), 2, 10, 7);
		}

	}
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(-125.f, 12, -195);
	modelStack.Rotate(180, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "Press E on me to bring up", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-127.5f, 8, -195);
	modelStack.Rotate(180, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "the instructions", Color(0, 1, 0));
	modelStack.PopMatrix();

	if (instructionTextBoxOn)
	{
		RenderMeshOnScreen(meshList[GEO_QUAD], 40, 30, -80, 140);
		RenderTextOnScreen(meshList[GEO_TEXT], "Instructions", Color(1, 1, 0), 2.f, 28, 37);
		RenderTextOnScreen(meshList[GEO_TEXT], "Help Link get the Master Sword", Color(1, 1, 0), 2.f, 10, 32);
		RenderTextOnScreen(meshList[GEO_TEXT], "Controls:", Color(1, 1, 0), 2.f, 30, 27);
		RenderTextOnScreen(meshList[GEO_TEXT], "WASD for Movement", Color(1, 1, 0), 2.f, 22, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], "Arrow Keys to view", Color(1, 1, 0), 2.f, 22, 17);
		RenderTextOnScreen(meshList[GEO_TEXT], "E to Interact", Color(1, 1, 0), 2.f, 26, 12);
		RenderTextOnScreen(meshList[GEO_TEXT], "Click to continue", Color(1, 1, 0), 2.f, 10, 5);
	}

	//RenderTextOnScreen(meshList[GEO_TEXT1], "X:" + std::to_string(camera.position.x) + " Z:" + std::to_string(camera.position.z), Color(0, 1, 0), 2, 0, 4);


	if(gotBombs && !caveEntranceBroken && textTimer > 0)
		RenderTextOnScreen(meshList[GEO_TEXT], "You got bombs!", Color(0, 1, 0), 4, 0, 0);
	else if(gotBombs && !caveEntranceBroken && camera.position.x > -25 && camera.position.x < 25 && camera.position.z > -230 && camera.position.z < -160)
		RenderTextOnScreen(meshList[GEO_TEXT], "Talk?", Color(0, 1, 0), 4, 0, 0);

	if (!chest1Opened && camera.position.x > 210 && camera.position.x < 240 && camera.position.z > -230 && camera.position.z < -210)
	{
		modelStack.PushMatrix();
		modelStack.Translate(240, 12, -225);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(2, 2, 2);
		RenderText(meshList[GEO_TEXT], "Take bombs?", Color(0, 1, 0));
		modelStack.PopMatrix();
	}

	if (darkLinkDefeated && camera.position.x > -15 && camera.position.x < 15 && camera.position.z > 90 && camera.position.z < 135)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Talk?", Color(0, 1, 0), 4, 0, 0);
	}

	if(Application::IsKeyPressed('E'))
		RenderTextOnScreen(meshList[GEO_TEXT], "", Color(0, 1, 0), 4, 0, 0);
	else
		RenderTextOnScreen(meshList[GEO_TEXT], "", Color(0, 1, 0), 4, 0, 0);
}

void Assignment2::Exit()
{
	// Cleanup VBO here
	//glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	//glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Assignment2::RenderMesh(Mesh* mesh, bool enableLight)
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

void Assignment2::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0.f, 0.5);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(-0.5, 0.f, 0);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0.5, 0.f, 0);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0.5f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(0, 0, 0, 1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, -0.5f, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(360, 0, 0, 1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0.f, -0.5);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void Assignment2::RenderText(Mesh* mesh, std::string text, Color color)
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

void Assignment2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void Assignment2::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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
