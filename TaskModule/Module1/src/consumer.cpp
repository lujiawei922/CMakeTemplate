#include "consumer.hpp"
#include <cmath>

Consumer::Consumer(double up_loss, int rtt, double nackRed, Consumer::Listener* listener): up_loss(up_loss), rtt(rtt), nackRed(nackRed), listener(listener){}
Consumer::~Consumer(){}

void Consumer::ReceiveRtp()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto timestamp = now_ms.time_since_epoch().count();
    std::vector<int> receiverRtps = this->listener->getRtp(timestamp);

    for(auto item: receiverRtps)
    {
        std::cout << "receive rtp:" << item << std::endl;        
        this->receiveRtp.insert(item);
        if (this->lastSeq == item)
        {
            continue;
        }
        else if (this->lastSeq + 1 != item)
        {
            uint16_t lostSeq = this->lastSeq + 1;
            do
            {
                std::cout << "create nack : " << lostSeq << std::endl;
                this->nackList.emplace(std::make_pair(lostSeq, NackInfo{
                                                                        timestamp,
                                                                        lostSeq}));
                ++lostSeq;
            } while (lostSeq < item);
        }
        this->lastSeq = item;
    }
}

void Consumer::ReceiveRtx()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto timestamp = now_ms.time_since_epoch().count();
    std::vector<int> receiverRtxs = this->listener->getRtx(timestamp);
    for(auto rtx : receiverRtxs)
    {
        std::cout << "receive rtx" << rtx << std::endl;
        this->receiveRtp.insert(rtx);
        this->nackList.erase(rtx);
    }
}

void Consumer::sendNack()
{
    if (nackList.empty())
    {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto timestamp = now_ms.time_since_epoch().count();        

    if (this->lastNackTime == 0 || this->lastNackTime + this->nackTimer < timestamp)
    {
        double now_red = this->lastRemainRed + this->nackRed;
        this->lastRemainRed = now_red - std::floor(now_red);
        int sendtime = 1 + std::floor(now_red);            
        this->lastNackTime = timestamp;
        auto iter = this->nackList.begin();        
        std::vector<int> sendingNack;    
        for(; iter != this->nackList.end();)
        {
            if(iter->second.sentAtMs == 0 || iter->second.sentAtMs + this->rtt < timestamp)
            {
                iter->second.sentAtMs = timestamp;
                sendingNack.push_back(iter->first);
                ++(iter->second.retries);
            }

            if(iter->second.retries >= 20 || timestamp - iter->second.createdAtMs > 300)
            {
                iter = this->nackList.erase(iter);
                std::cout <<"remove nack:" <<  iter->first << std::endl;
            }
            else
            {
                ++iter;
            }
        }

        while (sendtime > 0)
        {        
            if(!LostPercentChance(up_loss))
            {
                this->listener->sendNack(sendingNack, timestamp);
            }
            --sendtime;
        }               
    }


}