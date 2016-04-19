//
// Created by ashish on 4/18/16.
//

#ifndef MYGAME_MODULEENTRIES_HPP
#define MYGAME_MODULEENTRIES_HPP

#include "PlayModule.hpp"

class ModuleEntries {
public:
    static PlayModule *getNextModule(size_t index,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset);
    static size_t getTotal();


private:

    typedef std::function<PlayModule* (const PlayModule::ModuleInfo & info,B2PhysicsSystem *,MainCamera *,const b2Vec2 &)> consFunc;
    struct ModuleHolder{
        ModuleHolder(const PlayModule::ModuleInfo & info,const  consFunc & func):info(info),func(func){}
        PlayModule::ModuleInfo info;
        consFunc func;

    };
    static std::vector<ModuleHolder> entries;
    static bool initDone;
    static void initVector();
};


#endif //MYGAME_MODULEENTRIES_HPP
