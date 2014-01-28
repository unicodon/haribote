#include "StdAfx.h"
#include ".\mesh.h"
#include "inc.h"

extern CMaterial matr_red;
extern CMaterial matr_blue;
extern CMaterial matr_shadow;

extern dReal massCenter[NUM_BODY][3];

extern GLdouble drop_shadow_mat[16];

extern GLfloat dif_shadow[4];

enum STATE
{
	NOP = 0,
	AN,
	CANMA,
	SEMICOLON,
	BEGIN_BRACE,
	END_BRACE
};

const char table[128] = {
	//0
	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,

	//1
	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,

	//2
	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
	NOP,	NOP,	NOP,	NOP,	CANMA,	AN,		AN,		NOP,

	//3
	AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,		
	AN,		AN,		NOP,	SEMICOLON,NOP,	NOP,	NOP,	NOP,		

	//4
	NOP,	AN,		AN,		AN,		AN,		AN,		AN,		AN,		
	AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,		

	//5
	AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,		
	AN,		AN, 	AN, 	NOP,	NOP,	NOP,	NOP,	AN,

	//6
	NOP,	AN,		AN,		AN,		AN,		AN,		AN,		AN,		
	AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,		

	//7
	AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,		
	AN,		AN, 	AN, 	BEGIN_BRACE,	NOP,	END_BRACE,	NOP,	NOP

};

void SkipBlock(char* &str, int &index){
	int depth = 0;
	while(1){
		if(str[index] == 0)
			break;

		switch(table[str[index++]]){
			case BEGIN_BRACE:
				depth++;
				break;
			case END_BRACE:
				depth--;
				if(depth == 0)
					return;
				if(depth < 0){
					index--;
					return;
				}
				break;
		}
	}
}

void normalizeVector3 (float v[3])
{
	float len = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	if (len <= 0.0f) {
		v[0] = 1;
		v[1] = 0;
		v[2] = 0;
	}
	else {
		len = 1.0f / sqrt(len);
		v[0] *= len;
		v[1] *= len;
		v[2] *= len;
	}
}


void CalcNormal(dReal vec[3][3], float* normal){
	//vec[0,1,2][x,y,z]
	float u[3],v[3];
	u[0] = float( vec[1][0] - vec[0][0] );
	u[1] = float( vec[1][1] - vec[0][1] );
	u[2] = float( vec[1][2] - vec[0][2] );
	v[0] = float( vec[2][0] - vec[0][0] );
	v[1] = float( vec[2][1] - vec[0][1] );
	v[2] = float( vec[2][2] - vec[0][2] );
	dCROSS (normal,=,u,v);
	normalizeVector3(normal);
}

using namespace std;
using namespace MyMesh;

void bool2d::Parse(char* &str, int &index)
{
}

void ColorRGB::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	//red
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	red = atof(token);

	//green
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	green = atof(token);

	//blue
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	blue = atof(token);
}

void ColorRGBA::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	//red
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	red = atof(token);

	//green
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	green = atof(token);

	//blue
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	blue = atof(token);

	//alpha
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	alpha = atof(token);
}

void Coords2d::Parse(char* &str, int &index)
{
}

void Header::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;
	while(table[str[index++]] != BEGIN_BRACE){
	}

	//major
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	major = atoi(token);

	//minor
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	minor = atoi(token);

	//flags
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	flags = atoi(token);

	while(table[str[index++]] != END_BRACE){
	}
}

void IndexedColor::Parse(char* &str, int &index)
{
}

void Material::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	while(table[str[index++]] != BEGIN_BRACE){
	}

	//faceColor
	while(table[str[index]] == NOP){
		index++;
	}
	faceColor.Parse(str, index);
	while(table[str[index]] == SEMICOLON){
		index++;
	}

	//power
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	power = atof(token);
	while(table[str[index]] == SEMICOLON){
		index++;
	}

	//specular
	while(table[str[index]] == NOP){
		index++;
	}
	specularColor.Parse(str, index);
	while(table[str[index]] == SEMICOLON){
		index++;
	}

	//emissive
	while(table[str[index]] == NOP){
		index++;
	}
	emissiveColor.Parse(str, index);
	while(table[str[index]] == SEMICOLON){
		index++;
	}


	while(table[str[index++]] != END_BRACE){
	}
}

void MaterialWrap::Parse(char* &str, int &index)
{
}

void Matrix4x4::Parse(char* &str, int &index)
{
}

