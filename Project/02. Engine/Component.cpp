#include "Component.h"

CComponent::CComponent()
{
}

CComponent::~CComponent()
{
}

void CComponent::Save(FILE * _pFile)
{
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
	fwrite(&m_bActive, sizeof(bool), 1, _pFile);	
}

void CComponent::Load(FILE * _pFile)
{
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	fread(&m_bActive, sizeof(bool), 1, _pFile);
}
