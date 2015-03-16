//
//  Utils.cpp
//  Lalune
//
//  Created by Xin Xu on 7/16/14.
//
//

#include "Utils.h"
#include <random>
#include <stdarg.h>
#include <memory>
#include "Constants.h"
#include "BoidsMath.h"
//#include "AI.h"
#include "Replay.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace cocos2d;

void Utils::split(const std::string& input, std::vector<std::string>& output, char delimiter) {
	output.clear();

	std::string s;
	for (unsigned int i = 0; i < input.size(); ++i)	{
		if (input[i] == delimiter) {
			output.push_back(s);
			s.clear();
		}
		else {
			s += input[i];
		}
	}
	if (!s.empty())
	{
		output.push_back(s);
	}
}

void Utils::split(const std::string& input, std::vector<int>& output, char delimiter) {
	output.clear();
	std::vector<std::string> output_s;
	split(input, output_s, delimiter);
	output.reserve(output_s.size());

	for (unsigned int i = 0; i < output_s.size(); ++i)
	{
		int v;
		sscanf(output_s[i].c_str(), "%d", &v);
		output.push_back(v);
	}
}

void Utils::split(const std::string& input, std::vector<std::pair<int, int>>& output, char delimiter)
{
	output.clear();
	std::vector<std::string> output_s;
	split(input, output_s, delimiter);
	output.reserve(output_s.size());

	for (unsigned int i = 0; i < output_s.size(); ++i)
	{
		int v1, v2;
		sscanf(output_s[i].c_str(), "%d,%d", &v1, &v2);
		output.push_back(std::make_pair(v1, v2));
	}
}

void Utils::split(const std::string& input, std::set<int>& output, char delimiter) {
	output.clear();
	std::vector<std::string> output_s;
	split(input, output_s, delimiter);

	for (unsigned int i = 0; i < output_s.size(); ++i)
	{
		int v;
		sscanf(output_s[i].c_str(), "%d", &v);
		output.insert(v);
	}
}

std::pair<int, int> Utils::parsePos(const std::string& v) {

	int x, y;
	sscanf(v.c_str(), "(%d,%d)", &x, &y);
	return std::make_pair(x, y);
}

std::pair<int, int> Utils::parsePosNoParentheses(const std::string& v) {

	int x, y;
	sscanf(v.c_str(), "%d,%d", &x, &y);
	return std::make_pair(x, y);

}

cocos2d::Vec2 Utils::pos2Vec2NoParentheses(const std::string& v) {

	int x, y;
	sscanf(v.c_str(), "%d,%d", &x, &y);
	return cocos2d::Vec2(x, y);

}

float Utils::parseValueOrPercent(const std::string& input, bool& isPercent) {
    char lastC = input[input.size() - 1];
    float ret = 0;
    if (lastC == 'p') {
        isPercent = true;
        if (input.size() > 1) {
            auto numberString = input.substr(0, input.size() - 1);
            ret = atof(numberString.c_str());
            ret /= 100;
        }
    } else {
        isPercent = false;
        ret = atof(input.c_str());
    }
    return ret;
}

inline int hex2dec(char a)
{
	if (a >= 'a') return (a - 'a') + 10;
	return a - '0';
}

int Utils::colorCharToInt(char a1, char a2)
{
	return hex2dec(a1) * 16 + hex2dec(a2);
}

int Utils::toInt(std::string v) {
	int i = -1;
	sscanf(v.c_str(), "%d", &i);
	return i;
}

std::string Utils::toStr(int v)
{
	char buf[30];
	sprintf(buf, "%d", v);
	return buf;
}

double Utils::toDouble(std::string v) {
	double d = -1.0;
	sscanf(v.c_str(), "%lf", &d);
	return d;
}

cocos2d::Point Utils::toPoint(std::pair<int, int> p)
{
	return cocos2d::Point(p.first, p.second);
}

point Utils::toPt(const cocos2d::Point& pt)
{
	return point(pt.x, pt.y);
}

cocos2d::Point Utils::toCocosPt(point pt)
{
	return cocos2d::Point(pt.get<0>(), pt.get<1>());
}