void Mesh::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;
	while(str[index] && table[str[index++]] != BEGIN_BRACE ){
	}

	//nVertices
	while(str[index] && table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(str[index] && table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nVertices = atoi(token);

	//vertices
	for(int i = 0;i < nVertices;i++){
		Vector vec;
		vec.Parse(str, index);
		vertices.push_back(vec);
		while(str[index] &&  (table[str[index]] == NOP || table[str[index]] == CANMA)){
			index++;
		}
	}
	while(str[index] &&  (table[str[index]] == NOP || table[str[index]] == SEMICOLON)){
		index++;
	}

	//nFaces
	while(str[index] && table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(str[index] && (table[str[index]] != SEMICOLON && table[str[index]] != NOP)){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nFaces = atoi(token);

	//faces
	for(int i = 0;i < nFaces;i++){
		MeshFace face;
		face.Parse(str, index);
		faces.push_back(face);
		while(str[index] && (table[str[index]] == NOP || table[str[index]] == CANMA)){
			index++;
		}
	}

	while(str[index] && table[str[index]] != AN){
		index++;
	}

	while(str[index] && table[str[index]] != END_BRACE){		
		while(str[index] && table[str[index]] == NOP){
			index++;
		}

		tokind = 0;
		while(str[index] && table[str[index]] == AN){
			token[tokind++] = str[index++];
		}
		token[tokind] = 0;index++;

		if(strcmp(token, "MeshMaterialList") == 0){
			materialList.Parse(str, index);
			continue;
		}

		if(strcmp(token, "MeshNormals") == 0){
			normals.Parse(str, index);
			continue;
		}

		//skip
		SkipBlock(str, index);
		while(str[index] && table[str[index]] == NOP){
			index++;
		}
	}
}

void MeshFace::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	//nFaceVertexIndices
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nFaceVertexIndices = atoi(token);

	//faceVertexIndices
	while(table[str[index]] == NOP){
		index++;
	}
	for(int i = 0;i < nFaceVertexIndices;i++){
		DWORD ver;
		tokind = 0;
		while(table[str[index]] != CANMA && table[str[index]] != NOP && table[str[index]] != SEMICOLON){
			token[tokind++] = str[index++];
		}
		token[tokind] = 0;index++;
		ver = atoi(token);
		faceVertexIndices.push_back(ver);
	}
}

void MeshFaceWraps::Parse(char* &str, int &index)
{
}

void MeshMaterialList::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	while(table[str[index++]] != BEGIN_BRACE){
	}


	//nMaterials
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nMaterials = atoi(token);

	//nFaceIndexes
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nFaceIndexes = atoi(token);

	//faceIndexes
	for(int i = 0;i < nFaceIndexes; i++){
		DWORD ind;
		while(table[str[index]] == NOP){
			index++;
		}
		tokind = 0;
		while(table[str[index]] != SEMICOLON && table[str[index]] != NOP && table[str[index]] != CANMA){
			token[tokind++] = str[index++];
		}
		token[tokind] = 0;index++;
		ind = atoi(token);
		faceIndexes.push_back(ind);

		while(table[str[index]] == CANMA || table[str[index]] == SEMICOLON){
			index++;
		}

	}
	while(table[str[index]] == NOP){
		index++;
	}

	for(int i = 0;i < nMaterials; i++){
		tokind = 0;
		while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
			token[tokind++] = str[index++];
		}
		token[tokind] = 0;index++;

		if(strcmp(token, "Material") == 0){
			Material mat;
			mat.Parse(str, index);
			materials.push_back(mat);

			while(table[str[index]] == NOP || table[str[index]] == CANMA){
				index++;
			}
		}

	}

	while(table[str[index++]] != END_BRACE){
	}

}

void MeshNormals::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	while(table[str[index++]] != BEGIN_BRACE){
	}

	//nNormals
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nNormals = atoi(token);

	//normals
	for(int i = 0;i < nNormals;i++){
		Vector vec;
		vec.Parse(str, index);
		normals.push_back(vec);
		while(table[str[index]] == NOP || table[str[index]] == CANMA){
			index++;
		}
	}
	while(table[str[index]] == NOP || table[str[index]] == SEMICOLON){
		index++;
	}

	//nFaceNormals
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	nFaceNormals = atoi(token);

	//faceNormals
	for(int i = 0;i < nFaceNormals;i++){
		MeshFace face;
		face.Parse(str, index);
		faceNormals.push_back(face);
		while(table[str[index]] == NOP || table[str[index]] == CANMA){
			index++;
		}
	}

	while(table[str[index++]] != END_BRACE){
	}

}

void MeshTextureCoords::Parse(char* &str, int &index)
{
}

void MeshVertexColors::Parse(char* &str, int &index)
{
}

void TextureFilename::Parse(char* &str, int &index)
{
}

