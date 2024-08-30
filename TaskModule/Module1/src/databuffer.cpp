#include "databuffer.hpp"
DataBuffer::DataBuffer(int rtt) : rtt(rtt)
{

}

DataBuffer::~DataBuffer(){}


void DataBuffer::sendRtp(int seq, long int now)
{
    std::lock_guard<std::mutex> lock(rtp_mtx);
    rtpqueue.emplace(std::make_pair(now, seq));
}

void DataBuffer::sendRtx(int seq, long int now)
{
    std::lock_guard<std::mutex> lock(rtx_mtx);
    rtxqueue.emplace(std::make_pair(now, seq));
}

void DataBuffer::sendNack(std::vector<int> nacklist, long int now)
{
    std::lock_guard<std::mutex> lock(nack_mtx);
    std::cout << "send nack:";
    for(auto nack : nacklist)
    {
        std::cout <<  nack << " ";
    }
    std::cout << std::endl;
    nackqueue.emplace(std::make_pair(now, nacklist));
}

std::vector<int> DataBuffer::getRtp(long int now)
{
    std::lock_guard<std::mutex> lock(rtp_mtx);
    auto iter = rtpqueue.begin();
    std::vector<int> arrivedRtp;
    for( ;iter != rtpqueue.end(); )
    {
        if(iter->first + rtt/2 < now)
        {
            arrivedRtp.push_back(iter->second);
            iter = rtpqueue.erase(iter);
        }
        else
        {
            break;
        }
    }
    return arrivedRtp;
}

std::vector<int> DataBuffer::getRtx(long int now)
{
    std::lock_guard<std::mutex> lock(rtx_mtx);
    auto iter = rtxqueue.begin();
    std::vector<int> arrivedRtx;
    for( ;iter != rtxqueue.end(); )
    {
        if(iter->first + rtt/2 < now)
        {
            arrivedRtx.push_back(iter->second);
            iter = rtxqueue.erase(iter);
        }
        else
        {
            break;
        }
    }
    return arrivedRtx;
}

std::vector<int> DataBuffer::getNack(long int now)
{
    std::lock_guard<std::mutex> lock(nack_mtx);
    auto iter = nackqueue.begin();
    std::vector<int> arrivedNack;

    std::set<int> filternack;

    for( ;iter != nackqueue.end(); )
    {
        if(iter->first + rtt/2 < now)
        {
            for(auto nackseq : iter->second)
            {
                filternack.insert(nackseq);
            }
            iter = nackqueue.erase(iter);
        }
        else
        {
            break;
        }
    }
    arrivedNack.insert(arrivedNack.end(), filternack.begin(), filternack.end());
    return arrivedNack;
}
