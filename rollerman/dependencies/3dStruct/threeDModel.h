#ifndef _threeDMODEL_H
#define _threeDMODEL_H

#include "Vector3d.h"
#include <vector>
#include <tuple>
#include <string>

#include "glew.h"
#include "OBJLoader.h"
#include "Octree.h"

const int NUM_OF_VERTS_IN_TRIANGLE		= 3;
const int DIMENSION_IN_3D				= 3;
const int DIMENSION_IN_2D				= 2;
const int NUM_OF_VBOS_WITHOUT_TRI_IDS	= 3;

class ObjFace;
class CShader;

/************************************************************
*  Class: ThreeDModel
* 
*  Purpose: To render a 3d model. It can be constructed from 
*           geometry loaded in using the OBJLoader
* 
/**************************************************************/

class CThreeDModel
{
private: 
	int							m_iNumberOfVertices;  
	int							m_iNumberOfFaceNormals;
	int							m_iNumberOfVertNormals;
	int							m_iNumberOfTriangles; 
	int							m_iNumberOfTexCoords;
	int							m_iNumberOfMaterials;

	Vector3d*					m_pvVertices;
	Vector3d*					m_pvFaceNormals;
	Vector3d*					m_pvVertNormals;
	Vector3d*					m_pvTexCoords;
	ObjFace*					m_pobTriangles;

	std::vector<std::tuple<int, int, int>>	m_vuiFaceIndexRangeForTrisWithSameTexture;

	unsigned int				m_uiNumOfVBOs;
	GLuint*						m_puiVBOs;
	GLuint						m_uiVaoID;

	Vector3d					m_obCentrePoint;
	COctree*					m_pobOctree;

	float						Material_Ambient[4];
	float						Material_Diffuse[4];
	float						Material_Specular[4];
	float						Material_Shininess;

	void						CalcFaceNormals();
	void						DeleteVertexFaceData();
	void						CalcVertNormalsUsingOctree();
	void						ConstructOctree();


public:
	CThreeDModel(float Material_Amb[4], float Material_Dif[4], float Material_Spec[4], float Material_Shin);

	~CThreeDModel();

	CThreeDModel(const CThreeDModel& p);	

	void						operator=(const CThreeDModel& p);

	void						ConstructModelFromOBJLoader(COBJLoader& refOBJLoader);
	void						CalcVertNormals(int* VertList, int VertListSize, int* TriList, int TriListSize);
	
	void						CentreOnZero();
	void						CalcCentrePoint();
	Vector3d*					GetCentrePoint();
	void						InitVBO(CShader* myShader);
	void						DrawElementsUsingVBO(CShader* myShader);
	void						DrawBoundingBox(CShader* shader);
	void						DrawAllBoxesForOctreeNodes(CShader* shader);
	void						DrawOctreeLeaves(CShader* shader);
	
	int							GetOctreeTriangleListSize();
	int							GetOctreeVertexListSize();
	bool						IsTriangleIntersectingAABB(double boxcenter[DIMENSION_IN_3D],double boxhalfsize[DIMENSION_IN_3D], int PrimIndex);
	bool						IsVertexIntersectingAABB(double min[DIMENSION_IN_3D], double max[DIMENSION_IN_3D], int VertIndex);
	void						CalcBoundingBox(double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ);
	inline COctree*				GetCOCTREE() { return m_pobOctree; }
};

#endif