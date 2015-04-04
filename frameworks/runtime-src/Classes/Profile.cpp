#include "Profile.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/reader.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "Utils.h"
#include "cocos2d.h"
#include <fstream>

std::string user_id;

#define RANDOM_UID_EVERY_TIME

void load_profile(const std::string& path)
{
	std::string profile_content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	if (!profile_content.empty())
	{
		rapidjson::Document d;
		d.Parse<0>(profile_content.c_str());
		if (d.HasParseError())
		{
			cocos2d::log("profile ParseError %s", d.GetParseError());
		}
		else
		{
			if (d.HasMember("user_id"))
			{
				user_id = d["user_id"].GetString();
			}
		}
	}
}

void initialize_profile()
{
#ifndef RANDOM_UID_EVERY_TIME
	std::string profile_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "boids.profile";
	cocos2d::log("profile path: %s", profile_path.c_str());

	load_profile(profile_path);

	if (user_id.empty()) //ªπ «ø’µƒÀµ√˜÷Æ«∞∂¡»° ß∞‹¡À
	{
#endif
		Utils::initRandom2ByTime();
		user_id = Utils::generateRandomString(8); //œ÷‘⁄ «ÀÊª˙…˙≥…¡À£¨÷Æ∫Û“™‘Ÿ∏ƒ°£øÏÀŸ◊¢≤·£ø

#ifndef RANDOM_UID_EVERY_TIME
		rapidjson::Document d;
		d.SetObject();
		auto& allocator = d.GetAllocator();

		d.AddMember("user_id", user_id.c_str(), allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		std::ofstream file;
		file.open(profile_path);
		file << buffer.GetString();
		file.close();
	}

	std::string debug_profile_path = "boids.profile"; //∂¡µ±«∞ƒø¬ºœ¬µƒ°£µ˜ ‘µƒprofile¿Ô”–µƒ÷µª·∏≤∏«writableƒø¬º¿Ôµƒ÷µ
	load_profile(debug_profile_path);
#endif
}