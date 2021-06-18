#include "include/Shared_ptr.hpp"

unsigned int	Shared_ptr::count = 0;

Shared_ptr::Shared_ptr() : ptr(NULL), count(0)
{}

Shared_ptr::Shared_ptr(element_type p) : ptr(p), count(1)
{}

Shared_ptr::~Shared_ptr()
{
	if (ptr)
	{
		delete ptr;
	}
}

Shared_ptr&		Shared_ptr::operator=(Shared_ptr const& rhs)
{
	if (this != &rhs)
	{
		reset();
		ptr = rhs.ptr;
		rhs.count++;
		count = rhs.count; //?
	}
	return (*this);
}

void	Shared_ptr::reset()
{
	if (count > 1)
	{
		ptr = NULL;
		count--;
	}
	else if (count == 1)
	{
		delete ptr;
		count--;
	}
	return ;
}

int		Shared_ptr::use_count() const
{
	return (count);
}

Shared_ptr::element_type*	Shared_ptr::get() const
{
	return (ptr);
}

Shared_ptr::element_type&	Shared_ptr::operator*() const
{
	return   (*ptr);
}
