#ifndef FT_SHARED_PTR_HPP
# define FT_SHARED_PTR_HPP

#include "Session.hpp"

class shared_ptr
{
	public:
		typedef Session element_type;
	private:
		elemen_type *ptr;
		static unsigned int count;
		
	public:
		shared_ptr() : ptr(NULL), count(0)
		{}
		shared_ptr(element_type p) : ptr(p), count(1)
		{}
		void	reset()
		{

		}
		int		use_count() const
		{
			return (count);
		}
		element_type* get() const
		{
			return (ptr);
		}
		element_type& operator*() const
		{
			return   (*ptr);
		}
};

unsigned int shared_ptr::shared_cnt = 0;

#endif