cocos2d::Vec2 Utils::toCocosVec2(const segment& s)
{
	return cocos2d::Vec2(toCocosPt(s.first), toCocosPt(s.second));
}

std::string Utils::toCP936(const std::string& s)
{
//#ifdef WIN32
//	memset(wStr, 0, sizeof(wStr));
//	nWCount = MultiByteToWideChar(nCodePage, 0, pStr, nCount, wStr, 0);  
//	nWCount = MultiByteToWideChar(nCodePage, 0, pStr, nCount, wStr, nWCount); 
//#else
//	assert(false && "non-windows version of toCP936 haven't been implemented yet.");
//#endif
	//TODO
	return s;
}

std::string Utils::displayableText(const std::string& s)
{
#ifdef WIN32
	return toCP936(s);
#else
	return s;
#endif
}

bool Utils::parseFromBinary(FILE* f, cocos2d::Point& pt)
{
	if (fread(&pt.x, sizeof(float), 1, f) != 1) return false;
	if (fread(&pt.y, sizeof(float), 1, f) != 1) return false;
	return true;
}

void Utils::serializeToBinary(FILE* f, const cocos2d::Point& pt)
{
	fwrite(&pt.x, sizeof(float), 1, f);
	fwrite(&pt.y, sizeof(float), 1, f);
}

double Utils::percent2double(std::string p)
{
	double v = 0.0;
	sscanf(p.c_str(), "%lf%%", &v);
	return v * 0.01;
}

std::pair<int, int> Utils::toPair(cocos2d::Vec2 v)
{
	return std::make_pair((int)v.x, (int)v.y);
}

std::string Utils::stringFormat(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

std::string Utils::generateRandomString(int len)
{
	std::string ret(len, ' ');
	for (int i = 0; i < len; ++i)
	{
		int r = randomNumber2(62) - 1;
		if (r < 10)
		{
			ret[i] = '0' + r;
		}
		else if (r < 36)
		{
			ret[i] = 'a' + r - 10;
		}
		else
		{
			ret[i] = 'A' + r - 36;
		}
	}
	return ret;
}

std::string Utils::encodeHex(const unsigned char* data, unsigned len)
{
	char* buf = new char[len * 2 + 1];
	for (unsigned j = 0; j != len; ++j)
	{
		sprintf(buf + j * 2, "%02x", data[j]);
	}
	std::string ret(buf, len * 2);
	delete buf;
	return ret;
}

void Utils::logAI(std::string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
//	cocos2d::log("[%d] %s", AI::getInstance()->get_current_frame(), formatted.get());
}

void Utils::logr(const std::string& content, bool only_save_to_replay)
{
	if (replay_to_play)
	{
		replay_to_play->log(content);
		if (!only_save_to_replay)
		{
			cocos2d::log("[%d] %s", replay_to_play->_currentFrame, content.c_str());
		}
	}

	if (replay_to_save)
	{
		replay_to_save->log(content);
		if (!only_save_to_replay && !replay_to_play) //前面没打印，后面才打印，打印一遍就够了
		{
			cocos2d::log("[%d] %s", replay_to_save->_currentFrame, content.c_str());
		}
	}
}

cocos2d::Node* Utils::getCurrentMapLayer()
{
	//外部要自己判断是不是nullptr

	auto director = Director::getInstance();
	if (director)
	{
		auto scene = director->getRunningScene();
		if (scene)
		{
			auto battle_layer = scene->getChildByTag(BATTLE_LAYER_TAG);
			if (battle_layer)
			{
				return battle_layer->getChildByTag(MAP_LAYER_TAG);
			}
		}
	}

	return nullptr;
}

cocos2d::DrawNode* Utils::_draw_node = nullptr;

cocos2d::DrawNode* Utils::getDrawNode()
{
	if (_draw_node == nullptr)
	{
		auto layer = Utils::getCurrentMapLayer();
		if (layer)
		{
			_draw_node = DrawNode::create();
			layer->addChild(_draw_node, 1000, UTILS_DRAW_NODE_TAG);
		}
	}
	return _draw_node;
}

void Utils::clearDrawNode()
{
	if (_draw_node)
	{
		_draw_node->removeFromParent();
		_draw_node = nullptr;
	}
}

void Utils::drawVec(cocos2d::Point pt, cocos2d::Point pt2, cocos2d::DrawNode* node, cocos2d::Color4F color, bool with_arrow, bool with_endpoint)
{
	//画边
	node->drawSegment(pt, pt2, 1, color);

	if (with_arrow)
	{
		//画三角形(箭头)
#define TRIANGLE_HEIGHT (7.0f)
		cocos2d::Point triangle_top = pt + (pt2 - pt) * 0.8f;
		cocos2d::Vec2 triangle_height = pt2 - pt;
		cocos2d::Color4F triangle_color;
		if (triangle_height.lengthSquared() >= TRIANGLE_HEIGHT * TRIANGLE_HEIGHT)
		{
			triangle_color = Color4F::BLACK;
		}
		else
		{
			triangle_color = Color4F::RED;
		}
		triangle_height.normalize();
		triangle_height *= TRIANGLE_HEIGHT;
		cocos2d::Point triangle_base_center = triangle_top - triangle_height;
		cocos2d::Vec2 triangle_base_half = Geometry::anticlockwisePerpendicularVecToLine(cocos2d::Vec2(triangle_base_center, triangle_top)) * 0.5f;
		node->drawTriangle(triangle_base_center + triangle_base_half, triangle_base_center - triangle_base_half, triangle_top, triangle_color);
	}

	if (with_endpoint)
	{
		node->drawDot(pt2, 6, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.33f));
	}
}

