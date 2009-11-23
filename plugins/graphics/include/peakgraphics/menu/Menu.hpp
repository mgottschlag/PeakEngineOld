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

#ifndef _PEAKGRAPHICS_MENU_MENU_HPP_
#define _PEAKGRAPHICS_MENU_MENU_HPP_

#include "MenuElement.hpp"
#include <peakgraphics/Loadable.hpp>

#include <string>
#include <map>
#include <queue>

namespace lf
{
	namespace gui
	{
		class CGUITheme;
		class CGUIManager;
	}
}

namespace peak
{
	class Graphics;

	class Menu : public Loadable
	{
		public:
			Menu(Graphics *graphics, std::string themename);
			~Menu();

			virtual bool load();
			virtual bool destroy();

			virtual void show();
			virtual void hide();

			virtual std::string getName() = 0;
			std::string getThemeName();

			static void registerMenu(Menu *menu, std::string name);
			static void deregisterMenu(std::string name);
			static void deregisterMenu(Menu *menu);
			static Menu *getMenu(std::string name);

			void setActive(bool active);
			bool isActive();
			static Menu *getActiveMenu();

			void addRootElement(MenuElement *element);
			void removeRootElement(MenuElement *element);

			static void registerParentChange(MenuElement *element);
			void update();
			static void updateAll();

			lf::gui::CGUIManager *getGUIManager();
		private:
			Graphics *graphics;
			std::string themename;

			lf::gui::CGUIManager *guimgr;
			lf::gui::CGUITheme *theme;

			std::vector<MenuElementPointer> rootelements;

			Mutex mutex;

			static std::map<std::string, Menu*> menus;
			static Menu *activemenu;
			static Menu *shownmenu;

			static Mutex parentchangemutex;
			static std::queue<MenuElementPointer> parentchange;
	};
}

#endif
