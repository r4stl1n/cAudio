#ifndef CSTLALLOCATOR_H_INCLUDED
#define CSTLALLOCATOR_H_INCLUDED

#include "../include/cAudioDefines.h"
#include "../Headers/cMemoryOverride.h"

namespace cAudio
{
	template <typename T> class cSTLAllocator;

    // specialize for void:
    template <> class cSTLAllocator<void> 
	{
    public:
      typedef void*       pointer;
      typedef const void* const_pointer;
      // reference to void members are impossible.
      typedef void value_type;
      template <class U> 
	  struct rebind 
	  { 
		  typedef cSTLAllocator<U> other; 
	  };
    };

	template <typename T> class cSTLAllocator 
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;

		template<typename U>
		struct rebind
		{
			typedef cSTLAllocator<U> other;
		};

		inline explicit cSTLAllocator()
		{ }

		virtual ~cSTLAllocator()
		{ }

		inline cSTLAllocator( cSTLAllocator const& )
		{ }

		template <typename U>
		inline cSTLAllocator( cSTLAllocator<U> const& )
		{ }

		pointer address(reference x) const
		{
			return &x;
		}

		const_pointer address(const_reference x) const
		{
			return &x;
		}

		inline pointer allocate( size_type count, typename std::allocator<void>::const_pointer ptr = 0 )
		{
            (void)ptr;
			register size_type size = count*sizeof( T );
			pointer p  = static_cast<pointer>(CAUDIO_MALLOC(size));
			return p;
		}

		inline void deallocate( pointer p, size_type size )
		{
			CAUDIO_FREE(p);
		}

		size_type max_size() const throw()
		{
			return cAudio::getMemoryProvider()->getMaxAllocationSize();
		}

		void construct(pointer p, const T& val)
		{
			// call placement new
			new(static_cast<void*>(p)) T(val);
		}

		void destroy(pointer p)
		{
			p->~T();
		}
	};

	template <typename T1, typename T2>
	bool operator==(const cSTLAllocator<T1>&, const cSTLAllocator<T2>&)
	{
		return true;
	}

	template <typename T1, typename T2>
	bool operator!=(const cSTLAllocator<T1>&, const cSTLAllocator<T2>&)
	{
		return false;
	}

};

#endif //! CSTLALLOCATOR_H_INCLUDED