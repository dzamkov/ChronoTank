#ifndef NULLITY_INTERFACE_H
#define NULLITY_INTERFACE_H

#include <string>
#include <map>

#include "object.h"

namespace nullity {
	struct InterfaceClass;
	struct Interface;

	/// Identifier of a type of interface that
	/// is usually paired directly with a class that
	/// inherits Interface.
	struct InterfaceClass {
	public:

		/// Gets a short debug name for the class.
		virtual std::string		GetName() = 0;

		/// Gets other interfaces that can be used to interact with the specified
		/// interface. Base interfaces are expressed with the class of
		/// the base interface paired with the pointer to Interface that
		/// corresponds to that class. The map does not need to express
		/// bases of bases but can do so to resolve ambiguity.
		virtual std::map<InterfaceClass*, Interface*>		GetBases(Interface* Interface);
	};

	/// An interface that can be used to manipulate other objects and interfaces.
	struct Interface {
		Interface(InterfaceClass* Class, Ptr<IObject> Owner);

		/// The type of this interface.
		InterfaceClass*		Class;

		/// The object that owns this interface. If the owner is destroyed, the interface
		/// should also be destroyed.
		Ptr<IObject>		Owner;

		/// Gets a base interface with the specified type or a null interface if none exists.
		Interface*			GetBase(InterfaceClass* Class);
	};
}

/// This warning is raised when a class inherits a base class multiple times. When using interfaces,
/// a single interface can implement other interfaces, leaving this as normal behavior.
#pragma warning(disable:4584)

/// Adds interface boilerplate to an interface class body.
#define DECLARE_INTERFACE_CLASS(name) \
	name(Ptr<IObject> Owner); \
	static nullity::InterfaceClass* Class;

/// Defines an interfaces class, construct and GetClass method for interfaces that
/// store no data other than virtual functions.
#define SIMPLE_INTERFACE_CLASS(name) \
	struct _##name##ClassType : public nullity::InterfaceClass { \
		std::string GetName() { \
			return #name; \
		} \
	} _##name##Class; \
	nullity::InterfaceClass* name::Class = &_##name##Class; \
	name::name(Ptr<IObject> Owner) : nullity::Interface(name::Class, Owner) { }

/// Begins the definition of a more complicated interface class.
#define BEGIN_INTERFACE_CLASS(name) \
	struct _##name##ClassType : public nullity::InterfaceClass {

/// Sets the name of the class.
#define INTERFACE_CLASS_NAME(name) \
	std::string GetName() { \
		return #name; \
	}

/// Begins the definition of class bases.
#define BEGIN_INTERFACE_CLASS_BASES(name) \
	std::map<nullity::InterfaceClass*, nullity::Interface*> GetBases(nullity::Interface* Interface) { \
		name##* c = (##name##*)Interface; \
		std::map<nullity::InterfaceClass*, nullity::Interface*> map;

/// Adds a base to the class.
#define INTERFACE_CLASS_BASE(name) \
	map[name::Class] = static_cast<nullity::Interface*>(static_cast<name*>(c));

/// Closes BEGIN_INTERFACE_CLASS_BASES
#define END_INTERFACE_CLASS_BASES \
		return map; \
	}

/// Closes BEGIN_INTERFACE_CLASS with the specified class.
#define END_INTERFACE_CLASS(name) \
	} _##name##Class; \
	nullity::InterfaceClass* name::Class = &_##name##Class;


#endif