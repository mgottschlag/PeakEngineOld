/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKGRAPHICS_SCENENODE_HPP_
#define _PEAKGRAPHICS_SCENENODE_HPP_

#include <peakengine/support/Vector3.hpp>
#include <peakengine/support/ReferenceCounted.hpp>
#include <peakengine/support/Mutex.hpp>

#include <vector>

namespace lf
{
	namespace scene
	{
		class CSceneNode;
	}
}

namespace peak
{
	class Graphics;

	class SceneNode : public ReferenceCounted
	{
		public:
			SceneNode(Graphics *graphics);
			virtual ~SceneNode();

			void setTransformation(const Vector3F &position,
				const Vector3F &rotation, unsigned int time = 0);
			void getTransformation(Vector3F &position, Vector3F &rotation,
				unsigned int time = 0);

			void setParent(SceneNode *parent);
			SceneNode *getParent();

			void setMoving(bool moving);
			bool isMoving();

			virtual bool load() = 0;
			virtual bool isLoaded() = 0;

			void updateParent();

			virtual void update(unsigned int time);
		protected:
			void removeChild(SceneNode *child);

			struct TransformationInfo
			{
				Vector3F position;
				Vector3F rotation;
				int time;
			};

			TransformationInfo transformation[3];
			unsigned int transformationcount;
			bool moving;
			SceneNode *parent;
			SceneNode *newparent;
			std::vector<SharedPointer<SceneNode> > children;

			lf::scene::CSceneNode *node;

			Graphics *graphics;

			Mutex mutex;
	};

	typedef SharedPointer<SceneNode> SceneNodePointer;
}

#endif
