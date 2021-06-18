#ifndef FT_SHARED_PTR_HPP
# define FT_SHARED_PTR_HPP

#include "Session.hpp"

class Shared_ptr
{
	public:
		typedef Session			element_type;
		typedef element_type*	pointer;
	private:
		pointer		ptr;
		static unsigned int	count;
		
	public:
		Shared_ptr();
		Shared_ptr(pointer p);
		//Shared_ptr(Shared_ptr const& rhs);
		~Shared_ptr();
		Shared_ptr&	operator=(Shared_ptr const& rhs);
		void	reset();
		int		use_count() const;
		element_type*	get() const;
		element_type&	operator*() const;
};

#endif
