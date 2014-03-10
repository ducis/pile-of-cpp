//#pragma once
//
//#include "ISceneNodeBoxy.h"
//#include <algorithm>
//#include <functional>
////
////template< class _Wrapper >
////class CSceneNodeBoxyWrapper: public ISceneNodeBoxy{
////public:
////	virtual void EnlargeAABB(AABB &aabb) const{ GetWrapped().EnlargeAABB(aabb); }
////	virtual void Render() const{ GetWrapped().Render(); }
////	virtual void OnInsertion(){}{ GetWrapped().OnInsertion(); }
////	virtual void UpdateTransform(const matrix44 &parentTransform){ GetWrapped().UpdateTransform(parentTransform); }
////	virtual void SetAABB(const AABB &aabb){ GetWrapped().SetAABB(aabb); }
////	virtual const AABB GetAABB() const{ return GetWrapped().GetAABB(); }
////private:
////	_Wrapper &GetWrapped(){
////		return const_cast<const _Wrapper*>(static_cast<_Wrapper*>(this))->GetWrappedSceneNodeBoxy();
////	}
////	const _Wrapper &GetWrapped() const{
////		return static_cast<const _Wrapper*>(this)->GetWrappedSceneNodeBoxy();
////	}
////};
//
//
//template< class _Wrapper >
//class CSceneNodeBoxyMultiWrapper: public ISceneNodeBoxy{
//public:
//	virtual void EnlargeAABB(AABB &aabb) const{
//		std::for_each(Begin(),End(),std::bind2nd(std::mem_fun(&ISceneNodeBoxy::EnlargeAABB),aabb));
//	}
//	virtual void Render() const{
//		std::for_each(Begin(),End(),std::mem_fun(&ISceneNodeBoxy::Render));
//	}
//	virtual void ISceneNodeBoxy::OnInsertion(){
//		std::for_each(Begin(),End(),std::mem_fun(&ISceneNodeBoxy::OnInsertion));
//	}
//	virtual void ISceneNodeBoxy::UpdateTransform(const matrix44 &parentTransform){
//		std::for_each(Begin(),End(),std::bind2nd(std::mem_fun(&ISceneNodeBoxy::UpdateTransform),parentTransform));
//	}
//	virtual void ISceneNodeBoxy::SetAABB(const AABB &aabb){
//		std::for_each(Begin(),End(),std::bind2nd(std::mem_fun
//	}
//	virtual const AABB GetAABB() const{
//		return Union(first_type::GetAABB(),second_type(GetAABB());
//	}
//private:
//};
