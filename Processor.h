#ifndef _Processor_H_
#define _Processor_H_

#include <util/tc_singleton.h>

//
using namespace tars;

/**
 *请求处理类
 *
 */
class Processor
{
public:
	/**
	 * 
	*/
	Processor();

	/**
	 * 
	*/
	~Processor();

public:
};

//singleton
typedef TC_Singleton<Processor, CreateStatic, DefaultLifetime> ProcessorSingleton;

#endif

