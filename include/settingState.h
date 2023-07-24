#pragma once

#include "state.h"
#include <vector>
#include <time.h>
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
        std::tm GetTimeSetting() const { return timeSetting; }
    private:
        sf::Sprite hourUpArrow;
        sf::Sprite hourDownArrow;

        sf::Sprite minuteUpArrow;
        sf::Sprite minuteDownArrow;

        sf::Sprite secondUpArrow;
        sf::Sprite secondDownArrow;
        
        std::tm timeSetting;
        
        void InitArrow(sf::Sprite& arrow, const sf::Texture& texture, float rotation, const sf::Vector2f& position);
    };
} 
