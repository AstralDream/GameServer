#include "stdafx.h"

ConfigManager::ConfigManager()
{

}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::reloadConfig()
{
	loadConfig("../config/config.json");
	loadMsgConfig("../config/cfg/msg.json");
	loadMapJson("../config/cfg/map.json");
	loadCharJson("../config/cfg/character.json");
	loadNpcJson("../config/cfg/npc.json");
	loadPropertyJson("../config/cfg/property.json");
	loadRobotJson("../config/cfg/robot.json");
	loadTaskJson("../config/cfg/task.json");
	loadTaskStepJson("../config/cfg/taskStep.json");
	loadCardJson("../config/cfg/card.json");
	loadGameLevelJson("../config/cfg/gamelevel.json");
	loadProductJson("../config/cfg/product.json");
	LOG_DEBUG(LogSystem::csl_color_green, "Load Config success");
	return true;
}

ServerConfig& ConfigManager::getConfig(const std::string& Name)
{
	return mMapServerConfig[Name];
}

const std::string ConfigManager::getMsg(int32 id)
{
	auto itr = mMapMsgConfig.find(id);
	if (itr == mMapMsgConfig.end())
		return "";
	return mMapMsgConfig[id];
}

bool ConfigManager::loadJson(const std::string& path, Json::Reader& jsonReader, Json::Value& jsonRoot)
{
	FILE* fp = NULL;
	fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		LOG_ERROR("%s read error!", path.c_str());
		return false;
	}

	int32 fileSize = Shared::GetFileLength(fp);
	char* jsonStr = new char[fileSize + 1];
	fread(jsonStr, 1, fileSize, fp);
	jsonStr[fileSize] = 0;
	fclose(fp);
	if (!jsonReader.parse(jsonStr, jsonRoot)) {
		delete[] jsonStr;
		LOG_ERROR("%s read error!", path.c_str());
		return false;
	}
	delete[] jsonStr;

	LOG_INFO("load json <%s> success", path.c_str());
	return true;
}

void ConfigManager::loadConfig(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;
	if (!loadJson(path, jsonReader, jsonRoot))
		return;
	mMapServerConfig.clear();
	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];
		ServerConfig sCfg;
		sCfg.Name = v["Name"].asString();
		sCfg.Host = v["Host"].asString();
		sCfg.Port = v["Port"].asInt();
		sCfg.User = v["User"].asString();
		sCfg.Password = v["Password"].asString();
		sCfg.DbName = v["DbName"].asString();
		mMapServerConfig.insert(std::make_pair(sCfg.Name, sCfg));
	}
}

void ConfigManager::loadMsgConfig(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;
	mMapMsgConfig.clear();
	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];
		mMapMsgConfig.insert(std::make_pair(v["ID"].asUInt(), v["Msg"].asString()));
	}
}

void ConfigManager::loadMapJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;
	mMapMapJson.clear();
	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0;i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		MapJson mapJson;
		mapJson.ID = v["ID"].asUInt();
		mapJson.Name = v["Name"].asString();
		mapJson.Type = v["Type"].asUInt();

		mapJson.Row = v["Row"].asUInt();
		mapJson.Col = v["Col"].asUInt();
		mapJson.Cell = v["Cell"].asFloat();
		mapJson.LogicRow = v["LogicRow"].asUInt();
		mapJson.LogicCol = v["LogicCol"].asUInt();
		mapJson.ViewCell = v["ViewCell"].asUInt();
		mapJson.LineMax = v["LineMax"].asUInt();
		mMapMapJson.insert(std::make_pair(mapJson.ID, mapJson));
	}
}

void ConfigManager::loadCharJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapCharJson.clear();
	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		CharJson charJson;
		charJson.ID = v["ID"].asUInt();
		charJson.Name = v["Name"].asString();
		charJson.ActorId = v["ActorId"].asUInt();
		charJson.PropertyId = v["PropertyId"].asUInt();
		charJson.Type = v["Type"].asUInt();
		charJson.Sex = v["Sex"].asUInt();
		mMapCharJson.insert(std::make_pair(charJson.ID, charJson));
	}
}

void ConfigManager::loadNpcJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapNpcJson.clear();
	mMapMapNpcJson.clear();

	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		NpcJson npcJson;
		npcJson.ID = v["ID"].asUInt();
		npcJson.Name = v["Name"].asString();
		npcJson.Type = v["Type"].asInt();
		npcJson.CharId = v["CharId"].asUInt();
		npcJson.MapId = v["MapId"].asUInt();
		npcJson.PosX = v["PosX"].asInt();
		npcJson.PosY = v["PosY"].asInt();
		npcJson.script = v["script"].asString();
		mMapNpcJson.insert(std::make_pair(npcJson.ID, npcJson));

		auto itr = mMapMapNpcJson.find(npcJson.MapId);
		if (itr != mMapMapNpcJson.end())
		{
			std::vector<NpcJson*>& lstNpcJson = itr->second;
			lstNpcJson.push_back(&mMapNpcJson[npcJson.ID]);
		}
		else
		{
			std::vector<NpcJson*> lstNpcJson;
			lstNpcJson.push_back(&mMapNpcJson[npcJson.ID]);
			mMapMapNpcJson.insert(std::make_pair(npcJson.MapId, lstNpcJson));
		}
	}
}

