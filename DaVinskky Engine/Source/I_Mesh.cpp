#include "I_Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::InitMesh()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	LOG("%s", stream.user);
}

void Mesh::CleanUp()
{
	aiDetachAllLogStreams();
}
