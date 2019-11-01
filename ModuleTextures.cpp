#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include <iostream>


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

	ilLoadImage("../kirbo.jpg");
	ILenum Error;
	Error = ilGetError();

	//ilEnable(IL_FILE_OVERWRITE);
	//ilSaveImage("WWWWWWWWWW.jpg");

	//Kirbo dimensions
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilutRenderer(ILUT_OPENGL);	
	texture = ilutGLBindTexImage();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ILubyte *data = ilGetData();

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	ilDeleteImages(1, &imageName);


	return true;
}

update_status ModuleTextures::PreUpdate() {
	return UPDATE_CONTINUE;
}
update_status ModuleTextures::Update() {
	
	glBindTexture(GL_TEXTURE_2D, texture);
	return UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp() {
	return true;
}