void ConfigManager::loadPropertyJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mPropertyJson.clear();

	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		PropertyJson propertyJson;
		propertyJson.ID = v["ID"].asUInt();
		propertyJson.Speed = v["Speed"].asFloat();
		propertyJson.MaxHp = v["MaxHp"].asUInt();
		propertyJson.MaxMp = v["MaxMp"].asUInt();
		propertyJson.Attack = v["Attack"].asUInt();
		propertyJson.Defense = v["Defense"].asUInt();
		propertyJson.SpeedUp = v["SpeedUp"].asFloat();
		propertyJson.MaxHpUp = v["MaxHpUp"].asUInt();
		propertyJson.MaxMpUp = v["MaxMpUp"].asUInt();
		propertyJson.AttackUp = v["AttackUp"].asUInt();
		propertyJson.DefenseUp = v["DefenseUp"].asUInt();
		mPropertyJson.insert(std::make_pair(propertyJson.ID, propertyJson));
	}
}

void ConfigManager::loadRobotJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapRobotJson.clear();

	Json::Value cf = jsonRoot["config"];

	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		RobotJson robotJson;
		robotJson.ID = v["ID"].asUInt();
		robotJson.User = v["User"].asString();
		robotJson.Password = v["Password"].asString();
		mMapRobotJson.insert(std::make_pair(robotJson.ID, robotJson));
	}
}

void ConfigManager::loadTaskJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapTaskJson.clear();

	Json::Value cf = jsonRoot["config"];
	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		TaskJson taskJson;
		taskJson.ID = v["ID"].asUInt();
		taskJson.Name = v["Name"].asString();
		taskJson.NextTaskId = v["NextTaskId"].asUInt();
		taskJson.PrevTaskId = v["PrevTaskId"].asUInt();
		mMapTaskJson[taskJson.ID] = taskJson;
	}
}

void ConfigManager::loadTaskStepJson(const std::string& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapTaskStepJson.clear();
	mMapTaskStepJsonMap.clear();

	Json::Value cf = jsonRoot["config"];
	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		TaskStepJson taskStepJson;
		taskStepJson.ID = v["ID"].asUInt();
		taskStepJson.Name = v["Name"].asString();
		taskStepJson.TaskId = v["TaskId"].asUInt();
		taskStepJson.TaskStepId = v["TaskStepId"].asUInt();
		mMapTaskStepJson[taskStepJson.ID] = taskStepJson;

		TaskStepJson* taskStep = &mMapTaskStepJson[taskStepJson.ID];
		auto itr = mMapTaskStepJsonMap.find(taskStepJson.TaskId);
		if (itr != mMapTaskStepJsonMap.end())
		{
			auto& mapTaskStep = itr->second;
			mapTaskStep[taskStepJson.TaskStepId] = taskStep;
		}
		else
		{
			std::map<int32, TaskStepJson*> mapTaskStep;
			mapTaskStep[taskStepJson.TaskStepId] = taskStep;
			mMapTaskStepJsonMap[taskStepJson.TaskId] = mapTaskStep;
		}
	}
}

void ConfigManager::loadCardJson(cstring& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapCard.clear();
	mMapNameCard.clear();
	Json::Value cf = jsonRoot["config"];
	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		CardJson cardJson;
		cardJson.ID = v["ID"].asUInt();
		cardJson.Number = v["Number"].asUInt();
		cardJson.Color = v["Color"].asInt();
		cardJson.Name = v["Name"].asString();
		mMapCard.insert(std::make_pair(cardJson.ID, cardJson));
		mMapNameCard[cardJson.Name] = getCardJson(cardJson.ID);
	}
}

void ConfigManager::loadGameLevelJson(cstring& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapGameLevelJson.clear();

	Json::Value cf = jsonRoot["config"];
	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		GameLevelJson gamelvJson;
		gamelvJson.ID = v["ID"].asUInt();
		gamelvJson.Mingold = v["Mingold"].asUInt();
		gamelvJson.Maxgold = v["Maxgold"].asUInt();
		gamelvJson.Poundage = v["Poundage"].asUInt();
		gamelvJson.Maxround = v["Maxround"].asUInt();
		gamelvJson.Desc = v["Desc"].asString();

		mMapGameLevelJson.insert(std::make_pair(gamelvJson.ID, gamelvJson));
	}
}

