/*-----------------------------------------------------------------------

Matt Marchant 2017
http://trederia.blogspot.com

crogine test application - Zlib license.

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

#include "BackgroundController.hpp"
#include "Messages.hpp"

#include <crogine/core/Clock.hpp>
#include <crogine/ecs/components/Model.hpp>
#include <crogine/util/Random.hpp>

#include <array>

namespace
{
    std::array<float, 200u> noiseTable;
}


BackgroundController::BackgroundController(cro::MessageBus& mb)
    : cro::System   (mb, typeid(BackgroundController)),
    m_speed         (0.f),
    m_currentSpeed  (0.f),
    m_currentMode   (Mode::Scroll),
    m_currentIndex  (0)
{
    requireComponent<BackgroundComponent>();
    requireComponent<cro::Model>();

    for (auto& t : noiseTable)
    {
        t = cro::Util::Random::value(-0.0008f, 0.0008f);
    }
}

//public
void BackgroundController::process(cro::Time dt)
{
    float dtSec = dt.asSeconds();
    m_currentSpeed += ((m_speed - m_currentSpeed) * dtSec);

    m_offset.x += m_currentSpeed * dtSec;

    if(m_currentMode == Mode::Shake)
    {
        m_offset.x += noiseTable[m_currentIndex];
        noiseTable[m_currentIndex] = -noiseTable[m_currentIndex]; //averages out overall movement so it doesn't drift over time
        m_currentIndex = (m_currentIndex + 1) % noiseTable.size();
    }

    auto& entities = getEntities();
    for (auto& entity : entities)
    {
        //TODO... aren't the shader properties stored in the component?
        entity.getComponent<cro::Model>().setMaterialProperty(0, "u_textureOffset", m_offset);
    }
}

void BackgroundController::setScrollSpeed(float speed)
{
    m_speed = speed;

    auto* msg = postMessage<BackgroundEvent>(MessageID::BackgroundSystem);
    msg->type = BackgroundEvent::SpeedChange;
    msg->value = m_speed;
}

void BackgroundController::setMode(Mode mode)
{
    m_currentMode = mode;
    if (mode == Mode::Shake)
    {
        m_speed = 0.f;
    }
}