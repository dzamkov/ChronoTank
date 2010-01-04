#ifndef NULLITY_OBJECT_H
#define NULLITY_OBJECT_H

#include <assert.h>
#include <map>

namespace nullity {
	class IObject;
	template <class Type> class Ptr;

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
	};

	/// Generic definition for a pointer to an object.
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
		void	_incref();
		void	_decref();

	public:
		Ptr();
		Ptr(const TypePtr& Target);
		Ptr(IObject* Owner, const TypePtr& Target);
		~Ptr();

		/// Sets the object this pointer points to.
		void	SetTarget(const TypePtr& To);

		/// Sets the owner of this pointer. Fails if target is not null. The pointer
		/// must be destroyed as a result of its owner being destroyed.
		void	SetOwner(IObject* Owner);

		bool					IsNull() const;
		void					MakeNull();
		TypePtr operator		=(const TypePtr& Other);
		TypePtr operator		->() const;
		operator				TypePtr() const;
	};

	//--
	template <class Type> void Ptr<Type>::_incref() {
		this->_target->_incref(this->_owner, 1);
	}

	//--
	template <class Type> void Ptr<Type>::_decref() {
		this->_target->_incref(this->_owner, -1);
	}

	//--
	template <class Type> Ptr<Type>::Ptr() {
		this->_target = NULL;
		this->_owner = NULL;
	}

	//--
	template <class Type> Ptr<Type>::Ptr(const TypePtr& Target) {
		this->_owner = NULL;
		this->_ptr = Target;
		this->_incref();
	}

	//--
	template <class Type> Ptr<Type>::Ptr(IObject* Owner, const TypePtr& Target) {
		this->_owner = Owner;
		this->_ptr = Target;
		this->_incref();
	}

	//--
	template <class Type> Ptr<Type>::~Ptr() {
		if(this->_target) {
			this->_decref();
		}
	}
	
	//--
	template <class Type> void Ptr<Type>::SetTarget(const TypePtr& Target) {
		if(this->_ptr != Target) {
			if(this->_target) {
				this->_decref();
			}
			this->_ptr = Target;
			if(this->_target) {
				this->_incref();
			}
		}
	}

	//--
	template <class Type> void Ptr<Type>::SetOwner(IObject* Owner) {
		assert(this->_target == NULL);
		this->_owner = Owner;
	}

	//--
	template <class Type> bool Ptr<Type>::IsNull() const {
		return this->_target == NULL;
	}

	//--
	template <class Type> void Ptr<Type>::MakeNull() {
		this->SetTarget(NULL);
	}

	//--
	template <class Type> Type* Ptr<Type>::operator =(const TypePtr& Other) {
		this->SetTarget(Other);
		return Other;
	}

	//--
	template <class Type> Type* Ptr<Type>::operator ->() const {
		return this->_ptr;
	}

	//--
	template <class Type> Ptr<Type>::operator TypePtr() const {
		return this->_ptr;
	}

	/// Casts a pointer to a different type that still derives from IObject.
	template <class To, class From> Ptr<To> Cast(const Ptr<From>& Pointer) {
		return (To*)((From*)Pointer);
	}
}

#endif