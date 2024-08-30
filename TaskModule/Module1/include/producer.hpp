
#pragma once
#include <vector>
#include <iostream>

class Producer
{
public:
    class Listener
    {
        public:
			virtual ~Listener() = default;
        public:
            virtual void sendRtp(int seq, long int now) = 0;
            virtual void sendRtx(int seq, long int now) = 0;
            virtual std::vector<int> getNack(long int now) = 0;
    };

public:
    Producer(int32_t total_rtp,  double down_loss, double sendRed, Producer::Listener* listener);
    ~Producer();

    void sendNowRtp();
    void dealNack(long int now);

    int nowseq;
    Producer::Listener* listener{ nullptr };
    double down_loss{ 0 };
    double sendRed{ 0 };
    int32_t total_rtp;
    int32_t now_seq{ -1 };
    double remain_red{ 0 };
    double remain_rtx_red{ 0 };
};
