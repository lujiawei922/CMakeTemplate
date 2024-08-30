

#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include "unit.hpp"

class Consumer
{
public:
    class Listener
    {
        public:
			virtual ~Listener() = default;
        public:
            virtual void sendNack(std::vector<int> nackList, long int now) = 0;
            virtual std::vector<int> getRtp(long int now) = 0;
            virtual std::vector<int> getRtx(long int now) = 0;
    };

public:
    Consumer(double up_loss, int rtt, double nackRed, Consumer::Listener* listener);
    ~Consumer();
    void ReceiveRtp();
    void ReceiveRtx();
    void sendNack();

    Consumer::Listener* listener{ nullptr };
    std::map<int, NackInfo> nackList;
    std::set<int> receiveRtp;
    int lastSeq { -1 };
    double up_loss{ 0 };
    int rtt{ 0 };
    double nackRed{ 0 };
    double lastRemainRed{ 0 };
    long int lastNackTime { 0 };
    const int nackTimer{ 20 };

};
