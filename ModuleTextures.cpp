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
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	once = true;
}


//HE CREAT UNA FUNCION PER CREAR TEXTURES. HE DE FER QUE ARRASTRANT UNA IMATGE SE CREI UNA TEXTURA I FER BIND IMAGE DINS ES MODEL

ImageData ModuleTextures::CreateTexture(const char* path) {

	ImageData textureToBeCreated;

	GLuint image;

	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(path);

	ILenum Error;
	Error = ilGetError();

	int w, h, s;

	//Image attributes
	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	s = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	GLuint texxt;

	texxt = ilutGLBindTexImage();

	GLubyte* data = ilGetData();

	if (data) {
		App->ui->my_log.AddLog("[TEXTURES] Image created correctly \n");
		App->ui->my_log.AddLog("Path: ");
		App->ui->my_log.AddLog(path);
		App->ui->my_log.AddLog("\n");

		glGenTextures(1, &texxt);
		glBindTexture(GL_TEXTURE_2D, texxt);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	else
		App->ui->my_log.AddLog("Could not create texture \n");

	return textureToBeCreated;
}

bool ModuleTextures::Init() {
	App->ui->my_log.AddLog("Init DevIL texture system \n");

	ilInit();
	iluInit();
	ilutInit();

	//Set renderer for OpenGL
	ilutRenderer(ILUT_OPENGL);

	return true;
}

update_status ModuleTextures::PreUpdate() {

	// set the texture wrapping parameters
	if (WRAPs)



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