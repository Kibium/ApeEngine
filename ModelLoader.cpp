#include "Application.h"
#include "ModuleCamera.h"
#include "ModelLoader.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"


#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/mesh.h>



ModelLoader::ModelLoader() {}

ModelLoader::~ModelLoader() {}

void ModelLoader::loadModel() {
	int ModelModel = glGetUniformLocation(App->program->modelProgram, "model");
	int ModelView = glGetUniformLocation(App->program->modelProgram, "view");
	int ModelProj = glGetUniformLocation(App->program->modelProgram, "proj");

	float4x4 proj = App->camera->proj;
	float4x4 view = App->camera->view;
	float4x4 model = float4x4::FromTRS(float3(0, 0, 0), float4x4::identity, float3(1, 1, 1));

	glUniformMatrix4fv(ModelModel, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, &view[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(ModelProj, 1, GL_TRUE, &proj[0][0]); //Calculating vertexs in the vertex shader

	modelShader.use();

	//glUseProgram(0);
}

bool ModelLoader::Init() {

	
	modelDir = "../models/baker/BakerHouse.fbx";

	textureDir = "../models/baker/Baker_house.png";
	previousTexture = textureDir;

	checkerDir = "../models/checker.jpg";


	return true;
}



update_status ModelLoader::Update() {

	if (hasChanged) {

		model = Model(modelDir.c_str(), textureDir.c_str(), App->program->defaultProgram);
		
		//This way, models will load from absolute paths from now on,
		//allowing the engine load models with a different algorythm
		//See Model.cpp TextureFromFile
		loadFromAbsolutePath = true;

		hasChanged = false;
	}

	if (activate_checker) {
		if (!iterated) {
			model = Model(modelDir.c_str(), checkerDir.c_str(), App->program->defaultProgram);
			iterated = true;
		}
	}

	else {
		if (iterated) {
			model = Model(modelDir.c_str(), previousTexture.c_str(), App->program->defaultProgram);
			iterated = false;
		}
			
	}

	model.Draw();

	return UPDATE_CONTINUE;
}

bool ModelLoader::CleanUp() {
	return true;
}