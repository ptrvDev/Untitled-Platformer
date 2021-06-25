#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ResourcesManager
{

public:

	static std::shared_ptr<sf::Texture> Acquire(const std::string& name);
	static void MurderOrphans(); //change name

private:

	static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureP;

};




