#ifndef ECALL_OFFLine_H
#define ECALL_OFFLine_H

#include "commonEnclave.h"
#include "functional"
#include <utility>

#include <iostream>
#include "ecallLz4.h"
using namespace std;

extern "C"
{
#include "config.h"
#include "xdelta3.h"
}
// #include "configure.h"



class OFFLineBackward
{
    private:
        string myName_ = "offline_backward";
        unordered_map<string, vector<string>> delta_map; // the map (basechunkFP,vector(deltachunkFP));
                                                        // the map (old_basechunk, new_basechunk)

        // the map (containerid vector(offset,sum_size)) record the chunk gonna be deleted in each container
        unordered_map<string, vector<pair<uint32_t, uint32_t>>> cold_basemap;
        Container_t hot_container;
        Container_t hot_delta_container;
        uint8_t *tmpOldContainer;
        uint8_t *tmpNewContainer;
        uint8_t *tmpDeltaContainer;
        uint8_t *tmpColdContainer;
        uint64_t GreeyOfflineSize = 0;

    public:
        unordered_map<string, string> local_basemap;
        uint64_t _offlineCompress_size = 0;
        uint64_t _offlineCurrBackup_size = 0;
        uint64_t _offlineDeltanum = 0;
        uint64_t _offlineDeDeltanum = 0;
        uint64_t _offlineDeletenum = 0;
        long long _Offline_DeltaSaveSize = 0;
        long long _baseChunkNum = 0;
        uint64_t _baseDataSize = 0;
        uint64_t _deltaChunkNum = 0;
        uint64_t _deltaDataSize = 0;
        long long _lz4SaveSize = 0;
        uint64_t _DeltaSaveSize = 0;
        uint64_t _offlinedeltaChunkNum = 0;
        uint64_t _offline_Ocall = 0;
        bool Offline_Flag;

        /**
         * @brief Construct a new Ecall Offline processer object
         * 
         */
        OFFLineBackward();
        
        /**
         * @brief Destory a new Ecall Offline processer object
         * 
         */
        ~OFFLineBackward();

        /**
         * @brief Print basic information
         * 
         */
        void Print();

        /**
         * @brief Init basic information
         * 
         */
        void Init();

        void Update();

        /**
         * @brief Update hot container
         * @param old_basechunkhash
         * @param delta_chunkhash
         * @param new_basechunkhash      
         */
        void Update_hot_container(string old_basechunkhash, string delta_chunkhash, string new_basechunkhash);

        /**
         * @brief Update cold container
         * @param upOutSGX
         * @param container_id
         * @param cold_basechunk     
         * @param cryptoObj_
         * @param cipherCtx
         */
        void Update_cold_container(UpOutSGX_t *upOutSGX, string container_id, vector<pair<uint32_t, uint32_t>> &cold_basechunk,
                                EcallCrypto *cryptoObj_, EVP_CIPHER_CTX *cipherCtx);

        static bool myCompare(pair<uint32_t, uint32_t> &v1, pair<uint32_t, uint32_t> &v2);

        /**
         * @brief Insert pair to Delta Index
         * @param basechunkhash
         * @param deltachunkhash
         */        
        void Insert_delta(string basechunkhash, string deltachunkhash);

        /**
         * @brief Insert pair to Delta Index
         * @param oldchunkhash
         * @param newchunkhash
         */     
        void Insert_local(string oldchunkhash, string newchunkhash);

        /**
         * @brief Get the chunk recipe
         * @param chunkhash
         */   
        RecipeEntry_t *GetRecipe(uint8_t *chunkhash);

        /**
         * @brief Get the content of old container
         * @param container_id
         * @param Cold_container
         * @param tmpContainerSize
         */   
        bool GetOldContainer(string container_id, uint8_t **Cold_container, size_t *tmpContainerSize);

        /**
         * @brief Get the content of new container
         * @param container_id
         * @param Cold_container
         * @param tmpContainerSize
         */   
        bool GetNewContainer(string container_id, uint8_t **Cold_container, size_t *tmpContainerSize);

        /**
         * @brief Get the content of delta container
         * @param container_id
         * @param Cold_container
         * @param tmpContainerSize
         */   
        bool GetDeltaContainer(string container_id, uint8_t **Cold_container, size_t *tmpContainerSize);

