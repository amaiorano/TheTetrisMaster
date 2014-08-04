#include "ASELoader.h"

#include <math.h>
#include <string>
#include "GLHelper.h" // TODO: remove
#include "ImageData.h"
#include "Renderer.h"
#include "ASEObject.h"
#include "ASEMaterial.h"

const float FLOAT_MIN = 1.175494351E-38f;
const float FLOAT_MAX = 3.402823466E+38f;

const char* NUMVERTEX		= "*MESH_NUMVERTEX";
const char* NUMFACES 		= "*MESH_NUMFACES";
const char* NUMTVERTEX		= "*MESH_NUMTVERTEX";
const char* NUMTVFACES		= "*MESH_NUMTVFACES";
const char* NUMCVERTEX		= "*MESH_NUMCVERTEX";
const char* VERTEX_LIST 	= "*MESH_VERTEX_LIST";
const char* VERTEX			= "*MESH_VERTEX";
const char* FACE_LIST		= "*MESH_FACE_LIST";
const char* FACE			= "*MESH_FACE";
const char* CVERTLIST		= "*MESH_CVERTLIST";
const char* VERTCOL			= "*MESH_VERTCOL";
const char* NORMALS			= "*MESH_NORMALS";
const char* FACENORMAL		= "*MESH_FACENORMAL";
const char* VERTEXNORMAL	= "*MESH_VERTEXNORMAL";
const char* TVERT 			= "*MESH_TVERT";
const char* TFACE 			= "*MESH_TFACE";
const char* TEXTURE_BITMAP = "*BITMAP";
const char* MATERIAL 		= "*MATERIAL";
const char* MATERIAL_COUNT = "*MATERIAL_COUNT";
const char* MATERIAL_REF	= "*MATERIAL_REF";
const char* UVW_U_TILING	= "*UVW_U_TILING";
const char* UVW_V_TILING	= "*UVW_V_TILING";
const char* UVW_U_OFFSET	= "*UVW_U_OFFSET";
const char* UVW_V_OFFSET	= "*UVW_V_OFFSET";
const char* GEOMOBJECT		= "*GEOMOBJECT";

const int MAX_DATA_LENGTH = 1024;

ASELoader::ASELoader()
{
	Reset();

	// State variables
	m_colorOverride = false;
}

ASELoader::~ASELoader()
{
	delete [] m_objects;
	delete [] m_materials;
}

void ASELoader::Reset()
{
	m_numObjects = 0;
	m_objects = NULL;
	m_numMaterials = 0;
	m_materials = NULL;
	m_currObject = -1;
	m_averagedNormals = false;
	m_filePath = NULL;	 
}

ASEInfo* ASELoader::Load(const char* fileName, bool averagedNormals /* = false */)
{
	Reset();

	// Rip the path from the fileName for later use
	std::string fileString(fileName);
	size_t pathEndIndex = fileString.rfind("/");
	if(pathEndIndex <= 0)
		 pathEndIndex = fileString.rfind("\\");
	
	if(pathEndIndex > 0)
	{
		// A path has been supplied, so let's save it
		size_t pathLength = pathEndIndex+1;
		m_filePath = new char [pathLength+1]; // Add 1 for \0
		strncpy(m_filePath, fileName, pathLength);
		m_filePath[pathLength] = '\0';
	}

	m_averagedNormals = averagedNormals;

	m_file = fopen(fileName, "r");
	if(!m_file)
	{
		ASEInfo *pAseInfo = new ASEInfo;
		pAseInfo->listName = -1;
		pAseInfo->size = -1;

		return pAseInfo;
	}

	LoadMaterials();
	LoadObjects();

	fclose(m_file);

	return CreateDisplayList();
}

void ASELoader::ColorOverride(const float r, const float g, const float b, const float a)
{
	m_colorOverride = true;
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;
}

void ASELoader::NoColorOverride()
{
	m_colorOverride = false;
}

