#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__
#include "Application.h"
#include "Module.h"

#include "MathGeoLib/include/MathGeoLib.h"

class ModuleCamera : public Module {
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void LookAt(float3&, float3&, float3&);
	void ProcessMatrixs();

	void SetHFOV(float&);
	void SetVFOV(float&);
	void SetAspectRatio(float&);
	void SetPlaneDistances(float&, float&);
	void SetProjMatrix(float& nearp, float& farp, float& vfov, float& hfov, float& aspect);

	void ResetCamera(bool aspectToo);
	
	void Focus(float3& target, float target_height);
	void AutoOrbit();
	

	float rotX = 2.3589f, rotY = 0.5f, rotZ = 0.5f;
	float nearP, farP, vFov, hFov, AR, sensitivity;
	float3 camSpeed;
	float speedValue;
	float aspectRatio;

	//TRUE = FREE MOVEMENT // FALSE = ORBIT
	bool mode;

	//Resets the camera once if the mode is set to ORBIT
	bool once;

	//Focus once per camera movement
	bool focusOnce = false;

	math::float3  camTarget, f, s, u;
	math::float3 lastPos, camDirection, camUp, camRight, camFront;
	math::float4x4 view, proj, model, transform, rotateMatrix;

	bool dirty = false;
	bool manualOrbit = false;

	Frustum frustum;


};

#endif