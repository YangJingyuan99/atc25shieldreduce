/**
 * @file chunkStructure.h
 * @author Ruilin Wu(202222080631@std.uestc.edu.cn)
 * @brief define the necessary data structure in deduplication
 * @version 0.1
 * @date 2022-12-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef BASICDEDUP_CHUNK_h
#define BASICDEDUP_CHUNK_h

#include "constVar.h"
#include <stdint.h>

typedef struct {
    uint32_t chunkSize;
    uint8_t data[MAX_CHUNK_SIZE];
} Chunk_t;

typedef struct {
    uint64_t fileSize;
    uint64_t totalChunkNum;
} FileRecipeHead_t;

typedef struct {
    union {
        Chunk_t chunk;
        FileRecipeHead_t recipeHead;
    };
    int dataType;
} Data_t;

typedef struct {
    uint32_t chunkSize;
    uint8_t chunkHash[CHUNK_HASH_SIZE];
} SegmentMeta_t;

typedef struct {
   uint32_t chunkNum; 
   uint32_t segmentSize;
   uint32_t minHashVal;
   uint8_t minHash[CHUNK_HASH_SIZE];
   uint8_t* buffer;
   SegmentMeta_t* metadata;
} Segment_t;

typedef struct {
    uint8_t containerName[CONTAINER_ID_LENGTH];
    uint8_t deltaFlag;
    uint32_t offset;
    uint32_t length;
    uint8_t basechunkHash[CHUNK_HASH_SIZE];
    uint8_t superfeature[3*CHUNK_HASH_SIZE];
} RecipeEntry_t;

typedef struct {
    uint64_t sendChunkBatchSize;
    uint64_t sendRecipeBatchSize;
    uint64_t topKParam;
} EnclaveConfig_t;

typedef struct {
    uint64_t uniqueChunkNum;
    uint64_t uniqueDataSize;
    uint64_t logicalChunkNum;
    uint64_t logicalDataSize;
    long long baseChunkNum;
    uint64_t baseDataSize;
    uint64_t deltaChunkNum;
    uint64_t deltaDataSize;
    long long lz4SaveSize;
    uint64_t DeltaSaveSize;
    uint64_t Online_DeltaSaveSize;
    long long Offline_DeltaSaveSize;
    uint64_t OfflinedeltaChunkNum;
    uint64_t compressedSize;
    

    double enclaveProcessTime;
    double enclaveSFTime;
    double enclaveOnlineTime;

    uint64_t onlineCompress_size;
    uint64_t offlineCompress_size;
    uint64_t offlineDeltanum;
    uint64_t offlineDeDeltanum;
    uint64_t offlineDeletenum;

    uint64_t inline_Ocall;
    uint64_t offline_Ocall;

    double restoreTime;

#if(EDR_BREAKDOWN == 1)
    double dataTranTime;
    double fpTime;
    double sfTime;
    double freqTime;
    double firstDedupTime;
    double secondDedupTime;
    double checkTime;
    double lz4compressTime;
    double deltacompressTime;
    double encTime;
#endif

#if (SGX_BREAKDOWN == 1)
    double dataTranTime;
    double fpTime;
    double freqTime;
    double firstDedupTime;
    double secondDedupTime;
    double compTime;
    double encTime;
#endif


} EnclaveInfo_t;

typedef struct {
    int messageType;
    uint32_t clientID;
    uint32_t dataSize;
    uint32_t currentItemNum;
} NetworkHead_t;

typedef struct {
    NetworkHead_t* header;
    uint8_t* sendBuffer;
    uint8_t* dataBuffer;
} SendMsgBuffer_t;

typedef struct {
    uint32_t recipeNum;
    uint8_t *entryFpList;
} Recipe_t;

typedef struct {
    uint32_t containerID; // the ID to current restore buffer
    uint32_t offset;
    uint32_t length;
    uint8_t deltaFlag;
    uint8_t basechunkHash[CHUNK_HASH_SIZE];
} EnclaveRecipeEntry_t;

typedef struct {
    uint8_t containerName[CONTAINER_ID_LENGTH]; 
    uint32_t offset;
} CacheIndex_t;

typedef struct {
    char containerID[CONTAINER_ID_LENGTH];
    uint8_t deltaFlag;
    uint8_t body[MAX_CONTAINER_SIZE];
    uint32_t currentSize;
} Container_t;

typedef struct {
    uint8_t segmentHash[CHUNK_HASH_SIZE];
    uint8_t binID_[SEGMENT_ID_LENGTH];
} PrimaryValue_t;

typedef struct {
    uint8_t chunkFp[CHUNK_HASH_SIZE];
    RecipeEntry_t address;
} BinValue_t;

typedef struct {
    RecipeEntry_t address;
    uint32_t chunkFreq;
    uint32_t idx;
} HeapItem_t;

typedef struct {
    uint8_t dedupFlag; // true: for duplicate, false: for unique 
    uint8_t deltaFlag;
    uint8_t offlineFlag;
    uint8_t chunkHash[CHUNK_HASH_SIZE];
    RecipeEntry_t chunkAddr;
    uint8_t superfeature[3*CHUNK_HASH_SIZE];
    uint8_t* containerbuffer;
    uint32_t containersize;
    RecipeEntry_t basechunkAddr;
} OutQueryEntry_t; // returned by the outside application for query

typedef struct {
    uint8_t dedupFlag; // true: for duplicate, false: for unique
    uint8_t deltaFlag;
    uint8_t chunkHash[CHUNK_HASH_SIZE];
    RecipeEntry_t chunkAddr;
    uint32_t chunkFreq;
    uint32_t chunkSize;
    uint8_t superfeature[3*CHUNK_HASH_SIZE];
    RecipeEntry_t basechunkAddr;

} InQueryEntry_t; // returned by the outside application for query

typedef struct {
    uint32_t queryNum;
    uint32_t currNum;
    OutQueryEntry_t* outQueryBase;
} OutQuery_t;

typedef struct {
    uint8_t* idBuffer;
    uint8_t** containerArray;
    uint32_t idNum;
} ReqContainer_t;

typedef struct {
    uint8_t* id;
    uint8_t* container;
} ReqOneContainer_t;

typedef struct {
    uint32_t QueryNum;
} DeltaMapInfo_t;

typedef struct {
    Container_t* curContainer;
    Container_t* curDeltaContainer;
    Recipe_t* outRecipe;
    OutQuery_t* outQuery;
    void* outClient;
    void* sgxClient;
    uint8_t* outcallcontainer;
    //for offline process
    uint8_t* process_buffer;
    uint8_t* out_buffer;
    uint8_t* test_buffer;
    DeltaMapInfo_t* deltaInfo;
} UpOutSGX_t;

typedef struct {
    ReqContainer_t* reqContainer;
    ReqOneContainer_t* reqOneContainer;
    SendMsgBuffer_t* sendChunkBuf;
    void* outClient; // the out-enclave client ptr
    void* sgxClient; // the sgx-client ptr
    OutQuery_t* baseQuery;
} ResOutSGX_t;

typedef struct _ra_msg4_struct {
    uint8_t status; // true: 1, false: 0
    //sgx_platform_info_t platformInfoBlob;
} ra_msg4_t;

typedef struct {
    uint8_t* secret;
    size_t length;
} DerivedKey_t;



#endif //BASICDEDUP_CHUNK_h