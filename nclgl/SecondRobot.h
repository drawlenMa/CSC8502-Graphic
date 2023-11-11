#pragma once

#include "..\nclgl\scenenode.h"

class SecondRobot : public SceneNode {
public:
	SecondRobot(Mesh* cube,Vector3 pos,int scale);
	~SecondRobot(void) {};
	void Update(float dt) override;

protected:
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
	


}; 
