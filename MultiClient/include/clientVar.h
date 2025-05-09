/**
 * @file clientVar.h
 * @author Ruilin Wu(202222080631@std.uestc.edu.cn)
 * @brief define the class to store the variable related to a client in the outside the enclave
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CLIENT_VAR_H
#define CLIENT_VAR_H

#define CONTAINERARRAY_VALUE 50
#include "define.h"
#include "chunkStructure.h"
#include "messageQueue.h"
#include "readCache.h"
#include "sslConnection.h"

using namespace std;

extern Configure config;

class ClientVar {
    private:
        string myName_ = "ClientVar";
        int optType_; // the operation type (upload / download)
        uint64_t sendChunkBatchSize_;
        uint64_t sendRecipeBatchSize_;
        string recipePath_;

        /**
         * @brief init the upload buffer
         * 
         */
        void InitUploadBuffer();

        /**
         * @brief destroy the upload buffer
         * 
         */
        void DestroyUploadBuffer();

        /**
         * @brief init the restore buffer
         * 
         */
        void InitRestoreBuffer();

        /**
         * @brief destroy the restore buffer
         * 
         */
        void DestroyRestoreBuffer();
    public:
        uint32_t _clientID;

        // for handling file recipe
        ofstream _recipeWriteHandler;
        ifstream _recipeReadHandler;
        string _tmpQueryBufferStr;
        string _tmpBatchQueryBufferStr;

        // for sgx context 
        UpOutSGX_t _upOutSGX; // pass this structure to the enclave for upload
        ResOutSGX_t _resOutSGX; // pass this structure to the enclave for restore

        // upload buffer parameters
        Container_t _curContainer; // current container buffer
        Container_t _curDeltaContainer; //current delta container buffer

        uint8_t* outRefContainerBuffer_;

        OutQuery_t _outQuery; // the buffer to store the encrypted chunk fp
        MessageQueue<Container_t>* _inputMQ;
        SendMsgBuffer_t _recvChunkBuf;
        Recipe_t _outRecipe; // the buffer to store ciphertext recipe

        // restore buffer parameters
        OutQuery_t _baseoutQuery;
        uint8_t* _readRecipeBuf;
        ReqContainer_t _reqContainer;
        ReadCache* _containerCache;
        SendMsgBuffer_t _sendChunkBuf;
        ReqOneContainer_t _reqOneContainer;
        uint8_t* _outCallcontainer;

        //buffer for offline
        uint8_t* _process_buffer;
        uint8_t* _out_buffer;
        uint8_t* _test_buffer;
        DeltaMapInfo_t _deltaInfo;
        

        SSL* _clientSSL; // connection

        // upload logical data size
        uint64_t _uploadDataSize = 0;

        /**
         * @brief Construct a new ClientVar object
         * 
         * @param clientID the client ID
         * @param clientSSL the client SSL
         * @param optType the operation type (upload / download)
         * @param recipePath the file recipe path
         */
        ClientVar(uint32_t clientID, SSL* clientSSL, 
            int optType, string& recipePath);

        /**
         * @brief Destroy the Client Var object
         * 
         */
        ~ClientVar();
};

#endif