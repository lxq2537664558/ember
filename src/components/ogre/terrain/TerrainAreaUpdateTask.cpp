/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "TerrainAreaUpdateTask.h"
#include "TerrainArea.h"
#include <Mercator/Terrain.h>
#include <Mercator/Area.h>
namespace EmberOgre
{

namespace Terrain
{
TerrainAreaUpdateTask::TerrainAreaUpdateTask(Mercator::Terrain& terrain, TerrainArea& terrainArea, const TerrainShader* shader, ShaderUpdateSlotType markForUpdateSlot)
: mShader(shader), mMarkForUpdateSlot(markForUpdateSlot), TerrainAreaTaskBase::TerrainAreaTaskBase(terrain, terrainArea)
{

}

TerrainAreaUpdateTask::~TerrainAreaUpdateTask()
{
}

void TerrainAreaUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	mTerrain.updateArea(mTerrainArea.getArea());
}

void TerrainAreaUpdateTask::executeTaskInMainThread()
{
	if (mShader) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		mMarkForUpdateSlot(mShader, mTerrainArea.getArea());
	}
}


}

}