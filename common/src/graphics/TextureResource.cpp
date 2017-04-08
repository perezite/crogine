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

#include <crogine/graphics/TextureResource.hpp>
#include <crogine/graphics/Image.hpp>

using namespace cro;

TextureResource::TextureResource()
    : m_fallbackColour(Colour::Magenta())
{

}

//public
Texture& TextureResource::get(const std::string& path)
{
    if (m_textures.count(path) == 0)
    {
        std::unique_ptr<Texture> tex = std::make_unique<Texture>();
        if (!tex->loadFromFile(path))
        {
            //find the fallback
            if (m_fallbackTextures.count(m_fallbackColour) == 0)
            {
                Image img;
                img.create(32, 32, m_fallbackColour);
                std::unique_ptr<Texture> fbTex = std::make_unique<Texture>();
                fbTex->create(32, 32);
                fbTex->update(img.getPixelData());

                m_fallbackTextures.insert(std::make_pair(m_fallbackColour, std::move(fbTex)));
            }
            //and return it
            return *m_fallbackTextures.find(m_fallbackColour)->second;
        }
        m_textures.insert(std::make_pair(path, std::move(tex)));
    }
    return *m_textures.find(path)->second;
}

void TextureResource::setFallbackColour(Colour colour)
{
    m_fallbackColour = colour;
}

Colour TextureResource::getFallbackColour() const
{
    return m_fallbackColour;
}