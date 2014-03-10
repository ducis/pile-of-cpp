#pragma once

/*
point:
	"abstract tuple"? boost.fusion style associative tuples has no storage semantics like plain structs while eliminating duplicate get*(),set*() functions. but why using type as indices rather than enums?
	Just use random access sequence + enum types. Because dimensions of a space has implied ordering.
	mutable/immutable: assignment
	deletion (reference decrement for reference points)
	creation?
	(optional) copy-construction.
	number of dimensions.
	move
	//identifiers/names of dimensions? Shouldn't be in the "core" part
	problem of dynamic point dimensions and static point dimensions.
	//point dimensions should be determined at runtime.

	Some algorithms and data types requiring compile-time dimensions needs 
		static const size_t num_dimensions;
	Well this may be useless anyway since TMP has no loops, and I would probably have to resort to recursions.

	Those that use run-time dimension data should require 
		size_t NumDimensions() const;
	And a point's NumDimensions() should return the same number as long as it is referenced by any space.
	
	should point factory be implemented as transforms?

	expansion view

	use external functions like at_c() to access states instead of member functions. Because operator. can't be overloaded, external functions can abstract away pointer dereferencing.
	Well I use at_c() for now but may add member function templates in the future since concept binding needs wrapper types anyway. And member functions can use template<typename _EntityConcept::Labels id> at_c(), reducing chances of error.

	something like boost::qvm::make is needed.(So number of constructors can be minimized)
*/

#if 0

//concept example

struct RigidBody{
	enum{
		Position,
		Orientation,
		NDIM
	};
};

#endif