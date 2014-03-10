#pragma once
//һЩ�����3D�Ķ���
#include "windowsnominmax.h"
#include <gl/gl.h>
#include <glut.h>
#include "mtxlib.h"
#include <utility>
typedef std::pair<vector3,vector3> AABB;//��������ƽ�еĳ�����,first�Ǹ������ϵ���Сֵ,second�Ǹ������ϵ����ֵ
#include "srl.h"
#include "MyUtilities.h"
#include <algorithm>

inline void SetMinimumAABB(AABB &aabb){
	aabb.first.x = aabb.first.y = aabb.first.z = 10000000.0f;
	aabb.second.x = aabb.second.y = aabb.second.z = -10000000.0f;
}

inline const AABB MinimumAABB(){
	return AABB(vector3(10000000.0f,10000000.0f,10000000.0f),vector3(-10000000.0f,-10000000.0f,-10000000.0f));
}

inline void Enlarge(AABB &aabb,const vector3 &v){//����aabbʹ�����v��
	for(int i=0;i<3;++i){
		if(v[i]<aabb.first[i]){
			aabb.first[i] = v[i];
		}
		if(v[i]>aabb.second[i]){
			aabb.second[i] = v[i];
		}
	}
}

inline void Enlarge(AABB &aabb,const AABB &rhs){//����aabbʹ�����rhs
	for(int i=0;i<3;++i){
		if(rhs.first[i]<aabb.first[i]){
			aabb.first[i] = rhs.first[i];
		}
		if(rhs.second[i]>aabb.second[i]){
			aabb.second[i] = rhs.second[i];
		}
	}
}

inline const vector3 GetDim(const AABB &aabb){//����aabb�ĳߴ�
	return aabb.second-aabb.first;
}

inline float GetDim(const AABB &aabb,int iAxis){//����aabb��ĳһ�����ϵĳߴ�,0:x 1:y 2:z
	return aabb.second[iAxis]-aabb.first[iAxis];
}


inline const vector3 GetCenter(const AABB &aabb){//����aabb������
	return 0.5*(aabb.second+aabb.first);
}


inline const float GetCenter(const AABB &aabb, int iAxis){//����aabb��ĳһ�����ϵ�����
	return 0.5f*(aabb.first[iAxis]+aabb.second[iAxis]);
}


inline bool Equal(const AABB &lhs, const AABB &rhs, int iAxis){//�ж�lhs��rhs��iAxis���������(0x1y2z)�ϵ�ͶӰ�Ƿ�������
	const float epsilon = 0.001;
	return abs(lhs.first[iAxis]-rhs.first[iAxis])<epsilon && abs(lhs.second[iAxis]-rhs.second[iAxis])<epsilon;
}


inline const AABB AABBFromTwoPoints(const vector3 &a, const vector3 &b){//���ذ���a,b�������СAABB
	using namespace std;
	return AABB(vector3(min(a.x,b.x),min(a.y,b.y),min(a.z,b.z)) , vector3(max(a.x,b.x),max(a.y,b.y),max(a.z,b.z)));
}

inline const AABB AABBFromPoints(
								 const vector3 &p0, 								 
								 const vector3 &p1, 
								 const vector3 &p2, 
								 const vector3 &p3, 
								 const vector3 &p4, 
								 const vector3 &p5, 
								 const vector3 &p6,
								 const vector3 &p7
								 )
{//���ذ���p0��p7��˸������СAABB
	AABB r(p0,p0);
	Enlarge(r,p1);
	Enlarge(r,p2);
	Enlarge(r,p3);
	Enlarge(r,p4);
	Enlarge(r,p5);
	Enlarge(r,p6);
	Enlarge(r,p7);
	return r;
}


inline bool Contains(const AABB &aabb, const vector3 &point){//����aabb(������)�Ƿ����point
	for(int i=0;i<3;++i){
		if(!MyUtilities::IsInClosedRange(point[i],aabb.first[i],aabb.second[i])) return false;
	}
	return true;
}


inline bool OpenAABBContains(const AABB &aabb, const vector3 &point){//����aabb(������)�Ƿ����point
	for(int i=0;i<3;++i){
		if(!MyUtilities::IsInOpenRange(point[i],aabb.first[i],aabb.second[i])) return false;
	}
	return true;
}


inline bool ClosedAABBContains(const AABB &aabb, const vector3 &point){//����aabb(������)�Ƿ����point
	for(int i=0;i<3;++i){
		if(!MyUtilities::IsInClosedRange(point[i],aabb.first[i],aabb.second[i])) return false;
	}
	return true;
}


inline bool IsValid(const AABB &aabb){//����aabb�Ƿ����(��СֵС�ڵ������ֵ)
	return aabb.first.x<=aabb.second.x && aabb.first.y<=aabb.second.y && aabb.first.z<=aabb.second.z;
}


inline const vector3 Clamped(const AABB &aabb, const vector3 &p){//���ؽ�p������ľ����Ƶ�aabb�ڲ��Ľ��
	vector3 r(p);
	for(int i=0;i<3;++i){
		MyUtilities::Clamp( r[i], aabb.first[i], aabb.second[i] );
	}
	return r;
}


