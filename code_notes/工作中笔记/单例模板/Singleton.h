#pragma once
#include <cassert>
using namespace std;

template <typename T> 
class Singleton
{
private:
	static T* & ms_Singleton()
	{
		static T* ms_Singleton_ = new T ;
		return ms_Singleton_;
	}

public:
	Singleton()
	{
		assert( !ms_Singleton());
		ms_Singleton() = static_cast< T* >( this );
	}
	~Singleton()
	{  
		assert(ms_Singleton());  
		ms_Singleton() = 0;  
	}
	static T& getSingleton()
	{  
		assert( ms_Singleton());  
		return ( *ms_Singleton());  
	}
	static T* getSingletonPtr()
	{ 
		return (ms_Singleton());  
	}
};
