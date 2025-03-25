/* Standard includes. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* CBOR and OTA includes. */
#include "ota.h"
#include "ota_private.h"
#include "ota_cbor_private.h"

/* Unity framework includes. */
// #include "unity_fixture.h"
// #include "unity.h"

#define CBOR_TEST_MESSAGE_BUFFER_SIZE    ( OTA_FILE_BLOCK_SIZE * 2 )
#define CBOR_TEST_CLIENTTOKEN_VALUE      "TestClientToken"
#define CBOR_TEST_MAX_NUM_BLOCKS_REQUEST 32
#define CBOR_TEST_BITMAP_VALUE           0xAAAAAAAA
#define CBOR_TEST_BLOCKIDENTITY_VALUE    0


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0 || size > CBOR_TEST_MESSAGE_BUFFER_SIZE) {
        return 0;
    }

    int32_t fileId, blockIndex, blockSize, payloadSize = -1;
    size_t encodedSize = 0;
    uint8_t decodedPayload[ OTA_FILE_BLOCK_SIZE ] = { 0 };
    uint8_t * pDecodedPayload = decodedPayload;
    uint32_t bitmap = CBOR_TEST_BITMAP_VALUE;
    bool msgValidity = true;

    uint8_t cborWork[CBOR_TEST_MESSAGE_BUFFER_SIZE];

    if (!createOtaStreamingMessage(
            cborWork,
            sizeof( cborWork ),
            CBOR_TEST_BLOCKIDENTITY_VALUE,
            data,
            size,
            &encodedSize,
            msgValidity)) {
        return 0;
    }

    if (!OTA_CBOR_Decode_GetStreamResponseMessage(
            cborWork, 
            encodedSize, 
            &fileId, 
            &blockIndex, 
            &blockSize, 
            &pDecodedPayload, 
            &payloadSize)) {
        return 0;
    }

    // memset(cborWork, 0, sizeof(cborWork));  // Clear buffer

    if (!OTA_CBOR_Encode_GetStreamRequestMessage(
            cborWork, 
            sizeof(cborWork), 
            &encodedSize, 
            CBOR_TEST_CLIENTTOKEN_VALUE, 
            fileId, 
            OTA_FILE_BLOCK_SIZE, 
            0, 
            ( uint8_t * ) &bitmap,
            sizeof( bitmap ), 
            CBOR_TEST_MAX_NUM_BLOCKS_REQUEST)) {
        return 0; 
    }

    int i;

    for( i = 0; i < ( int ) size; ++i ) {
        if (cborWork[i] != data[i]) {
            abort();
        }
    }

    return 0;
}
