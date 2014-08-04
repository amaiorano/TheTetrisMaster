#include "ASEObject.h"

#define NULL 0

ASEObject::ASEObject()
{
   m_vertices = NULL;
   m_normals = NULL;
   m_colors = NULL;
   m_textureVertices = NULL;
   m_faces = NULL;
   m_numVertices = 0;
   m_numFaces = 0;
   m_material = -1;
}

ASEObject::~ASEObject()
{
   delete [] m_vertices;
   delete [] m_normals;
   delete [] m_colors;
   delete [] m_textureVertices;
   delete [] m_faces;
}