int ASELoader::LoadMaterials()
{
	char data[MAX_DATA_LENGTH] = {0};

	int currMaterial = -1;

	// Scan the ASE file.
	while(!feof(m_file))
	{
		// Read the first delimited string on the current line.
		ReadString(&data);

		if(!strcmp(data, MATERIAL_COUNT))
		{
			ReadInt(&m_numMaterials);
			m_materials = new ASEMaterial [m_numMaterials];
		}
		else if(!strcmp(data, MATERIAL))
		{
			++currMaterial;
		}
		else if(!strcmp(data, TEXTURE_BITMAP))
		{
			char textureBitmapName[255] = {0};
			ReadString(&textureBitmapName);

			// Remove the last double quote by replacing it by a NULL character.
			size_t doubleQuoteIndex = strrchr(textureBitmapName, '\"') - textureBitmapName;
			textureBitmapName[doubleQuoteIndex] = '\0';

			assert(m_materials);
			m_materials[currMaterial].FileName(&(textureBitmapName[1]));
		}
		else if(!strcmp(data, UVW_U_TILING))
		{
			ReadFloat(&(m_materials[currMaterial].m_texUTile));
		}
		else if(!strcmp(data, UVW_V_TILING))
		{
			ReadFloat(&(m_materials[currMaterial].m_texVTile));
		}
		else
		{
			fgets(data, MAX_DATA_LENGTH, m_file);
		}
	}

	return 0;
}

int ASELoader::LoadObjects()
{
	char data[MAX_DATA_LENGTH] = {0};

	rewind(m_file);

	while(!feof(m_file))
	{
		ReadString(&data);

		if(!strcmp(data, GEOMOBJECT))
			++m_numObjects;
		else
			fgets(data, 255, m_file);
	}

	rewind(m_file);

	m_objects = new ASEObject* [m_numObjects];

	while(!feof(m_file))
	{
		ReadString(&data);

		if(!strcmp(data, GEOMOBJECT))
		{
			m_objects[++m_currObject] = new ASEObject();
		}
		else if(!strcmp(data, NUMVERTEX))
		{
			int val = -1;
			ReadInt(&val);
			m_objects[m_currObject]->m_numVertices = val;
			if(val)
				m_objects[m_currObject]->m_vertices = new float [3 * val];
		}
		else if(!strcmp(data, NUMCVERTEX))
		{
			int val = -1;
			ReadInt(&val);
			m_objects[m_currObject]->m_numColors = val;
			if(val)
				m_objects[m_currObject]->m_colors = new float [3 * val];
		}
		else if(!strcmp(data, NUMTVERTEX))
		{
			int val = -1;
			ReadInt(&val);
			m_objects[m_currObject]->m_numTextureVertices = val;
			if(val)
				m_objects[m_currObject]->m_textureVertices = new float [2 * val];
		}
		else if(!strcmp(data, NUMFACES))
		{
			int val = -1;
			ReadInt(&val);
			m_objects[m_currObject]->m_numFaces = val;
			if(val)
				m_objects[m_currObject]->m_faces = new int [3 * val];
		}
		else if(!strcmp(data, NORMALS))
		{
			int val = m_objects[m_currObject]->m_numFaces;
			m_objects[m_currObject]->m_numNormals = val;
			if(val)
				m_objects[m_currObject]->m_normals = new float [3 * val];
		}
		else
		{
			fgets(data, 255, m_file);
		}
	}

	rewind(m_file);
	m_currObject = -1;

	while(!feof(m_file))
	{
		ReadString(&data);

		if(!strcmp(data, GEOMOBJECT))
			++m_currObject;
		else if(!strcmp(data, VERTEX))
			LoadVertex();
		else if(!strcmp(data, TVERT))
			LoadTextureVertex();
		else if(!strcmp(data, FACE))
			LoadFace();
		else if(!strcmp(data, VERTCOL))
			LoadColorVertex();
		else if(!strcmp(data, MATERIAL_REF))
			ReadInt(&(m_objects[m_currObject]->m_material));
		else if(!strcmp(data, FACENORMAL))
			LoadNormal();
		else
			fgets(data, 255, m_file);
	}

	// Average and normalize the normal values if desired.
	if(m_averagedNormals)
	{
		for(int objIndex = 0; objIndex < m_numObjects; ++objIndex)
		{
			ASEObject* object = m_objects[objIndex];

			float* newNormals = new float [3 * object->m_numVertices];

			for(int i = 0; i < object->m_numVertices; ++i)
			{
				float xSum = 0.0f;
				float ySum = 0.0f;
				float zSum = 0.0f;
				int count = 0;

				for(int j = 0; j < (3 * object->m_numFaces); ++j)
				{
					if(object->m_faces[j] == i)
					{
						xSum += object->m_normals[3 * (j / 3) + 0];
						ySum += object->m_normals[3 * (j / 3) + 1];
						zSum += object->m_normals[3 * (j / 3) + 2];
						++count;
					}
				}

				float nx = xSum / count;
				float ny = ySum / count;
				float nz = zSum / count;

				float norm = (float)sqrt((nx * nx) + (ny * ny) + (nz * nz));

				newNormals[(3 * i) + 0] = nx / norm;
				newNormals[(3 * i) + 1] = ny / norm;
				newNormals[(3 * i) + 2] = nz / norm;
			}

			delete [] object->m_normals;
			object->m_normals = newNormals;
			object->m_numNormals = object->m_numVertices;
		}
	}

	// Multiply the texture vertices by the tile values of the appropriate material.
	for(int objIndex = 0; objIndex < m_numObjects; ++objIndex)
	{
		ASEObject* object = m_objects[objIndex];

		if(object->m_material == -1)
			 continue;

		float texUTile = m_materials[object->m_material].m_texUTile;
		float texVTile = m_materials[object->m_material].m_texVTile;

		float* textureVertices = object->m_textureVertices;

		if(texUTile)
		{
			for(int i = 0; i < object->m_numTextureVertices; ++i)
				textureVertices[(2 * i) + 0] *= texUTile;
		}

		if(texVTile)
		{
			for(int i = 0; i < object->m_numTextureVertices; ++i)
				textureVertices[(2 * i) + 1] *= texVTile;
		}
	}

	return 0;
}

