/*
Copyright (c) 2009, Mathias Gottschlag, Christian Reiser

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

#ifndef _PEAKPHYSICS_PHYSICS_HPP_
#define _PEAKPHYSICS_PHYSICS_HPP_

#include <list>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btAxisSweep3;

namespace peak
{
	class Body;

	class Physics
	{
		public:
			Physics();
			~Physics();

			bool init();
			bool shutdown();

			/**
			 * Adds a body to the world.
			 */
			void addBody(Body *body);
			/**
			 * Removes a body from the world.
			 */
			void removeBody(Body *body);

			void update();
		private:
			btDefaultCollisionConfiguration *config;
			btCollisionDispatcher *dispatcher;
			btSequentialImpulseConstraintSolver *solver;
			btDynamicsWorld *world;
			btAxisSweep3 *worldbox;
	};
}

#endif
