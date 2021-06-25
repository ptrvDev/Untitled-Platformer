#include "ResourcesManager.h"

std::unordered_map<std::string, std::shared_ptr<sf::Texture>> ResourcesManager::textureP;

std::shared_ptr<sf::Texture> ResourcesManager::Acquire(const std::string& name)
{
	const auto i = textureP.find(name);
	if (i != textureP.end())
	{
		return i->second;
	}
	else
	{
		std::cout << "sprite acquired\n";

		auto pTex = std::make_shared<sf::Texture>();
		pTex->loadFromFile(name);
		textureP.insert({ name,pTex });
		return pTex;
	}
}

void ResourcesManager::MurderOrphans()
{
	for (auto i = textureP.begin(); i != textureP.end(); )
	{
		if (i->second.unique())
		{
			i = textureP.erase(i);
		}
		else
		{
			++i;
		}
	}
}