void Vector::Parse(char* &str, int &index)
{
	char token[256];
	int tokind = 0;

	//x
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	x = atof(token);

	//y
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	y = atof(token);

	//z
	while(table[str[index]] == NOP){
		index++;
	}
	tokind = 0;
	while(table[str[index]] != SEMICOLON && table[str[index]] != NOP){
		token[tokind++] = str[index++];
	}
	token[tokind] = 0;index++;
	z = atof(token);

}


CMesh::CMesh(void):
isValid(false)
{
	dispList_init_flag[0] = 
	dispList_init_flag[1] = false;

	centerShift[0] = 
	centerShift[1] = 
	centerShift[2] = 0.0f;
}

CMesh::~CMesh(void)
{
}

bool CMesh::Load(const char* filename)
{
	isValid = false;
	CFile file;
	if(!file.Open(filename,CFile::modeRead)){
		return false;
	}

	int size = file.GetLength();
	int full_size = size;
	char *buf = new char[size+1];
	buf[size] = 0;

	while(size > 0){
		size -= file.Read(buf, size);
	}

	file.Close();

	Parse(buf, full_size);

	delete[] buf;


	int nVertices = mesh.nVertices*3;
	int nIndices = 0;
	for(int i = 0;i < mesh.nFaces;i++){
		int n = mesh.faces[i].nFaceVertexIndices;
		if(n == 3)nIndices+=3;
		if(n == 4)nIndices+=6;
	}

	ASSERT(mesh.nVertices == mesh.vertices.size());

	trimeshData.nVertices = nVertices;
	trimeshData.vertices = new dReal[nVertices];
	trimeshData.nIndices = nIndices;
	trimeshData.indices = new int[nIndices];
	trimeshData.normals = new float[nIndices];

	for(int i = 0;i < mesh.nVertices;i++){
		trimeshData.vertices[3*i  ] = mesh.vertices[i].x;
		trimeshData.vertices[3*i+1] = mesh.vertices[i].y;
		trimeshData.vertices[3*i+2] = mesh.vertices[i].z;
	}

	int ind = 0;
	for(int i = 0;i < mesh.nFaces;i++){
		int n = mesh.faces[i].nFaceVertexIndices;
		if(n == 3){
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[0];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[1];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[2];
		}else if(n == 4){
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[0];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[1];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[2];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[2];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[3];
			trimeshData.indices[ind++] = mesh.faces[i].faceVertexIndices[0];
		}
	}

	dReal vec[3][3];
	for(int i = 0; i < trimeshData.nIndices / 3;i++){
		vec[0][0] = trimeshData.vertices[3* (trimeshData.indices[3*i+0]) + 0];
		vec[0][1] = trimeshData.vertices[3* (trimeshData.indices[3*i+0]) + 1];
		vec[0][2] = trimeshData.vertices[3* (trimeshData.indices[3*i+0]) + 2];
		vec[1][0] = trimeshData.vertices[3* (trimeshData.indices[3*i+1]) + 0];
		vec[1][1] = trimeshData.vertices[3* (trimeshData.indices[3*i+1]) + 1];
		vec[1][2] = trimeshData.vertices[3* (trimeshData.indices[3*i+1]) + 2];
		vec[2][0] = trimeshData.vertices[3* (trimeshData.indices[3*i+2]) + 0];
		vec[2][1] = trimeshData.vertices[3* (trimeshData.indices[3*i+2]) + 1];
		vec[2][2] = trimeshData.vertices[3* (trimeshData.indices[3*i+2]) + 2];

		CalcNormal(vec, &trimeshData.normals[3*i]);
	}

	triData = dGeomTriMeshDataCreate();

	isValid = true;

	return true;
}

