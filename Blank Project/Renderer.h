#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SecondRobot.h"
class Camera;
class Shader;
class HeightMap;
class SceneNode;
class Mesh;
class MeshAnimation;
class MeshMaterial;





class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawShadowScene();
	void DrawMainScene();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	HeightMap* heightMap;
	Mesh* quad;
	Light* light;
	Camera* camera;

	GLuint cubeMap;
	GLuint waterTex;
	GLuint earthTex;
	GLuint earthBump;

	float waterRotate;
	float waterCycle;


	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	Mesh* eyesQuad;
	Mesh* beginQuad;

	SceneNode* root;
	SceneNode* extraRobotRoot;
	Shader* eyesShader;
	GLuint texture;
	GLuint platformtexture;
	Frustum frameFrustum;

	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;

	Mesh* cube;
	Shader* Cubeshader;

	bool b = true;  
	float currentTime = 0.0f;


	Mesh* mesh;
	Shader* Robotshader;
	MeshAnimation* anim;
	MeshMaterial* material;
	vector <GLuint> matTextures;

	int currentFrame;
	float frameTime;

	void theSoldier();
	float soldierMoveFlap = 0.0f;



	Light* followlight;

	Vector3 manPosition;
	bool onPerson;


	
	SceneNode* worldroot;

	void CameraMove();
	float cammove = 0.0f;
	bool ifCameraMove = true;

};
