#pragma once
#include "Application.h"
#include "Module.h"

#include "MathGeoLib/include/MathGeoLib.h"


class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();
	
	void LookAt();
	void ProcessMatrixs();

	void SetFOV(float);
	void SetAspectRatio(float);
	void SetPlaneDistances();
	void GetProjMatrix();
	void GetViewMatrix();

	float rotX = 2.3589, rotY = 0.5, rotZ = 0.5;

	math::float3  up, target, f, s, u;
	math::float3 cameraPos, camDirection, camUp, camRight;
	math::float4x4 view, proj, model, transform, rotateMatrix;
	Frustum frustum;

	float fov;
	float aspect;

};