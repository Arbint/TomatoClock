#pragma once

#include "state.h"
#include <vector>
namespace TC
{
    class SettingState : public State
    {
    public:
        SettingState();
        virtual void HandleEvent(const sf::Event& event, Application* app) override;
        virtual void OnStateSet(Application* app) override;
        virtual void OnStateOff(Application* app) override;
        virtual void Draw(sf::RenderWindow& window) override;

    private:
        sf::Sprite hourUpArrow;
        sf::Sprite hourDownArrow;

        sf::Sprite minuteUpArrow;
        sf::Sprite minuteDownArrow;

        sf::Sprite secondUpArrow;
        sf::Sprite secondDownArrow;

        void InitArrow(sf::Sprite& arrow, const sf::Texture& texture, float rotation, const sf::Vector2f& position);
    };
} 
