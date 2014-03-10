#include "CSimpleMaterial.h"
#include "ptree.h"
#include "singleton.h"
#include "CProgram.h"
#include "CTexture.h"

CSimpleMaterial::CSimpleMaterial(boost::property_tree::wptree &tr):
m_identifier(tr.get<std::wstring>(L"Identifier")),
m_pTex(GetSingleton<CProgram>()->GetTexture(tr.get<std::wstring>(L"Texture")))
{
}


void CSimpleMaterial::Apply() const{
	glEnable(GL_TEXTURE_2D);
	NTexture::Bind(*m_pTex);
	glDisable(GL_COLOR_MATERIAL);
}


const CIdentifier &CSimpleMaterial::GetIdentifier() const{
	return m_identifier;
}
