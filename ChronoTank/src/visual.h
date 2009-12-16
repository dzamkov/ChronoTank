#ifndef CTANK_VISUAL_H
#define CTANK_VISUAL_H

#include <irrlicht.h>
#include <string>

namespace ctank {
	class IObject;
	typedef float	TimeStep;

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
	/// of an object. The visual uses the data that the object produces to render
	/// it in the scene. The object is not usually aware of its visual.
	class IVisual {
	public:
		virtual ~IVisual() { }

		/// Gets the object this visual represents.
		virtual IObject*		GetTarget() = 0;

		/// Updates the visual by the specified amount of time.
		virtual void			Update(TimeStep Time) = 0;

		/// Renders the visual. This will be called after the scene manager
		/// renders.
		virtual void			Render();

		/// Destroys the visual. This should be called instead of delete.
		virtual void			Destroy();

	};

	/// Loads a visual for the object from the specified file. 
	IVisual*	LoadVisualFromFile(VisualParameters Params, irr::io::IReadFile* File, IObject* Object);


}

#endif