#ifndef _APPEXCEPTION_H_
#define _APPEXCEPTION_H_

#include <string>

namespace ShockWaveApp
{

struct AppException	
{
	std::string Text;
	AppException(const std::string& inMessage): Text(inMessage){};
};

}
#endif  //_APPEXCEPTION_H_
