//
//  RuntimeRubyImpl.cpp
//  Simulator
//
//

#include "RuntimeRubyImpl.h"

#include <cstdio>
#include <string>

#include "Runtime.h"
#include "ConfigParser.h"
#include "FileServer.h"

#include "RubyEngine.h"

extern std::string g_projectPath; // Runtime.cpp

USING_NS_CC;
using namespace std;


RuntimeRubyImpl *RuntimeRubyImpl::create()
{
    auto instance = new RuntimeRubyImpl();
    instance->init();
    return instance;
}


void RuntimeRubyImpl::onStartDebuger( const rapidjson::Document & dArgParse, rapidjson::Document & dReplyParse )
{
}


bool reloadScript( const string & file )
{
	auto director = Director::getInstance();
    FontFNT::purgeCachedData();
    if (director->getOpenGLView())
    {
        SpriteFrameCache::getInstance()->removeSpriteFrames();
        director->getTextureCache()->removeAllTextures();
    }
    FileUtils::getInstance()->purgeCachedEntries();
    string modulefile = file;
    
    auto engine = RubyEngine::getInstance();
    if (! modulefile.empty())
    {
    }
    else
    {
        modulefile = ConfigParser::getInstance()->getEntryFile().c_str();
    }
    
    return engine->executeString(modulefile.c_str());
}


void RuntimeRubyImpl::onReload( const rapidjson::Document & dArgParse, rapidjson::Document & dReplyParse )
{
	if (dArgParse.HasMember("modulefiles"))
    {
        rapidjson::Value bodyvalue(rapidjson::kObjectType);
        const rapidjson::Value& objectfiles = dArgParse["modulefiles"];
        for (rapidjson::SizeType i = 0; i < objectfiles.Size(); i++)
        {
            if (!reloadScript(objectfiles[i].GetString()))
            {
                bodyvalue.AddMember(objectfiles[i].GetString(), 1, dReplyParse.GetAllocator());
            }
        }
        if (0 == objectfiles.Size())
        {
            reloadScript("");
        }
        dReplyParse.AddMember("body", bodyvalue, dReplyParse.GetAllocator());
    }
    
    dReplyParse.AddMember("code", 0, dReplyParse.GetAllocator());
}


void RuntimeRubyImpl::startScript( const std::string & strDebugArg )
{
	auto engine = RubyEngine::getInstance();

	const ProjectConfig &project = RuntimeEngine::getInstance()->getProjectConfig();

	string path = FileUtils::getInstance()->fullPathForFilename(project.getScriptFileRealPath().c_str());
    size_t pos;
    while ((pos = path.find_first_of("\\")) != std::string::npos)
    {
        path.replace(pos, 1, "/");
    }
    size_t p = path.find_last_of("/");
    string workdir;
    if (p != path.npos)
    {
        workdir = path.substr(0, p);
		engine->addSearchPath( path.c_str() );
        FileUtils::getInstance()->addSearchPath(workdir);
    }

	auto code = ConfigParser::getInstance()->getEntryFile().c_str();
	engine->executeString( code );
}


void RuntimeRubyImpl::init()
{
    auto engine = RubyEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
}
