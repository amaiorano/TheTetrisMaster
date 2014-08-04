#ifndef _ASE_OBJECT_H_
#define _ASE_OBJECT_H_

class ASEObject
{
   public:
      ASEObject();
      ~ASEObject();

      int m_numVertices;
      float* m_vertices;
      int m_numNormals;
      float* m_normals;
      int m_numColors;
      float* m_colors;
      int m_numTextureVertices;
      float* m_textureVertices;
      int m_numFaces;
      int* m_faces;
      int m_material;
};

#endif _ASE_OBJECT_H_