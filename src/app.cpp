#include <iostream>
#include <string>
#include <vector>

#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"
#include "tinyxml2.h"
#include "special_agent.hpp"
#include "simple_decorator2.hpp"

class InitManager {
public:
    InitManager() { }
    InitManager(std::string path) {
        // tinyxml2의 XMLDocument 인스턴스를 통해 
        // InitManager의 생성자 매개변수 path에 해당하는 xml을 연다
        doc.LoadFile(path.c_str()); 
        double x, y, heading, speed, drange;
        
        // CAgent 생성 과정
        {
            // XML 파일에서 "scenario"에 해당하는 첫번째 자식 요소 밑에 있는 "AgentList" 자식 요소를 가져오고 있음
            tinyxml2::XMLElement* agListElem = doc.FirstChildElement("scenario")
                                                    ->FirstChildElement("AgentList");      
            
            for(
                tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
                agElem != NULL;
                agElem = agElem->NextSiblingElement()
            ) {
                // XML 파일로 부터 값을 가져와서 변수에 저장
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                // 저장된 값을 기반으로 CAgent instacne 생성
                CAgent* ag = new CAgent(x, y, heading, speed, drange);

                // #01 과제와 동일한 출력을 위해 추가
                ag = new SimpleDecorator(new SimpleDecorator2(new StochasticDecorator(ag)));

                // 생성된 agent 객체를 리스트에 추가
                m_agentList.push_back(ag);
            }
        }

        // CSpecialAgent 생성 과정
        {
            tinyxml2::XMLElement* agListElem = doc.FirstChildElement("scenario")
                                                    ->FirstChildElement("SAgentList");      
            for(
                tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
                agElem != NULL; 
                agElem = agElem->NextSiblingElement()
            ) {
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                // CAgent instance가 아닌 CSpecialAgent instance를 생성하고 있음
                CAgent* ag = new CSpecialAgent(x, y, heading, speed, drange);

                // #01 과제와 동일한 출력을 위해 추가
                ag = new SimpleDecorator(new SimpleDecorator2(new StochasticDecorator(ag)));

                m_agentList.push_back(ag);
            }
        }
    }

    std::vector<CAgent*>& get_agent_list() {
        return m_agentList;
    }    

private:
    tinyxml2::XMLDocument doc;
    std::vector<CAgent*> m_agentList;
};

int main(int argc, char** argv) {

    // XML 파일 path를 전달하여 tinyxml2를 통해 agent의 값을 가져와 agent 생성
    InitManager init_manager("test.xml");

    /* Simulation Engine */
    CManeuverManager mmanager;

    for (
        std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); 
        ++iter 
    ) {
        mmanager.register_publisher(*iter); 
    }

    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for (sim_time = 0; sim_time < 30; sim_time += time_step) {   
        mmanager.svc(time_step);

        std::cout << "----" << std::endl;
        for(
            std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
            iter != init_manager.get_agent_list().end(); 
            ++iter 
        ) {
            std::cout << "Time: " << sim_time << ", " << **iter << std::endl;
        }
    }
    
    return 0;
}
