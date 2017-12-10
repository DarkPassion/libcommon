#ifndef _SMART_PTR_WEAK_PTR_
#define _SMART_PTR_WEAK_PTR_


#include "smart_ptr/ref_count.h"

namespace libcommon {
    
    template <typename T> class SupportsWeakPtr;
    template <typename T> class WeakPtr;
    
    namespace internal {
        
        // These classes are part of the WeakPtr implementation.
        // DO NOT USE THESE CLASSES DIRECTLY YOURSELF.
        
        class   WeakReference {
        public:
            // While Flag is bound to a specific thread, it may be deleted from another
            // via base::WeakPtr::~WeakPtr().
            class Flag : public RefCountedThreadSafe<Flag> {
            public:
                Flag();
                
                void Invalidate();
                bool IsValid() const;
                
                
            private:
                friend class libcommon::RefCountedThreadSafe<Flag>;
                
                ~Flag();
                
                bool is_valid_;
            };
            
            WeakReference();
            explicit WeakReference(const Flag* flag);
            ~WeakReference();
            
            bool is_valid() const;
            
        private:
            scoped_refptr<const Flag> flag_;
        };
        
        class   WeakReferenceOwner {
        public:
            WeakReferenceOwner();
            ~WeakReferenceOwner();
            
            WeakReference GetRef() const;
            
            bool HasRefs() const {
                return flag_.get() && !flag_->HasOneRef();
            }
            
            void Invalidate();
            
            
        private:
            mutable scoped_refptr<WeakReference::Flag> flag_;
        };
        
        // This class simplifies the implementation of WeakPtr's type conversion
        // constructor by avoiding the need for a public accessor for ref_.  A
        // WeakPtr<T> cannot access the private members of WeakPtr<U>, so this
        // base class gives us a way to access ref_ in a protected fashion.
        class   WeakPtrBase {
        public:
            WeakPtrBase();
            ~WeakPtrBase();
            
        protected:
            explicit WeakPtrBase(const WeakReference& ref);
            
            WeakReference ref_;
        };
        
        // This class provides a common implementation of common functions that would
        // otherwise get instantiated separately for each distinct instantiation of
        // SupportsWeakPtr<>.
        class SupportsWeakPtrBase {
        public:
            // A safe static downcast of a WeakPtr<Base> to WeakPtr<Derived>. This
            // conversion will only compile if there is exists a Base which inherits
            // from SupportsWeakPtr<Base>. See base::AsWeakPtr() below for a helper
            // function that makes calling this easier.
            template<typename Derived>
            static WeakPtr<Derived> StaticAsWeakPtr(Derived* t) {
                return AsWeakPtrImpl<Derived>(t, *t);
            }
            
        private:
            // This template function uses type inference to find a Base of Derived
            // which is an instance of SupportsWeakPtr<Base>. We can then safely
            // static_cast the Base* to a Derived*.
            template <typename Derived, typename Base>
            static WeakPtr<Derived> AsWeakPtrImpl(
                                                  Derived* t, const SupportsWeakPtr<Base>&) {
                WeakPtr<Base> ptr = t->Base::AsWeakPtr();
                return WeakPtr<Derived>(ptr.ref_, static_cast<Derived*>(ptr.ptr_));
            }
        };
        
    }  // namespace internal
    
    template <typename T> class WeakPtrFactory;
    
    // The WeakPtr class holds a weak reference to |T*|.
    //
    // This class is designed to be used like a normal pointer.  You should always
    // null-test an object of this class before using it or invoking a method that
    // may result in the underlying object being destroyed.
    //
    // EXAMPLE:
    //
    //   class Foo { ... };
    //   WeakPtr<Foo> foo;
    //   if (foo)
    //     foo->method();
    //
    template <typename T>
    class WeakPtr : public internal::WeakPtrBase {
    public:
        WeakPtr() : ptr_(NULL) {
        }
        
        // Allow conversion from U to T provided U "is a" T.
        template <typename U>
        WeakPtr(const WeakPtr<U>& other) : WeakPtrBase(other), ptr_(other.get()) {
        }
        
