#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include <iostream>


//Always after glew.h






ModuleTextures::ModuleTextures() {}
ModuleTextures::~ModuleTextures() {}


bool ModuleTextures::Init() {
	App->ui->my_log.AddLog("Init DevIL texture system \n");

	ilInit();
	ilGenImages(1, &imageName);
	ilBindImage(imageName);

	ilLoadImage("../kirbo.jpg");
	ILenum Error;
	Error = ilGetError();

	//Kirbo dimensions
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	App->ui->my_log.AddLog("Loaded image with %d px width and %d px height \n", width, height);

	ilutRenderer(ILUT_OPENGL);	
	texture = ilutGLBindTexImage();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	data = ilGetData();

	
	


	return true;
}

update_status ModuleTextures::PreUpdate() {

	// set the texture wrapping parameters
	if (WRAPs) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	
	if (WRAPt) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	if (MINfilter) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	if (MAGfilter) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	return UPDATE_CONTINUE;
}
update_status ModuleTextures::Update() {

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	if(mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	
	return UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp() {

	ilDeleteImages(1, &imageName);

	return true;
}