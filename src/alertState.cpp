#include "alertState.h"
#include "Application.h"
#include "assetManager.h"

namespace TC
{
    AlertState::AlertState()
        : State{},
        alertSound{},
        repeatTimer{},
        repeatInterval{2.f}

    {
        auto alertSoundBuffer = AssetManager::Get().LoadAudio("alertSound.wav");
        alertSound.setBuffer(*alertSoundBuffer);
    }

    void AlertState::OnStateSet(Application *app)
    {
        app->SetButtonText("Restart");
        alertSound.play();
        repeatTimer = 0.f;
    }

    void AlertState::OnStateOff(Application *app)
    {
        alertSound.pause();
    }

    void AlertState::HandleEvent(const sf::Event &event, Application *app)
    {
         if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(app->IsMouseOnButton())
            {
                app->ReStartCounting();
            }
        }
    }

    void AlertState::TickState(Application *app)
    {
        if(alertSound.getStatus() != sf::Sound::Status::Playing)
        {
            repeatTimer += app->GetDeltaTime();
            if(repeatTimer > repeatInterval)
            {
                repeatTimer = 0;
                alertSound.play();
            }
        }
    }

} // namespace TC
