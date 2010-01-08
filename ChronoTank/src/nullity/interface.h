#ifndef NULLITY_INTERFACE_H
#define NULLITY_INTERFACE_H

#include <string>
#include <map>

#include "object.h"

namespace nullity {
	struct InterfaceClass;
	class IInterface;
	class Clonable;

	/// Identifier of a type of interface that
	/// is usually paired directly with a class that
	/// inherits IInterface.
	struct InterfaceClass {
	public:

		/// Gets a short debug name for the class.
		virtual std::string		GetName() = 0;

		/// Gets other interfaces that can be used to interact with the specified
		/// interface. Base interfaces are expressed with the class of
		/// the base interface paired with the pointer to IInterface that
		/// corresponds to that class. The map does not need to express
		/// bases of bases but can do so to resolve ambiguity.
		virtual std::map<InterfaceClass*, IInterface*>		GetBases(IInterface* Interface);
	};

	/// Derived classes of this type hold a set of methods
	/// that can be used to manipulate objects and store
	/// data.
	class IInterface {
	public:
		IInterface(IObject* Owner);

		/// Gets the class of this interface.
		virtual InterfaceClass*		GetClass() = 0;

		/// Gets the base of this interface that corresponds to the specified
		/// class. This is capable of traversing the tree of interface bases and
		/// may find indirect interfaces. This is usually costly for complex interfaces
		/// so the resulting base should be stored. This will return null if the base is
		/// not found and will return a pseudorandom interface of the class if there are
		/// multiple bases of the class(make sure to resolve ambiguous bases with multiple
		/// calls to GetBase)
		IInterface*			GetBase(InterfaceClass* Class);

		/// Gets the object that has this interface. This can be used to store
		/// pointers(which must have this as their owner) and perform actions on
		/// the object.
		IObject*			GetOwner();

	private:
		IObject*		_owner;
	};
}

/// This warning is raised when a class inherits a base class multiple times. When using interfaces,
/// a single interface can implement other interfaces, leaving this as normal behavior.
#pragma warning(disable:4584)

/// Adds interface boilerplate to an interface class body.
#define DECLARE_INTERFACE_CLASS(name) \
	name(IObject* Owner); \
	static nullity::InterfaceClass* Class; \
	nullity::InterfaceClass* GetClass();


/// Defines an interfaces class, construct and GetClass method for interfaces that
/// store no data other than virtual functions.
#define SIMPLE_INTERFACE_CLASS(name) \
	struct : public nullity::InterfaceClass { \
		std::string GetName() { \
			return #name; \
		} \
	} _##name##Class; \
	nullity::InterfaceClass* name##::Class = &_##name##Class; \
	name##::##name##(IObject* Owner) : nullity::IInterface(Owner) { } \
	nullity::InterfaceClass* name##::GetClass() { return name##::Class; } 

/// Begins the definition of a more complicated interface class.
#define BEGIN_INTERFACE_CLASS \
	struct : public nullity::InterfaceClass {

/// Sets the name of the class.
#define INTERFACE_CLASS_NAME(name) \
	std::string GetName() { \
		return #name; \
	}

/// Begins the definition of class bases.
#define BEGIN_INTERFACE_CLASS_BASES(name) \
	std::map<InterfaceClass*, IInterface*> GetBases(IInterface* Interface) { \
		name##* c = (##name##*)Interface; \
		std::map<InterfaceClass*, IInterface*> map;

/// Adds a base to the class.
#define INTERFACE_CLASS_BASE(name) \
	map[##name##::Class] = (##name##*)c;

/// Closes BEGIN_INTERFACE_CLASS_BASES
#define END_INTERFACE_CLASS_BASES \
		return map; \
	}

/// Closes BEGIN_INTERFACE_CLASS with the specified class.
#define END_INTERFACE_CLASS(name) \
	} _##name##Class; \
	nullity::InterfaceClass* name##::Class = &_##name##Class; \
	nullity::InterfaceClass* name##::GetClass() { return name##::Class; } 


#endif