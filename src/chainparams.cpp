// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Copyright (c) 2021 The DECENOMY Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

#define DISABLED 0xFFFFFFFF;

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Standalone blockchain Developed for ADA Police NFT project on cardano 2022";
    const CScript genesisOutputScript = CScript() << ParseHex("04b3fa9f648a6b1f477db94dc72db5df018b5539f081fa32258d6c7ba47a6b47b48187909c1fa62b2087d9e17e115fbfb20b042ff7c9c34f35c5d17f76c458e7f4") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of(0, uint256("0x00"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1615902863, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x00"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1533446237,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x00"));

static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1533446237,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";



        
        genesis = CreateGenesisBlock(1650888895, 874979, 0x1e0ffff0, 1, 0 * COIN);
        if(genesis.GetHash()!=uint256("00000c150d8ed6077714a13bfe9184e21ace8eb3e10cd31749792f1deef41cbc"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }
        
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000c150d8ed6077714a13bfe9184e21ace8eb3e10cd31749792f1deef41cbc"));
        assert(genesis.hashMerkleRoot == uint256S("0x51661c9aa55a5a4576bcbd43bc75c0a4f7cc6acf8444292df6d3a7e981fb094c"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit = ~UINT256_ZERO >> 2;
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 30 * 24 * 60; // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;       // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20; // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 210000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24; // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;                    // 1h
        consensus.nStakeMinDepth = 60;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "0322f7ebc48611609357761b88b8cdb9cf7cb63ed20ed8505301e51a7bf201722b";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint = DISABLED;
        consensus.height_last_ZC_WrappedSerials = DISABLED;
        consensus.height_start_InvalidUTXOsCheck = DISABLED;
        consensus.height_start_ZC_InvalidSerials = DISABLED;
        consensus.height_start_ZC_SerialRangeCheck = DISABLED;
        consensus.height_ZC_RecalcAccumulators = DISABLED;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                               "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                               "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                               "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                               "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                               "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637; // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;         // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1893456000;   // 01/01/2030 @ 12:00am (UTC)
        consensus.ZC_WrappedSerialsSupply = 0; //4131563 * COIN;   // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight = 1001;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight = 1441;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight = 1441;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight = 1541;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight = 1641;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight = 1741;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock = uint256S("0x0");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x57;
        pchMessageStart[1] = 0x69;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0x78;
        nDefaultPort = 11911;

        vSeeds.push_back(CDNSSeedData("1", "138.68.182.37"));
        vSeeds.push_back(CDNSSeedData("2", "147.182.219.130"));
        vSeeds.push_back(CDNSSeedData("3", "128.199.67.149"));

        base58Prefixes[PUBKEY_ADDRESS]  = std::vector<unsigned char>(1, 23);//A
        base58Prefixes[SCRIPT_ADDRESS]  = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 87); // c
        base58Prefixes[SECRET_KEY]      = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY]  = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]  = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x03)(0x51).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main)); // added

        // Sapling
        //bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ps";
        //bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviews";
        //bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivks";
        //bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-main";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v1)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        // // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        // /////////////////////////////////////////////////////////////////

        // uint32_t nGenesisTime = 1615902864;

        // arith_uint256 test;
        // bool fNegative;
        // bool fOverflow;
        // test.SetCompact(0x1e0ffff0, &fNegative, &fOverflow);
        // std::cout << "Test threshold: " << test.GetHex() << "\n\n";

        // int genesisNonce = 0;
        // uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // for (int i = 0; i < 40000000; i++) {
        //     genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e0ffff0, 1, 0 * COIN);
        //     //genesis.hashPrevBlock = TempHashHolding;
        //     consensus.hashGenesisBlock = genesis.GetHash();

        //     arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
        //     if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        //         BestBlockHash = consensus.hashGenesisBlock;
        //         std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        //         std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        //     }

        //     TempHashHolding = consensus.hashGenesisBlock;

        //     if (BestBlockHashArith < test) {
        //         genesisNonce = i - 1;
        //         break;
        //     }
        //     //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        // }
        // std::cout << "\n";
        // std::cout << "\n";
        // std::cout << "\n";

        // std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        // std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        // std::cout << "Genesis Merkle 0x" << genesis.hashMerkleRoot.GetHex() << std::endl;

        // exit(0);

        // /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1615902864, 42773, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x00000c5ee0ad5d40f6b26125819d873afabc0c0d47034c97d7b1dd74511215bb"));
        //assert(genesis.hashMerkleRoot == uint256S("0x4a133058bc8507376b74f38805812a3d3288485689b73c61abadf7766d78b4e2"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit = ~UINT256_ZERO >> 6; // adapolice starting difficulty
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;    // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3; // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4; // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 3000000000 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5; // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "02f41826b4cb2b38cc896526dbf386c36e31c96a4a7f99bb0eec6de4cb7949ebf1";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = DISABLED;
        consensus.height_last_ZC_WrappedSerials = DISABLED;
        consensus.height_start_InvalidUTXOsCheck = DISABLED;
        consensus.height_start_ZC_InvalidSerials = DISABLED;
        consensus.height_start_ZC_SerialRangeCheck = DISABLED;
        consensus.height_ZC_RecalcAccumulators = DISABLED;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                               "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                               "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                               "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                               "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                               "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637; // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;         // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1501776000;
        consensus.ZC_WrappedSerialsSupply = 0; // WrappedSerials only on main net

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight = 501;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight = 701;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight = 701;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight = 801;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight = 901;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight = 1001;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock = uint256S("0x0");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x06;
        pchMessageStart[1] = 0x16;
        pchMessageStart[2] = 0xe6;
        pchMessageStart[3] = 0x15;
        nDefaultPort = 14333;

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testnet1", "testnet1.4444.tools", true));
        vSeeds.push_back(CDNSSeedData("testnet2", "testnet2.4444.tools", true));
        vSeeds.push_back(CDNSSeedData("testnet.572133.club", "testnet.572133.club", true));
        vSeeds.push_back(CDNSSeedData("testnet2.572133.club", "testnet2.572133.club", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 90);   // Testnet adapolice addresses start with 'd'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);   // Testnet adapolice script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125); // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);      // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet adapolice BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet adapolice BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet adapolice BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // Sapling
        //bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        //bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        //bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivktestsapling";
        //bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]         = "p-secret-spending-key-test";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        // // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        // // /////////////////////////////////////////////////////////////////

        // uint32_t nGenesisTime = 1615902865;

        // arith_uint256 test;
        // bool fNegative;
        // bool fOverflow;
        // test.SetCompact(0x1fffffff, &fNegative, &fOverflow);
        // std::cout << "Test threshold: " << test.GetHex() << "\n\n";

        // int genesisNonce = 0;
        // uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // for (int i = 0; i < 40000000; i++) {
        //     genesis = CreateGenesisBlock(nGenesisTime, i, 0x1fffffff, 1, 0 * COIN);
        //     //genesis.hashPrevBlock = TempHashHolding;
        //     consensus.hashGenesisBlock = genesis.GetHash();

        //     arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
        //     if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        //         BestBlockHash = consensus.hashGenesisBlock;
        //         std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        //         std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        //     }

        //     TempHashHolding = consensus.hashGenesisBlock;

        //     if (BestBlockHashArith < test) {
        //         genesisNonce = i - 1;
        //         break;
        //     }
        //     //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        // }
        // std::cout << "\n";
        // std::cout << "\n";
        // std::cout << "\n";

        // std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        // std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        // std::cout << "Genesis Merkle 0x" << genesis.hashMerkleRoot.GetHex() << std::endl;

        // exit(0);

        // // /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1615902865, 652043, 0x1fffffff, 1, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x000817f2ab1c84d7ae24a42b8cfe71f401f6c05bce5499b9550128e73606a238"));
        //assert(genesis.hashMerkleRoot == uint256S("0x4a133058bc8507376b74f38805812a3d3288485689b73c61abadf7766d78b4e2"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit = ~UINT256_ZERO >> 2; // starting difficulty
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;    // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3; // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 5;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4; // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 3000000000 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5; // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "04ae5f52611fc9f3dcd3f2db88ad4a9b10ab218389cacd1ed07cc893af6ad2f3e6618090edb54038e5a26d2c6d04db4a6c901a8bf52a72fef484f478519fdf0b02";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 310; // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 300;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                               "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                               "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                               "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                               "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                               "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637; // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;         // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0; // not implemented on regtest
        consensus.ZC_WrappedSerialsSupply = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight = 451;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xad;
        pchMessageStart[1] = 0xa5;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0x93;
        nDefaultPort = 15333;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
