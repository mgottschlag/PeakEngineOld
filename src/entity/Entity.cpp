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

#include "peakengine/entity/Entity.hpp"
#include "peakengine/entity/Property.hpp"

namespace peak
{
	Entity::Entity(EntityManager *manager) : manager(manager), id(0)
	{
	}
	Entity::~Entity()
	{
	}

	bool Entity::hasChanged(unsigned int time)
	{
		for (unsigned int i = 0; i < properties.size(); i++)
		{
			if (properties[i]->getLastChange() > time)
				return true;
		}
		return false;
	}
	void Entity::setState(Buffer *buffer)
	{
		for (unsigned int i = 0; i < properties.size(); i++)
		{
			int changed = buffer->readUnsignedInt(1);
			if (changed)
				properties[i]->deserialize(buffer);
		}
	}
	void Entity::getState(Buffer *buffer)
	{
		for (unsigned int i = 0; i < properties.size(); i++)
		{
			// Only send changed properties
			if (properties[i]->hasChanged())
			{
				// Bit set: Property changed.
				buffer->writeUnsignedInt(1, 1);
				// Write the property to the stream.
				properties[i]->serialize(buffer);
			}
			else
			{
				// Bit not set: Property remained unchanged.
				buffer->writeUnsignedInt(0, 1);
			}
		}
	}
	void Entity::applyUpdate(Buffer *buffer, unsigned int time)
	{
		// Update all properties.
		for (unsigned int i = 0; i < properties.size(); i++)
		{
			int changed = buffer->readUnsignedInt(1);
			if (changed)
			{
				properties[i]->deserialize(buffer);
			}
		}
		// TODO: Callback
	}
	void Entity::getUpdate(Buffer *buffer, unsigned int time)
	{
		for (unsigned int i = 0; i < properties.size(); i++)
		{
			// TODO: Property flags
			if (properties[i]->getLastChange() > time)
			{
				// Bit set: Property changed.
				buffer->writeUnsignedInt(1, 1);
				// Write the property to the stream.
				properties[i]->serialize(buffer);
			}
			else
			{
				// Bit not set: Property remained unchanged.
				buffer->writeUnsignedInt(0, 1);
			}
		}
	}

	EntityManager *Entity::getManager()
	{
		return manager;
	}

	void Entity::setID(unsigned int id)
	{
		this->id = id;
	}
	unsigned int Entity::getID()
	{
		return id;
	}

	void Entity::update()
	{
	}

	void Entity::addProperty(Property *property)
	{
		properties.push_back(property);
	}
}
