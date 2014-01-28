#pragma once
#include "body.h"
#include <vector>

namespace MyMesh{

	typedef float FLOAT;
	typedef CString STRING;

	struct XHeader{
		char magicNumber[4];
		char versionNumber[4];
		char formatType[4];
		char floatSize[4];
	};

	struct Header{
		void Parse(char* &pointer, int &index);
		WORD major;
		WORD minor;
		DWORD flags;
	};

	struct Vector {
		void Parse(char* &pointer, int &index);
		FLOAT x;
		FLOAT y;
		FLOAT z;
	};

	struct Coords2d {
		void Parse(char* &pointer, int &index);
		FLOAT u;
		FLOAT v;
	};

	struct Matrix4x4 {
		void Parse(char* &pointer, int &index);
		FLOAT matrix[16];
	};

	struct ColorRGBA {
		void Parse(char* &pointer, int &index);
		FLOAT red;
		FLOAT green;
		FLOAT blue;
		FLOAT alpha;
	};

	struct ColorRGB {
		void Parse(char* &pointer, int &index);
		FLOAT red;
		FLOAT green;
		FLOAT blue;
	};

	struct IndexedColor {
		void Parse(char* &pointer, int &index);
		DWORD index;
		ColorRGBA indexColor;
	};

	struct bool2d {
		void Parse(char* &pointer, int &index);
		bool u;
		bool v;
	};

	struct MaterialWrap {
		void Parse(char* &pointer, int &index);
		bool u;
		bool v;
	};

	struct TextureFilename {
		void Parse(char* &pointer, int &index);
		STRING filename;
	};

	struct Material {
		void Parse(char* &pointer, int &index);
		ColorRGBA faceColor;
		FLOAT power;
		ColorRGB specularColor;
		ColorRGB emissiveColor;
	};

	struct MeshFace {
		void Parse(char* &pointer, int &index);
		DWORD nFaceVertexIndices;
		std::vector<DWORD> faceVertexIndices;
	};

	struct MeshFaceWraps {
		void Parse(char* &pointer, int &index);
		DWORD nFaceWrapValues;
		bool2d faceWrapValues;
	};

	struct MeshTextureCoords {
		void Parse(char* &pointer, int &index);
		DWORD nTextureCoords;
		std::vector<Coords2d> textureCoords;
	};

	struct MeshMaterialList {
		void Parse(char* &pointer, int &index);
		DWORD nMaterials;
		DWORD nFaceIndexes;
		std::vector<DWORD> faceIndexes;
		std::vector<Material> materials;
	};

	struct MeshNormals {
		void Parse(char* &pointer, int &index);
		DWORD nNormals;
		std::vector<Vector> normals;
		DWORD nFaceNormals;
		std::vector<MeshFace> faceNormals;
	};

	struct MeshVertexColors {
		void Parse(char* &pointer, int &index);
		DWORD nVertexColors;
		std::vector<IndexedColor> vertexColors;
	};

	struct Mesh {
		void Parse(char* &pointer, int &index);
		DWORD nVertices;
		std::vector<Vector> vertices;
		DWORD nFaces;
		std::vector<MeshFace> faces;
		MeshMaterialList materialList;
		MeshNormals normals;
	};
};

struct Vector3df{
	float vec[3];
};

struct TrimeshData{
	int nIndices;
	int nVertices;
	int *indices;
	dReal* vertices;
	float* normals;

	TrimeshData(){
		indices = 0;
		vertices = 0;
		normals = 0;
		nIndices = 0;
		nVertices = 0;
	};

	~TrimeshData(){
		if(indices)
			delete[] indices;
		if(vertices)
			delete[] vertices;
		if(normals)
			delete[] normals;
	};
};

class CMesh :
	public CBody
{
private:
	void Parse(char* pointer, int size);
	void SkipTemplate(char* &pointer, int &index);

	MyMesh::Header header;
	MyMesh::Mesh mesh;
	MyMesh::MeshMaterialList mesh_material_list;

	bool dispList_init_flag[2];

	GLuint dispList_polygon[2];
	GLuint dispList_wire[2];
	GLuint dispList_shadow[2];
	

	void DrawNoMesh(bool bWireFrame);
	void DrawArrow(bool bWireFrame);
	void DrawMassCenter(bool bWireFrame);

	GLuint MakeDisplayList_Polygon();
	GLuint MakeDisplayList_Wire();
	GLuint MakeDisplayList_Shadow();

public:
	void InitDisplayList();

	dReal centerShift[3];

	bool isValid;
	dBodyID body;
	dGeomID geom;
	dGeomID trans;
	TrimeshData trimeshData;
	dTriMeshDataID triData;

	bool Load(const char* filename);

	CMesh(void);
	~CMesh(void);
	void Draw(bool bWireFrame = false);
	void DrawShadow();
};
