/*
 *  Signal.h
 *  A lightweight signals and slots implementation using fast delegates
 *
 *  Created by Patrick Hogan on 5/18/09.
 *
 */

#ifndef _UTIL_SIGNAL_H_
#define _UTIL_SIGNAL_H_

#include <set>
#include "util/delegate.h"
#include "thread/lock.h"

namespace libcommon {


    template< class Param0 = void >
    class Signal0
    {
    public:
        typedef Delegate0< void > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)() )
        {
            AutoLock __lock(mutex);
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)() const )
        {
            AutoLock __lock(mutex);
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)() )
        {
            AutoLock __lock(mutex);
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)() const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit() const
        {
            AutoLock __lock((ILock&)mutex);

            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))();
            }
        }
        
        void operator() () const
        {

            Emit();
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1 >
    class Signal1
    {
    public:
        typedef Delegate1< Param1 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1 );
            }
        }
        
        void operator() ( Param1 p1 ) const
        {
            Emit( p1 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2 >
    class Signal2
    {
    public:
        typedef Delegate2< Param1, Param2 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2 ) const
        {
            Emit( p1, p2 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3 >
    class Signal3
    {
    public:
        typedef Delegate3< Param1, Param2, Param3 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3 ) const
        {
            Emit( p1, p2, p3 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3, class Param4 >
    class Signal4
    {
    public:
        typedef Delegate4< Param1, Param2, Param3, Param4 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3, p4 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3, Param4 p4 ) const
        {
            Emit( p1, p2, p3, p4 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3, class Param4, class Param5 >
    class Signal5
    {
    public:
        typedef Delegate5< Param1, Param2, Param3, Param4, Param5 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3, p4, p5 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5 ) const
        {
            Emit( p1, p2, p3, p4, p5 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3, class Param4, class Param5, class Param6 >
    class Signal6
    {
    public:
        typedef Delegate6< Param1, Param2, Param3, Param4, Param5, Param6 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3, p4, p5, p6 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6 ) const
        {
            Emit( p1, p2, p3, p4, p5, p6 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7 >
    class Signal7
    {
    public:
        typedef Delegate7< Param1, Param2, Param3, Param4, Param5, Param6, Param7 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) const )
        {
            AutoLock __lock((ILock&)mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) const
        {
            AutoLock __lock(mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3, p4, p5, p6, p7 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7 ) const
        {
            Emit( p1, p2, p3, p4, p5, p6, p7 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
    };
    
    
    template< class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class Param8 >
    class Signal8
    {
    public:
        typedef Delegate8< Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8 > _Delegate;
        
    private:
        typedef std::set<_Delegate> DelegateList;
        typedef typename DelegateList::const_iterator DelegateIterator;
        DelegateList delegateList;
        CMutex      mutex;
        
    public:
        void Connect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( delegate );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Connect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.insert( MakeDelegate( obj, func ) );
        }
        
        void Disconnect( _Delegate delegate )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( delegate );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        template< class X, class Y >
        void Disconnect( Y * obj, void (X::*func)( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) const )
        {
            AutoLock __lock(mutex);
            
            delegateList.erase( MakeDelegate( obj, func ) );
        }
        
        void Clear()
        {
            AutoLock __lock(mutex);
            
            delegateList.clear();
        }
        
        void Emit( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) const
        {
            AutoLock __lock((ILock&)mutex);
            
            for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); )
            {
                (*(i++))( p1, p2, p3, p4, p5, p6, p7, p8 );
            }
        }
        
        void operator() ( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8 ) const
        {
            Emit( p1, p2, p3, p4, p5, p6, p7, p8 );
        }
        
        bool Empty() const
        {
            return delegateList.empty();
        }
};


} // namespace

#endif //_SIGNAL_H_
