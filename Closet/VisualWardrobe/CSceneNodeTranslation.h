#pragma once

#include "CSceneNodeDecorator.h"
#include "ITranslate.h"
#include "mtxlib.h"

class CSceneNodeTranslation: public CSceneNodeDecorator, public ITranslate{//带有位置信息的Decorator
public:
	virtual void Render() const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	CSceneNodeTranslation(ISceneNode *pObj, const vector3 &translation):CSceneNodeDecorator(pObj),m_translation(translation){}
	virtual const vector3 GetTranslation();
	virtual void SetTranslation(const vector3 &translation);
private:
	vector3 m_translation;
	matrix44 m_parentTransform;
};
