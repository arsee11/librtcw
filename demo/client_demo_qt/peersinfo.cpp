#include "peersinfo.h"
#include <algorithm>

void PeerInfoList::add(int id, const std::string &name)
{
    PeersInfo p;
    p.name =name;
    p.id = id;
    _peers.push_back(p);
}

void PeerInfoList::del(int id)
{
    auto it = std::find_if(_peers.begin(), _peers.end(), [id](const PeersInfo& p){
            return p.id == id;});
    if(it != _peers.end()){
        _peers.erase(it);
    }
}

int PeerInfoList::size()
{
    return _peers.size();
}

PeersInfo PeerInfoList::operator[](int idx)
{
    return _peers[idx];
}
