#pragma once

#include <OIS.h>
#include <CEGUI.h>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/fusion/include/vector_tie.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/include/for_each.hpp>

//#include <WRAP_TMP_PROC/boost_range.hpp>

class OISSystem{
private:
	OIS::InputManager *m_pIM;
	std::vector<OIS::Keyboard *> m_keyboards;
	std::vector<OIS::Mouse *> m_mice;
	void CreateDevices(){
		//for(int i=m_pIM->numKeyBoards();i;--i){
		if(m_pIM->getNumberOfDevices(OIS::OISKeyboard)){
			m_keyboards.push_back(dynamic_cast<OIS::Keyboard *>( m_pIM->createInputObject(OIS::OISKeyboard,true) ));
			assert(m_keyboards.back());
		}
		if(m_pIM->getNumberOfDevices(OIS::OISMouse)){
			m_mice.push_back(dynamic_cast<OIS::Mouse*>( m_pIM->createInputObject( OIS::OISMouse, true ) ));
			assert(m_mice.back());
		}
	}
public:
	OISSystem(Ogre::RenderWindow *pWindow)
	{
		std::size_t windowHandle=0;
		pWindow->getCustomAttribute("WINDOW",&windowHandle);
		assert(windowHandle);
		m_pIM = OIS::InputManager::createInputSystem(windowHandle);
		CreateDevices();
		unsigned int width, height, depth;
		int top, left;
		pWindow->getMetrics(width, height, depth, left, top);
		BOOST_FOREACH( OIS::Mouse *p, m_mice )
		{
			p->getMouseState().width = width;
			p->getMouseState().height = height;
		}
	}

	const std::vector<OIS::Keyboard *> &Keyboards(){ return m_keyboards; }
	const std::vector<OIS::Mouse *> &Mice(){ return m_mice; }
	~OISSystem(){
		using namespace boost;
		//using namespace boost::bind;
		//function template wrapper needed again;
		//fusion::for_each
		//(
		//	fusion::vector_tie(m_mice,m_keyboards),
		//	bind
		//	( 
		//		range::for_each_PROC_OBJ_2() , 
		//		_1 , bind(mem_fun(&OIS::InputManager::destroyInputObject),m_pIM,_1)
		//	)
		//);
		//bind( &range::for_each< std::vector<OIS::Mouse *> >, _1 , bind(mem_fn(&OIS::InputManager::destroyInputObject),m_pIM,_1) )	(m_mice);
		//range::for_each_PROC_OBJ_2()(m_mice,bind(mem_fn(&OIS::InputManager::destroyInputObject),m_pIM,_1));
		range::for_each(m_mice,bind(mem_fn(&OIS::InputManager::destroyInputObject),m_pIM,_1));
		range::for_each(m_keyboards,bind(mem_fn(&OIS::InputManager::destroyInputObject),m_pIM,_1));
		OIS::InputManager::destroyInputSystem(m_pIM);
	}
};

//template<typename _OISDevice,typename _Target>
//class OISInputRouter;
//
//template<>
//class OISInputRouter<OIS::Mouse,CEGUI::System>: public OIS::MouseListener
//{
//public:
//	
//};

template<typename _Dest, typename _Src> _Dest TranslateEnum(_Src s);

template<>
CEGUI::MouseButton TranslateEnum(OIS::MouseButtonID s){
	return (CEGUI::MouseButton)s;
}


class OIS2CEGUIMouseListener: public OIS::MouseListener
{
	CEGUI::System &m_cegui;
public:
	virtual bool 	mouseMoved (const OIS::MouseEvent &arg){
		//m_cegui.injectMousePosition(arg.state.X.abs,arg.state.Y.abs);
		m_cegui.injectMouseMove(arg.state.X.rel,arg.state.Y.rel);
		m_cegui.injectMouseWheelChange(arg.state.Z.rel);
		return true;
	}
	virtual bool 	mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id){
		m_cegui.injectMouseButtonDown(TranslateEnum<CEGUI::MouseButton>(id));
		return true;
	}
	virtual bool 	mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id){
		m_cegui.injectMouseButtonUp(TranslateEnum<CEGUI::MouseButton>(id));
		return true;
	}
	OIS2CEGUIMouseListener(CEGUI::System &cegui):m_cegui(cegui){}
};


class OIS2CEGUIKeyListener: public OIS::KeyListener
{
	CEGUI::System &m_cegui;
public:
	virtual bool 	keyPressed (const OIS::KeyEvent &arg){
		m_cegui.injectKeyDown(arg.key);
		return true;
	}
	virtual bool 	keyReleased (const OIS::KeyEvent &arg){
		m_cegui.injectKeyUp(arg.key);
		return true;
	}
	OIS2CEGUIKeyListener(CEGUI::System &cegui):m_cegui(cegui){}
};

template<typename _OISDevice>
struct GetOISDeviceListenerInterface;

template<> struct GetOISDeviceListenerInterface<OIS::Mouse>{
	typedef OIS::MouseListener Type;
};
template<> struct GetOISDeviceListenerInterface<OIS::Keyboard>{
	typedef OIS::KeyListener Type;
};

template<typename _OISDevice,typename _CopyableListener>
void RouteOISUserInput( _OISDevice &ois, _CopyableListener l)
{
	assert(ois.buffered());
	typename GetOISDeviceListenerInterface<_OISDevice>::Type *p = ois.getEventCallback();
	ois.setEventCallback(&l);
	ois.capture();
	ois.setEventCallback(p);
}
