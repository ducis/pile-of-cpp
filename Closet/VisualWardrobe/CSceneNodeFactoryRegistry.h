#pragma once

#include <map>
#include <string>


class CSceneNodeFactoryRegistry{
public:
	//根据名字来查找SceneNode工厂类的类
	CSceneNodeFactoryRegistry();
	class ISceneNodeFactory *GetFactory( const std::wstring &name );//返回名字是name的工厂
private:	
	void Insert(class ISceneNodeFactory *pFac, const std::wstring &name );//添加一个name名字的工厂pFac
	typedef std::map<std::wstring,class ISceneNodeFactory *> CONTAINER;
	CONTAINER m_facs;
};

