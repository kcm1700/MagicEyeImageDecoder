#ifndef _EXCEPTIONS_H__
#define _EXCEPTIONS_H__
#include <stdexcept>

class invalid_pointer : public std::logic_error
{
public:
	explicit invalid_pointer(const std::string& what_arg)
		: std::logic_error(what_arg) {}
};
class not_supported : public std::runtime_error
{
public:
	explicit not_supported(const std::string& what_arg)
		: std::runtime_error(what_arg) {}
};

#endif
