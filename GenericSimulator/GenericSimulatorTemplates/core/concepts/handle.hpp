#pragma once

/*
	A handle is a virtual pointer, which has operator* and operator-> defined as if it's a pointer. 
	It's used to identify and query the reference of objects.
	It's used as an identifier, and should have operator< defined (between the same type).
	Hash function is good, but it's not easy to devise a good generic solution, so I'm not going to offer hashing in this version.
	metafunction: 
		GetHandleElementType< _Handle >
	//function:
		//GetRawPointerFromHandle// use &*
		
	The handle does not ensure that the actual memory address of the element remains the same (actually this concept is created to 
	encapsulate the change of actual memory position), so there is no point in forcing the handle to return lvalue reference of the element.
	But,sometimes I still need a temporary raw pointers for efficiency( which implies lvalue reference ), 
	and proxies didn't work well anyway, so I'm going to drop support for proxy handles in favor of lvalue references.

	handles which allows proxies should be defined as "long handles" which implies non-trival performance cost for access.
*/
