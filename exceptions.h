/*
 * Copyright (c) 2011, ChanMin Kim (kcm1700@gmail.com)
 *
 * This files is licensed under a Creative Commons license:
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 */

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
