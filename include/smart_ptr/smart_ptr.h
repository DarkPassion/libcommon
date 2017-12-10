#ifndef _SMART_PTR_SP_INCLUDE_
#define _SMART_PTR_SP_INCLUDE_

#include <stdint.h>
#include <assert.h>
#include <algorithm>

namespace libcommon {
    
    inline void atomic_increment( int32_t * pw )
    {
        __sync_fetch_and_add( pw, 1 );
    }
    
    inline int32_t atomic_decrement( int32_t * pw )
    {
        return __sync_fetch_and_add( pw, -1 );
    }
    
    inline int32_t atomic_conditional_increment( int32_t * pw )
    {
        // long r = *pw;
        // if( r != 0 ) ++*pw;
        // return r;
        
        int32_t r = *pw;
        
        for( ;; )
        {
            if( r == 0 )
            {
                return r;
            }
            
            int32_t r2 = __sync_val_compare_and_swap( pw, r, r + 1 );
            
            if( r2 == r )
            {
                return r;
            }
            else
            {
                r = r2;
            }
        }
    }
    
    template<class T> inline void checked_delete(T * x)
    {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete x;
    }
    
    template<class T> inline void checked_array_delete(T * x)
    {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete [] x;
    }
    
    class sp_counted_base
    {
    private:
        
        sp_counted_base( sp_counted_base const & );
        sp_counted_base & operator= ( sp_counted_base const & );
        
        int32_t use_count_;        // #shared
        int32_t weak_count_;       // #weak + (#shared != 0)
        
    public:
        
        sp_counted_base(): use_count_( 1 ), weak_count_( 1 )
        {
        }
        
        virtual ~sp_counted_base() // nothrow
        {
        }
        
        // dispose() is called when use_count_ drops to zero, to release
        // the resources managed by *this.
        
        virtual void dispose() = 0; // nothrow
        
        // destroy() is called when weak_count_ drops to zero.
        
        virtual void destroy() // nothrow
        {
            delete this;
        }
        
        //virtual void * get_deleter( sp_typeinfo const & ti ) = 0;
        //virtual void * get_local_deleter( sp_typeinfo const & ti ) = 0;
        //virtual void * get_untyped_deleter() = 0;
        
        void add_ref_copy()
        {
            atomic_increment( &use_count_ );
        }
        
        bool add_ref_lock() // true on success
        {
            return atomic_conditional_increment( &use_count_ ) != 0;
        }
        
        void release() // nothrow
        {
            if( atomic_decrement( &use_count_ ) == 1 )
            {
                dispose();
                weak_release();
            }
        }
        
        void weak_add_ref() // nothrow
        {
            atomic_increment( &weak_count_ );
        }
        
        void weak_release() // nothrow
        {
            if( atomic_decrement( &weak_count_ ) == 1 )
            {
                destroy();
            }
        }
        
        long use_count() const // nothrow
        {
            return const_cast< int32_t const volatile & >( use_count_ );
        }
    };
    
    
    template<class X> class sp_counted_impl_p: public sp_counted_base
    {
    private:
        
        X * px_;
        
        sp_counted_impl_p( sp_counted_impl_p const & );
        sp_counted_impl_p & operator= ( sp_counted_impl_p const & );
        
        typedef sp_counted_impl_p<X> this_type;
        
    public:
        
        explicit sp_counted_impl_p( X * px ): px_( px )
        {
        }
        
        virtual void dispose() // nothrow
        {
            libcommon::checked_delete( px_ );
        }
        
        
        void * operator new( std::size_t )
        {
            return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
        }
        
        void operator delete( void * p )
        {
            std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
        }
    };
    
    
    template<class P, class D> class sp_counted_impl_pd: public sp_counted_base
    {
    private:
        
        P ptr; // copy constructor must not throw
        D del; // copy constructor must not throw
        
        sp_counted_impl_pd( sp_counted_impl_pd const & );
        sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );
        
        typedef sp_counted_impl_pd<P, D> this_type;
        
    public:
        
        // pre: d(p) must not throw
        
        sp_counted_impl_pd( P p, D & d ): ptr( p ), del( d )
        {
        }
        
        sp_counted_impl_pd( P p ): ptr( p ), del()
        {
        }
        
        virtual void dispose() // nothrow
        {
            del( ptr );
        }

        void * operator new( std::size_t )
        {
            return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
        }
        
