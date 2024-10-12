#include "special_agent.hpp"

CSpecialAgent::CSpecialAgent(double x, double y, double heading, double speed, double drange)
:CAgent(x, y, heading, speed, drange)
{

}

std::string CSpecialAgent::to_string() const
{
    std::cout << "!!!Special!!!" << std::endl;
    return CAgent::to_string();
}