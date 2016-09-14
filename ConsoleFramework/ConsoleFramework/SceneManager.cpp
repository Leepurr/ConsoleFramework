/*
Original code by Martyn Leeper (www.martynleeper.com)
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source
distribution.
*/
#include "SceneManager.h"
#include "Core\GObject.h"
#include "Game\Character.h"
#include <stdexcept>
#include <algorithm>


bool SceneManager::_constructed = false;

SceneManager::SceneManager()
{
	if (_constructed)
	{
		throw std::runtime_error("Scene Manager already exists!\n");
	}
	_constructed = true;
}

SceneManager::~SceneManager()
{
	_constructed = false;
}

bool SceneManager::AddEntity(std::shared_ptr<GObject> object)
{
	bool status = false;
	if (object != nullptr)
	{
		gameObjects.push_back(object);
		status = true;
	}
	return status;
}

bool SceneManager::DeleteEntity(std::shared_ptr<GObject> object)
{
	bool status = false;
	if (object != nullptr && !gameObjects.empty())
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), object), gameObjects.end());
		status = true;
	}
	return status;
}

GObject* SceneManager::GetGObjectByID(const char *GObjectID) const
{
	if (!gameObjects.empty())
	{
		std::vector<std::shared_ptr<GObject>>::const_iterator itr;
		for (itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
		{
			if (strcmp((*itr).get()->GetID().c_str(), GObjectID))
			{
				return itr->get();
			}
		}
	}
	return nullptr;
}

const std::vector<std::shared_ptr<GObject>>& SceneManager::GetGObects() const
{
	return gameObjects;
}