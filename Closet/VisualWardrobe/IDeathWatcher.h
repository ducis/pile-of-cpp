#pragma once
//��ʹ��

template<class T> IDeathWatched;

template<class T>
class IDeathWatcher{
public:
	OnDeath( T *pWatched );
};