ASEInfo* ASELoader::CreateDisplayList()
{
	float xMin = FLOAT_MAX;
	float xMax = -FLOAT_MAX;
	float yMin = FLOAT_MAX;
	float yMax = -FLOAT_MAX;
	float zMin = FLOAT_MAX;
	float zMax = -FLOAT_MAX;

	GLuint listName = glGenLists(1);

	
	glNewList(listName, GL_COMPILE);

	glPushAttrib(GL_LIGHTING_BIT);
//	glPushAttrib(GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

	// Loop through all objects.
	for(int i = 0; i < m_numObjects; ++i)
	{
		ASEObject* object = m_objects[i];

		int* faces = object->m_faces;
		float* vertices = object->m_vertices;
		float* normals = object->m_normals;
		float* textureVertices = NULL;
		float* colors = NULL;


 		if ( m_colorOverride )
		{
			glColor4fv(m_color);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			textureVertices = object->m_textureVertices;
			colors = object->m_colors;
		}


		const int TEXTURE_LOAD_ERROR = -1;
		char* textureFileName = NULL;
		GLuint textureId = TEXTURE_LOAD_ERROR;

		if(textureVertices && (m_objects[i]->m_material != -1))
		{
			textureFileName = m_materials[m_objects[i]->m_material].ShortFileName();

			if(textureFileName)
			{
				// If there is a texture file, load it and enable texturing.				

				// Prepend the path, if necessary
				if (m_filePath)
				{
					size_t length = strlen(textureFileName) + strlen(m_filePath);
					char *tempString = new char [length + 1];
					strcpy(tempString, m_filePath);
					strcat(tempString, textureFileName);

					ImageData imgData = ImageData::LoadTGA(tempString);
					textureId = Renderer::Instance().LoadTexture(imgData);
					//	LoadTexture(tempString);

					delete [] tempString;
				}
				else
				{
					Renderer::Instance().LoadTexture(ImageData::LoadTGA(textureFileName));
					//textureId = LoadTexture(textureFileName);
				}

				assert(textureId != TEXTURE_LOAD_ERROR);
			}
		}

		// At the minimum, we need faces and vertices to draw.
		/*assert(faces); */ // Disabling this assert - no faces will still work properly (dead object?)
		assert(vertices);

		// Make sure lighting is disabled (state will be restored with glPopAttrib() below)
		glDisable(GL_LIGHTING);
 
		if ( textureVertices )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
		
		// Draw each face of the object.
		glBegin(GL_TRIANGLES);
			for(int j = 0; j < object->m_numFaces; ++j)
			{
				for(int k = 0; k < 3; ++k)
				{
					int vertex = faces[(3 * j) + k];

					if(colors)
						glColor3fv(&colors[3 * vertex]);

					if(normals)
					{
						if(m_averagedNormals)
							glNormal3fv(&normals[3 * vertex]);
						else
							glNormal3fv(&normals[3 * j]);
					}

					if(textureVertices)
						glTexCoord2fv(&textureVertices[2 * vertex]);

					if(vertices)
						glVertex3fv(&vertices[3 * vertex]);
				}
			}
		glEnd();


		if ( m_colorOverride )
		{
			glDisable(GL_BLEND);
		}
		else if ( !textureVertices )
		{
			glEnable(GL_TEXTURE_2D);
		}

		// Find maximum and minimum x, y, and z values among all
		// 	geometric objects.
		for(int j = 0; j < object->m_numVertices; ++j)
		{
			float x = object->m_vertices[(3 * j) + 0];
			float y = object->m_vertices[(3 * j) + 0];
			float z = object->m_vertices[(3 * j) + 0];

			if(x < xMin)
				xMin = x;
			else if(x > xMax)
				xMax = x;

			if(y < yMin)
				yMin = y;
			else if(y > yMax)
				yMax = y;

			if(z < zMin)
				zMin = z;
			else if(z > zMax)
				zMax = z;
		}
	} // end for all objects


	glPopAttrib();
	glEndList();



	// From the max and min x, y, and z values, compute the largest
	// 	sizes in x, in y, and in z.
	float xSize = xMax - xMin;
	float ySize = yMax - yMin;
	float zSize = zMax - zMin;

	// The size of the object is then the largest of these three values.
	float size = xSize;
	if(size < ySize)
		size = ySize;
	if(size < zSize)
		size = zSize;

	// Return a structure containing the name of the display list to use
	// 	to draw the object in the ASE file and the size of the object.
	ASEInfo* pAseInfo = new ASEInfo;
	pAseInfo->listName = listName;
	pAseInfo->size = size;

	return pAseInfo;
}

