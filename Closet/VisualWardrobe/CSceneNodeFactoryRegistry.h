#pragma once

#include <map>
#include <string>


class CSceneNodeFactoryRegistry{
public:
	//��������������SceneNode���������
	CSceneNodeFactoryRegistry();
	class ISceneNodeFactory *GetFactory( const std::wstring &name );//����������name�Ĺ���
private:	
	void Insert(class ISceneNodeFactory *pFac, const std::wstring &name );//���һ��name���ֵĹ���pFac
	typedef std::map<std::wstring,class ISceneNodeFactory *> CONTAINER;
	CONTAINER m_facs;
};

