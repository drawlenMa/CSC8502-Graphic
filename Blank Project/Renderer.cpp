#include "Renderer.h"
#include "../nclgl/Light.h"
#include "../nclgl/Heightmap.h"
#include "../nclgl/Shader.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/SecondRobot.h"

#include <algorithm> 

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{

	quad = Mesh::GenerateQuad();
	heightMap = new HeightMap(TEXTUREDIR"smile1.png");
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthBump = SOIL_load_OGL_texture(TEXTUREDIR"glass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"rusted_west.jpg", //west
		TEXTUREDIR"rusted_east.jpg",//east
		TEXTUREDIR"rusted_up.jpg", //up
		TEXTUREDIR"rusted_down.jpg",//down
		TEXTUREDIR"rusted_south.jpg", //south
		TEXTUREDIR"rusted_north.jpg",//north
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);


	if (!earthTex || !earthBump || !cubeMap || !waterTex)
	{
		return;
	}

	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);
	SetTextureRepeating(waterTex, true);
	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new Shader("bumpvertex.glsl", "bumpfragment.glsl");


	if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !lightShader->LoadSuccess())
	{
		return;
	}
	Vector3 heightmapSize = heightMap->GetHeightmapSize();

	camera = new Camera(0.0f, 0.0f, heightmapSize * Vector3(0.5f, 18.0f, 2.0f));

	light = new Light(heightmapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 0, 0, 1), heightmapSize.x);
	followlight = new Light(heightmapSize * Vector3(0.8f, 1.5f, 0.5f), Vector4(0.5, 0.9, 0.2, 1), heightmapSize.x * 0.3f);



	eyesQuad = Mesh::GenerateQuad();
	beginQuad = Mesh::GenerateQuad();

	eyesShader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
	texture = SOIL_load_OGL_texture(TEXTUREDIR"particle.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	platformtexture = SOIL_load_OGL_texture(TEXTUREDIR"floor.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (!eyesShader->LoadSuccess() || !texture)
	{
		return;
	}
	root = new SceneNode();
	extraRobotRoot = new SceneNode();

	for (int i = 0; i < 5; ++i)//left eye
	{
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		s->SetTransform(Matrix4::Translation(Vector3(10000.0f, 5000.0f, -300.0f + 100.0f + 100 * i)));
		s->SetModelScale(Vector3(6000.0f, 6000.0f, 6000.0f));
		s->SetBoundingRadius(10000.0f);
		s->SetMesh(eyesQuad);
		s->SetTexture(texture);
		root->AddChild(s);
	}
	for (int i = 0; i < 5; ++i)//right eye
	{
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		s->SetTransform(Matrix4::Translation(Vector3(24000.0f, 5000.0f, -300.0f + 100.0f + 100 * i)));
		s->SetModelScale(Vector3(6000.0f, 6000.0f, 6000.0f));
		s->SetBoundingRadius(10000.0f);
		s->SetMesh(eyesQuad);
		s->SetTexture(texture);
		root->AddChild(s);
	}

	for (int i = 0; i < 7; ++i)
	{
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 0.0f, 0.8f));
		s->SetTransform(Matrix4::Translation(Vector3(24000.0f, 5000.0f, 25000.0f + 100.0f + 100 * i)));
		s->SetModelScale(Vector3(100000.0f, 10000.0f, 10000.0f));
		s->SetBoundingRadius(10000.0f);
		s->SetMesh(eyesQuad);
		s->SetTexture(texture);
		root->AddChild(s);
	}
	worldroot = new SceneNode();
	for (int i = 0; i < 2; ++i)
	{
		SceneNode* floor = new SceneNode();
		floor->SetColour(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
		floor->SetTransform(Matrix4::Translation(Vector3(24000.0f, 2500.0f, 38000.0f + 100.0f + 100 * i)) * Matrix4::Rotation(90, Vector3(1, 0, 0)));
		floor->SetModelScale(Vector3(100000.0f, 10000.0f, 10000.0f));
		floor->SetBoundingRadius(10000.0f);
		floor->SetMesh(beginQuad);
		floor->SetTexture(platformtexture);
		worldroot->AddChild(floor);
	}

	cube = Mesh::LoadFromMeshFile("OffsetCubeZ.msh");
	Cubeshader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");

	if (!Cubeshader->LoadSuccess()) {
		return;
	}
	root->AddChild(new CubeRobot(cube, heightmapSize * Vector3(0.49f, 1.0f, 0.5f), 20));
	extraRobotRoot->AddChild(new SecondRobot(cube, heightmapSize * Vector3(0.35f, 1.0f, 0.8f),30));

	Robotshader = new Shader("SkinningVertex.glsl", "texturedFragment.glsl");
	if (!Robotshader->LoadSuccess())
	{
		return;
	}
	mesh = Mesh::LoadFromMeshFile("Role_T.msh");
	anim = new MeshAnimation("Role_T.anm");
	material = new MeshMaterial("Role_T.mat");
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
	{
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);

	}

	projMatrix = Matrix4::Perspective(1.0f, 30000.0f, (float)width / (float)height, 450.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	currentFrame = 0;
	frameTime = 0.0f;
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;

}
Renderer ::~Renderer(void)
{
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;

	delete eyesQuad;
	delete eyesShader;
	glDeleteTextures(1, &texture);

	delete root;
	delete Cubeshader;
	delete cube;

	delete mesh;
	delete anim;
	delete material;
	delete Robotshader;

	delete followlight;
	delete beginQuad;
	delete worldroot;
	delete extraRobotRoot;


}

