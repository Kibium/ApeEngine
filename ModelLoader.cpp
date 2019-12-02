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

#define PAR_SHAPES_IMPLEMENTATION
#include "Util/par_shapes.h"



ModelLoader::ModelLoader() {}

ModelLoader::~ModelLoader() {}

void ModelLoader::loadModel() {
	
	
}

bool ModelLoader::Init() {

	
	modelDir = "../models/BakerHouse/BakerHouse.fbx";

	textureDir = "../models/BakerHouse/BakerHouse.png";
	previousTexture = textureDir;

	checkerDir = "../models/checker.jpg";


	return true;
}

void ModelLoader::GenerateMesh(const char* name, const math::float3& pos, const math::Quat& rot, par_shapes_mesh* shape)
{
	Mesh dst_mesh;

	dst_mesh.name = name;
	dst_mesh.transform = math::float4x4(rot, pos);

	glGenBuffers(1, &dst_mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);

	// Positions

	for (unsigned i = 0; i< unsigned(shape->npoints); ++i)
	{
		math::float3 point(shape->points[i * 3], shape->points[i * 3 + 1], shape->points[i * 3 + 2]);
		point = dst_mesh.transform.TransformPos(point);
		for (unsigned j = 0; j < 3; ++j)
		{
			min_v[j] = min(min_v[j], point[i]);
			max_v[j] = max(max_v[j], point[i]);
		}
	}

	unsigned offset_acc = sizeof(math::float3);

	if (shape->normals)
	{
		dst_mesh.normals_offset = offset_acc;
		offset_acc += sizeof(math::float3);
	}

	dst_mesh.vertex_size = offset_acc;

	glBufferData(GL_ARRAY_BUFFER, dst_mesh.vertex_size*shape->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3)*shape->npoints, shape->points);

	// normals

	if (shape->normals)
	{
		glBufferSubData(GL_ARRAY_BUFFER, dst_mesh.normals_offset*shape->npoints, sizeof(math::float3)*shape->npoints, shape->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// indices

	glGenBuffers(1, &dst_mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*shape->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*shape->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(shape->ntriangles * 3); ++i)
	{
		*(indices++) = shape->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	dst_mesh.material = 0;
	dst_mesh.num_vertices = shape->npoints;
	dst_mesh.num_indices = shape->ntriangles * 3;


	GenerateVAO(dst_mesh);

	meshes.push_back(dst_mesh);

	bsphere.center = (max_v + min_v)*0.5f;
	bsphere.radius = (max_v - min_v).Length()*0.5f;
}

void ModelLoader::GenerateVAO(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.normals_offset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normals_offset*mesh.num_vertices));
	}

	if (mesh.texcoords_offset != 0)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texcoords_offset*mesh.num_vertices));
	}

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelLoader::CreateSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
	unsigned slices, unsigned stacks, const math::float4& color) {

	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh)
	{
		par_shapes_scale(mesh, size, size, size);

		GenerateMesh(name, pos, rot, mesh);
		par_shapes_free_mesh(mesh);

		meshes.back().material = materials.size();


		Material mat;
		mat.program = App->program->defaultProgram;
		mat.object_color = color;

		materials.push_back(mat);
	}

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

	glUseProgram(App->program->phongProgram);
	CreateSphere("sphere0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	glUseProgram(App->program->defaultProgram);
	model.Draw();

	return UPDATE_CONTINUE;
}

bool ModelLoader::CleanUp() {
	return true;
}