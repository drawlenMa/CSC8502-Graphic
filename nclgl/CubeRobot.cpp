#include "CubeRobot.h"

 CubeRobot::CubeRobot(Mesh * cube,Vector3 pos,float sca) {

	 SetModelScale(Vector3{ 1,1,1 }*sca);
	 //SetMesh(cube);
	 SetTransform(Matrix4::Translation(pos));
	 //SetMesh(cube); // Uncomment if you want a local origin marker!
		
		 SceneNode * body = new SceneNode(cube, Vector4(255, 203, 0, 1)); //Red!
	 body->SetModelScale(Vector3(10, 15, 5)*sca);
	 body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	 AddChild(body);
	
		 head = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Green!
	 head->SetModelScale(Vector3(5, 5, 5)*sca);
	 head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	 body->AddChild(head);
	
		 leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	 leftArm->SetModelScale(Vector3(3, -18, 3) *sca);
	 leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	 body->AddChild(leftArm);
	
		 rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	 rightArm->SetModelScale(Vector3(3, -18, 3)*sca );
	 rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, 5)));
	 body->AddChild(rightArm);
	
		 SceneNode * leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	 leftLeg->SetModelScale(Vector3(3, -17.5, 3) *sca);
	 leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	 body->AddChild(leftLeg);
	
		 SceneNode * rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	 rightLeg->SetModelScale(Vector3(3, -17.5, 3)*sca);
	 rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	 body->AddChild(rightLeg);
	
	 bullet = new SceneNode(cube, Vector4(1, 1, 1, 1));
	 bullet->SetModelScale(Vector3(12, 1, 5));
	 bullet->SetTransform(Matrix4::Translation(Vector3(10, 20, 5)));
	 body->AddChild(bullet);
	 

	 body->SetBoundingRadius(15.0f);
	 head->SetBoundingRadius(5.0f);
	 
	 leftArm->SetBoundingRadius(18.0f);
	 rightArm->SetBoundingRadius(18.0f);
	 
	  leftLeg->SetBoundingRadius(18.0f);
	  rightLeg->SetBoundingRadius(18.0f);
	
}



 void CubeRobot::Update(float dt) {
	  transform = transform *Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));
	 
	  head->SetTransform(head->GetTransform() *Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	  leftArm->SetTransform(leftArm->GetTransform() *Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
		  
	  rightArm->SetTransform(rightArm->GetTransform() *Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)));
	  bullet->SetTransform(Matrix4::Translation(Vector3(5.0f * dt, 5.0f * dt, 5.0f * dt)));
		  
	  SceneNode::Update(dt);
		  
 }


