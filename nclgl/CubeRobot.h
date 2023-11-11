#pragma once

#include "..\nclgl\scenenode.h"

 class CubeRobot : public SceneNode {
 public:
	 CubeRobot(Mesh * cube,Vector3 pos,float sca);
	 ~CubeRobot(void) {};
	 void Update(float dt) override;
	
 protected:
	 SceneNode * head;
	 SceneNode * leftArm;
	 SceneNode * rightArm;
	 SceneNode* bullet;
	 

};

