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

#ifndef CRO_CAMERA_HPP_
#define CRO_CAMERA_HPP_

#include <crogine/Config.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cro
{
    /*!
    \brief Represents a camera within the scene.
    Use MeshRenderer::setActiveCamera() to use an entity with
    a camera component as the current view
    */
    struct CRO_EXPORT_API Camera final
    {
        glm::mat4 projection;

        Camera()
        {
#ifdef __ANDROID__
            const float ratio = 16.f / 9.f;
#else
            const float ratio = 4.f / 3.f;
#endif //__ANDROID__

            projection = glm::perspective(0.6f, ratio, 0.1f, 50.f);
        }
    };
}

#endif //CRO_CAMERA_HPP_