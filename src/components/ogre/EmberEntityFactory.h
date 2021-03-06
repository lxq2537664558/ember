/*
 Copyright (C) 2004  Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef DIMEENTITYFACTORY_H
#define DIMEENTITYFACTORY_H

#include "EmberOgrePrerequisites.h"

#include <Eris/Factory.h>

#include <Atlas/Objects/Entity.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <set>

namespace Eris
{
class Entity;
class View;
class TypeService;
class TypeInfo;
class Avatar;
}

namespace Ember
{
class EmberEntity;
namespace EntityMapping
{
class EntityMapping;
class EntityMappingManager;
}
namespace OgreView
{

class ViewEmberEntity;
class WorldEmberEntity;
class Scene;

/**
 * @brief Creates the EmberEntities required.
 *
 * Basically this attaches to Eris and creates Entites on demand.
 * @see Eris::Factory
 */
class EmberEntityFactory: public Eris::Factory, public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 *
	 * This should be instantiated by EmberOgre or similar high level object. Note that Eris upon shutdown will delete all registered factories, so don't delete an instance of this yourself.
	 */
	EmberEntityFactory(Eris::View& view, Scene& scene, EntityMapping::EntityMappingManager& mappingManager);
	virtual ~EmberEntityFactory();

	/**
	 * @brief Will always return true as we want all entities created to be handled by this class.
	 */
	virtual bool accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type);

	/**
	 Creates instances of EmberEntity and its subclasses.
	 */
	virtual Eris::Entity* instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w);

	/** retrieve this factory's priority level; higher priority factories
	 get first chance to process a received Atlas entity. The default implementation
	 returns one. */
	virtual int priority();

	/**
	 * @brief Emitted when the factory is being deleted.
	 */
	sigc::signal<void> EventBeingDeleted;

protected:

	Eris::View& mView;

	Eris::TypeService& mTypeService;

	/**
	 * @brief The scene which will handle the entities.
	 */
	Scene& mScene;

	EntityMapping::EntityMappingManager& mMappingManager;

	/**
	 * @brief Deletes the entity mapping.
	 *
	 * This serves as a callback to Entity::BeingDeleted, allowing us to remove any bound mappings.
	 *
	 * @param mapping The mapping which should be deleted.
	 */
	void deleteMapping(EntityMapping::EntityMapping* mapping);

};

}

}

#endif // DIMEENTITYFACTORY_H