GLuint CMesh::MakeDisplayList_Polygon()
{
	GLuint dispList = glGenLists(1);

	int nSubmesh = mesh.materialList.nMaterials;

	typedef vector<DWORD> DWORDlist;
	vector<DWORDlist> faceList;
	vector<CMaterial> materialList;

	for(int i = 0;i < nSubmesh;i++){
		DWORDlist dlist;
		faceList.push_back(dlist);

		CMaterial mat;
		mat.diffuse[0] = mesh.materialList.materials[i].faceColor.red;
		mat.diffuse[1] = mesh.materialList.materials[i].faceColor.green;
		mat.diffuse[2] = mesh.materialList.materials[i].faceColor.blue;
		mat.diffuse[3] = mesh.materialList.materials[i].faceColor.alpha;

		mat.ambient[0] = 0;
		mat.ambient[1] = 0;
		mat.ambient[2] = 0;
		mat.ambient[3] = 0;

		mat.specular[0] = mesh.materialList.materials[i].specularColor.red;
		mat.specular[1] = mesh.materialList.materials[i].specularColor.green;
		mat.specular[2] = mesh.materialList.materials[i].specularColor.blue;
		mat.specular[3] = 1.0;

		mat.shininess = 40;

		materialList.push_back(mat);
	}

	for(int i = 0;i < mesh.materialList.nFaceIndexes; i++){
		DWORD matInd = mesh.materialList.faceIndexes[i];//マテリアル番号

		faceList[matInd].push_back(i);
	}


	glNewList(dispList, GL_COMPILE);

	for(int m = 0;m < nSubmesh;m++){
		materialList[m].setMaterial();

		for(int k = 0;k < faceList[m].size(); k++){
			int i = faceList[m][k];
			int n = mesh.faces[i].nFaceVertexIndices;
			glBegin(GL_POLYGON);
			for(int j = 0;j < n; j++){
				glNormal3f(
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].x,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].y,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].z
					);
				glVertex3f(
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].x,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].y,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].z
					);
			}			
			glEnd();
		}
	}

	glEndList();

	return dispList;
}

GLuint CMesh::MakeDisplayList_Wire()
{
	GLuint dispList = glGenLists(1);

	int nSubmesh = mesh.materialList.nMaterials;

	typedef vector<DWORD> DWORDlist;
	vector<DWORDlist> faceList;
	vector<CMaterial> materialList;

	for(int i = 0;i < nSubmesh;i++){
		DWORDlist dlist;
		faceList.push_back(dlist);

		CMaterial mat;
		mat.diffuse[0] = mesh.materialList.materials[i].faceColor.red;
		mat.diffuse[1] = mesh.materialList.materials[i].faceColor.green;
		mat.diffuse[2] = mesh.materialList.materials[i].faceColor.blue;
		mat.diffuse[3] = mesh.materialList.materials[i].faceColor.alpha;

		mat.ambient[0] = 0;
		mat.ambient[1] = 0;
		mat.ambient[2] = 0;
		mat.ambient[3] = 0;

		mat.specular[0] = mesh.materialList.materials[i].specularColor.red;
		mat.specular[1] = mesh.materialList.materials[i].specularColor.green;
		mat.specular[2] = mesh.materialList.materials[i].specularColor.blue;
		mat.specular[3] = 1.0;

		mat.shininess = 40;

		materialList.push_back(mat);
	}

	for(int i = 0;i < mesh.materialList.nFaceIndexes; i++){
		DWORD matInd = mesh.materialList.faceIndexes[i];//マテリアル番号

		faceList[matInd].push_back(i);
	}

	glNewList(dispList, GL_COMPILE);


	glColor3f(1,1,1);
	for(int m = 0;m < nSubmesh;m++){
		materialList[m].setMaterial();

		for(int k = 0;k < faceList[m].size(); k++){
			int i = faceList[m][k];
			int n = mesh.faces[i].nFaceVertexIndices;
			glBegin(GL_LINE_LOOP);
			for(int j = 0;j < n; j++){
				glNormal3f(
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].x,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].y,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].z
					);
				glVertex3f(
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].x,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].y,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].z
					);
			}			
			glEnd();
		}
	}

	glPushMatrix();
	glTranslatef(-centerShift[0], -centerShift[1], -centerShift[2]);
	matr_blue.setMaterial();
	DrawOctahedron(0.005);
	glPopMatrix();

	glEndList();

	return dispList;
}

GLuint CMesh::MakeDisplayList_Shadow()
{
	GLuint dispList = glGenLists(1);

	int nSubmesh = mesh.materialList.nMaterials;

	typedef vector<DWORD> DWORDlist;
	vector<DWORDlist> faceList;
	vector<CMaterial> materialList;

	for(int i = 0;i < nSubmesh;i++){
		DWORDlist dlist;
		faceList.push_back(dlist);

		CMaterial mat;
		mat.diffuse[0] = mesh.materialList.materials[i].faceColor.red;
		mat.diffuse[1] = mesh.materialList.materials[i].faceColor.green;
		mat.diffuse[2] = mesh.materialList.materials[i].faceColor.blue;
		mat.diffuse[3] = mesh.materialList.materials[i].faceColor.alpha;

		mat.ambient[0] = 0;
		mat.ambient[1] = 0;
		mat.ambient[2] = 0;
		mat.ambient[3] = 0;

		mat.specular[0] = mesh.materialList.materials[i].specularColor.red;
		mat.specular[1] = mesh.materialList.materials[i].specularColor.green;
		mat.specular[2] = mesh.materialList.materials[i].specularColor.blue;
		mat.specular[3] = 1.0;

		mat.shininess = 0.2;

		materialList.push_back(mat);
	}

	for(int i = 0;i < mesh.materialList.nFaceIndexes; i++){
		DWORD matInd = mesh.materialList.faceIndexes[i];//マテリアル番号

		faceList[matInd].push_back(i);
	}


	glNewList(dispList, GL_COMPILE);

	glDisable(GL_LIGHTING);
	glColor4fv(dif_shadow);

	for(int m = 0;m < nSubmesh;m++){

		for(int k = 0;k < faceList[m].size(); k++){
			int i = faceList[m][k];
			int n = mesh.faces[i].nFaceVertexIndices;
			glBegin(GL_POLYGON);
			for(int j = 0;j < n; j++){
				glNormal3f(
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].x,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].y,
					mesh.normals.normals[mesh.normals.faceNormals[i].faceVertexIndices[j]].z
					);
				glVertex3f(
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].x,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].y,
					mesh.vertices[mesh.faces[i].faceVertexIndices[j]].z
					);
			}			
			glEnd();
		}
	}
	glEnable(GL_LIGHTING);
	glEndList();

	return dispList;
}




