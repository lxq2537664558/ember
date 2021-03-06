//
// C++ Implementation: TerrainMod
//
// Description:
//
//
// Author: Tamas Bates <rhymre@gmail.com>, (C) 2008
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainMod.h"

#include "components/terrain/TerrainModTranslator.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

TerrainMod::TerrainMod(Eris::Entity& entity, const Atlas::Message::MapType& data) :
		mEntity(entity), mTranslator(new Ember::Terrain::TerrainModTranslator(data))
{
}

TerrainMod::~TerrainMod()
{
	mTranslator->reset();
	onModDeleted();
	delete mTranslator;
}

void TerrainMod::init()
{
	observeEntity();
}

const std::string& TerrainMod::getEntityId() const
{
	return mEntity.getId();
}

void TerrainMod::attributeChanged(const Atlas::Message::Element& attributeValue)
{
	if (attributeValue.isMap()) {
		mTranslator = new Ember::Terrain::TerrainModTranslator(attributeValue.Map());
	}
	onModChanged();
}

void TerrainMod::entity_Moved()
{
	onModChanged();
}

void TerrainMod::observeEntity()
{
	mAttrChangedSlot.disconnect();
	mAttrChangedSlot = sigc::mem_fun(*this, &TerrainMod::attributeChanged);
	mEntity.observe("terrainmod", mAttrChangedSlot);
	mEntity.Moved.connect(sigc::mem_fun(*this, &TerrainMod::entity_Moved));
}

Eris::Entity& TerrainMod::getEntity() const
{
	return mEntity;
}

const Ember::Terrain::TerrainModTranslator* TerrainMod::getTranslator() const
{
	return mTranslator;
}

void TerrainMod::onModDeleted()
{
	EventModDeleted.emit();
}

void TerrainMod::onModChanged()
{
	EventModChanged.emit();
}

}
}
}
