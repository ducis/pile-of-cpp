#pragma once

namespace NumericRange{
	template<typename T> void ClampR( T &x, const T &upperLimit ){//¡Óx = min(x,upperLimit)
		if(upperLimit<x){
			x = upperLimit;
		}
	}
	template<typename T> void ClampL( T &x, const T &lowerLimit ){//¡Óx = max(x,lowerLimit)
		if(x<lowerLimit){
			x = lowerLimit;
		}
	}
	template<typename T> void Clamp( T &x, const T &lowerLimit, const T &upperLimit ){//¡Óx = min(max(x,lowerLimit),upperLimit)
		assert(lowerLimit<=upperLimit);
		if(x<lowerLimit){
			x = lowerLimit;
		}else if(upperLimit<x){
			x = upperLimit;
		}
		//ClampR(x,upperLimit);
		//ClampL(x,lowerLimit);
	}
	template<typename T> void ExtendR( const T &x, T &upperLimit ){
		if(upperLimit<x) upperLimit = x;
	}
	template<typename T> void ExtendL( const T &x, T &lowerLimit ){
		if(x<lowerLimit) lowerLimit = x;
	}
	template<typename T> void Extend( const T &x, T &lowerLimit, T &upperLimit ){
		ExtendR(x,upperLimit);
		ExtendL(x,lowerLimit);
	}
	template<typename T> const T Clamped( const T &x, const T &lowerLimit, const T &upperLimit ){//∑µªÿmin(max(x,lowerLimit),upperLimit)
		if(lowerLimit>x){
			return lowerLimit;
		}else if(upperLimit<x){
			return upperLimit;
		}else{
			return x;
		}
	}
}