        T* get() const { return ref_.is_valid() ? ptr_ : NULL; }
        operator T*() const { return get(); }
        
        T& operator*() const {
            //DCHECK(get() != NULL);
            return *get();
        }
        T* operator->() const {
            //DCHECK(get() != NULL);
            return get();
        }
        
        void reset() {
            ref_ = internal::WeakReference();
            ptr_ = NULL;
        }
        
    private:
        friend class internal::SupportsWeakPtrBase;
        friend class SupportsWeakPtr<T>;
        friend class WeakPtrFactory<T>;
        
        WeakPtr(const internal::WeakReference& ref, T* ptr)
        : WeakPtrBase(ref),
        ptr_(ptr) {
        }
        
        // This pointer is only valid when ref_.is_valid() is true.  Otherwise, its
        // value is undefined (as opposed to NULL).
        T* ptr_;
    };
    
    // A class may extend from SupportsWeakPtr to expose weak pointers to itself.
    // This is useful in cases where you want others to be able to get a weak
    // pointer to your class.  It also has the property that you don't need to
    // initialize it from your constructor.
    template <class T>
    class SupportsWeakPtr : public internal::SupportsWeakPtrBase {
    public:
        SupportsWeakPtr() {}
        
        WeakPtr<T> AsWeakPtr() {
            return WeakPtr<T>(weak_reference_owner_.GetRef(), static_cast<T*>(this));
        }
        
        
    protected:
        ~SupportsWeakPtr() {}
        
    private:
        internal::WeakReferenceOwner weak_reference_owner_;
        DISALLOW_COPY_AND_ASSIGN(SupportsWeakPtr);
    };
    
    // Helper function that uses type deduction to safely return a WeakPtr<Derived>
    // when Derived doesn't directly extend SupportsWeakPtr<Derived>, instead it
    // extends a Base that extends SupportsWeakPtr<Base>.
    //
    // EXAMPLE:
    //   class Base : public base::SupportsWeakPtr<Producer> {};
    //   class Derived : public Base {};
    //
    //   Derived derived;
    //   base::WeakPtr<Derived> ptr = base::AsWeakPtr(&derived);
    //
    // Note that the following doesn't work (invalid type conversion) since
    // Derived::AsWeakPtr() is WeakPtr<Base> SupportsWeakPtr<Base>::AsWeakPtr(),
    // and there's no way to safely cast WeakPtr<Base> to WeakPtr<Derived> at
    // the caller.
    //
    //   base::WeakPtr<Derived> ptr = derived.AsWeakPtr();  // Fails.
    
    template <typename Derived>
    WeakPtr<Derived> AsWeakPtr(Derived* t) {
        return internal::SupportsWeakPtrBase::StaticAsWeakPtr<Derived>(t);
    }
    
    // A class may alternatively be composed of a WeakPtrFactory and thereby
    // control how it exposes weak pointers to itself.  This is helpful if you only
    // need weak pointers within the implementation of a class.  This class is also
    // useful when working with primitive types.  For example, you could have a
    // WeakPtrFactory<bool> that is used to pass around a weak reference to a bool.
    template <class T>
    class WeakPtrFactory {
    public:
        explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {
        }
        
        ~WeakPtrFactory() {
            ptr_ = NULL;
        }
        
        WeakPtr<T> GetWeakPtr() {
            //DCHECK(ptr_);
            return WeakPtr<T>(weak_reference_owner_.GetRef(), ptr_);
        }
        
        // Call this method to invalidate all existing weak pointers.
        void InvalidateWeakPtrs() {
            //DCHECK(ptr_);
            weak_reference_owner_.Invalidate();
        }
        
        // Call this method to determine if any weak pointers exist.
        bool HasWeakPtrs() const {
            //DCHECK(ptr_);
            return weak_reference_owner_.HasRefs();
        }
        
        
    private:
        internal::WeakReferenceOwner weak_reference_owner_;
        T* ptr_;
        DISALLOW_IMPLICIT_CONSTRUCTORS(WeakPtrFactory);
    };
    
}  // namespace libcommon


#endif













