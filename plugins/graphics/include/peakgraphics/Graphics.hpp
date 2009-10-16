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

#ifndef _PEAKGRAPHICS_GRAPHICS_HPP_
#define _PEAKGRAPHICS_GRAPHICS_HPP_

#include "peakengine/support/ConditionVariable.hpp"

namespace lf
{
	class CResourceManager;

	namespace render
	{
		class IRenderWindow;
	}
	namespace scene
	{
		class CScene;
	}
}

namespace peak
{
	class Thread;

	class Graphics
	{
		public:
			Graphics();
			~Graphics();

			bool init(int width, int height, bool fullscreen);
			bool shutdown();

			void runThread();
		private:
			Thread *thread;
			volatile bool stopping;
			bool stopped;
			ConditionVariable cond;

			bool fullscreen;
			int width;
			int height;

			lf::render::IRenderWindow *window;
			lf::CResourceManager *resmgr;
			lf::scene::CScene *scene;

			unsigned int fps;
	};
}

#endif
