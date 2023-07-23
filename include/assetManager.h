#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "core.h"

namespace TC
{
	/*
	* Class the help mamage assets
	* Call Init to assign the path of the assets before loading.
	*/
	class AssetManager
	{
	public:
		static AssetManager& Get();

		Shared<sf::Texture> LoadTexture(const std::string& textureName);
		Shared<sf::Font> LoadFont(const std::string& fontName);

		void Init(const std::string& assetPath);
		void CleanUnusedAssets();
		const std::string& GetAssetPath() const { return assetPath; }
		~AssetManager();
	private:
		AssetManager();

		std::map<std::string, Shared<sf::Texture>> LoadedTextures;
		std::map<std::string, Shared<sf::Font>> LoadedFonts;

		static AssetManager* manager;

		std::string assetPath;

		void CleanUnusedTextures();
		void CleanUnusedFonts();
	};
}