void ConfigManager::loadProductJson(cstring& path)
{
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!loadJson(path, jsonReader, jsonRoot))
		return;

	mMapProductJson.clear();

	Json::Value cf = jsonRoot["config"];
	for (uint32 i = 0; i < cf.size(); ++i)
	{
		Json::Value v = cf[i];

		ProductJson productJson;
		productJson.ID = v["ID"].asUInt();
		productJson.Desc = v["Desc"].asString();
		productJson.Price = v["Price"].asUInt();
		mMapProductJson.insert(std::make_pair(productJson.ID, productJson));
	}
}

MapJson* ConfigManager::getMapJson(uint32 id)
{
	auto itr = mMapMapJson.find(id);
	if (itr != mMapMapJson.end())
		return &itr->second;

	return NULL;
}

CharJson* ConfigManager::getCharJson(uint32 id)
{
	auto itr = mMapCharJson.find(id);
	if (itr != mMapCharJson.end())
		return &itr->second;

	return NULL;
}

uint32 ConfigManager::getCharJsonRandId()
{
	int32 idx = rand() % mMapCharJson.size();

	for (auto itr : mMapCharJson)
	{
		if (idx == 0)
			return itr.first;
		idx--;
	}
	return 0;
}

NpcJson* ConfigManager::getNpcJson(uint32 npcId)
{
	auto itr = mMapNpcJson.find(npcId);
	if (itr != mMapNpcJson.end())
		return &itr->second;

	return NULL;
}

std::vector<NpcJson*>* ConfigManager::getNpcJsons(uint32 mapId)
{
	auto itr = mMapMapNpcJson.find(mapId);
	if (itr != mMapMapNpcJson.end())
		return &itr->second;

	return NULL;
}

PropertyJson* ConfigManager::getPropertyJson(uint32 pId)
{
	auto itr = mPropertyJson.find(pId);
	if (itr != mPropertyJson.end())
		return &itr->second;

	return NULL;
}

RobotJson* ConfigManager::getRobotJson(uint32 id)
{
	auto itr = mMapRobotJson.find(id);
	if (itr != mMapRobotJson.end())
		return &itr->second;

	return NULL;
}

TaskJson* ConfigManager::getTaskJson(uint32 id)
{
	auto itr = mMapTaskJson.find(id);
	if (itr != mMapTaskJson.end())
		return &itr->second;

	return NULL;
}

TaskStepJson* ConfigManager::getTaskStepJson(uint32 id)
{
	auto itr = mMapTaskStepJson.find(id);
	if (itr != mMapTaskStepJson.end())
		return &itr->second;

	return NULL;
}

MapTaskStepJson* ConfigManager::GetTaskSteps(uint32 taskId)
{
	auto itr = mMapTaskStepJsonMap.find(taskId);
	if (itr != mMapTaskStepJsonMap.end())
		return &itr->second;
	return NULL;
}

bool ConfigManager::GetTaskStepIsEndStep(uint32 ID)
{
	TaskStepJson* taskStep = getTaskStepJson(ID);
	if (taskStep == NULL) return false;
	MapTaskStepJson* mapTaskStepJson = GetTaskSteps(taskStep->TaskId);
	if (mapTaskStepJson == NULL) return false;
	auto itr = mapTaskStepJson->find(taskStep->TaskStepId + 1);
	if (itr != mapTaskStepJson->end())
		return false;
	return true;
}

CardJson* ConfigManager::getCardJson(uint32 id)
{
	auto itr = mMapCard.find(id);
	if (itr != mMapCard.end())
		return &itr->second;

	return NULL;
}

CardJson* ConfigManager::getCardJsonByName(cstring& name)
{
	auto itr = mMapNameCard.find(name);
	if (itr != mMapNameCard.end())
		return itr->second;

	return NULL;
}

GameLevelJson* ConfigManager::getGameLevelJson(uint32 id)
{
	auto itr = mMapGameLevelJson.find(id);
	if (itr != mMapGameLevelJson.end())
		return &itr->second;
	return NULL;
}

ProductJson* ConfigManager::getProductJson(uint32 id)
{
	auto itr = mMapProductJson.find(id);
	if (itr != mMapProductJson.end())
		return &itr->second;
	return NULL;
}

void ConfigManager::loadAllFiles()
{
	std::vector<std::string> files;
	
	Shared::GetDirectoryFiles("config", files);
	for (auto path : files)
	{
		FILE* fp = NULL;
		fopen_s(&fp, path.c_str(), "rb");
		if (fp == NULL)
			continue;
		int32 fileSize = Shared::GetFileLength(fp);
		if (fileSize <= 0)
			continue;
		int8* data = new int8[fileSize];
		fread_s(data, fileSize, fileSize, 1, fp);
		mMapFilesStream.insert(std::make_pair(path, std::make_pair(data, fileSize)));
		fclose(fp);
	}
}
