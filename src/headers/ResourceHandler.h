#ifndef RESOURCEHANDLE
#define RESOURCEHANDLE

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

namespace Textures{
     enum ID { Landscape, Player, Henchman, Boss, Gunman, Brute,
	 		Block, Ladder, Platform, Door, HealthBar, Bullet, Logo,
			PlayerW, HenchW, Sword, BruteW, BossW,
			PlayerA};
}

namespace Fonts{
	enum ID {MenuFont};
}


// Class to store resources, taken from the book
// SFML Game Development
// Jan Haller, Henrik Vogelius Hansson, Et al
// https://www.packtpub.com/game-development/sfml-game-development

template <typename Resource, typename Identifier>
class ResourceHandler
{
	public:
		void load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource& get(Identifier id);
		const Resource& get(Identifier id) const;


	private:
		void insertResource(Identifier id, std::unique_ptr<Resource> resource);


	private:
		std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHandler<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHandler<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHandler<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHandler<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHandler<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource) 
{
	// Insert and check success
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}
#endif