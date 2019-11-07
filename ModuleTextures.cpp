#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include <iostream>


//Always after glew.h

ModuleTextures::ModuleTextures() {}
ModuleTextures::~ModuleTextures() {}

const void ModuleTextures::RenderTexture(ImageData &d) {

	glBindTexture(GL_TEXTURE_2D, d.texture);
	if (d.data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d.width, d.height, 0, GL_RGB, GL_UNSIGNED_BYTE, d.data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	once = true;

}

bool ModuleTextures::Init() {
	App->ui->my_log.AddLog("Init DevIL texture system \n");

	ilInit();
	iluInit();
	ilutInit();

	ilGenImages(1, &kirb.imageName);
	ilBindImage(kirb.imageName);
	ilLoadImage("../kirbo.jpg");
	iluFlipImage();

	ILenum Error;
	Error = ilGetError();

	//Kirbo dimensions
	kirb.width = ilGetInteger(IL_IMAGE_WIDTH);
	kirb.height = ilGetInteger(IL_IMAGE_HEIGHT);

	kirb.size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	//Set renderer for OpenGL
	ilutRenderer(ILUT_OPENGL);	

	kirb.texture = ilutGLBindTexImage();

	glGenTextures(1, &kirb.texture);
	glBindTexture(GL_TEXTURE_2D, kirb.texture);

	kirb.data = ilGetData();

	ilGenImages(1, &muffin.imageName);
	ilBindImage(muffin.imageName);
	ilLoadImage("../muffin.jpg");
	iluFlipImage();

	muffin.width = ilGetInteger(IL_IMAGE_WIDTH);
	muffin.height = ilGetInteger(IL_IMAGE_HEIGHT);

	muffin.size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	muffin.texture = ilutGLBindTexImage();

	glGenTextures(1, &muffin.texture);
	glBindTexture(GL_TEXTURE_2D, muffin.texture);

	muffin.data = ilGetData();

	ilGenImages(1, &lenna.imageName);
	ilBindImage(lenna.imageName);
	ilLoadImage("../lenna.png");


	lenna.width = ilGetInteger(IL_IMAGE_WIDTH);
	lenna.height = ilGetInteger(IL_IMAGE_HEIGHT);

	lenna.size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	lenna.texture = ilutGLBindTexImage();

	glGenTextures(1, &lenna.texture);
	glBindTexture(GL_TEXTURE_2D, lenna.texture);

	lenna.data = ilGetData();

	


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

	if (imageButtonValue >= 3)
		imageButtonValue = 0;

	if (imageButtonValue == 0 && !once) {

		RenderTexture(kirb);
		App->ui->my_log.AddLog("Loaded image with %d px width and %d px height \n", kirb.width, kirb.height);
		App->ui->my_log.AddLog("Image size: %d bytes\n", kirb.size);
		once = true;

	}

	if (imageButtonValue == 1 && !once) {
		RenderTexture(muffin);
		App->ui->my_log.AddLog("Loaded image with %d px width and %d px height \n", muffin.width, muffin.height);
		App->ui->my_log.AddLog("Image size: %d bytes\n", muffin.size);
		once = true;
	}

	if (imageButtonValue == 2 && !once) {
		RenderTexture(lenna);
		App->ui->my_log.AddLog("Loaded image with %d px width and %d px height \n", lenna.width, lenna.height);
		App->ui->my_log.AddLog("Image size: %d bytes\n", lenna.size);
		once = true;
	}



	

	if(mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	
	return UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp() {

	ilDeleteImages(1, &kirb.imageName);

	return true;
}