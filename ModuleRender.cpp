#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "SDL.h"

using namespace std;


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{

}

// Called before render is available
bool ModuleRender::Init()
{

	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GLContext glcontext = SDL_GL_CreateContext(App->window->window);
	SDL_GL_MakeCurrent(App->window->window, glcontext);
	//glClearColor(0.225f, 0, 0.225f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(App->window->window);

	GLenum err = glewInit();// ... check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	clear_color.x = 0.225f;
	clear_color.y = 0;
	clear_color.z = 0.225;
	clear_color.w = 1;

	static const GLfloat tri[] = { -1.f, -1.f, 0.0f,
									1.0f, -1.f, 0.0f,
									0.0f, 1.f, 0.0f };

	//Cam position
	cameraPos = float3(0, 0, 1);

	//Where is pointing to
	target = float3(0, 0, 0);

	math::float3 f(target - cameraPos);
	f.Normalize();
	math::float3 s(f.Cross(up));
	s.Normalize();
	math::float3 u(s.Cross(f));


	//View Matrix
	view[0][0] = s.x;
	view[0][1] = s.y;
	view[0][2] = s.z;

	view[1][0] = u.x;
	view[1][1] = u.y;
	view[1][2] = u.z;

	view[2][0] = -f.x;
	view[2][1] = -f.y;
	view[2][2] = -f.z;

	view[0][3] = -s.Dot(cameraPos);
	view[1][3] = -u.Dot(cameraPos);
	view[2][3] = f.Dot(cameraPos);

	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	view[3][3] = 1.0f;

	//Frustum generates a projection matrix
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * DegToRad(90)); //aspect ratio 
	proj = frustum.ProjectionMatrix();


	//Model Matrix
	model = float4x4::FromTRS(float3(0, 0, -4), float3x3::RotateY(math::pi / 4), float3(1, 1, 1));

	transform = proj * view* float4x4(model);

	//Creates a new vbo
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Setting buffer to be used
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Assigns data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

	//Passing transform matrix to the shader from the ModuleProgram.cpp

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClearColor(0.225f, 0, 0.225f, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Triangle
	glEnableVertexAttribArray(0); // attribute 0            
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0,                  // attribute 0                    
		3,                  // number of componentes (3 floats)                    
		GL_FLOAT,           // data type                    
		GL_FALSE,           // should be normalized?                    
		0,                  // stride                    
		(void*)0);            // array buffer offset   

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris    
	//glBindTexture(GL_TEXTURE_2D, App->textures->texture);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{



	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{



	//SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

