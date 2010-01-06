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

	protected:

		/// Gets other interfaces that can be used to interact with this
		/// interface. Base interfaces are expressed with the class of
		/// the base interface paired with the pointer to IInterface that
		/// corresponds to that class. The map does not need to express
		/// bases of bases but can do so to resolve ambiguity.
		virtual std::map<InterfaceClass*, IInterface*>		GetBases();

	private:
		IObject*		_owner;
	};
}

/// Defines an interface class and sets the value of the static
/// attribue Class to point to it. Also defines the GetClass method.
#define INTERFACE_CLASS(name) \
	static struct : public nullity::InterfaceClass { \
		std::string GetName() { \
			return #name; \
		} \
	} _##name##Class; \
	static nullity::InterfaceClass* name##Class = &_##name##Class; \
	nullity::InterfaceClass* name##GetClass() { return name##Class; }

#endif