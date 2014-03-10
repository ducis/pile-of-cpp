#pragma once

#include "IPickable.h"
#include "IDraggable.h"

template<
	class _Class, 
	float (_Class::*_PickFuncPtr)(const vector2 &, const matrix44 &) const, 
	void (_Class::*_VisualizePickFuncPtr)() const, 
	const vector2 (_Class::*_DragFuncPtr)(const vector2 &, const matrix44 &viewProj)
>
class DragHandle:public IDraggable,public IPickable{
	//用于将没有IDraggable和IPickable接口的对象包装起来并提供IDraggable和IPickable的模版类,
	//一个类可以被多个DragHandle包装
public:
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj){
		return (m_pDelegate->*_DragFuncPtr)
			(displacementHS,viewProj);
	}
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const{
		return (const_cast<const _Class*>(m_pDelegate)->*_PickFuncPtr)(posHS,viewProj);
	}
	virtual void VisualizePick() const{
		(const_cast<const _Class*>(m_pDelegate)->*_VisualizePickFuncPtr)();
	}
	explicit DragHandle(_Class *pDelegate):m_pDelegate(pDelegate){}
private:
	_Class *m_pDelegate;
};