void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += dt * 3.0f; 
	waterCycle += dt * 0.25f; 

	float timeToWink = 3.0f;
	currentTime += dt;
	if (b) {
		if (currentTime > timeToWink) {
			b = false;
			currentTime = 0.0f;
			for (int i = 0; i < 10; i++) {
				root->GiveMeTheLovelyChild(i)->SetModelScale(Vector3(6000.0f, 500.0f, 6000.0f));
				root->GiveMeTheLovelyChild(i)->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			light->SetColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	else {
		if (currentTime > 2.0f) {
			b = true;
			currentTime = 0.0f;
			for (int i = 0; i < 10; i++) {
				root->GiveMeTheLovelyChild(i)->SetModelScale(Vector3(6000.0f, 6000.0f, 6000.0f));
				root->GiveMeTheLovelyChild(i)->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			light->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		}
	}

	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(dt);
	worldroot->Update(dt);
	extraRobotRoot->Update(dt);

	frameTime -= dt;
	while (frameTime < 0.0f)
	{
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();

	}

	CameraMove();

}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightmap();
	DrawWater();


	BuildNodeLists(root);
	BuildNodeLists(worldroot);
	BuildNodeLists(extraRobotRoot);
	SortNodeLists();
	BindShader(eyesShader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(eyesShader->GetProgram(), "diffuseTex"), 0);
	DrawNodes();
	ClearNodeLists();

	BindShader(Cubeshader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(eyesShader->GetProgram(), "diffuseTex"), 1);
	DrawNode(root);
	DrawNode(worldroot);
	DrawNode(extraRobotRoot);


	theSoldier();



}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	BindShader(skyboxShader);
	UpdateShaderMatrices();
	quad->Draw();
	glDepthMask(GL_TRUE);

}

void Renderer::DrawHeightmap()
{
	BindShader(lightShader);


	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);

	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightPos"), 1, (float*)&(*light).GetPosition());

	glUniform4fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightColour"), 1, (float*)&(*light).GetColour());

	glUniform1f(glGetUniformLocation(lightShader->GetProgram(),
		"lightRadius"), (*light).GetRadius());

	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightPos1"), 1, (float*)&(*followlight).GetPosition());

	glUniform4fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightColour1"), 1, (float*)&(*followlight).GetColour());

	glUniform1f(glGetUniformLocation(lightShader->GetProgram(),
		"lightRadius1"), (*followlight).GetRadius());

	modelMatrix.ToIdentity(); 
	textureMatrix.ToIdentity(); 

	UpdateShaderMatrices();



	heightMap->Draw();

}

void Renderer::DrawWater()
{
	BindShader(reflectShader);

	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	Vector3 hSize = heightMap->GetHeightmapSize();

	modelMatrix = Matrix4::Translation(hSize * 0.5f) * Matrix4::Scale(hSize * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));

	textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	
	quad->Draw();

}


void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
		{
			transparentNodeList.push_back(from);

		}
		else
		{
			nodeList.push_back(from);

		}

	}

	for (vector <SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists((*i));

	}
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.rbegin(), 
		transparentNodeList.rend(), 
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);

}