        void operator delete( void * p )
        {
            std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
        }
    };
    
    
    class weak_count;
    class shared_count
    {
    private:
        
        sp_counted_base * pi_;
        
        friend class weak_count;
        
    public:
        
         shared_count(): pi_(0) // nothrow
        {
        }
        
        explicit shared_count( sp_counted_base * pi ): pi_( pi ) // nothrow
        {
        }
        
        template<class Y> explicit shared_count( Y * p ): pi_( 0 )
        {
            pi_ = new sp_counted_impl_p<Y>( p );
        }

        template<class P, class D> shared_count( P p, D d ): pi_(0)
        {
            pi_ = new sp_counted_impl_pd<P, D>(p, d);
        }
        
        ~shared_count() // nothrow
        {
            if( pi_ != 0 ) pi_->release();
        }
        
        shared_count(shared_count const & r): pi_(r.pi_) // nothrow
        {
            if( pi_ != 0 ) pi_->add_ref_copy();
        }
        
        explicit shared_count(weak_count const & r); // throws bad_weak_ptr when r.use_count() == 0
        
        
        shared_count & operator= (shared_count const & r) // nothrow
        {
            sp_counted_base * tmp = r.pi_;
            
            if( tmp != pi_ )
            {
                if( tmp != 0 ) tmp->add_ref_copy();
                if( pi_ != 0 ) pi_->release();
                pi_ = tmp;
            }
            
            return *this;
        }
        
        void swap(shared_count & r) // nothrow
        {
            sp_counted_base * tmp = r.pi_;
            r.pi_ = pi_;
            pi_ = tmp;
        }
        
        long use_count() const // nothrow
        {
            return pi_ != 0? pi_->use_count(): 0;
        }
        
        bool unique() const // nothrow
        {
            return use_count() == 1;
        }
        
        bool empty() const // nothrow
        {
            return pi_ == 0;
        }
        
        friend inline bool operator==(shared_count const & a, shared_count const & b)
        {
            return a.pi_ == b.pi_;
        }
        
        friend inline bool operator<(shared_count const & a, shared_count const & b)
        {
            return std::less<sp_counted_base *>()( a.pi_, b.pi_ );
        }
        
    };
    
    
    class weak_count
    {
    private:
        
        sp_counted_base * pi_;
        friend class shared_count;
        
    public:
        
        weak_count(): pi_(0) // nothrow
        {
        }
        
        weak_count(shared_count const & r): pi_(r.pi_) // nothrow
        {
            if(pi_ != 0) pi_->weak_add_ref();
        }
        
        weak_count(weak_count const & r): pi_(r.pi_) // nothrow
        {
            if(pi_ != 0) pi_->weak_add_ref();
        }
        
        ~weak_count() // nothrow
        {
            if(pi_ != 0) pi_->weak_release();
        }
        
        weak_count & operator= (shared_count const & r) // nothrow
        {
            sp_counted_base * tmp = r.pi_;
            
            if( tmp != pi_ )
            {
                if(tmp != 0) tmp->weak_add_ref();
                if(pi_ != 0) pi_->weak_release();
                pi_ = tmp;
            }
            
            return *this;
        }
        
        weak_count & operator= (weak_count const & r) // nothrow
        {
            sp_counted_base * tmp = r.pi_;
            
            if( tmp != pi_ )
            {
                if(tmp != 0) tmp->weak_add_ref();
                if(pi_ != 0) pi_->weak_release();
                pi_ = tmp;
            }
            
            return *this;
        }
        
        void swap(weak_count & r) // nothrow
        {
            sp_counted_base * tmp = r.pi_;
            r.pi_ = pi_;
            pi_ = tmp;
        }
        
        long use_count() const // nothrow
        {
            return pi_ != 0? pi_->use_count(): 0;
        }
        
        bool empty() const // nothrow
        {
            return pi_ == 0;
        }
        
        friend inline bool operator==(weak_count const & a, weak_count const & b)
        {
            return a.pi_ == b.pi_;
        }
        
        friend inline bool operator<(weak_count const & a, weak_count const & b)
        {
            return std::less<sp_counted_base *>()(a.pi_, b.pi_);
        }
    };
    
    shared_count::shared_count(weak_count const & r) : pi_( r.pi_ )
    {
        if (r.use_count() == 0)
        {
            assert(false);
        } else
        {
            if( pi_ != 0 ) pi_->add_ref_copy();
        }
    }
    
    // sp_element, element_type
    
    template< class T > struct sp_element
    {
        typedef T type;
    };
    
    
    template<class T> class weak_ptr;
    template<class T> class shared_ptr;
    template<class T> class enable_shared_from_this;
    
    template< class X, class Y, class T > inline void sp_enable_shared_from_this( libcommon::shared_ptr<X> const * ppx,
                                                                                 Y const * py,
                                                                                 libcommon::enable_shared_from_this< T > const * pe )
    {
        if( pe != 0 )
        {
            pe->_internal_accept_owner( ppx, const_cast< Y* >( py ) );
        }
    }

    inline void sp_enable_shared_from_this( ... )
    {
    }
    
    
    template< class T, class Y > inline void sp_pointer_construct( libcommon::shared_ptr< T > * ppx, Y * p, libcommon::shared_count & pn )
    {
        libcommon::shared_count( p ).swap( pn );
        libcommon::sp_enable_shared_from_this( ppx, p, p );
    }
    
    
    template< class T, class Y > inline void sp_deleter_construct( libcommon::shared_ptr< T > * ppx, Y * p )
    {
        libcommon::sp_enable_shared_from_this( ppx, p, p );
    }

    template< class Y, class T > inline void sp_assert_convertible()
    {
        T* p = static_cast< Y* >( 0 );
        (void)p;
    }


    
    template< class T > struct sp_dereference
    {
        typedef T & type;
    };
    
    template<> struct sp_dereference< void >
    {
        typedef void type;
    };
    
    
    // sp_member_access, return type of operator->
    
    template< class T > struct sp_member_access
    {
        typedef T * type;
    };
    
    
    // sp_array_access, return type of operator[]
    
    template< class T > struct sp_array_access
    {
        typedef void type;
    };
    
    template<class T> class shared_ptr
    {
    private:
        
        // Borland 5.5.1 specific workaround
        typedef shared_ptr<T> this_type;
        
    public:
        
        typedef typename libcommon::sp_element< T >::type element_type;
        
        shared_ptr() : px( 0 ), pn()
        {
        }
        
        
        template<class Y>
        explicit shared_ptr( Y * p ): px( p ), pn() // Y must be complete
        {
            libcommon::sp_pointer_construct( this, p, pn );
        }
        
        //
        // Requirements: D's copy constructor must not throw
        //
        // shared_ptr will release p by calling d(p)
        //
        
        template<class Y, class D> shared_ptr( Y * p, D d ): px( p ), pn( p, d )
        {
            libcommon::sp_deleter_construct( this, p );
        }
        
        // As above, but with allocator. A's copy constructor shall not throw.
        
        template<class Y, class D, class A> shared_ptr( Y * p, D d, A a ): px( p ), pn( p, d, a )
        {
            libcommon::sp_deleter_construct( this, p );
        }
        
        template<class Y> shared_ptr( shared_ptr<Y> const & r ) : px( r.px ), pn( r.pn )
        {
            libcommon::sp_assert_convertible< Y, T >();
        }

        
        
        template<class Y>
        explicit shared_ptr( weak_ptr<Y> const & r ): pn( r.pn ) // may throw
        {
            libcommon::sp_assert_convertible< Y, T >();

            // it is now safe to copy r.px, as pn(r.pn) did not throw
            px = r.px;
        }
        
        // aliasing
        template< class Y >
        shared_ptr( shared_ptr<Y> const & r, element_type * p ) : px( p ), pn( r.pn )
        {
        }
        
        
        shared_ptr & operator=( shared_ptr const & r )
        {
            this_type(r).swap(*this);
            return *this;
        }
        
        void reset()
        {
            this_type().swap(*this);
        }
        
        template<class Y> void reset( Y * p ) // Y must be complete
        {
            // BOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
            this_type( p ).swap( *this );
        }
        
        template<class Y, class D> void reset( Y * p, D d )
        {
            this_type( p, d ).swap( *this );
        }
        
        template<class Y, class D, class A> void reset( Y * p, D d, A a )
        {
            this_type( p, d, a ).swap( *this );
        }
        
        template<class Y> void reset( shared_ptr<Y> const & r, element_type * p )
        {
            this_type( r, p ).swap( *this );
        }
        
        element_type * get() const
        {
            return px;
        }
        
        
        typename libcommon::sp_dereference< T >::type operator* () const
        {
            //BOOST_ASSERT( px != 0 );
            return *px;
        }
        
        typename libcommon::sp_member_access< T >::type operator-> () const
        {
            //BOOST_ASSERT( px != 0 );
            return px;
        }
        
        typename libcommon::sp_array_access< T >::type operator[] ( std::ptrdiff_t i ) const
        {
            return static_cast< typename libcommon::sp_array_access< T >::type >( px[ i ] );
        }
        
        bool unique() const
        {
            return pn.unique();
        }
        
        long use_count() const
        {
            return pn.use_count();
        }
        
        void swap( shared_ptr & other )
        {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
        
        template<class Y> bool owner_before( shared_ptr<Y> const & rhs ) const
        {
            return pn < rhs.pn;
        }
        
        template<class Y> bool owner_before( weak_ptr<Y> const & rhs ) const
        {
            return pn < rhs.pn;
        }
        
        
        bool _internal_equiv( shared_ptr const & r ) const
        {
            return px == r.px && pn == r.pn;
        }
        
        libcommon::shared_count _internal_count() const
        {
            return pn;
        }
        
        // Tasteless as this may seem, making all members public allows member templates
        // to work in the absence of member template friends. (Matthew Langston)
        
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        
    private:
        
        template<class Y> friend class shared_ptr;
        template<class Y> friend class weak_ptr;
        
        
#endif
        
        element_type * px;                 // contained pointer
        libcommon::shared_count pn;        // reference counter
        
    };  // shared_ptr
    
    template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() == b.get();
    }
    
    template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() != b.get();
    }
    
    
    template<class T> class weak_ptr
    {
    private:
        
        // Borland 5.5.1 specific workarounds
        typedef weak_ptr<T> this_type;
        
    public:
        
        typedef typename libcommon::sp_element< T >::type element_type;
        
        weak_ptr() : px(0), pn()
        {
        }
        
        template<class Y>
        weak_ptr( shared_ptr<Y> const & r ) : px( r.px ), pn( r.pn )
        {
            //boost::detail::sp_assert_convertible< Y, T >();
        }
        
        
        template<class Y>
        weak_ptr & operator=( weak_ptr<Y> const & r )
        {
            //boost::detail::sp_assert_convertible< Y, T >();
            
            px = r.lock().get();
            pn = r.pn;
            
            return *this;
        }
        
        
        template<class Y>
        weak_ptr & operator=( shared_ptr<Y> const & r )
        {
            //boost::detail::sp_assert_convertible< Y, T >();
            
            px = r.px;
            pn = r.pn;
            
            return *this;
        }
        
        
        shared_ptr<T> lock() const
        {
            return shared_ptr<T>( *this );
        }
        
        long use_count() const
        {
            return pn.use_count();
        }
        
        bool expired() const
        {
            return pn.use_count() == 0;
        }
        
        bool _empty() const // extension, not in std::weak_ptr
        {
            return pn.empty();
        }
        
        void reset()
        {
            this_type().swap(*this);
        }
        
        void swap(this_type & other)
        {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
        
        template<typename Y>
        void _internal_aliasing_assign(weak_ptr<Y> const & r, element_type * px2)
        {
            px = px2;
            pn = r.pn;
        }
        
        template<class Y> bool owner_before( weak_ptr<Y> const & rhs ) const
        {
            return pn < rhs.pn;
        }
        
        template<class Y> bool owner_before( shared_ptr<Y> const & rhs ) const
        {
            return pn < rhs.pn;
        }
        
        // Tasteless as this may seem, making all members public allows member templates
        // to work in the absence of member template friends. (Matthew Langston)
        
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        
    private:
        
        template<class Y> friend class weak_ptr;
        template<class Y> friend class shared_ptr;
        
#endif
        
        element_type * px;            // contained pointer
        libcommon::weak_count pn; // reference counter
        
    };  // weak_ptr
    
    template<class T, class U> inline bool operator<(weak_ptr<T> const & a, weak_ptr<U> const & b)
    {
        return a.owner_before( b );
    }
    
    template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b)
    {
        a.swap(b);
    }
    
    
    
    template<class T> class enable_shared_from_this
    {
    protected:
        
        enable_shared_from_this()
        {
        }
        
        enable_shared_from_this(enable_shared_from_this const &)
        {
        }
        
        enable_shared_from_this & operator=(enable_shared_from_this const &)
        {
            return *this;
        }
        
        ~enable_shared_from_this() // ~weak_ptr<T> newer throws, so this call also must not throw
        {
        }
        
    public:
        
        shared_ptr<T> shared_from_this()
        {
            shared_ptr<T> p( weak_this_ );
            return p;
        }
        
        shared_ptr<T const> shared_from_this() const
        {
            shared_ptr<T const> p( weak_this_ );
            return p;
        }
        
        weak_ptr<T> weak_from_this()
        {
            return weak_this_;
        }
        
        weak_ptr<T const> weak_from_this() const
        {
            return weak_this_;
        }
        
    public: // actually private, but avoids compiler template friendship issues
        
        // Note: invoked automatically by shared_ptr; do not call
        template<class X, class Y> void _internal_accept_owner( shared_ptr<X> const * ppx, Y * py ) const
        {
            if( weak_this_.expired() )
            {
                weak_this_ = shared_ptr<T>( *ppx, py );
            }
        }
        
    private:
        mutable weak_ptr<T> weak_this_;
    };
}



#endif




