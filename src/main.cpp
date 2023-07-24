#include "Application.h"
#include <memory>
#include "assetManager.h"
#include "config.h"
int main()
{
    TC::AssetManager::Get().Init(GetResouceDir());
    TC::ApplicationDef appDef{800,1000, "Tomato Clock", sf::Style::Close|sf::Style::Titlebar};
    std::unique_ptr<TC::Application> app{new TC::Application{appDef}};
    app->Run();
}