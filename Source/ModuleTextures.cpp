#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include <iostream>

ModuleTextures::ModuleTextures() {}
ModuleTextures::~ModuleTextures() {}

const void ModuleTextures::RenderTextures() {

	for (unsigned int i = 0; i < images.size(); ++i) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, images[i].width, images[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i].data);
		
	}
	//If deactivated, any change on the image will "de-render" the image
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
}

const std::vector<ImageData> ModuleTextures::getTextures() {
	return images;
}

ImageData ModuleTextures::CreateTexture(ILenum type, const char* path) {

	//I'll handle a single texture for the moment
	//TODO Handle more than one texture at the time
	images.clear();

	ImageData id;

	ilGenImages(1, &id.imageName);
	ilBindImage(id.imageName);
	ilLoad(type, path);

	ILenum Error;
	Error = ilGetError();

	//Image attributes
	id.width = ilGetInteger(IL_IMAGE_WIDTH);
	id.height = ilGetInteger(IL_IMAGE_HEIGHT);
	id.size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	//So DDS images work!
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);


	id.texture = ilutGLBindTexImage();

	id.data = ilGetData();
	Error = ilGetError();
	if (id.data) {
		//The engine's gonna pop this message for every mesh in the 3D model
		//TODO: Avoid this^
		App->ui->my_log.AddLog("[TEXTURES] Image created correctly \n");
		App->ui->my_log.AddLog("[TEXTURES] Path: ");
		App->ui->my_log.AddLog(path);
		App->ui->my_log.AddLog("\n");

		glGenTextures(1, &id.texture);
		glBindTexture(GL_TEXTURE_2D, id.texture);

		images.push_back(id);
		return id;
	}

	else {
		Error = ilGetError();
		App->ui->my_log.AddLog("Could not create texture \n");
		App->ui->my_log.AddLog("%s \n", Error);
		return id;
	}
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

	if (nearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (mirrored) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else if (edge) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else if (border) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	

	return UPDATE_CONTINUE;
}
update_status ModuleTextures::Update() {

	return UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {

	RenderTextures();

	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp() {

	return true;
}