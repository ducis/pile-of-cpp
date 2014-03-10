#pragma once

#include "ptree.h"
#include <stack>
#include <streambuf>
#include <sstream>
#include <utility>

namespace Stupid{
	template<class _IStream> void StupidlyReadXmlToCreateWPTree( _IStream &is, boost::property_tree::wptree &root ){
		//boost::property_tree��xml��parser�ڴ������ĵ�ʱ����bug,�������Ӧ����parser
		//���ܷǳ����ޣ������ռ䡢attribute��ע�ͺͱ�׼�ļ�ͷ�ȵȶ�����ʶ��дxml��ʱ��
		//��ð����ṩ��xml��,������ȥ��TinyXML֮��
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
