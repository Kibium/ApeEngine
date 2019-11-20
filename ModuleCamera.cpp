#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"


#include "GL/glew.h"

ModuleCamera::ModuleCamera(){}
ModuleCamera::~ModuleCamera(){}

void ModuleCamera::LookAt(float3& eye, float3& target, float3& up) {


	float3 cameraDirection = eye - target;
	cameraDirection.Normalize();

	camRight = up.Cross(cameraDirection);
	camRight.Normalize();

	f = target - eye;
	f.Normalize();
	s = camRight;
	s.Normalize();
	u = s.Cross(f);

	frustum.up = s.Cross(f);

	
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
	view[2][3] = f.Dot(eye);

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[3][3] = 1.0f;
}

void ModuleCamera::SetProjMatrix(float& nearp, float& farp, float& vfov, float& hfov, float& aspect) {

	frustum.pos += camSpeed;
	frustum.nearPlaneDistance = nearp;
	frustum.farPlaneDistance = farp;
	frustum.verticalFov = vfov;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * DegToRad(aspect)); //aspect ratio 
	proj = frustum.ProjectionMatrix();

}

void ModuleCamera::ProcessMatrixs() {
	LookAt(frustum.pos, frustum.pos + frustum.front, float3(0, 1, 0));

	SetProjMatrix(nearP, farP, vFov, hFov, aspectRatio);

}

void ModuleCamera::Orbit() {
	LookAt(frustum.pos, frustum.pos + frustum.front, frustum.up);
	SetProjMatrix(nearP, farP, vFov, hFov, aspectRatio);
	rotateMatrix = float3x3::RotateZ(0) * float3x3::RotateY(rotY) * float3x3::RotateX(0);

	model = float4x4::FromTRS(float3(0, 0, 0), rotateMatrix, float3(1, 1, 1));
}

void ModuleCamera::SetHFOV(float& f) {
}

void ModuleCamera::SetVFOV(float &f) {
}

void ModuleCamera::SetAspectRatio(float& f) {

	aspectRatio = f;
}

void ModuleCamera::SetPlaneDistances(float& n, float& f) {
	
}

void ModuleCamera::ResetCamera() {
	
}

bool ModuleCamera::Init(){

	App->ui->my_log.AddLog("Init Camera\n");


	speedValue = 0.1f;
	sensitivity = 0.5f;

	//ORBIT MODE
	mode = true;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 4, 10);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance  = 100.0f;
	frustum.verticalFov  = math::pi / 4.0f;
	aspectRatio = 60;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *(aspectRatio));
	proj = frustum.ProjectionMatrix();

	camSpeed = float3(0, 0, 0);

	nearP = frustum.nearPlaneDistance;
	farP = frustum.farPlaneDistance;
	vFov = frustum.verticalFov;
	hFov = frustum.horizontalFov;

	LookAt(frustum.pos, frustum.pos + frustum.front, float3(0, 1, 0));	
	model = float4x4::FromTRS(frustum.pos, float3x3::RotateY(0), float3(1.0f, 1.0f, 1.0f));
	
	ProcessMatrixs();
	
	return true;
}

update_status ModuleCamera::Update() {

	if (dirty){
		//App->ui->my_log.AddLog("%f, %f, %f \n", App->camera->frustum.pos.x, App->camera->frustum.pos.y, App->camera->frustum.pos.z);

		ProcessMatrixs();
		App->program->Use(App->program->defaultProgram);
		glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]); //Calculating vertexs in the vertex shader
		//glUseProgram(0);

	}

	if (!mode) {

		rotY += 0.001;
		Orbit();
		glUniformMatrix4fv(App->program->modelLocation, 1, GL_TRUE, &App->camera->model[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->viewLocation, 1, GL_TRUE, &App->camera->view[0][0]); //Calculating vertexs in the vertex shader
		glUniformMatrix4fv(App->program->projLocation, 1, GL_TRUE, &App->camera->proj[0][0]); //Calculating vertexs in the vertex shader
	}

	

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp() {
	return true;
}