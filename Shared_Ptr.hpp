#ifndef FT_SHARED_PTR_HPP
# define FT_SHARED_PTR_HPP

#include "User.hpp"

class shared_ptr
{
	private:
		User *ptr;
		static unsigned int shared_cnt;
		
		shared_ptr(User *p) : ptr(p)
		{
			shared_cnt ++;
		}
		~shared_ptr()
		{
			if (ptr)
				delete ptr;
		}
		shared_ptr& operator=(shared_ptr const& sptr)
		{
			this->ptr = sptr.ptr;
			shared_cnt ++;
			return (*this);
		}
		void reset()
		{
			this->ptr = NULL;
			shared_cnt--;
		}

};

unsigned int shared_ptr::shared_cnt = 0;

#endif
