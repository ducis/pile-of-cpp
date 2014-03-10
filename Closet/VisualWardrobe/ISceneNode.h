#pragma once

#include "3d.h"

class ISceneNode{//����ͼ�ڵ�ӿ�
public:
	virtual ~ISceneNode(){}

	virtual void EnlargeAABB(AABB &aabb) const{}//��ʹ��
	virtual void Render() const = 0;//��ʾ
	virtual void OnInsertion(){};//���볡������Ҫ����һЩ����
	virtual void UpdateTransform(const matrix44 &parentTransform){}//ĳ�ڵ���������������transform�䶯����Ҫ�������������������������ÿ���ڵ��transform����(��Ϊ��Щ�ڵ���Ҫ�洢�Լ�����������)
};
