#include	"mesh.h"
#include	"tiny_obj_loader.h"

#include	<stdio.h>
#include	<algorithm>
#include	<fstream>

#include	<glm\ext.hpp>
using namespace glm;

// ----------------------------------------------------------------------------

MeshBuffer* LoadMeshFromFile(const std::string& filename)
{
//	std::vector<Mesh> iterationMeshes;
//	std::vector<MeshVertex> verts;
//	std::vector<MeshTriangle> tris;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	const std::string error = tinyobj::LoadObj(shapes, materials, filename.c_str());
	if (!error.empty())
	{
		printf("tinyobj::LoadObj error: %s\n", error.c_str());
		return nullptr;
	}

	if (shapes.size() != 1)
	{
		printf("Error: only 1 model per file supported (loaded %d)\n", shapes.size());
		return nullptr;
	}

	const tinyobj::mesh_t& mesh = shapes[0].mesh;

	MeshBuffer* buffer = new MeshBuffer;

	buffer->numVertices = mesh.positions.size() / 3;
	buffer->vertices = (MeshVertex*)malloc(sizeof(MeshVertex) * buffer->numVertices);

	for (int i = 0; i < buffer->numVertices; i++)
	{
		const float* pos = &mesh.positions[i * 3];
		const float* normal = &mesh.normals[i * 3];

		buffer->vertices[i].xyz[0] = pos[0];
		buffer->vertices[i].xyz[1] = pos[1];
		buffer->vertices[i].xyz[2] = pos[2];
		buffer->vertices[i].xyz[3] = pos[3];

		buffer->vertices[i].normal[0] = normal[0];
		buffer->vertices[i].normal[1] = normal[1];
		buffer->vertices[i].normal[2] = normal[2];
		buffer->vertices[i].normal[3] = normal[3];
	}
	
	buffer->numTriangles = mesh.indices.size() / 3;
	buffer->triangles = (MeshTriangle*)malloc(sizeof(MeshTriangle) * buffer->numTriangles);

	for (int i = 0; i < buffer->numTriangles; i++)
	{
		const unsigned int* indices = &mesh.indices[i * 3];
		buffer->triangles[i].indices_[0] = indices[0];
		buffer->triangles[i].indices_[1] = indices[1];
		buffer->triangles[i].indices_[2] = indices[2];
	}

	return  buffer;
}



// ----------------------------------------------------------------------------

void Mesh::initialise()
{
	glGenVertexArrays(1, &vertexArrayObj_);
	glGenBuffers(1, &vertexBuffer_);
	glGenBuffers(1, &indexBuffer_);

	glBindVertexArray(vertexArrayObj_);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)(sizeof(glm::vec4) * 1));

	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

void Mesh::uploadData(const MeshBuffer* buffer)
{
	if (!buffer)
	{
		return;
	}

	glBindVertexArray(vertexArrayObj_);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * buffer->numVertices, &buffer->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MeshTriangle) * buffer->numTriangles, &buffer->triangles[0], GL_STATIC_DRAW);

	printf("Mesh: %d vertices\n", buffer->numVertices);
	numIndices_ = 3 * buffer->numTriangles;

	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

void Mesh::destroy()
{
	glDeleteBuffers(1, &indexBuffer_);
	glDeleteBuffers(1, &vertexBuffer_);
	glDeleteVertexArrays(1, &vertexArrayObj_);
}