        /**
         * @brief Get the content of cold container
         * @param container_id
         * @param Cold_container
         * @param tmpContainerSize
         * @param delta_flag
         */   
        bool GetColdContainer(string container_id, uint8_t **Cold_container, size_t *tmpContainerSize, uint8_t &delta_flag);

        /**
         * @brief Get the content of target chunk
         * @param tmpcontainer
         * @param tmprecipe
         * @param cold_flag
         * @param tmppair
         */
        uint8_t *GetChunk_content(uint8_t *tmpcontainer, RecipeEntry_t *tmprecipe, bool cold_flag, pair<uint32_t, uint32_t> &tmppair);

        /**
         * @brief Get the content of new deltachunk
         * @param old_deltachunk
         * @param old_deltasize
         * @param old_basechunk
         * @param old_basesize
         * @param new_basechunk
         * @param new_basesize
         * @param new_delta_size
         * @param delta_flag
         */
        uint8_t *GetNew_deltachunk(uint8_t *old_deltachunk, size_t old_deltasize, uint8_t *old_basechunk, size_t old_basesize, uint8_t *new_basechunk, size_t new_basesize, size_t *new_delta_size, bool &delta_flag);

        /**
         * @brief Get the content of new deltachunk
         * @param old_deltachunk
         * @param new_basechunk
         */
        uint8_t *GetBase_deltachunk(uint8_t *old_deltachunk, uint8_t *new_basechunk);

        /**
         * @brief Save new delta chunks
         * @param tmpchunkcontent pointer to deltachunk content
         * @param tmprecipe pointer to deltachunk recipe
         * @param tmpfp pointer to deltachunk hash
         * @param tmpsf pointer to deltachunk superfeature
         * @param tmpIV pointer to deltachunk iv
         */
        void InsertHot_container(uint8_t *tmpchunkcontent, RecipeEntry_t *tmprecipe, uint8_t *tmpfp, uint8_t *tmpsf,
                                uint8_t *tmpIV);

        /**
         * @brief Save new container to disk
         * @param newContainer 
         */
        void SaveHot_container(Container_t &newContainer);

        /**
         * @brief offline phase
         * @param upOutSGX
         * @param cryptoObj_
         */
        void Easy_update(UpOutSGX_t *upOutSGX, EcallCrypto *cryptoObj_);

        /**
         * @brief the the content of superfeature
         * @param tmpcontainer
         * @param tmprecipe
         */
        uint8_t *GetChunk_SF(uint8_t *tmpcontainer, RecipeEntry_t *tmprecipe);

        /**
         * @brief the the content of iv key
         * @param tmpcontainer
         * @param tmprecipe
         */
        uint8_t *GetChunk_IV(uint8_t *tmpcontainer, RecipeEntry_t *tmprecipe);

        /**
         * @brief do delta compression
         * 
         * @param in target chunk buffer 
         * @param in_size target chunk size
         * @param ref base chunk buffer
         * @param ref_size base chunk size
         * @param res_size delta chunk size
         * @return delta chunk buffer
         */
        uint8_t *xd3_encode(const uint8_t *in, size_t in_size, const uint8_t *ref, size_t ref_size, size_t *res_size);

        /**
         * @brief do delta decompression
         * 
         * @param in delta chunk buffer 
         * @param in_size delta chunk size
         * @param ref base chunk buffer
         * @param ref_size base chunk size
         * @param res_size target chunk size
         * @return target chunk buffer
         */
        uint8_t *xd3_decode(const uint8_t *in, size_t in_size, const uint8_t *ref, size_t ref_size, size_t *res_size);

        /**
         * @brief the the content of hash
         * @param tmpcontainer
         * @param tmprecipe
         */
        uint8_t *GetChunk_FP(uint8_t *tmpcontainer, RecipeEntry_t *tmprecipe);

        /**
         * @brief update the index
         * @param key chunkhash
         * @param buffer chunkhash buffer
         * @param bufferSize chunkhash size
         */
        bool UpdateIndexStore(const string &key, const char *buffer,
                            size_t bufferSize);

        void CleanLocal_Index();
};
#endif
