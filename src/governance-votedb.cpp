// Copyright (c) 2015-2017 The Triad Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "governance-votedb.h"

CGovernanceObjectVoteFile::CGovernanceObjectVoteFile()
    : nMemoryVotes(0),
      listVotes(),
      mapVoteIndex()
{}

CGovernanceObjectVoteFile::CGovernanceObjectVoteFile(const CGovernanceObjectVoteFile& other)
    : nMemoryVotes(other.nMemoryVotes),
      listVotes(other.listVotes),
      mapVoteIndex()
{
    RebuildIndex();
}

void CGovernanceObjectVoteFile::AddVote(const CGovernanceVote& vote)
{
    listVotes.push_front(vote);
    mapVoteIndex[vote.GetHash()] = listVotes.begin();
    ++nMemoryVotes;
}

bool CGovernanceObjectVoteFile::HasVote(const uint256& nHash) const
{
    vote_m_cit it = mapVoteIndex.find(nHash);
    if(it == mapVoteIndex.end()) {
        return false;
    }
    return true;
}

bool CGovernanceObjectVoteFile::GetVote(const uint256& nHash, CGovernanceVote& vote) const
{
    vote_m_cit it = mapVoteIndex.find(nHash);
    if(it == mapVoteIndex.end()) {
        return false;
    }
    vote = *(it->second);
    return true;
}

std::vector<CGovernanceVote> CGovernanceObjectVoteFile::GetVotes() const
{
    std::vector<CGovernanceVote> vecResult;
    for(vote_l_cit it = listVotes.begin(); it != listVotes.end(); ++it) {
        vecResult.push_back(*it);
    }
    return vecResult;
}

void CGovernanceObjectVoteFile::RemoveVotesFromMasternode(const CTxIn& vinMasternode)
{
    vote_l_it it = listVotes.begin();
    while(it != listVotes.end()) {
        if(it->GetVinMasternode() == vinMasternode) {
            mapVoteIndex.erase(it->GetHash());
            listVotes.erase(it++);
        }
        else {
            ++it;
        }
    }
}

CGovernanceObjectVoteFile& CGovernanceObjectVoteFile::operator=(const CGovernanceObjectVoteFile& other)
{
    nMemoryVotes = other.nMemoryVotes;
    listVotes = other.listVotes;
    RebuildIndex();
    return *this;
}

void CGovernanceObjectVoteFile::RebuildIndex()
{
    mapVoteIndex.clear();
    for(vote_l_it it = listVotes.begin(); it != listVotes.end(); ++it) {
        CGovernanceVote& vote = *it;
        mapVoteIndex[vote.GetHash()] = it;
    }
}
