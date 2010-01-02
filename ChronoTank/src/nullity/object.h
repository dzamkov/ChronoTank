#ifndef NULLITY_OBJECT_H
#define NULLITY_OBJECT_H

#include <assert.h>
#include <map>

/// Warning 4355 warns when 'this' is used in a
/// initializer list. Since Ptr needs to be initialized
/// with an owner during construction, using 'this' in
/// the initializer list is required to properly setup a Ptr
/// as an object member.
#pragma warning(disable:4355)

namespace nullity {
	class IObject;
	template <class Type> class Ptr;
	template <class Type> class StackPtr;

	/// A generic representation of a logical object or system.
	class IObject {
	public:
		IObject();
		virtual ~IObject();

		/// Deletes this object and all resources and managed objects it
		/// uses. All derived objects must call their base's destroy method.
		virtual void		Destroy();

		/// Deallocates the object from memory.
		virtual void		Deallocate();

	private:
		static const int	_flag_none					=	0x00000000;
		static const int	_flag_destroyed				=	0x00000001;
		static const int	_flag_deallocated			=	0x00000002;

		std::map<IObject*, int>		_refs_to;
		int							_refs;
		int							_flags;

		void		_incref(IObject* From, int Amount);

		template<class> friend class	Ptr;
		template<class> friend class	StackPtr;
	};

	/// Pointer to an object. The pointer is owned by another object with the
	/// notion that when the owner object is destroyed, so is the pointer.
	template <class Type>
	class Ptr {
	public:
		/// The actual pointer type used in this pointer.
		typedef Type*	TypePtr;

		/// Pass by reference pointer.
		typedef const Ptr<Type>&		Ref;

	private:		
		IObject*		_owner;
		union {
			IObject*	_target;
			TypePtr		_ptr;
		};
		void	_incref() { this->_target->_incref(this->_owner, 1); }
		void	_decref() { this->_target->_incref(this->_owner, -1); }

	public:
		Ptr() { assert(false); } // Default should never be called but is left accesible.
		Ptr(IObject* Owner) { this->_owner = Owner; this->_target = NULL; }
		Ptr(IObject* Owner, TypePtr Ptr) { this->_owner = Owner; this->_ptr = Ptr; if(this->_target) this->_incref(); }
		~Ptr() { if(this->_target) this->_decref(); }
		
		/// Sets the object this pointer points to.
		void	SetTarget(const TypePtr& To) {
			if(this->_target) this->_decref(); 
			this->_target = To; 
			if(this->_target) this->_incref(); 
		}

		bool					IsNull() { return this->_ptr == NULL; }
		void					MakeNull() { this->SetTarget(NULL); }
		TypePtr operator		=(const TypePtr& Other) { this->SetTarget(Other); return this->_ptr; }
		TypePtr operator		->() { return this->_ptr; }
		operator				TypePtr() const { return this->_ptr; }
	};

	/// Pointer on the stack that points to an object. This is like Ptr but with no option
	/// for an owner object.
	template <class Type>
	class StackPtr {
	public:
		/// The actual pointer type used in this pointer.
		typedef Type*	TypePtr;

		/// Pass by reference pointer. By using a simple "::Ref" at the end of the type in
		/// a parameter you're saying, you can give me the actual pointer, not just a copy, and
		/// I promise not to screw with it.
		typedef const StackPtr<Type>&		Ref;

	private:
		union {
			IObject*	_target;
			TypePtr		_ptr;
		};

	public:
		StackPtr() { this->_target = NULL; }
		StackPtr(const TypePtr& To) { this->_ptr = To; this->_target->_incref(NULL, 1); }
		StackPtr(const StackPtr& Other) { this->_ptr = Other._ptr; this->_target->_incref(NULL, 1); }
		StackPtr(const Ptr<Type>& Other) { this->_ptr = Other; this->_target->_incref(NULL, 1); }
		~StackPtr() { if(this->_target) this->_target->_incref(NULL, -1); }

		/// Sets the object this pointer points to.
		void	SetTarget(const TypePtr& To) {
			if(this->_target) this->_target->_incref(NULL, -1); 
			this->_target = To; 
			if(this->_target) this->_target->_incref(NULL, 1);
		}

		bool					IsNull() { return this->_ptr == NULL; }
		void					MakeNull() { this->SetTarget(NULL); }
		TypePtr operator		=(const TypePtr& Other) { this->SetTarget(Other); return this->_ptr; }
		TypePtr operator		->() const { return this->_ptr; }
		operator				TypePtr() { return this->_ptr; }

	};

	template <class Type> Ptr<Type> ToPtr(IObject* Obj, typename StackPtr<Type> Target) {
		return Ptr<Type>(Obj, Target);
	}
}

#endif