void ASELoader::LoadVertex()
{
	float* vertices = m_objects[m_currObject]->m_vertices;

	int index = -1;
	ReadInt(&index);

	// The second value is actually z and the third is y.
	ReadFloat(&vertices[(index * 3) + 0]);
	ReadFloat(&vertices[(index * 3) + 2]);
	ReadFloat(&vertices[(index * 3) + 1]);

	// Negative z goes out of the screen in 3D Studio Max, we want it to go in.
	vertices[(index * 3) + 2] = -vertices[(index * 3) + 2];
}

void ASELoader::LoadFace()
{
	int* faces = m_objects[m_currObject]->m_faces;

	int index = -1;
	ReadInt(&index);

	fscanf(m_file, ":\tA:\t%d B:\t%d C:\t%d", &faces[(index * 3) + 0],
														&faces[(index * 3) + 1],
														&faces[(index * 3) + 2]);
}

void ASELoader::LoadTextureVertex()
{
	float* textureVertices = m_objects[m_currObject]->m_textureVertices;

	int index = -1;
	ReadInt(&index);

	ReadFloat(&textureVertices[(index * 2) + 0]);
	ReadFloat(&textureVertices[(index * 2) + 1]);
}

void ASELoader::LoadColorVertex()
{
	float* colorVertices = m_objects[m_currObject]->m_colors;

	int index = -1;
	ReadInt(&index);

	// The second value is actually z and the third is y.
	ReadFloat(&colorVertices[(index * 3) + 0]);
	ReadFloat(&colorVertices[(index * 3) + 1]);
	ReadFloat(&colorVertices[(index * 3) + 2]);
}

void ASELoader::LoadNormal()
{
	float* normals = m_objects[m_currObject]->m_normals;

	int face = -1;
	ReadInt(&face);

	ReadFloat(&normals[(face * 3) + 0]);
	ReadFloat(&normals[(face * 3) + 1]);
	ReadFloat(&normals[(face * 3) + 2]);
}

void ASELoader::ReadString(void* location)
{
	assert(!feof(m_file));
	fscanf(m_file, "%s", location);
}

void ASELoader::ReadInt(void* location)
{
	assert(!feof(m_file));
	fscanf(m_file, "%d", location);
}

void ASELoader::ReadFloat(void* location)
{
	assert(!feof(m_file));
	fscanf(m_file, "%f", location);
}