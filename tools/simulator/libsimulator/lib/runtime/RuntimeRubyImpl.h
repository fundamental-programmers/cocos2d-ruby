//
//  RuntimeRubyImpl.h
//  Simulator
//
//

#ifndef __Simulator__RuntimeRubyImpl__
#define __Simulator__RuntimeRubyImpl__

#include "RuntimeProtocol.h"

class RuntimeRubyImpl : public RuntimeProtocol
{
public:
    static RuntimeRubyImpl * create();
    
    void onStartDebuger( const rapidjson::Document & dArgParse, rapidjson::Document & dReplyParse );
    void onReload( const rapidjson::Document & dArgParse, rapidjson::Document & dReplyParse );
    void startScript( const std::string & strDebugArg );
    
private:
    void init();
};

#endif /* defined(__Simulator__RuntimeRubyImpl__) */
