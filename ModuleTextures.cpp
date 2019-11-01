#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleUI.h"


//Always after glew.h
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>





ModuleTextures::ModuleTextures() {}
ModuleTextures::~ModuleTextures() {}


bool ModuleTextures::Init() {
	App->ui->my_log.AddLog("Init DevIL texture system \n");

	ilInit();
	ILuint imageName;
	ilGenImages(1, &imageName);
	ilBindImage(imageName);

	ilLoadImage("kirbo.jpg");
	ILenum Error;
	Error = ilGetError();

	ilutRenderer(ILUT_OPENGL);
	
	texture = ilutGLBindTexImage();

	float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 200, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	ilDeleteImages(1, &imageName);
	
	glGenerateMipmap(texture);
	glActiveTexture(texture);

	

	return true;
}

update_status ModuleTextures::PreUpdate() {
	return UPDATE_CONTINUE;
}
update_status ModuleTextures::Update() {


	return UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp() {
	return true;
}