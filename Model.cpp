#include "Model.h"
#include <assimp/Importer.hpp>
#include "Application.h"
#include "ModuleUI.h"
#include "ModelLoader.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

Model::Model() {}

Model::Model(const char *_filename, const char* _texture, GLuint _program) : filename(_filename), textureFile(_texture), program(_program) {
	
	texture = App->textures->CreateTexture(textureFile);

	loadModel(filename);
}

void Model::Draw() {
	
	
	for (unsigned int i = 0; i < meshes.size(); ++i) {

		meshes[i].Draw();
	}
}

void Model::loadModel(std::string _path)
{
	App->ui->my_log.AddLog("[ASSIMP] Trying model loading...\n");
	scene = aiImportFile(_path.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	path = _path + "\n";

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string err = "[ASSIMP] Model with path: " + _path + " could not be loaded \n";
		App->ui->my_log.AddLog(err.c_str());
		return;
	}

	else {
		App->ui->my_log.AddLog("[ASSIMP] Model loaded from: ");
		App->ui->my_log.AddLog(path.c_str());

		directory = _path.substr(0, _path.find_last_of('/'));
	//	processNode(scene->mRootNode, scene);
		App->ui->my_log.AddLog("------------------------------\n");

		numMeshes = scene->mNumMeshes;
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{

			aiMesh *mesh = scene->mMeshes[i];
			meshes.push_back(processMesh(mesh, scene));


		}
	}

	
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		numVertices++;

		Vertex vertex;
		float3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			float2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = float2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		numPolys++;
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	//std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	//std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);

			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.

		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(std::string _path, std::string &directory)
{
	std::string filename = std::string(_path);

	if (!App->modelLoader->loadFromAbsolutePath) {
		filename = directory + '/' + filename;

		//App->ui->my_log.AddLog("[ASSIMP] Model's path: ");
		//App->ui->my_log.AddLog(_path.c_str());
		//App->ui->my_log.AddLog("\n");

		//App->ui->my_log.AddLog("[ASSIMP] Model's directory: ");
		//App->ui->my_log.AddLog(directory.c_str());
		//App->ui->my_log.AddLog("\n");

		App->ui->my_log.AddLog("[ASSIMP] Model's texture path: ");
		App->ui->my_log.AddLog(filename.c_str());
		App->ui->my_log.AddLog("\n");


	}

	else {
		directory = path.substr(0, path.find_last_of('\\'));

		filename = directory + '\\' + filename;

		//	App->ui->my_log.AddLog("[ASSIMP] Model's path: ");
		//	App->ui->my_log.AddLog(_path.c_str());
		//	App->ui->my_log.AddLog("\n");

		//	App->ui->my_log.AddLog("[ASSIMP] Model's directory: ");
		//	App->ui->my_log.AddLog(directory.c_str());
		//	App->ui->my_log.AddLog("\n");

		App->ui->my_log.AddLog("[ASSIMP] Model's texture path: ");
		App->ui->my_log.AddLog(filename.c_str());
		App->ui->my_log.AddLog("\n");

	}

	ILuint textureID;

	ilGenImages(1, &textureID);
	ilBindImage(textureID);
	ilLoadImage(filename.c_str());

	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	GLuint texture = ilutGLBindTexImage();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	ILenum Error;
	Error = ilGetError();

	//std::cout << Error << std::endl;

	ILubyte* data = ilGetData();

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		ilDeleteImages(1, &textureID);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		ilDeleteImages(1, &textureID);
	}
	return textureID;


}