#include "include/Shared_Ptr.hpp"

unsigned int	Shared_ptr::shared_cnt = 0;

Shared_ptr::Shared_ptr() : ptr(NULL), count(0)
{}

Shared_ptr::Shared_ptr(element_type p) : ptr(p), count(1)
{}

void	Shared_ptr::reset()
{
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
