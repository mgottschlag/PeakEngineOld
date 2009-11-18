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

#ifndef _PEAKENGINE_ENTITY_ENTITY_HPP_
#define _PEAKENGINE_ENTITY_ENTITY_HPP_

#include "../support/Buffer.hpp"

#include <vector>
#include <string>

namespace peak
{
	class EntityManager;
	class Property;
	class Buffer;

	class Entity
	{
		public:
			Entity(EntityManager *manager);
			virtual ~Entity();

			bool hasChanged(unsigned int time);
			void setState(Buffer *buffer);
			void getState(Buffer *buffer);
			void applyUpdate(Buffer *buffer, unsigned int time);
			void getUpdate(Buffer *buffer, unsigned int time);

			bool hasChangedClient(unsigned int time);
			void applyClientUpdate(Buffer *buffer, unsigned int time);
			void getClientUpdate(Buffer *buffer, unsigned int time);

			void createBackup();
			bool hasBackup();
			void applyBackup();
			void discardBackup();

			EntityManager *getManager();

			void setID(unsigned int id);
			unsigned int getID();

			virtual void receiveMessage(Buffer *buffer) = 0;

			virtual std::string getType() = 0;

			virtual void update();

			void addProperty(Property *property);
			void addClientProperty(Property *property);
		private:
			std::vector<Property*> properties;
			std::vector<Property*> clientproperties;
			EntityManager *manager;
			unsigned int id;

			BufferPointer backup;
			bool backupvalid;
	};
}

#endif
