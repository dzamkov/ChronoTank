#ifndef NULLITY_VISUAL_H
#define NULLITY_VISUAL_H

#include <irrlicht.h>
#include <string>

#include "object.h"
#include "entity.h"

namespace nullity {

	/// Set of parameters used to create a visual. Specifies render target, location, etc.
	struct VisualParameters {
		irr::IrrlichtDevice*			Device;
		irr::scene::ISceneManager*		SceneManager;
		irr::scene::ISceneNode*			ParentNode;
		irr::s32						ID;

		/// Creates the visual parameters to specifiy that the visual should be a root
		/// node of the specified manager.
		static VisualParameters			AsRootNode(irr::scene::ISceneManager* Manager);

		/// Creates the visual parameters to specifiy that the visual should be a root
		/// node of the scene manager of the specified device.
		static VisualParameters			AsRootNode(irr::IrrlichtDevice* Device);
	};

	/// A visual representation of - VISUAL IS A NOUN. A visual is a representation
	/// of an entity. The visual uses the data that the entity produces to render
	/// it in the scene. The entity is not usually aware of its visual.
	class IVisual : public IObject {
	public:
		virtual ~IVisual() { }

		/// Updates the visual to render the entity with the specified interface.
		virtual void			Update(IInterface* MainInterface);
			
		/// Renders the visual. This will be called after the scene manager
		/// renders.
		virtual void			Render();

	};

	/// Loads a visual from the specified file. 
	IVisual*	LoadVisualFromFile(VisualParameters Params, irr::io::IReadFile* File);

}

#endif