void CMesh::DrawShadow()
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!isValid)
	{
		return;
	}

	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(drop_shadow_mat);
	glMultMatrixd(position);
	glTranslatef(centerShift[0],centerShift[1],centerShift[2]);
	glCallList(dispList_shadow[nIndex]);
	glPopMatrix();

}

void CMesh::InitDisplayList()
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if (!dispList_init_flag[nIndex])
	{
		dispList_polygon[nIndex]	= MakeDisplayList_Polygon();
		dispList_wire[nIndex]		= MakeDisplayList_Wire();
		dispList_shadow[nIndex]		= MakeDisplayList_Shadow();

		dispList_init_flag[nIndex] = true;
	}
}

void CMesh::Draw(bool bWireFrame)
{
	if(!isValid)
	{
		return;
	}

	if(bDrawArrow)
	{
		DrawArrow(bWireFrame);
	}

	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}


	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(position);
	glTranslatef(centerShift[0],centerShift[1],centerShift[2]);
	if(bWireFrame)
		glCallList(dispList_wire[nIndex]);
	else
		glCallList(dispList_polygon[nIndex]);
	glPopMatrix();

}

void CMesh::DrawNoMesh(bool bWireFrame)
{
	return;

	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(position);
	glTranslatef(centerShift[0],centerShift[1],centerShift[2]);
	glPopMatrix();
}

void CMesh::DrawMassCenter(bool bWireFrame)
{
	if ( !bWireFrame)
	{
		return;
	}

	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(position);
	if(bWireFrame)
	{
		matr_blue.setMaterial();
		DrawSphere(0.005, false);
	}
	else
	{
//		auxSolidSphere(5);
//		auxSolidCone(5,50);
	}
	glPopMatrix();
}


void CMesh::DrawArrow(bool bWireFrame)
{
	if ( !bWireFrame)
	{
		return;
	}

	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(position);
	glTranslatef(centerShift[0],centerShift[1],centerShift[2]);
	if(bWireFrame)
	{
		matr_red.setMaterial();
		DrawCube(0.006);
		glPushMatrix();
		glTranslatef(0,0,0.010);
		DrawBox(0.003,0.003,0.020);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0,0,0.020);
		DrawCone(0.006,0.020);
		glPopMatrix();
	}
	else
	{
//		auxSolidSphere(5);
//		auxSolidCone(5,50);
	}
	glPopMatrix();
}

void CMesh::SkipTemplate(char* &str, int &index)
{
	while(table[str[index++]] != END_BRACE){
	}
}


void CMesh::Parse(char* str, int size)
{
	int index = 0;
	XHeader xheader;
	memcpy(&xheader, str, sizeof(xheader));
	index += sizeof(xheader);

	if(memcmp(xheader.magicNumber, "xof", 3))return;
	if(memcmp(xheader.formatType, "txt", 3))return;

	char tag[256];
	while(index < size){
		//tag読み
		while(table[str[index]] == NOP){
			index++;
			if(index < size)
				break;

		}

		int tagind = 0;
		while(table[str[index]] == AN){
			tag[tagind++] = str[index];
			index++;
		}
		tag[tagind] = 0;

		if(strcmp(tag, "template") == 0){
			SkipTemplate(str, index);
			continue;
		}

		if(strcmp(tag, "Header") == 0){
			header.Parse(str, index);
			continue;
		}

		if(strcmp(tag, "Mesh") == 0){
			mesh.Parse(str, index);
			continue;
		}
		index++;
	}

}