inline const AABB operator*(const matrix44 &m, const AABB &aabb){//��aabb��������任���任��Ľ�������任ǰ�Ľ��
	//return AABBFromTwoPoints((m*vector4(aabb.first,1.0f)).xyz(), (m*vector4(aabb.second,1.0f)).xyz());
	return AABBFromPoints(
		(m*vector4(aabb.first.x,aabb.first.y,aabb.first.z,1.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.first.y,aabb.second.z,1.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.second.y,aabb.first.z,1.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.second.y,aabb.second.z,1.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.first.y,aabb.first.z,1.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.first.y,aabb.second.z,1.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.second.y,aabb.first.z,1.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.second.y,aabb.second.z,1.0f)).xyz()
		);
}


inline const AABB TransformWithoutTranslation(const matrix44 &m, const AABB &aabb){//��aabb��������任����ȥ����m�е�ƽ�ƣ��任��Ľ�������任ǰ�Ľ��
	//return AABBFromTwoPoints((m*vector4(aabb.first,0.0f)).xyz(), (m*vector4(aabb.second,0.0f)).xyz());
	return AABBFromPoints(
		(m*vector4(aabb.first.x,aabb.first.y,aabb.first.z,0.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.first.y,aabb.second.z,0.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.second.y,aabb.first.z,0.0f)).xyz(),
		(m*vector4(aabb.first.x,aabb.second.y,aabb.second.z,0.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.first.y,aabb.first.z,0.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.first.y,aabb.second.z,0.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.second.y,aabb.first.z,0.0f)).xyz(),
		(m*vector4(aabb.second.x,aabb.second.y,aabb.second.z,0.0f)).xyz()
		);
}


inline const AABB Union(const AABB &a , const AABB &b){//AABB�Ĳ�
	AABB aabb(a);
	Enlarge(aabb,b);
	return aabb;
}

inline const AABB operator+(const AABB &lhs, const vector3 &rhs){//lhsƽ��rhs
	return AABB(lhs.first+rhs,lhs.second+rhs);
}

inline const AABB operator+(const AABB &lhs, const AABB &rhs){//lhs��min,max�ֱ�ƽ��
	return AABB(lhs.first+rhs.first,lhs.second+rhs.second);
}

inline const AABB operator-(const AABB &lhs, const vector3 &rhs){//lhsƽ��-rhs
	return AABB(lhs.first-rhs,lhs.second-rhs);
}

inline bool operator!=(const AABB &a, const AABB &b){
	return a.first!=b.first||a.second!=b.second;
}

template< typename V > bool IsNormalized( const V &a ){
	return abs(a.lengthSqr()-1)<0.00001;
}

template< typename V > bool NormalizedVectorsAreInTheSameDirection( const V &a, const V &b){//��a,b��ģ��Ϊ1��������ж��������Ƿ�ƽ��ͬ��
	return abs(DotProduct(a,b)-1)<0.00001;
}

template< typename V > bool AreNormalizedAndInTheSameDirection( const V &a, const V &b ){//�ж��������Ƿ�ģ��Ϊһ��ƽ��ͬ��
	return IsNormalized(a)&&IsNormalized(b)&&NormalizedVectorsAreInTheSameDirection(a,b);
}

template<typename T> inline bool Intersect(const std::pair<T,T> &a, const std::pair<T,T> &b){//�ж������Ƿ��ཻ��Ҫ��a,bΪ�����ҿ��д�С
	return a.first<b.second && b.first<a.second;
}
template<> inline bool Intersect( const std::pair<vector2,vector2> &a, const std::pair<vector2,vector2> &b){//�ж�������ƽ�еľ����Ƿ��ཻ
	return Intersect(std::make_pair(a.first.x,a.second.x),std::make_pair(b.first.x,b.second.x))
		&& Intersect(std::make_pair(a.first.y,a.second.y),std::make_pair(b.first.y,b.second.y));
}
template<> inline bool Intersect( const std::pair<vector3,vector3> &a, const std::pair<vector3,vector3> &b){//�ж�������ƽ�е��������Ƿ��ཻ
	return Intersect(std::make_pair(a.first.x,a.second.x),std::make_pair(b.first.x,b.second.x))
		&& Intersect(std::make_pair(a.first.y,a.second.y),std::make_pair(b.first.y,b.second.y))
		&& Intersect(std::make_pair(a.first.z,a.second.z),std::make_pair(b.first.z,b.second.z));
}

inline const std::pair<vector2,vector2> MakeRect(const vector2 a, const vector2 b){//���ذ���a,b����С������ƽ�о���
	std::pair<vector2,vector2> r(a,b);
	if(r.first.x>r.second.x){
		std::swap(r.first.x,r.second.x);
	}
	if(r.first.y>r.second.y){
		std::swap(r.first.y,r.second.y);
	}
	return r;
}

inline const AABB UnitAABB(){
	return AABB(vector3(-0.5f,-0.5f,-0.5f),vector3(0.5f,0.5f,0.5f));
}

inline bool RoughlyIn(const AABB &sub, const AABB &obj, float epsilon){//����sub�Ƿ����obj,����epsilon�����
	return sub.second.x < obj.second.x + epsilon
		&& sub.second.y < obj.second.y + epsilon
		&& sub.second.z < obj.second.z + epsilon
		&& sub.first.x > obj.first.x - epsilon
		&& sub.first.y > obj.first.y - epsilon
		&& sub.first.z > obj.first.z - epsilon;
}
#define PI 3.1415926535897932384626433832795
