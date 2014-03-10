#pragma once

#include "ptree.h"
#include <stack>
#include <streambuf>
#include <sstream>
#include <utility>

namespace Stupid{
	template<class _IStream> void StupidlyReadXmlToCreateWPTree( _IStream &is, boost::property_tree::wptree &root ){
		//boost::property_tree读xml的parser在处理中文的时候有bug,现在这个应急的parser
		//功能非常有限，命名空间、attribute、注释和标准文件头等等都不能识别，写xml的时候
		//最好按照提供的xml改,或者再去用TinyXML之类
		using namespace boost::property_tree;
		std::stack<wptree *> wptPtrStack;
		wptPtrStack.push(&root);
		while(true){
			std::ws(is);
			if(is.eof()) break;
			if(L'<'==is.peek()){
				is.ignore();
				std::ws(is);
				assert(L'>'!=is.peek());
				bool isEnd(false);
				if(L'/'==is.peek()){
					isEnd = true;
					is.ignore();
					std::ws(is);
				}
				std::wstringbuf namebuf;
				is.get(namebuf,'>');
				assert(L'>'==is.peek());
				is.ignore();
				if(isEnd){
					wptree *p(wptPtrStack.top());
					wptPtrStack.pop();
					wptPtrStack.top()->add_child(namebuf.str(),*p);
				}else{
					wptPtrStack.push(new wptree);
				}
			}else{
				std::wstringbuf databuf;
				is.get(databuf,'<');
				wptPtrStack.top()->put_value(databuf.str());
			}
		}
		assert(wptPtrStack.size()==1);
	}
}
