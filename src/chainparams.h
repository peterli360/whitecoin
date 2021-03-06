// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAIN_PARAMS_H
#define BITCOIN_CHAIN_PARAMS_H

#include "bignum.h"
#include "scriptnum10.h"
#include "uint256.h"
#include "util.h"

#include <vector>

using namespace std;

#define MESSAGE_START_SIZE 4
typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

class CAddress;
class CBlock;

struct CDNSSeedData {
    string name, host;
    CDNSSeedData(const string &strName, const string &strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,     // BIP16
        EXT_PUBLIC_KEY, // BIP32
        EXT_SECRET_KEY, // BIP32
        EXT_COIN_TYPE,  // BIP44

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    const vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    const map<std::string, vector<unsigned char> >& mapMessKey() const { return mapBroadcastMessPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }
    const CBigNum& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    int MaxReorganizationDepth() const { return nMaxReorganizationDepth; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
    virtual const CBlock& GenesisBlock() const = 0;
    virtual bool RequireRPCPassword() const { return true; }
    const string& DataDir() const { return strDataDir; }
    virtual Network NetworkID() const = 0;
    const vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char> &Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    virtual const vector<CAddress>& FixedSeeds() const = 0;
    int RPCPort() const { return nRPCPort; }
    int LastPOWBlock() const { return nLastPOWBlock; }
    int64_t FirstForkTime()     const {return 	nFirstForkTime;}
    int64_t SecondForkTime() const {return 	nSecondForkTime;}
    int64_t PosIncreaseTime() const {return 	nPosIncreaseTime;}

    int64_t FirstHalfTime()       const {return 	nFirstHalfTime;     }
    int64_t SecondHalfTime() const {return      nSecondHalfTime;}
    int64_t ThirdHalfTime()     const {return      nThirdHalfTime;  }
    int64_t FouthHalfTime()    const {return      nFouthHalfTime;   }
    int64_t MamDriftTime()    const {return      nMainNetDriftTime;}
    int64_t GetDestroyedCoins() const {return nDestroyedCoins;}
    int64_t GetPreMineCoins() const {return nPreMineCoins;}

protected:
    CChainParams() {};

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    // Raw pub key bytes for the broadcast alert signing key.
    vector<unsigned char> vAlertPubKey;
    std::map<std::string, vector<unsigned char> > mapBroadcastMessPubKey;
    int nDefaultPort;
    int nRPCPort;
    CBigNum bnProofOfWorkLimit;
    int nMaxReorganizationDepth;
    int nSubsidyHalvingInterval;
    string strDataDir;
    vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    int nLastPOWBlock;
    int64_t nFirstForkTime;
    int64_t nSecondForkTime;
    int64_t nPosIncreaseTime;
    int64_t nFirstHalfTime;
    int64_t nSecondHalfTime;
    int64_t nThirdHalfTime;
    int64_t nFouthHalfTime;
    int64_t nMainNetDriftTime;
    int64_t nDestroyedCoins;
    int64_t nPreMineCoins;

};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams &Params();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

inline bool TestNet() {
    // Note: it's deliberate that this returns "false" for regression test mode.
    return Params().NetworkID() == CChainParams::TESTNET;
}

#endif
