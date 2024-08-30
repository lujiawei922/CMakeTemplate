#include <iostream>
#include <thread>
#include "producer.hpp"
#include "consumer.hpp"
#include "databuffer.hpp"


std::int32_t total_rtp = (2000000) / (1200 * 8) * 120;
double rtp_loss = 0.5;
double nack_loss = 0.5;
double m = 0;
double r = 3;
int rtt = 40;

DataBuffer buffer(rtt);
Producer sender(total_rtp, rtp_loss, m, &buffer);
Consumer receiver(nack_loss, rtt, r, &buffer);



void producer() {
    long int lastSendTime {0};

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto timestamp = now_ms.time_since_epoch().count();    
        if(timestamp - lastSendTime >= 4)
        {
            sender.sendNowRtp();
            lastSendTime = timestamp;
        }    
        sender.dealNack(timestamp);
        if(sender.now_seq >= total_rtp && receiver.nackList.empty() && buffer.nackqueue.empty())
        {
            break;
        }
    }
}

void consumer() {
    while (true) 
    {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        receiver.ReceiveRtx();
        receiver.ReceiveRtp();
        receiver.sendNack();
        if(sender.now_seq >= total_rtp && receiver.nackList.empty() && buffer.nackqueue.empty() && buffer.rtxqueue.empty())
        {
            break;
        }
    }
}


int main() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto starttime = now_ms.time_since_epoch().count();    
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    auto endnow = std::chrono::system_clock::now();
    auto end_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(endnow);
    auto endtime = end_ms.time_since_epoch().count();    

    double remind_rate = (total_rtp - receiver.receiveRtp.size())/double(total_rtp);
    std::cout << "usetime:" << endtime - starttime << std::endl;
    std::cout << "total packet"<< total_rtp <<" receive packet"<< receiver.receiveRtp.size() << " remind_rate:" <<remind_rate << std::endl;
    return 0;
}
