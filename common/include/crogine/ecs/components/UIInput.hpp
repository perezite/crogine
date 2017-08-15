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

#ifndef CRO_UI_INPUT_HPP_
#define CRO_UI_INPUT_HPP_

#include <crogine/Config.hpp>
#include <crogine/detail/Types.hpp>
#include <crogine/graphics/Rectangle.hpp>

#include <array>

namespace cro
{
    /*!
    \brief Used to trigger callbacks when events occur in the Input's area
    */
    class CRO_EXPORT_API UIInput final
    {
    public:
        enum CallbackID
        {
            MouseEnter = 1,
            MouseExit,
            MouseDown,
            MouseUp,
            MouseMotion,
            Count
        };

        FloatRect area;
        bool active;
        std::array<uint32, CallbackID::Count> callbacks{};
        int32 ID = -1;
    };
}

#endif //CRO_UI_INPUT_HPP_