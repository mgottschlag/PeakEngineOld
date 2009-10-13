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

#ifndef _PEAKENGINE_CORE_ENGINE_HPP_
#define _PEAKENGINE_CORE_ENGINE_HPP_

#include <string>

namespace peak
{
	class Game;

	/**
	 * Main engine class
	 */
	class Engine
	{
		public:
			/**
			 * Constructor.
			 */
			Engine();
			/**
			 * Destructor.
			 */
			~Engine();

			/**
			 * Loads the game implementation from a shared object/DLL. The
			 * library has to contain a symbol called "game" with the
			 * implementation of the Game class.
			 */
			bool loadGame(std::string filename);
			/**
			 * Used a game implementation from within this executable.
			 */
			bool loadGame(Game *game);
			/**
			 * Returns the game implementation.
			 */
			Game *getGame();

			/**
			 * Sets the directory of all game media.
			 */
			void setGameDirectory(std::string directory);
			/**
			 * Returns the directory of the game.
			 */
			std::string getGameDirectory();

			/**
			 * Runs the engine in normal mode.
			 */
			bool run();
			/**
			 * Runs the engine from within the editor.
			 */
			bool runFromEditor();
			/**
			 * Starts a game server-
			 */
			bool runServer();
			/**
			 * Stops the engine.
			 */
			void stop();

			/**
			 * Renders one frame into the current OpenGL context. This has to be
			 * the context with which the engine was started.
			 */
			void render();
		private:
			Game *game;
			std::string directory;
			bool stopping;
	};
}

#endif
