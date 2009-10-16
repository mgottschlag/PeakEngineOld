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

#include "peakgraphics/SceneNode.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	SceneNode::SceneNode(Graphics *graphics) : ReferenceCounted(),
		positioncount(0), parent(0), newparent(0), node(0), graphics(graphics)
	{
	}
	SceneNode::~SceneNode()
	{
	}

	void SceneNode::setPosition(const Vector3F &position, unsigned int time)
	{
		ScopedLock lock(mutex);
	}
	Vector3F SceneNode::getPosition(unsigned int time)
	{
		// We do not have any position information yet
		if (positioncount == 0)
			return Vector3F();
	}
	void SceneNode::setRotation(const Vector3F &position, unsigned int time)
	{
		ScopedLock lock(mutex);
	}
	Vector3F SceneNode::getRotation(unsigned int time)
	{
		// We do not have any rotation information yet
		if (positioncount == 0)
			return Vector3F();
	}

	void SceneNode::setParent(SceneNode *parent)
	{
		ScopedLock lock(mutex);
		// Register for parent change
		newparent = parent;
		graphics->registerParentChange(this);
	}
	SceneNode *SceneNode::getParent()
	{
		return parent;
	}

	void SceneNode::updateParent()
	{
		ScopedLock lock(mutex);
		if (!newparent)
			return;
		// We have to make sure the scene node does not get deleted
		grab();
		if (node)
			node->grab();
		// Remove the node from the parent node
		if (this->parent)
			this->parent->removeChild(this);
		// Add to the new parent
		this->parent = parent;
		parent->children.push_back(this);
		if (parent->node && node)
			parent->node->addChild(node);
		newparent = 0;
		if (node)
			node->drop();
		drop();
	}

	void SceneNode::update(unsigned int time)
	{
		ScopedLock lock(mutex);
	}

	void SceneNode::removeChild(SceneNode *child)
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i].get() == child)
			{
				if (node && child->node)
					node->removeChild(node);
				children.erase(children.begin() + i);
				return;
			}
		}
	}
}