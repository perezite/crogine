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

#ifndef CRO_GUI_CLIENT_HPP_
#define CRO_GUI_CLIENT_HPP_

#include <crogine/Config.hpp>

#include <functional>

namespace cro
{
    /*!
    \brief Allows registering GUI controls with the default output
    window, or registering custom windows using the Nim namespace.
    If crogine is built with USE_IMGUI defined then classes which
    inherit this interface may register controls with the imgui
    renderer. This is usually used for debugging output or when
    creating tooling. USE_IMGUI can be undefined for optimisation
    on platforms such as mobile devices.
    */
    class CRO_EXPORT_API GuiClient
    {
    public:
        GuiClient() = default;
        virtual ~GuiClient();
        GuiClient(const GuiClient&) = default;
        GuiClient(GuiClient&&) = default;
        GuiClient& operator = (const GuiClient&) = default;
        GuiClient& operator = (GuiClient&&) = default;

        /*!
        \brief Registers one or more gui controls with the default status window.
        The given function should include the Nim/ImGui functions as they would
        appear between the Begin() and End() commands *without* Begin() and End()
        themselves. These controls will then appear in the default status window
        all the time the object which inherits this interface exists.
        */
        void registerStatusControls(const std::function<void()>&);

        /*!
        \brief Registers a custom window with the ImGui renderer.
        The given function should include the Begin() and End() calls to create a
        fully stand-alone window with ImGui. The window will exist all the time
        the object which inherits this interface exists.
        */
        void registerWindow(const std::function<void()>&);
    };
}

#endif //CRO_GUI_CLIENT_HPP_