void Utils::runTest()
{
	unsigned char data[5];
	data[0] = 143;
	data[1] = 9;
	data[2] = 201;
	data[3] = 0;
	data[4] = 75;
	std::string encoded_hex = encodeHex(data, 5);
}

std::string Utils::lastComponentOfPath(const std::string& path) {
    auto a = path.rfind('/');
    if (a == std::string::npos) {
        return path;
    } else {
        return path.substr(a + 1);
    }
}

std::string Utils::getFileNameByTime()
{
	time_t rawtime;
	time(&rawtime);
	tm* timeinfo = localtime(&rawtime);

	char buf[100];
	sprintf(buf, "%02d%02d%02d_%02d%02d%02d", timeinfo->tm_year - 100, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	return buf;
}

int Utils::randomNumber(int upper_bound) //返回[1, upper_bound]
{
	//int v = boost::random::uniform_int_distribution<>(1, upper_bound)(int_rand_gen);
	int v = r.randomNumber(upper_bound) + 1;
	char buf[50];
	sprintf(buf, "ri: %d(%d)", v, upper_bound);
	Utils::logr(buf, true);
	return v;
}

float Utils::randomFloat() //返回[0..1)之间浮点数
{
	//float v = boost::random::uniform_01<float>()(float_rand_gen);
	float v = r.randomFloat();
	char buf[30];
	sprintf(buf, "rf: %.4f", v);
	Utils::logr(buf, true);
	return v;
}

unsigned Utils::random_i()
{
	return r.random_i();
}

void Utils::initRandom2ByTime()
{
	/*another_int_rand_gen.seed(time(NULL));
	another_float_rand_gen.seed(time(NULL));*/
	another_r.seed(time(NULL));
}

int Utils::randomNumber2(int upper_bound)
{
	//return boost::random::uniform_int_distribution<>(1, upper_bound)(another_int_rand_gen);
	return another_r.randomNumber(upper_bound) + 1;
}

float Utils::randomFloat2()
{
	//return boost::random::uniform_01<float>()(another_float_rand_gen);
	return another_r.randomFloat();
}

void Utils::saveReplay(const std::string& replay_name)
{
	if (replay_to_save)
	{
		replay_to_save->save(replay_name);
	}
}

//boost::mt19937 Utils::int_rand_gen;
//boost::rand48 Utils::int_rand_gen;
//boost::lagged_fibonacci607 Utils::float_rand_gen;
//boost::mt19937 Utils::another_int_rand_gen;
//boost::lagged_fibonacci607 Utils::another_float_rand_gen;
Random Utils::r;
Random Utils::another_r;

ReplayToPlay* Utils::replay_to_play = nullptr;
ReplayToSave* Utils::replay_to_save = nullptr;