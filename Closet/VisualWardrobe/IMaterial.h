#pragma once


class IMaterial{//材质基类
public:
	virtual void Apply() const = 0;//渲染前调用此函数以设定相应的渲染状态
	virtual const class CIdentifier &GetIdentifier() const = 0;
};
