//
//  Utils.h
//  Lalune
//
//  Created by Xin Xu on 7/16/14.
//
//

#ifndef __Lalune__Utils__
#define __Lalune__Utils__

#include "cocos2d.h"
#include "AI/BoostGeometryDef.h"
#include <algorithm>
//#include <boost/random.hpp>
#include "Random.h"

class Utils {

public:
	//字符串分割
	static void split(const std::string& input, std::vector<std::string>& output, char delimiter);
	static void split(const std::string& input, std::vector<int>& output, char delimiter);
	static void split(const std::string& input, std::vector<std::pair<int, int>>& output, char delimiter);
	static void split(const std::string& input, std::set<int>& output, char delimiter);
	static std::pair<int, int> parsePos(const std::string& v);
	static std::pair<int, int> parsePosNoParentheses(const std::string& v); 
	static cocos2d::Vec2 pos2Vec2NoParentheses(const std::string& v);
    static float parseValueOrPercent(const std::string& input, bool& isPercent);

	//类型互转
	static int colorCharToInt(char a1, char a2);
	static int toInt(std::string v);
	static std::string toStr(int v);
	static double toDouble(std::string v);
	static double percent2double(std::string p);
	static std::pair<int, int> toPair(cocos2d::Vec2 v);
	static cocos2d::Point toPoint(std::pair<int, int> p);
	static point toPt(const cocos2d::Point& pt);
	static cocos2d::Point toCocosPt(point pt);
	static cocos2d::Vec2 toCocosVec2(const segment& s);

	//编码
	static std::string toCP936(const std::string& s);
	static std::string displayableText(const std::string& s);
	
	//序列化
	static bool parseFromBinary(FILE* f, cocos2d::Point& pt);
	static void serializeToBinary(FILE* f, const cocos2d::Point& pt);

	//STL补充
	template<class T>
	static bool contains(const std::vector<T>& container, T obj)
	{
		return std::find(container.begin(), container.end(), obj) != container.end();
	}

	//字符串处理
    static std::string stringFormat(const std::string fmt, ...);
	static std::string encodeHex(const unsigned char* data, unsigned len);

	//调试相关
	static void logAI(std::string fmt, ...);
	static void logr(const std::string& content, bool only_save_to_replay); //会存到录像里

	//cocos相关
	static cocos2d::Node* getCurrentMapLayer();
	static cocos2d::DrawNode* getDrawNode();
	static void clearDrawNode();
	static void drawVec(cocos2d::Point pt, cocos2d::Point pt2, cocos2d::DrawNode* node, cocos2d::Color4F color = cocos2d::Color4F::MAGENTA, bool with_arrow = true, bool with_endpoint = true);

	static void runTest();

	//路径相关
    static std::string lastComponentOfPath(const std::string& path);
	static std::string getFileNameByTime();

	//随机
	static int randomNumber(int upper_bound); //返回[1, upper_bound]
	static float randomFloat(); //返回[0..1)之间浮点数
	static unsigned random_i(); //for replay check

	static void initRandom2ByTime(); //用时间初始化一下，使得每台机器能不一样些
	static int randomNumber2(int upper_bound); //返回[1, upper_bound]。和正常用的randomFloat不共用随机引擎，通常是调试用
	static float randomFloat2(); //返回[0..1)之间浮点数。和正常用的randomFloat不共用随机引擎，通常是调试用

	static std::string generateRandomString(int len = 4);

	static void saveReplay(const std::string& replay_name);
    
    static cocos2d::Point randomPositionInRange( const cocos2d::Point& center, float range );

protected:
	static cocos2d::DrawNode* _draw_node;

	//static boost::mt19937 int_rand_gen;
	/*static boost::rand48 int_rand_gen;
	static boost::lagged_fibonacci607 float_rand_gen;
	static boost::mt19937 another_int_rand_gen;
	static boost::lagged_fibonacci607 another_float_rand_gen;*/
	static Random r, another_r;

	static class ReplayToPlay* replay_to_play;
	static class ReplayToSave* replay_to_save;

	friend class ReplayToPlay;
	friend class ReplayToSave;
	friend class ReplayComparer;
};

#endif /* defined(__Lalune__Utils__) */
