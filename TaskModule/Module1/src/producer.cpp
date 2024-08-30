#include "producer.hpp"
#include "unit.hpp"
#include <cmath>
#include <chrono>

Producer::Producer(int32_t total_rtp, double down_loss, double sendRed, Producer::Listener *listener) : total_rtp(total_rtp), down_loss(down_loss), sendRed(sendRed), listener(listener) {}

Producer::~Producer(){}

void Producer::sendNowRtp()
{
    ++this->now_seq;
    if(now_seq >= total_rtp)
    {
        return;
    }

    double now_red = this->remain_red + this->sendRed;
    this->remain_red = now_red - std::floor(now_red);
    int sendtime = 1 + std::floor(now_red);
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto timestamp = now_ms.time_since_epoch().count();
    while (sendtime > 0)
    {        
        if(!LostPercentChance(down_loss))
        {
            std::cout<< "now Rtp:"<<this->now_seq<<std::endl;
            this->listener->sendRtp(now_seq, timestamp);
        }
        --sendtime;
    }
}

void Producer::dealNack(long int now)
{
    std::vector<int> receiverNacks = this->listener->getNack(now);

    for(auto item : receiverNacks)
    {
        double now_red = this->remain_rtx_red + this->sendRed;
        this->remain_rtx_red = now_red - std::floor(now_red);
        int sendtime = 1 + std::floor(now_red);
        while (sendtime > 0)
        {        
            if(!LostPercentChance(down_loss))
            {
                std::cout << "send rtx:" << item << std::endl;
                this->listener->sendRtx(item, now);
            }
            --sendtime;
        }
    }
}