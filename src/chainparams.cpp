// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

 void MineGenesis(CBlock genesis){
      // This will figure out a valid hash and Nonce if you're creating a different genesis block:
      uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
      printf("Target: %s\n", hashTarget.GetHex().c_str());
      uint256 newhash = genesis.GetHash();
      uint256 besthash;
      memset(&besthash,0xFF,32);
      while (newhash > hashTarget) {
        ++genesis.nNonce;
          if (genesis.nNonce == 0){
              printf("NONCE WRAPPED, incrementing time");
              ++genesis.nTime;
          }
    newhash = genesis.GetHash();
    if(newhash < besthash){
        besthash=newhash;
        printf("New best: %s\n", newhash.GetHex().c_str());
    }
      }
      printf("Found Genesis, Nonce: %ld, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
      printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
  }

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xe2;
        pchMessageStart[1] = 0x5b;
        pchMessageStart[2] = 0xa2;
        pchMessageStart[3] = 0x4c;
        vAlertPubKey = ParseHex("04d9554786c6f8cb39c13608865f1d5fdfeee349ae168301037c993b054007d40105ab5f98970ed914a956e5fd7a0877f36110e55f82ffeb40e06ab6cca2f883d4");
        nDefaultPort = 8273;
        nRPCPort = 8272;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        //CBlock(hash=000001faef25dec4fbcf906e6242621df2c183bf232f263d0ba5b101911e4563, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90, nTime=1393221600, nBits=1e0fffff, nNonce=164482, vtx=1, vchBlockSig=)
        //  Coinbase(hash=12630d16a9, nTime=1393221600, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
        //    CTxOut(empty)
        //  vMerkleTree: 12630d16a9
        const char* pszTimestamp = "KETOsis launch";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1547444627, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1547444627;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 6073897;


        hashGenesisBlock = genesis.GetHash();
        //LogPrintf("genesis.hashMerkleRoot=%s\n", genesis.hashMerkleRoot.ToString());
      //  LogPrintf("hashGenesisBlock=%s\n", hashGenesisBlock.ToString());
      //  printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
      //  printf("genesis.hashMerkleRoot=%s\n", genesis.hashMerkleRoot.ToString());


          assert(hashGenesisBlock == uint256("0xcbb2db2e2d6adebfda703a92aa73d9cc51e650559da6f8cf73f05c9db898fc22"));
          assert(genesis.hashMerkleRoot == uint256("0x7af4a2b5867c5545d1452dfe116325681d19949708e8d9836489f576dab8cfab"));

        //MineGenesis(genesis);

        vSeeds.push_back(CDNSSeedData("", "")); // Vultr cloud node 1
        vSeeds.push_back(CDNSSeedData("", "")); // Vultr cloud node 2
        vSeeds.push_back(CDNSSeedData("", "")); // Vultr cloud node 3



        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 70); //V
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 83);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, (23+128));
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xC2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0xDD).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 21000;
        nPOSStartBlock = 100;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x7a;
        pchMessageStart[1] = 0x3b;
        pchMessageStart[2] = 0x25;
        pchMessageStart[3] = 0x14;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("040dcd06d0afee40040ae8ea67d4c56d0e25af8010f5fa46ab54a837ee470da99cb5588e282b21354353761125c7a799d6ca707377b15fbe65d5da0ff33c08e13f");
        nDefaultPort = 8271;
        nRPCPort = 8270;

        strDataDir = "testnet";
        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 5073111;
        genesis.nTime    = 1497752710;

        //hashGenesisBlock = genesis.GetHash();

        //assert(hashGenesisBlock == uint256("0x"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("45.63.7.226", "45.63.7.226")); // Vultr cloud node 1
        vSeeds.push_back(CDNSSeedData("104.156.238.199", "104.156.238.199")); // Vultr cloud node 2
        vSeeds.push_back(CDNSSeedData("45.76.126.193", "45.76.126.193")); // Vultr cloud node 3

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 132); // ketosis test net start with v
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 65 + 128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1411111111;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1815802;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 43670;
        strDataDir = "regtest";
        //MineGenesis(genesis);
        //assert(hashGenesisBlock == uint256("0x0000084215677b959303b4f774ddcc8a1bdc46b7b38cdcf59d9b9e4885c79aab"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
