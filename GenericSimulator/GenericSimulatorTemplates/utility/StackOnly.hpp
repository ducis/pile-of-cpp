#pragma once

namespace Utility{
	class StackOnly{
	private:
		void* operator new(std::size_t);
		void* operator new(std::size_t,const std::nothrow_t&);
		void* operator new(std::size_t,void*);
	};
}
