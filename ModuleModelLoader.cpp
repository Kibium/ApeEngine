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

	//This is a model
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



	return true;
}

update_status ModuleModelLoader::Update() {


	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp() {
	return true;
}