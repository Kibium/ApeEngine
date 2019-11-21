#pragma once
#include "MathGeoLib.h"
#include <vector>
#include "Shader.h"

struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;

	float3 Tangent;
	float3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};



class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Draw();
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};