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
#define CBOR_TEST_MAX_NUM_BLOCKS_REQUEST 16
// #define CBOR_TEST_BITMAP_VALUE           0xAAAAAAAA
// #define CBOR_TEST_BLOCKIDENTITY_VALUE    0


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0 || size > CBOR_TEST_MESSAGE_BUFFER_SIZE) {
        return 0;
    }

    int32_t fileId, blockIndex, blockSize = 0;
    uint8_t *pDecodedPayload = NULL;
    size_t payloadSize = 0;

    uint8_t cborWork[CBOR_TEST_MESSAGE_BUFFER_SIZE] = {0};
    memcpy(cborWork, data, size);

    if (OTA_CBOR_Decode_GetStreamResponseMessage(cborWork, size, &fileId, &blockIndex, &blockSize, &pDecodedPayload, &payloadSize) == 0) {
        return 0;
    }

    size_t encodedSize = 0;
    memset(cborWork, 0, sizeof(cborWork));  // Clear buffer

    if (OTA_CBOR_Encode_GetStreamRequestMessage(cborWork, sizeof(cborWork), &encodedSize, CBOR_TEST_CLIENTTOKEN_VALUE, fileId, blockSize, 0, NULL, 0, CBOR_TEST_MAX_NUM_BLOCKS_REQUEST) == 0) {
        return 0; 
    }

    if (memcmp(data, cborWork, size < encodedSize ? size : encodedSize) != 0) {
        __builtin_trap();
    }

    return 0;
}
