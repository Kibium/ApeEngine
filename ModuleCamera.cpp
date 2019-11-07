#include "ModuleCamera.h"

ModuleCamera::ModuleCamera(){}
ModuleCamera::~ModuleCamera(){}

void ModuleCamera::LookAt() {

	f = float3(target - cameraPos);
	f.Normalize();
	s = float3(f.Cross(up));
	s.Normalize();
	u = float3(s.Cross(f));

	//View Matrix - Look at computation
	view[0][0] = s.x;
	view[0][1] = s.y;
	view[0][2] = s.z;

	view[1][0] = u.x;
	view[1][1] = u.y;
	view[1][2] = u.z;

	view[2][0] = -f.x;
	view[2][1] = -f.y;
	view[2][2] = -f.z;

	view[0][3] = -s.Dot(cameraPos);
	view[1][3] = -u.Dot(cameraPos);
	view[2][3] = f.Dot(cameraPos);

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[3][3] = 1.0f;
}

void ModuleCamera::ProcessMatrixs() {
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * fov) * DegToRad(aspect)); //aspect ratio 
	proj = frustum.ProjectionMatrix();

	LookAt();
	rotateMatrix = float3x3::RotateZ(rotX) * float3x3::RotateY(rotY) * float3x3::RotateX(rotZ);

	//Model Matrix

	//It modifies the original position, rottion and scale of the camera.
	model = float4x4::FromTRS(float3(0, 0, 0), rotateMatrix, float3(1, 1, 1));

	transform = proj * view* float4x4(model);
}

void ModuleCamera::SetFOV(float f) {
	fov = f;
}

void ModuleCamera::SetAspectRatio(float f) {
	aspect = f;
}



bool ModuleCamera::Init(){

	//Cam position
	cameraPos = float3(0, 0, 6);

	//Where is pointing to
	target = float3(0, 0, 0);

	//Frustum generates a projection matrix
	fov = 0.5;
	aspect = 60;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	

	ProcessMatrixs();

	return true;
}

update_status ModuleCamera::Update() {
	ProcessMatrixs();

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp() {
	return true;
}