#pragma once
#include "base_decorator.hpp"

class SimpleDecorator2: public BaseDecorator
{
public:
    SimpleDecorator2(CAgent* agent):BaseDecorator(agent){}; // 생성자: 객체 생성하는 애
    
public:
    virtual bool detect(CAgent* pTarget) {                  // 재정의된 메소드: 
        std::cout << "Simple Decorator2" << std::endl;
        return m_wrappee->detect(pTarget);
    };
};