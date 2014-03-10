#pragma once
/*
space: gives access to objects( "points" store original objects or references or copies )
	dimensions (better named "states" since they can be non-scalars)

	domain( space whose dimensions are a subset of the dimensions of the original space)
	new space by merging dimensions with finite domains ( wrapping or copying )
	enumerate points( through iterator? ), one dimensional space only
	//sorting by one dimension ( sorting by multiple dimensions is supported through merging dimensions )
	sorting is supported by sorting transform (which operates on the original space or stores references) and enumeration
	conversion or wrapping into transformed spaces ( linear transform for numerical dimensions for example )
	//partition into subspaces by one or more discrete dimensions(must be of types that can be implicitly cast into, but will not fall out of, integral type 0..n-1
		(direct mapping,0..n-1 maps to n subspaces)( binary tree, quad tree, octree, etc. )
		(transformed, i.e. partition by comparing to a value or non-orthogonal partition, is supported by transforming wrapper)
	space partition is done by partition transform
	It seems like multidimensional partitioning still needs to be implemented.I can't do transform-to-1d-then-partition, since BSP needs the transform to happen after the partitioning of the parent space. 
	proximity access
	insertion and deletion
	iterator for one-dimensional space only( multi-dimensional space must be transformed to offer iterators )
	accessor( immutable random-access iterator )
	gridding is supported by dimension discretization transform succeeded by merging transform succeeded by partition transform
	log time access is supported by wrapper implemented in terms of partition transform
	recursive transform-partition(BSP)
	proximity?
	containers are spaces too, maybe an STL wrapper is needed
	correlated dimensions( derivatives for example, may need to be stored explicitly for precision )?
	space of spaces? nested structures can be seen as "portals" spaces
	A reference frame is a space. Ref frame transformation is transformation between spaces with identical number and types of dimensions.
*/
