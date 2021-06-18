#ifndef FT_SHARED_PTR_HPP
# define FT_SHARED_PTR_HPP

#include "Session.hpp"

class Shared_ptr
{
	public:
		typedef Session element_type;
	private:
		elemen_type			*ptr;
		static unsigned int	count;
		
	public:
		Shared_ptr();
		Shared_ptr(element_type p);
		void	reset();
		int		use_count() const;
		element_type*	get() const;
		element_type&	operator*() const;
};

#endif
