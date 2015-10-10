#include "singleton.h"

using namespace Core;

Singleton * Singleton::p_instance = 0;
SingletonDestroyer Singleton::destroyer;

SingletonDestroyer::~SingletonDestroyer() {
    delete p_instance;
}
void SingletonDestroyer::initialize(Singleton * p) {
    p_instance = p;
}
Singleton& Singleton::getInstance() {
    if(!p_instance) {
        p_instance = new Singleton();
        destroyer.initialize(p_instance);
    }
    return * p_instance;
}
