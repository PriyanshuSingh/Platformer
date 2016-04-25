//
// Created by ashish on 4/18/16.
//

#include "./Modules/TestModule/TestModule.hpp"
#include "ModuleEntries.hpp"

std::vector<ModuleEntries::ModuleHolder> ModuleEntries::entries;
bool ModuleEntries::initDone = false;

void ModuleEntries::initVector() {
    if(initDone)
        return;
    initDone = true;

    typedef PlayModule::staticInfo inf;
    inf i1("Platformer/Modules/TestModule/Static/Box2D/scene3.json","Platformer/Modules/TestModule/Static/Cocos/TestModule.csb");
    entries.push_back(ModuleHolder(i1,TestModule::create));
    entries.push_back(ModuleHolder(i1,TestModule::create));
    entries.push_back(ModuleHolder(i1,TestModule::create));
    entries.push_back(ModuleHolder(i1,TestModule::create));



}




PlayModule *ModuleEntries::getNextModule(size_t index,B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) {
    initVector();
    ModuleHolder hold = entries.at(index);
    return hold.func(hold.info,system,cam,offset);


}


size_t ModuleEntries::getTotal() {
    return entries.size();
}

