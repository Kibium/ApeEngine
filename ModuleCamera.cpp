#include "ModuleCamera.h"
#include "ModuleUI.h"

#include "ModuleProgram.h"

ModuleCamera::ModuleCamera(){}
ModuleCamera::~ModuleCamera(){}

void ModuleCamera::LookAt(float3& eye, float3& target, float3& up) {

	//target = camPos + float3(0, 0, -1);//camPos; +float3(1, 0, 1);
	//camDirection = cameraPos - target;
	//camDirection.Normalize();
	//target.Normalize();
	//camRight = up.Cross(camDirection);
	//camRight.Normalize();

	//up = camDirection.Cross(camRight);

	f = target - eye;
	f.Normalize();
	s = f.Cross(up);
	s.Normalize();
	u = s.Cross(f);

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

	view[0][3] = -s.Dot(eye);
	view[1][3] = -u.Dot(eye);
	view[2][3] = -f.Dot(eye);

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[3][3] = 1.0f;
}

void ModuleCamera::SetProjMatrix(float& nearp, float& farp, float& vfov, float& hfov, float& aspect) {

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = farp;
	frustum.farPlaneDistance = nearp;
	frustum.verticalFov = vfov;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * DegToRad(aspect)); //aspect ratio 
	proj = frustum.ProjectionMatrix();

}

void ModuleCamera::ProcessMatrixs() {
	

	LookAt(cameraPos, cameraPos + camFront, camUp);
	rotateMatrix = float3x3::RotateZ(rotX) * float3x3::RotateY(rotY) * float3x3::RotateX(rotZ);

	SetProjMatrix(nearP, farP, vFov, hFov, AR);

	//It modifies the original position, rottion and scale of the camera.
	model = float4x4::FromTRS(float3(0, 0, 0), rotateMatrix, float3(1, 1, 1));

	transform = proj * view* float4x4(model);


}

void ModuleCamera::SetHFOV(float& f) {
	hFov = f;
}

void ModuleCamera::SetVFOV(float &f) {
	vFov = f;
}

void ModuleCamera::SetAspectRatio(float& f) {
	AR = f;
	dirty = true;
}

void ModuleCamera::SetPlaneDistances(float& n, float& f) {
	nearP = n;
	farP = f;
}

void ModuleCamera::ResetCamera() {
	//Cam position
	cameraPos = float3(0, 3, -10);
	camUp = float3(0, 1, 0);
	speedValue = 0.1f;
	sensitivity = 0.5f;
	//Where is pointing to
	camTarget = float3(0, 0, 0);

	camSpeed = float3(0, 0, 0);

	camFront = float3(0, 0, -1);

	rotX = rotY = rotZ = 0;

	//ORBIT MODE
	mode = true;

	//Frustum generates a projection matrix
	nearP = 0.1f;
	farP = 30;
	AR = 60;
	vFov = math::pi / 4;
	hFov = 2.f * atanf(tanf(vFov / 2) * DegToRad(AR)); //aspect ratio 
}

bool ModuleCamera::Init(){

	
	ResetCamera();
	LookAt(cameraPos, cameraPos + camFront, camUp);
	ProcessMatrixs();

	return true;
}

update_status ModuleCamera::Update() {

	//Means, if a change on the camera has been made, update it, so it's not made every frame
	if (dirty) {


		cameraPos += camSpeed;

		glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]); //Calculating vertexs in the vertex shader

		ProcessMatrixs();
		dirty = false;
	}

	if (!mode) {

		
		glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]); //Calculating vertexs in the vertex shader
		rotY += 0.01;
		ProcessMatrixs();

	}

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp() {
	return true;
}