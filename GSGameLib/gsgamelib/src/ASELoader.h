#ifndef _ASE_LOADER_H_
#define _ASE_LOADER_H_

#include <stdio.h>

class ASEObject;
class ASEMaterial;

struct ASEInfo
{
   int listName;
   float size;
};

class ASELoader
{
   public:
      ASELoader();
      ~ASELoader();

      ASEInfo* Load(const char* fileName, bool averagedNormals = false);
      void ColorOverride(const float r, const float g, const float b, const float a);
	  void NoColorOverride();

   private:
      void Reset();
      int LoadMaterials();
      int LoadObjects();
      ASEInfo* CreateDisplayList();

      void LoadVertex();
      void LoadFace();
      void LoadTextureVertex();
      void LoadColorVertex();
      void LoadNormal();

      void ReadString(void* location);
      void ReadInt(void* location);
      void ReadFloat(void* location);

      ASEObject** m_objects;
      int m_numObjects;
      int m_currObject;
      ASEMaterial* m_materials;
      int m_numMaterials;

      bool m_averagedNormals;

      FILE* m_file;
	  char *m_filePath;

      float m_color[4];
      bool m_colorOverride;
};

#endif _ASE_LOADER_H_