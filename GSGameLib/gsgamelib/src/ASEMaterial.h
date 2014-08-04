#ifndef _ASE_MATERIAL_H_
#define _ASE_MATERIAL_H_

#include <assert.h>
#include <string.h>

class ASEMaterial
{
   public:
      ASEMaterial();
      ~ASEMaterial();

      inline void FileName(const char* fileName);
      inline char* FileName() const;
      inline char* ShortFileName() const;

      float m_texUTile;
      float m_texVTile;

   private:
      char* m_fileName;
};

inline void ASEMaterial::FileName(const char* fileName)
{
   assert(!m_fileName);

   m_fileName = new char [strlen(fileName) + 1];
   m_fileName[0] = '\0';

   strcpy(m_fileName, fileName);
}

inline char* ASEMaterial::FileName() const
{
   if(m_fileName)
      return m_fileName;
   else
      return NULL;
}

inline char* ASEMaterial::ShortFileName() const
{
   if(m_fileName)
   {
      char* shortFileName = strrchr(m_fileName, '\\');
      if(shortFileName)
         return shortFileName + 1;
      else
         return m_fileName;
   }
   else
   {
      return NULL;
   }
}

#endif _ASE_MATERIAL_H_