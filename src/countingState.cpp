#include "countingState.h"
#include "Application.h"
namespace TC
{
    void CountingState::OnStateSet(Application *app)
    {
        app->SetButtonText("Stop");
    }

    void CountingState::TickState(Application *app)
    {
        timeElapsed+= app->GetDeltaTime();
        while (timeElapsed>1.f)
        {
            timeElapsed-=1.f;
            if(!app->IncrementSec(-1))
            {
                app->SwithToAleartState();
            }
        }
    }

    CountingState::CountingState()
    : timeElapsed{0.f}
    {

    }

    void CountingState::HandleEvent(const sf::Event &event, Application *app)
    {
         if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(app->IsMouseOnButton())
            {
                app->SwithToSettingState();
            }
        }
    }
}