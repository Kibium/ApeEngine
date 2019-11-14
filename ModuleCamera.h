#pragma once
#include "Application.h"
#include "Module.h"

#include "MathGeoLib/include/MathGeoLib.h"

enum CameraMode {
	ORBIT = 0,
	FREE
};

class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();
	
	void LookAt(float3&, float3&, float3&);
	void ProcessMatrixs();

	void SetHFOV(float&);
	void SetVFOV(float&);
	void SetAspectRatio(float&);
	void SetPlaneDistances(float&, float&);
	void SetProjMatrix(float& nearp, float& farp, float& vfov, float& hfov, float& aspect);
	void GetProjMatrix();
	void GetViewMatrix();

	float rotX = 2.3589, rotY = 0.5, rotZ = 0.5;
	float nearP, farP, vFov, hFov, AR, sensitivity;
	float3 camSpeed;
	float speedValue;

	//TRUE = FREE MOVEMENT // FALSE = ORBIT
	bool mode;

	

	math::float3  up, camTarget, f, s, u;
	math::float3 cameraPos, camDirection, camUp, camRight, camFront;
	math::float4x4 view, proj, model, transform, rotateMatrix;

	bool dirty = false;

	Frustum frustum;


};