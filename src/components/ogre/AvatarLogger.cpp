//
// C++ Implementation: AvatarLogger
//
// Description:
//
//
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

#include "AvatarLogger.h"
#include "Avatar.h"
#include "domain/EmberEntity.h"
#include "GUIManager.h"
#include "services/config/ConfigService.h"
#include "framework/Time.h"
#include "domain/EntityTalk.h"
#ifdef _WIN32
#include "platform/platform_windows.h"
#endif


#include "framework/osdir.h"

namespace Ember {
namespace OgreView {

AvatarLogger::AvatarLogger(EmberEntity& avatarEntity)
: mChatLogger(nullptr)
{
	//Put log files in a "logs" subdirectory of the home directory.
	const std::string dir = EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_DATA) + "logs/";
	try {
		//make sure the directory exists

		oslink::directory osdir(dir);

		if (!osdir.isExisting()) {
			oslink::directory::mkdir(dir.c_str());
		}
		//perform setup of the stream
		std::stringstream logFileSS;
		logFileSS << dir << "/" << avatarEntity.getName() << "_chatlog.log";
		mChatLogger = std::unique_ptr<std::ofstream>(new std::ofstream(logFileSS.str().c_str(), std::ios::app));
		S_LOG_VERBOSE("Chat Logging set to write in [ " << logFileSS.str() << " ]");

		*mChatLogger << "-------------------------------------------------------" << std::endl;
		*mChatLogger << "Chat Logging Initialized at " <<  Time::getLocalTimeStr() << std::endl;
		*mChatLogger << "-------------------------------------------------------" << std::endl;

		//wait with connecting until everything has been properly set up
		GUIManager::getSingleton().AppendIGChatLine.connect(sigc::mem_fun(*this, &AvatarLogger::GUIManager_AppendIGChatLine));

	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating directory for logs." << ex);
	}
}


AvatarLogger::~AvatarLogger()
{
	*mChatLogger << "-------------------------------------------------------" << std::endl;
	*mChatLogger << "Chat Logging Ended at " <<  Time::getLocalTimeStr() << std::endl;
	*mChatLogger << "-------------------------------------------------------" << std::endl;
}

void AvatarLogger::GUIManager_AppendIGChatLine(const EntityTalk& entityTalk, EmberEntity* entity)
{
	*mChatLogger << "[" << Time::getLocalTimeStr() << "] <" <<  entity->getName() << "> says: " << entityTalk.getMessage() << std::endl;
}

AvatarLoggerParent::AvatarLoggerParent(Avatar& avatar)
{
	//we either already have an entity, or we need to wait until it's creeated
	mLogger = std::unique_ptr<AvatarLogger>(new AvatarLogger(avatar.getEmberEntity()));
}



}
}
