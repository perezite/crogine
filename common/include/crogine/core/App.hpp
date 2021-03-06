/*-----------------------------------------------------------------------

Matt Marchant 2017
http://trederia.blogspot.com

crogine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#ifndef CRO_APP_HPP_
#define CRO_APP_HPP_

#include <crogine/Config.hpp>
#include <crogine/core/MessageBus.hpp>
#include <crogine/core/Window.hpp>
#include <crogine/core/Clock.hpp>
#include <crogine/detail/Types.hpp>

#include <crogine/graphics/Colour.hpp>

#include <vector>
#include <map>

#ifdef _DEBUG_
#define DPRINT(x, y) cro::App::debugPrint(x, y)
#else
#define DPRINT(x, y)
#endif //_DEBUG_

namespace cro
{
	namespace Detail
	{
		class SDLResource;
	}
	class Time;
    class GuiClient;

	/*!
	\brief Base class for crogine applications.
	One instance of this must exist before crogine may be used
	*/
	class CRO_EXPORT_API App
	{
	public:
		friend class Detail::SDLResource;
		App();
		virtual ~App();

		App(const App&) = delete;
		App(const App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (const App&&) = delete;

		void run();

		void setClearColour(Colour);
		const Colour& getClearColour() const;

        /*!
        \brief Use this to properly close down the application
        */
        static void quit();

        /*
        \brief Print a Name/Value string to the ImGui status window in debug mode
        */
        static void debugPrint(const std::string& name, const std::string& value);

        /*!
        \brief Returns a reference to the active window
        */
        static Window& getWindow();

        /*!
        \brief Returns a reference to the system message bus
        */
        MessageBus& getMessageBus() { return m_messageBus; }

        /*!
        \brief Returns the path to the current platform's directory
        for storing preference files.
        */
        static const std::string& getPreferencePath();

        /*!
        \brief Resets the frame clock.
        This should only be used when a state finishes loading assets for a long time
        to prevent the initial delta frame being a large value.
        */
        void resetFrameTime();

	protected:
		
		virtual void handleEvent(const Event&) = 0;
		virtual void handleMessage(const cro::Message&) = 0;
		/*!
        \brief Used to update the simulation with the time elapsed since the last update.
        It is left to the user to decide if a specific system (such as physics)
        requires fixed updates or not, which can be implemeted with a simple accumulator.
        */
        virtual void simulate(Time) = 0;
        /*!
        \brief Renders any drawables
        */
		virtual void render() = 0;

		/*!
        \brief Called on startup after window is created.
        Use it to perform initial operations such as setting the 
        window title, icon or custom loading screen
        */
        virtual void initialise() {}

        /*!
        \brief Called before the window is destroyed.
        Use this to clean up any resources which rely on the
        window's OpenGL context.
        */
        virtual void finalise() {};

	private:

		Window m_window;
		Colour m_clearColour;
        Clock* m_frameClock;
        bool m_running;

        void handleEvents();

        MessageBus m_messageBus;
        void handleMessages();

		static App* m_instance;

        std::map<int32, SDL_GameController*> m_controllers;
        std::map<int32, SDL_Joystick*> m_joysticks;
        friend class GameController;
//#ifdef USE_IMGUI
        std::vector<std::string> m_debugLines;
        std::vector<std::pair<std::function<void()>, const GuiClient*>> m_statusControls;
        std::vector<std::pair<std::function<void()>, const GuiClient*>> m_guiWindows;
        void doImGui();
        bool m_showStats;

        static void addStatusControl(const std::function<void()>&, const GuiClient*);
        static void removeStatusControls(const GuiClient*);

        static void addWindow(const std::function<void()>&, const GuiClient*);
        static void removeWindows(const GuiClient*);

        friend class GuiClient;
//#endif       
        std::string m_prefPath;
	};
}


#endif //CRO_APP_HPP_