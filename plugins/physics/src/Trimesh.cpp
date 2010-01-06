/*
Copyright (c) 2009-2010, Christian Reiser

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakphysics/Trimesh.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <iostream>
#include <fstream>
#include <vector>

namespace peak
{
	TrimeshData::TrimeshData()
	{
	}
	TrimeshData::~TrimeshData()
	{
	}

	bool TrimeshData::init(std::string name)
	{
		std::ifstream file(name.c_str());
		if (!file.is_open())
			return false;

		file.seekg(0, std::ios_base::end);
		int length = file.tellg();
		file.seekg(0, std::ios_base::beg);

		char *buffer = new char[length];
		file.read(buffer, length);
		file.close();


		std::vector<Vector3F> dynvertices = std::vector<Vector3F>();
		std::vector<int> dynindices = std::vector<int>();

		int p = 0;
		while (p != length)
		{
			if (buffer[p] == 'v')
			{
				p++;

				char buffer2[16] = "";
				int p2 = 0;
				float vertexcomponents[3] = {0.0f, 0.0f, 0.0f};
				int p3 = 0;
				while (p3 < 3)
				{
					if (buffer[p] == ' ' || p == length || buffer[p] == '\n')
					{	
						if (p2 != 0)
						{
							buffer[p2] = '\0';
							std::string vertexstring(buffer2);
							char *end = (char *)vertexstring.c_str() + vertexstring.length();
							vertexcomponents[p3] = (float)strtod(vertexstring.c_str(), &end);
							p2 = 0;
							p3++;
						}
					}
					if ((buffer[p] >= 48 && buffer[p] <= 57) || buffer[p] == '.')
					{
						buffer2[p2] = buffer[p];
						p2++;
					}
					if (p != length)
						p++;
				}
				dynvertices.push_back(Vector3F(vertexcomponents[0], vertexcomponents[1], vertexcomponents[2]));
			}
			else if (buffer[p] == 'f')
			{
				p++;

				char buffer2[16] = "";
				int p2 = 0;
				int facecomponents[3] = {0, 0, 0};
				int p3 = 0;
				while (p3 < 3)
				{
					if (buffer[p] == ' ' || p == length || buffer[p] == '\n')
					{
						buffer[p2] = '\0';
						if (p2 != 0)
						{
							std::string facestring(buffer2);
							char *end = (char *)facestring.c_str() + facestring.length();
							facecomponents[p3] = (int)strtol(facestring.c_str(), &end, 10);
							p2 = 0;
							p3++;
						}
					}
					if ((buffer[p] >= 48 && buffer[p] <= 57))
					{
						buffer2[p2] = buffer[p];
						p2++;
					}
					if (p != length)
						p++;
				}
				for (int i = 0; i < 3; i++)
					dynindices.push_back(facecomponents[i]);
			}
			else
			{
				while (buffer[p] != '\n' && p != length)
					p++;
				if (p != length)
					p++;
			}

		}
		
		vertexcount = dynvertices.size();
		vertices = new Vector3F[vertexcount];
		indexcount = dynindices.size();
		indices = new int[indexcount];

		for (int i = 0; i < vertexcount; i++)
			vertices[i] = dynvertices[i];
		for (int i = 0; i < indexcount; i++)
			indices[i] = dynindices[i];

		return true;
	}

	bool TrimeshData::init(Vector3F *vertices, int *indices, int vertexcount, int indexcount)
	{
		if (!vertices || !indices || !vertexcount || !indexcount)
			return false;

		this->vertices = vertices;
		this->indices = indices;
		this->vertexcount = vertexcount;
		this->indexcount = indexcount;
		return true;
	}
	bool TrimeshData::destroy()
	{
		if (!vertices || !indices)
			return false;
		delete vertices;
		delete indices;
		return true;
	}

	Vector3F *TrimeshData::getVertices() const
	{
		return vertices;
	}
	int *TrimeshData::getIndices() const
	{
		return indices;
	}
	int TrimeshData::getVertexCount() const
	{
		return vertexcount;
	}
	int TrimeshData::getIndexCount() const
	{
		return indexcount;
	}

	Trimesh::Trimesh() : Shape()
	{
	}
	Trimesh::~Trimesh()
	{
	}

	bool Trimesh::init(TrimeshData data, float mass)
	{
		trimesh = new btTriangleMesh();
		for (int i = 0; i < data.getIndexCount() * 3; i++)
		{
			int index0 = data.getIndices()[i * 3];
			int index1 = data.getIndices()[i * 3 + 1];
			int index2 = data.getIndices()[i * 3 + 2];

			btVector3 vertex0(data.getVertices()[index0].x, data.getVertices()[index0].y, data.getVertices()[index0].z);
			btVector3 vertex1(data.getVertices()[index1].x, data.getVertices()[index1].y, data.getVertices()[index1].z);
			btVector3 vertex2(data.getVertices()[index2].x, data.getVertices()[index2].y, data.getVertices()[index2].z);

			trimesh->addTriangle(vertex0, vertex1, vertex2);
		}
		btConvexShape *tmpshape = new btConvexTriangleMeshShape(trimesh);
		btShapeHull *hull = new btShapeHull(tmpshape);
		btScalar margin = tmpshape->getMargin();
		hull->buildHull(margin);
		tmpshape->setUserPointer(hull);
		shape = new btConvexHullShape();
		btConvexHullShape *convexshape = (btConvexHullShape*)(shape);
		for (int j = 0; j < hull->numVertices() ; j++)
		{
			convexshape->addPoint(btVector3(hull->getVertexPointer()[j]));
		}
		delete tmpshape;
		delete hull;
		// FIXME: Optimize this!
		transform = new btTransform();
		transform->setIdentity();
		transform->setOrigin(btVector3(0, 0, 0));

		inertia = Vector3F(0, 0, 0);
		if (mass != 0.0f)
		{
			btVector3 in;
			shape->calculateLocalInertia(mass, in);
			inertia = Vector3F(in.x(), in.y(), in.z());
		}
		this->mass = mass;
		return true;
	}
	bool Trimesh::destroy()
	{
		Shape::destroy();
		delete trimesh;
		return true;
	}
}
