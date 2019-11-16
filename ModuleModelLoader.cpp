#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"


#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/mesh.h>



ModuleModelLoader::ModuleModelLoader() {}

ModuleModelLoader::~ModuleModelLoader() {}

void ModuleModelLoader::loadModel() {
	
}

bool ModuleModelLoader::Init() {

	/*//This is a model
	const aiScene* scene = aiImportFile("../models/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	const char* error = aiGetErrorString();

	if (scene == nullptr) {
		App->ui->my_log.AddLog(error);
		App->ui->my_log.AddLog("\n");
	}

	else {
		App->ui->my_log.AddLog("Model Loaded! \n");

		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
		{
			//LoadMeshData(scene->mMeshes[i]);
		}
		for (unsigned i = 0; i < scene->mNumMaterials; ++i)
		{
			//LoadMaterialData(scene->mMaterials[i]);
		}
	}
	*/
	modelShader = Shader("../ModelViewer.vs", "../ModelViewer.fs");
	
	baker = Model("../models/BakerHouse.fbx");

	int ModelModel = glGetUniformLocation(App->program->programModel, "model");
	int ModelView = glGetUniformLocation(App->program->programModel, "view");
	int ModelProj = glGetUniformLocation(App->program->programModel, "proj");
	
	float4x4 proj = App->camera->proj;
	float4x4 view = App->camera->view;
	float4x4 model = float4x4::FromTRS(float3(0, 0, 0), float4x4::identity, float3(0.2f, 0.2f, 0.2f));;
		
	modelShader.use();
	glUniformMatrix4fv(ModelModel, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, &view[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelProj, 1, GL_TRUE, &proj[0][0]); //Calculating vertexs in the vertex shader
	glUseProgram(0);
	return true;
}

update_status ModuleModelLoader::Update() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	baker.Draw(modelShader);
	
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp() {
	return true;
}