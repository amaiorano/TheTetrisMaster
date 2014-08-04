#include "ASEMaterial.h"

ASEMaterial::ASEMaterial()
{
   m_fileName = NULL;
   m_texUTile = 0.0f;
   m_texVTile = 0.0f;
}

ASEMaterial::~ASEMaterial()
{
   delete [] m_fileName;
}