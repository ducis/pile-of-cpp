#pragma once


#define GENSIM_ENTITY_CONCEPT( conceptname, ... )		\
	struct conceptname{									\
		enum Labels{									\
			__VA_ARGS__ ,								\
			NDIM										\
		};												\
	};													

#if 0// example

GENSIM_ENTITY_CONCEPT(			RigidBody,
	Position,
	Orientation
	)

struct RigidBody{
	enum Labels{
		Position,
		Orientation,
		NDIM
	};
};


#endif