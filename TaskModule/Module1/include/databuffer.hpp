#include <mutex>
#include "producer.hpp"
#include "consumer.hpp"


class DataBuffer : public Producer::Listener,
	               public Consumer::Listener
{
public:
    DataBuffer(int rtt);
    ~DataBuffer();

    void sendRtp(int seq, long int now) override;
    void sendRtx(int seq, long int now) override;
    void sendNack(std::vector<int> nacklist, long int now) override;
    std::vector<int> getRtp(long int now) override;
    std::vector<int> getRtx(long int now) override;
    std::vector<int> getNack(long int now) override;

    std::mutex rtp_mtx;
    std::mutex rtx_mtx;
    std::mutex nack_mtx;
    std::multimap<long int, int> rtpqueue;
    std::multimap<long int, int> rtxqueue;
    std::multimap<long int, std::vector<int>> nackqueue;

    int rtt{ 0 };
};