#pragma once

/*
	A simulator is a function/functor taking a tuple of several entity (view) collections (this tuple it self is a entity) and a tuple of several parameters(including delta time if time is not discrete by itself)(is again an entity) and returning a tuple of one or more entity (view) collections(still an entity).
	Some simulators can only run forward in time(delta time>0).
	Simulators can be mutating or swapping or mutating with caching.
	if simulation is atomic then mutating is equivalent to swapping.
	swapping simulator can use mutating simulator interface.
	but mutating simulator can't use swapping interface.
	swapping: b = f(a,p);
	mutating: f(a,p);

	Most general definition: f(a,p), a is a reference to an entity(can be a tuple of collections of entities to be mutated or a normal "single" entity), p is a const reference to an entity( composed of delta time and other entities as "input to the system" );
*/
