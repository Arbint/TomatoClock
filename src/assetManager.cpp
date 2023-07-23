#include "assetManager.h"
#include "iostream"
#include "core.h"

namespace TC
{
	AssetManager* AssetManager::manager{nullptr};

	AssetManager& AssetManager::Get()
	{
		if (!manager)
		{
			manager = new AssetManager();
		}
		return *manager;
	}

	Shared<sf::Texture> AssetManager::LoadTexture(const std::string& textureName)
	{
		auto found = LoadedTextures.find(textureName);
		if (found != LoadedTextures.end())
		{
			return (*found).second;
		}

		Shared<sf::Texture> texture{ new sf::Texture{} };
		std::string fullPath = assetPath + '/' + textureName;
		if (texture->loadFromFile(fullPath))
		{
			LoadedTextures.insert(std::make_pair(textureName, texture));
			//LOG("Load resource: %s name successfully!", textureName.c_str());
			return texture;
		}else
		{
			//LOG("Cannot load reource: %s", textureName.c_str());
		}

		return nullptr;
	}

	Shared<sf::Font> AssetManager::LoadFont(const std::string& fontName)
	{
		auto found = LoadedFonts.find(fontName);
		if (found != LoadedFonts.end())
		{
			return (*found).second;
		}

		Shared<sf::Font> font{ new sf::Font{} };
		std::string fullPath = assetPath + '/' + fontName;
		if (font->loadFromFile(fullPath))
		{
			LoadedFonts.insert(std::make_pair(fontName, font));
			//LOG("Load resource: %s name successfully!", fontName.c_str());
			return font;
		}
		else
		{
			//LOG("Cannot load reource: %s", fontName.c_str());
		}

		return nullptr;
	}

	void AssetManager::Init(const std::string& assetPath)
	{
		this->assetPath = assetPath;
	}

	void AssetManager::CleanUnusedAssets()
	{
		CleanUnusedTextures();
	}

	AssetManager::~AssetManager()
	{

	}

	AssetManager::AssetManager()
	{
	}

	void AssetManager::CleanUnusedTextures()
	{
		for (auto iter = LoadedTextures.begin(); iter != LoadedTextures.end(); /*incrementation relys on if a erase is issued*/)
		{
			if ((*iter).second.unique())
			{
				//LOG("cleaning up unused texture: %s", (*iter).first.c_str());
				(*iter).second.reset();
				iter = LoadedTextures.erase(iter); //clean up and increment iter to the next.
			}
			else
			{
				++iter;
			}
		}
	}

	void AssetManager::CleanUnusedFonts()
	{
		for (auto iter = LoadedFonts.begin(); iter != LoadedFonts.end(); /*incrementation relys on if a erase is issued*/)
		{
			if ((*iter).second.unique())
			{
				//LOG("cleaning up unused font: %s", (*iter).first.c_str());
				(*iter).second.reset();
				iter = LoadedFonts.erase(iter); //clean up and increment iter to the next.
			}
			else
			{
				++iter;
			}
		}
	}

} // namespace ly

