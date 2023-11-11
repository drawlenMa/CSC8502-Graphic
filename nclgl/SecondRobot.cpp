#include "SecondRobot.h"

SecondRobot::SecondRobot(Mesh* cube,Vector3 pos,int scale) {

	SetModelScale(Vector3{ 2,1,1 });
	//SetMesh(cube);
	SetTransform(Matrix4::Translation(pos));
	//SetMesh(cube); // Uncomment if you want a local origin marker!

	SceneNode* body = new SceneNode(cube, Vector4(255, 203, 0, 1)); //Red!
	body->SetModelScale(Vector3(10, 18, 5)*scale );
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);

	head = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Green!
	head->SetModelScale(Vector3(10, 5, 5) * scale);
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	leftArm->SetModelScale(Vector3(3, -18, 3) * scale);
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	rightArm->SetModelScale(Vector3(3, -18, 3) * scale);
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, 5)));
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	leftLeg->SetModelScale(Vector3(3, -17.5, 3) * scale);
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); //Blue!
	rightLeg->SetModelScale(Vector3(3, -17.5, 3) * scale);
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightLeg);

	


	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);

}



void SecondRobot::Update(float dt) {
	transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));

	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));

	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)));
	

	SceneNode::Update(dt);

}

