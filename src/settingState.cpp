#include "settingState.h"
#include "Application.h"
#include "assetManager.h"
#include <vector>
namespace TC
{
    SettingState::SettingState()
        : State{},hourUpArrow{}, hourDownArrow{}, minuteUpArrow{}, minuteDownArrow{}, secondUpArrow{}, secondDownArrow{}
    {
        Shared<sf::Texture> arrowTexture = AssetManager::Get().LoadTexture("arrow.png");
        float startPosX = 240.f;
        float startPosY = 570.f;
        float horizontalStride = 160.f;
        float verticalStride = 120.f;
        InitArrow(hourUpArrow, *arrowTexture, 0.f, sf::Vector2f{startPosX, startPosY});
        InitArrow(hourDownArrow, *arrowTexture, 180.f, sf::Vector2f{startPosX, startPosY+verticalStride});

        InitArrow(minuteUpArrow, *arrowTexture, 0.f, sf::Vector2f{startPosX + horizontalStride, startPosY});
        InitArrow(minuteDownArrow, *arrowTexture, 180.f, sf::Vector2f{startPosX + horizontalStride, startPosY + verticalStride});

        InitArrow(secondUpArrow, *arrowTexture, 0.f, sf::Vector2f{startPosX + horizontalStride*2, startPosY});
        InitArrow(secondDownArrow, *arrowTexture, 180.f, sf::Vector2f{startPosX + horizontalStride*2, startPosY + verticalStride});
    }

    void SettingState::HandleEvent(const sf::Event &event, Application* app)
    {
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(app->IsMouseOnButton())
            {
                app->SwithToCountingState();
            }

            if(app->IsMouseOnSprite(hourUpArrow))
            {
                app->IncrementHour(1);
            }

            if(app->IsMouseOnSprite(hourDownArrow))
            {
                app->IncrementHour(-1);
            }

            if(app->IsMouseOnSprite(minuteUpArrow))
            {
                app->IncrementMinute(1);
            }
            
            if(app->IsMouseOnSprite(minuteDownArrow))
            {
                app->IncrementMinute(-1);
            }

            if(app->IsMouseOnSprite(secondUpArrow))
            {
                app->IncrementSec(1);
            }

            if(app->IsMouseOnSprite(secondDownArrow))
            {
                app->IncrementSec(-1);
            }
        }
    }

    void SettingState::OnStateSet(Application *app)
    {
        app->SetButtonText("Start");
    }

    void SettingState::OnStateOff(Application *app)
    {
        
    }

    void SettingState::Draw(sf::RenderWindow &window)
    {
        window.draw(hourUpArrow);
        window.draw(hourDownArrow);
        window.draw(minuteUpArrow);
        window.draw(minuteDownArrow);
        window.draw(secondUpArrow);
        window.draw(secondDownArrow);
    }

    void SettingState::InitArrow(sf::Sprite &arrow, const sf::Texture &texture, float rotation, const sf::Vector2f &position)
    {
        arrow.setTexture(texture);
        arrow.setOrigin(texture.getSize().x/2.f, texture.getSize().y/2.f);
        arrow.setRotation(rotation);
        arrow.setPosition(position);
        arrow.setScale(0.7, 0.7);
    }
}