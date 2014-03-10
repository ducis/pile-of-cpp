#pragma once;
#include "singleton.h"

//这个函数最好显式地实例化,在知道需要生成singleton的CPP文件里才包含这个源文件
//一般调用singleton的地方包含singleton.h即可,一方面避免singleton的滥用,
//另一方面防止因为是DLL而出现多份singleton之类的事情.

template <class CLASS>
CLASS *GetSingleton(){
	static CLASS object;
	return &object;
}
