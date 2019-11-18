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
	int ModelModel = glGetUniformLocation(App->program->programModel, "model");
	int ModelView = glGetUniformLocation(App->program->programModel, "view");
	int ModelProj = glGetUniformLocation(App->program->programModel, "proj");

	float4x4 proj = App->camera->proj;
	float4x4 view = App->camera->view;
	float4x4 model = float4x4::FromTRS(float3(0, 0, 0), float4x4::identity, float3(1, 1, 1));

	glUniformMatrix4fv(ModelModel, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, &view[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelProj, 1, GL_TRUE, &proj[0][0]); //Calculating vertexs in the vertex shader

	modelShader.use();

	glUseProgram(0);
}

bool ModuleModelLoader::Init() {

	//glDisable(GL_CULL_FACE);
	modelShader = Shader("../ModelViewer.vs", "../ModelViewer.fs");
	dir = "../models/baker/BakerHouse.fbx";

	
	

	return true;
}

update_status ModuleModelLoader::Update() {

	if (hasChanged) {
		
		model = Model(dir.c_str());
		loadModel();

	//This way, models will load from absolute paths from now on,
	//allowing the engine load models with a different algorythm
	//See Model.cpp TextureFromFile
		loadFromAbsolutePath = true;

		hasChanged = false;
	}

	model.Draw(modelShader);

	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp() {
	return true;
}