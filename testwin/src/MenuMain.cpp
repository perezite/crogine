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

#include "MainState.hpp"

#include <crogine/ecs/systems/SpriteRenderer.hpp>
#include <crogine/ecs/systems/TextRenderer.hpp>
#include <crogine/ecs/systems/UISystem.hpp>
#include <crogine/ecs/systems/SceneGraph.hpp>

#include <crogine/ecs/components/Sprite.hpp>
#include <crogine/ecs/components/Text.hpp>
#include <crogine/ecs/components/UIInput.hpp>
#include <crogine/ecs/components/Transform.hpp>

#include <crogine/graphics/Image.hpp>
#include <crogine/graphics/Font.hpp>

#include <crogine/core/App.hpp>

void MainState::createMainMenu()
{
    //test sprite sheet
    auto& testFont = m_fontResource.get(FontID::MenuFont);
    testFont.loadFromFile("assets/fonts/VeraMono.ttf");
    //cro::Image img;
    //img.loadFromFile("assets/fonts/sdf.png");
    //testFont.loadFromImage(img, { 32.f, 32.f }, cro::Font::Type::SDF);

    auto& mb = getContext().appInstance.getMessageBus();
    m_currentMenu = &m_mainMenuScene;
    m_spriteRenderer = &m_mainMenuScene.addSystem<cro::SpriteRenderer>(mb);
    m_textRenderer = &m_mainMenuScene.addSystem<cro::TextRenderer>(mb);
    m_mainMenuScene.addSystem<cro::SceneGraph>(mb);
    m_uiSystem = &m_mainMenuScene.addSystem<cro::UISystem>(mb);

    cro::FloatRect buttonArea(0.f, 0.f, 256.f, 64.f);
    auto mouseEnterCallback = m_uiSystem->addCallback([buttonArea](cro::Entity e, cro::uint64)
    {
        auto area = buttonArea;
        area.left = buttonArea.width;
        e.getComponent<cro::Sprite>().setTextureRect(area);
    });
    auto mouseExitCallback = m_uiSystem->addCallback([buttonArea](cro::Entity e, cro::uint64)
    {
        e.getComponent<cro::Sprite>().setTextureRect(buttonArea);
    });

    auto& uiTexture = m_textureResource.get("assets/sprites/menu.png");
    auto entity = m_mainMenuScene.createEntity();
    auto& titleSprite = entity.addComponent<cro::Sprite>();
    titleSprite.setTexture(uiTexture);
    titleSprite.setTextureRect({ 0.f, 64.f, 1024.f, 320.f });
    auto& titleTx = entity.addComponent<cro::Transform>();
    titleTx.setOrigin({ 512.f, 160.f, 0.f });
    titleTx.setPosition({ 960.f - 512.f, 900.f, 0.f });
    titleTx.setScale({ 1.5f, 1.5f, 1.5f });

    entity = m_mainMenuScene.createEntity();
    auto& gameSprite = entity.addComponent<cro::Sprite>();
    gameSprite.setTexture(uiTexture);
    gameSprite.setTextureRect(buttonArea);
    auto& gameTx = entity.addComponent<cro::Transform>();
    gameTx.setPosition({ 960.f - 256.f, 620.f, 0.f });
    gameTx.setScale({ 2.f, 2.f, 2.f });
    auto& gameControl = entity.addComponent<cro::UIInput>();
    gameControl.callbacks[cro::UIInput::MouseEnter] = mouseEnterCallback;
    gameControl.callbacks[cro::UIInput::MouseExit] = mouseExitCallback;
    gameControl.area = buttonArea;


    auto textEnt = m_mainMenuScene.createEntity();
    auto& gameText = textEnt.addComponent<cro::Text>(testFont);
    gameText.setString("Play");
    gameText.setColour(cro::Colour::Blue());
    auto& gameTextTx = textEnt.addComponent<cro::Transform>();
    gameTextTx.setPosition({ 90.f, 50.f, 0.f });
    gameTextTx.setParent(entity);

    entity = m_mainMenuScene.createEntity();
    auto& optionSprite = entity.addComponent<cro::Sprite>();
    optionSprite.setTexture(uiTexture);
    optionSprite.setTextureRect(buttonArea);
    auto& optionTx = entity.addComponent<cro::Transform>();
    optionTx.setPosition({ 960.f - 256.f, 460.f, 0.f });
    optionTx.setScale({ 2.f, 2.f, 2.f });
    auto& optionControl = entity.addComponent<cro::UIInput>();
    optionControl.callbacks[cro::UIInput::MouseEnter] = mouseEnterCallback;
    optionControl.callbacks[cro::UIInput::MouseExit] = mouseExitCallback;
    optionControl.area = buttonArea;

    textEnt = m_mainMenuScene.createEntity();
    auto& optionText = textEnt.addComponent<cro::Text>(testFont);
    optionText.setString("Options");
    optionText.setColour(cro::Colour::Magenta());
    auto& texTx = textEnt.addComponent<cro::Transform>();
    texTx.setParent(entity);
    texTx.move({ 64.f, 50.f, 0.f });

    entity = m_mainMenuScene.createEntity();
    auto& scoreSprite = entity.addComponent<cro::Sprite>();
    scoreSprite.setTexture(uiTexture);
    scoreSprite.setTextureRect(buttonArea);
    auto& scoreTx = entity.addComponent<cro::Transform>();
    scoreTx.setPosition({ 960.f - 256.f, 300.f, 0.f });
    scoreTx.setScale({ 2.f, 2.f, 2.f });
    auto& scoreControl = entity.addComponent<cro::UIInput>();
    scoreControl.callbacks[cro::UIInput::MouseEnter] = mouseEnterCallback;
    scoreControl.callbacks[cro::UIInput::MouseExit] = mouseExitCallback;
    scoreControl.area = buttonArea;

    textEnt = m_mainMenuScene.createEntity();
    auto& scoreText = textEnt.addComponent<cro::Text>(testFont);
    scoreText.setString("Scores");
    scoreText.setColour(cro::Colour::Green());
    auto& scoreTexTx = textEnt.addComponent<cro::Transform>();
    scoreTexTx.setParent(entity);
    scoreTexTx.move({ 74.f, 50.f, 0.f });

    entity = m_mainMenuScene.createEntity();
    auto& quitSprite = entity.addComponent<cro::Sprite>();
    quitSprite.setTexture(uiTexture);
    quitSprite.setTextureRect(buttonArea);
    auto& quitTx = entity.addComponent<cro::Transform>();
    quitTx.setPosition({ 960.f - 256.f, 140.f, 0.f });
    quitTx.setScale({ 2.f, 2.f, 2.f });

    textEnt = m_mainMenuScene.createEntity();
    auto& quitText = textEnt.addComponent<cro::Text>(testFont);
    quitText.setString("Quit");
    quitText.setColour(cro::Colour::Red());
    auto& quitTexTx = textEnt.addComponent<cro::Transform>();
    quitTexTx.setParent(entity);
    quitTexTx.move({ 88.f, 50.f, 0.f });

    auto quitCallback = m_uiSystem->addCallback([](cro::Entity, cro::uint64 flags)
    {
        if ((flags & cro::UISystem::LeftMouse)
            || flags & cro::UISystem::Finger)
        {
            cro::App::quit();
        }
    });
    auto& quitControl = entity.addComponent<cro::UIInput>();
    quitControl.callbacks[cro::UIInput::MouseUp] = quitCallback;
    quitControl.callbacks[cro::UIInput::MouseEnter] = mouseEnterCallback;
    quitControl.callbacks[cro::UIInput::MouseExit] = mouseExitCallback;
    quitControl.area = buttonArea;

    //entity = m_mainMenuScene.createEntity();
    //auto& buns = entity.addComponent<cro::Sprite>();
    //buns.setTexture(testFont.getTexture());
    ////buns.setTextureRect(testFont.getGlyph('@'));
    //entity.addComponent<cro::Transform>().move({ 30.f, 0.f, 0.f });
}