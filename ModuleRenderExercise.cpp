#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"

ModuleRenderExercise::ModuleRenderExercise() {}
ModuleRenderExercise::~ModuleRenderExercise() {}

bool ModuleRenderExercise::Init() {

	static const GLfloat tri1 []= {-1.f, -1.f, 0.0f,
									-0.5f, -1.f, 0.0f,
									-0.75f, -0.50f, 0.0f };



	static const GLfloat tri2[] = { -0.5f, -1.f, 0.0f,
									0.f, -1.f, 0.0f,
									-0.25f, -0.5f, 0.0f };

	static const GLfloat tri3[] = { -1.f, -1.f, 0.0f,
									1.0f, -1.f, 0.0f,
									0.0f, 1.f, 0.0f };

	static const GLfloat vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

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
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * DegToRad(60)); //aspect ratio 
	proj = frustum.ProjectionMatrix();


	//Model Matrix
	model = float4x4::FromTRS(float3(0, 0, -4), float3x3::RotateY(math::pi /4 ), float3(1, 1, 1));

	float4x4 transform = proj *view* float4x4(model);

	float4 asd1(-1.f, -1.f, 0.0f, 1);
	float4 asd2(1.0f, -1.f, 0.0f, 1);
	float4 asd3(0.0f, 1.f, 0.0f, 1);

	asd1 = transform * asd1;
	asd2 = transform * asd2;
	asd3 = transform * asd3;

	float3 v1 = float3(asd1.x / asd1.w, asd1.y / asd1.w, asd1.z / asd1.w);
	float3 v2 = float3(asd2.x / asd2.w, asd2.y / asd2.w, asd2.z / asd2.w);
	float3 v3 = float3(asd3.x / asd3.w, asd3.y / asd3.w, asd3.z / asd3.w);

	static const GLfloat tri4[] = { v1.x, v1.y, v1.z,
								  v2.x, v2.y, v2.z,
								  v3.x, v3.y, v3.z };

	//Check for the vertexs positions
	for (int i = 0; i < 9; ++i) {
		std::cout << tri4[i] << std::endl;

	}

	//Creates a new vbo
	glGenBuffers(1, &vbo); 

	//Setting buffer to be used
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 

	//Assigns data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri4), tri4, GL_STATIC_DRAW);

	//Without "useProgram" we can't get the uniform's information...

	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "transform"), 1, GL_TRUE, &transform[0][0]);
	
	glUseProgram(App->program->program);


	

	return true;
}


update_status ModuleRenderExercise::Update() {
	
	//Set the format of the vertex's data.
	//We gonna tell opengl, what we wanna draw

	glEnableVertexAttribArray(0); // attribute 0            
	glBindBuffer(GL_ARRAY_BUFFER, vbo);            
	glVertexAttribPointer(0,                  // attribute 0                    
						  3,                  // number of componentes (3 floats)                    
						  GL_FLOAT,           // data type                    
						  GL_FALSE,           // should be normalized?                    
						  0,                  // stride                    
						  (void*)0);            // array buffer offset   

	//glColor3f(204, 204, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris            
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp() {

	
	return true;
}
