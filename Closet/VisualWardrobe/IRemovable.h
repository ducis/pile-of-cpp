#pragma once

class IRemovable{
public:
	virtual bool Removable() const =0;
	virtual void Remove() =0;
};