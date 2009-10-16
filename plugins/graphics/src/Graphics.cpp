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

#include "peakgraphics/Graphics.hpp"
#include "peakgraphics/RootSceneNode.hpp"
#include <peakengine/support/Thread.hpp>
#include <peakengine/support/OS.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	Graphics::Graphics()
	{
	}
	Graphics::~Graphics()
	{
	}

	bool Graphics::init(int width, int height, bool fullscreen)
	{
		this->fullscreen = fullscreen;
		this->width = width;
		this->height = height;
		// Create server thread
		cond.lock();
		stopping = false;
		thread = new Thread();
		thread->create(new ClassFunctor<Graphics>(this, &Graphics::runThread));
		// Wait for graphics to initialize
		cond.wait();
		cond.unlock();
		// TODO: Check success
		return true;
	}
	bool Graphics::shutdown()
	{
		if (stopped)
			return true;
		cond.lock();
		stopping = true;
		cond.wait();
		cond.unlock();
		return true;
	}

	SceneNode *Graphics::getRootSceneNode()
	{
		return rootscenenode.get();
	}

	void Graphics::registerParentChange(SceneNode *node)
	{
		parentmutex.lock();
		parentchange.push(node);
		parentmutex.unlock();
	}

	void Graphics::runThread()
	{
		// Initialize graphics
		lf::initLF();
		window = CLFRender::getInstance().createRenderWindow(core::vector2di(0,0),
			core::vector2d<s32>(width,height), 32,	32,
			render::EWCF_AUTOCLOSE | (fullscreen ? render::EWCF_FULLSCREEN : 0));
		window->setWindowCaption(L"PeakEngine");
		window->setVisible(true);
		CLFRender::getInstance().setAutoSleep(1);
		resmgr = CResourceManager::getInstancePtr();
		scene = window->getRenderLayer3D()->getScene();
		fps = 0;
		// Create root scene node
		rootscenenode = new RootSceneNode(this, scene->getRootSceneNode());
		// We have finished initializaition
		cond.lock();
		cond.signal();
		cond.unlock();
		// Render loop
		while (!stopping)
		{
			// Update scene node tree
			parentmutex.lock();
			while (parentchange.size() > 0)
			{
				SceneNodePointer node = parentchange.front();
				parentchange.pop();
				parentmutex.unlock();
				node->updateParent();
				parentmutex.lock();
			}
			parentmutex.unlock();
			// Update scene node positions
			rootscenenode->update(OS::get().getTime());
			// Render
			if (!CLFRender::getInstance().update())
			{
				stopped = true;
				break;
			}
			// Update window caption
			unsigned int currentfps = window->getOneSecondFPS();
			if (currentfps != fps)
			{
				fps = currentfps;
				window->setWindowCaption((core::stringw(L"PeakEngine  FPS: ")
					+ core::stringw(fps)).c_str());
			}
		}
		// Shutdown graphics
		lf::deinitLF();
		// TODO
		cond.lock();
		cond.signal();
		cond.unlock();
	}
}