void Renderer::DrawNodes()
{
	for (const auto& i : nodeList) {
		DrawNode(i);

	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);

	}
}

void Renderer::DrawNode(SceneNode* n)
{
	if (n->GetMesh())
	{
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(eyesShader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(eyesShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		texture = n->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(eyesShader->GetProgram(), "useTexture"), texture);
		n->Draw(*this);

	}

}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}



void Renderer::theSoldier()
{
	BindShader(Robotshader);
	glUniform1i(glGetUniformLocation(Robotshader->GetProgram(), "diffuseTex"), 0);
	Vector3 heightmapSize = heightMap->GetHeightmapSize();
	int i = 0;

	float soldierMove = soldierMoveFlap * 0.05f;

	if (soldierMove < 0.8f)
	{
		modelMatrix = Matrix4::Translation(heightmapSize * Vector3(0.1f + soldierMove, 1.0f, 0.1f)) * Matrix4::Scale(Vector3(1500.0f, 1500.0f, 1500.0f)) * Matrix4::Rotation(90, Vector3(0.0f, 0.5f, 0.0f));
		soldierMoveFlap += 0.01f;
		followlight->SetPosition(heightmapSize * Vector3(0.1f + soldierMove, 1.0f, 0.1f));
		manPosition = Matrix4::Translation(heightmapSize * Vector3(0.1f + soldierMove, 1.0f, 0.1f)).GetPositionVector();
	}
	else if (soldierMove > 0.8f && soldierMove < 1.6f)
	{
		modelMatrix = Matrix4::Translation(heightmapSize * Vector3(1.7 - soldierMove, 1.0f, 0.1f)) * Matrix4::Scale(Vector3(1500.0f, 1500.0f, 1500.0f)) * Matrix4::Rotation(-90, Vector3(0.0f, 0.5f, 0.0f));
		soldierMoveFlap += 0.01f;
		followlight->SetPosition(heightmapSize * Vector3(1.7 - soldierMove, 1.0f, 0.1f));
		manPosition = Matrix4::Translation(heightmapSize * Vector3(1.7 - soldierMove, 1.0f, 0.1f)).GetPositionVector();
	}
	else
	{
		soldierMoveFlap = 0;
		soldierMove = 0;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_5))
	{
		onPerson = false;
	}

	if (!onPerson) {
		camera->SetPosition(manPosition + Vector3{ 0,5000,0 });
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_6))
	{
		onPerson = true;
		camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));
	}

	glUniformMatrix4fv(glGetUniformLocation(Robotshader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
	UpdateShaderMatrices();
	vector <Matrix4> frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);
	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i)
	{
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(Robotshader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		mesh->DrawSubMesh(i);

	}
}

void Renderer::CameraMove()
{
	Vector3 heightmapSize = heightMap->GetHeightmapSize();

	Vector3 pos1 = heightmapSize * Vector3(0.49f, 1.0f, 0.5f);
	float timee = 0.0f;
	timee = cammove * 0.05f;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_0))
	{
		ifCameraMove = true;

	}
	if (ifCameraMove)
	{
		if (timee < 1.3f)
		{
			camera->SetPosition(heightmapSize * Vector3(0.5f, 18.0f, 2.0f - timee));
			cammove += 0.03f;
		}
		if (timee > 1.3f && timee < 1.5f)
		{
			camera->SetPosition(heightmapSize * Vector3(0.5f, 18.0f, 0.7f));
			cammove += 0.1f;
		}
		if (timee > 1.5f && timee < 5.0f)
		{
			camera->SetPosition(heightmapSize * Vector3(0.5f, 18.0f - timee, 0.7f));

			cammove += 0.2f;
		}
		if (timee > 5.0f && timee < 6.0f)
		{
			camera->SetPosition(heightmapSize * Vector3(0.5f, 13.0f, 0.7f));

			cammove += 0.2f;
		}
		if (timee > 6.0f && timee < 8.0f)
		{

			camera->SetYaw(10.0 * timee);
			cammove += 0.08f;
		}
		if (timee > 8.0f && timee < 11.0f)
		{
			float temp2 = camera->GetYaw();
			camera->SetYaw(-80.0 * timee);
			cammove += 0.04f;
		}




	}

}







