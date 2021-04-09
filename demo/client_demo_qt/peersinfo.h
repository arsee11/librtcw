#ifndef PEERSINFO_H
#define PEERSINFO_H

#include <string>
#include <vector>

class PeersInfo
{
public:
    std::string name;
    int id;
};

class PeerInfoList
{
    using list_t=std::vector<PeersInfo>;

public:
    void add(int id, const std::string& name);
    void del(int id);
    int size();
    PeersInfo operator[](int idx);

private:
    list_t _peers;
};

#endif // PEERSINFO_H
