#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1
#define CTR 1
#define ECB 1

#include "aes.h"

static void phex(uint8_t* str);
static int test_encrypt_ecb(int faulty);
static int test_decrypt_ecb();
static state_t* test_encrypt_ecb_verbose(int faulty);


int main(){
    	int exit;

	#if defined(AES256)
    	printf("\nTesting AES256\n\n");
	#elif defined(AES192)
    	printf("\nTesting AES192\n\n");
	#elif defined(AES128)
	printf("\nTesting AES128\n\n");
	#else
    	printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    	return 0;
	#endif

    	state_t* c = test_encrypt_ecb_verbose(0);
	state_t* f = test_encrypt_ecb_verbose(1);
    	
	printf("\n");
	state_t* x;
  	for(int d = 1; d < 256; d++){
		x = dro(d);
		printf("a");
		for(int i = 1; i < 4; i++){
			printf("For subkey byte %i: \n", i);
			int drox = (*x)[i][FAULTCOL];
			for(int k = 0; k < 256; k++){
				int bdrox = bdro(c, f, i, k);
				if(bdrox==drox){	
					printf("delta:%i, key:%i \n",d,k);
				}
			}
			printf("\n \n \n");
		} 
	    	printf("\n");
	}
	return 1;
}


// prints string as he
static void phex(uint8_t* str){

	#if defined(AES256)
    	uint8_t len = 32;
	#elif defined(AES192)
    	uint8_t len = 24;
	#elif defined(AES128)
    	uint8_t len = 16;
	#endif

    	unsigned char i;
    	for (i = 0; i < len; ++i)
        	printf("%.2x ", str[i]);
    	printf("\n");
}

static state_t* test_encrypt_ecb_verbose(int faulty)
{
   	 // Example of more verbose verification

    	uint8_t i;

    	// 128bit key
    	uint8_t key[16] = { 
		(uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, 
		(uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, 
		(uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, 
		(uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, 
		(uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, 
		(uint8_t) 0x3c };
    	// 512bit text
    	uint8_t plain_text[64] = { 
		(uint8_t) 0x6b, (uint8_t) 0xc1, (uint8_t) 0xbe, 
		(uint8_t) 0xe2, (uint8_t) 0x2e, (uint8_t) 0x40, 
		(uint8_t) 0x9f, (uint8_t) 0x96, (uint8_t) 0xe9, 
		(uint8_t) 0x3d, (uint8_t) 0x7e, (uint8_t) 0x11, 
		(uint8_t) 0x73, (uint8_t) 0x93, (uint8_t) 0x17, 
		(uint8_t) 0x2a, (uint8_t) 0xae, (uint8_t) 0x2d, 
		(uint8_t) 0x8a, (uint8_t) 0x57, (uint8_t) 0x1e, 
		(uint8_t) 0x03, (uint8_t) 0xac, (uint8_t) 0x9c, 
		(uint8_t) 0x9e, (uint8_t) 0xb7, (uint8_t) 0x6f, 
		(uint8_t) 0xac, (uint8_t) 0x45, (uint8_t) 0xaf, 
		(uint8_t) 0x8e, (uint8_t) 0x51, (uint8_t) 0x30, 
		(uint8_t) 0xc8, (uint8_t) 0x1c, (uint8_t) 0x46, 
		(uint8_t) 0xa3, (uint8_t) 0x5c, (uint8_t) 0xe4, 
		(uint8_t) 0x11, (uint8_t) 0xe5, (uint8_t) 0xfb, 
		(uint8_t) 0xc1, (uint8_t) 0x19, (uint8_t) 0x1a, 
		(uint8_t) 0x0a, (uint8_t) 0x52, (uint8_t) 0xef,
                (uint8_t) 0xf6, (uint8_t) 0x9f, (uint8_t) 0x24, 
		(uint8_t) 0x45, (uint8_t) 0xdf, (uint8_t) 0x4f, 
		(uint8_t) 0x9b, (uint8_t) 0x17, (uint8_t) 0xad, 
		(uint8_t) 0x2b, (uint8_t) 0x41, (uint8_t) 0x7b, 
		(uint8_t) 0xe6, (uint8_t) 0x6c, (uint8_t) 0x37, 
		(uint8_t) 0x10 };

	    // print text to encrypt, key and IV
	    printf("ECB encrypt verbose:\n\n");
	    printf("plain text:\n");
	    for (i = (uint8_t) 0; i < (uint8_t) 4; ++i){
		phex(plain_text + i * (uint8_t) 16);
	    }
	    printf("\n");

	    printf("key:\n");
	    phex(key);
	    printf("\n");

	    // print the resulting cipher as 4 x 16 byte strings
	    printf("normal ciphertext:\n");
	    
	    struct AES_ctx ctx;
	    AES_init_ctx(&ctx, key);
	
	   state_t* c;
		
	for (i = 0; i < 4; ++i){
	     	c = AES_ECB_encrypt(&ctx, plain_text + (i * 16), faulty);
	      	phex(plain_text + (i * 16));
	}
	return c;
}


static int test_encrypt_ecb(int faulty)
{
    #if defined(AES256)
    uint8_t key[] = {
			0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 
			0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      	0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 
			0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    uint8_t out[] = { 
			0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c,
			0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8 };
    #elif defined(AES192)

    uint8_t key[] = { 	0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52,
			0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                      	0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };

    uint8_t out[] = { 	0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f, 
			0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc };
    #elif defined(AES128)

    uint8_t key[] = { 	0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 
			0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t out[] = { 	0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60,
			0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97 };
    #endif

    uint8_t in[]  = { 	0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 
			0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    AES_ECB_encrypt(&ctx, in, faulty);

    printf("ECB encrypt: ");

    if (0 == memcmp((char*) out, (char*) in, 16)) {
        printf("SUCCESS!\n");
	return(0);
    } else {
        printf("FAILURE!\n");
	return(1);
    }
}

static int test_decrypt_cbc(void){

    #if defined(AES256)
    uint8_t key[] = { 	0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 
			0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      	0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 
			0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    uint8_t in[]  = { 	0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1,
			0xba, 0x77, 0x9e, 0xab, 0xfb, 0x5f, 0x7b,
			0xfb, 0xd6, 0x9c, 0xfc, 0x4e, 0x96, 0x7e, 
			0xdb, 0x80, 0x8d, 0x67, 0x9f, 0x77, 0x7b, 
			0xc6, 0x70, 0x2c, 0x7d, 0x39, 0xf2, 0x33,
			0x69, 0xa9, 0xd9, 0xba, 0xcf, 0xa5, 0x30, 
			0xe2, 0x63, 0x04, 0x23, 0x14, 0x61,
                      	0xb2, 0xeb, 0x05, 0xe2, 0xc3, 0x9b, 0xe9, 
			0xfc, 0xda, 0x6c, 0x19, 0x07, 0x8c, 0x6a, 0x9d, 0x1b };
    #elif defined(AES192)
    uint8_t key[] = { 	0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 
			0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
	 		0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };
    uint8_t in[]  = { 	0x4f, 0x02, 0x1d, 0xb2, 0x43, 0xbc, 0x63, 0x3d, 0x71, 
			0x78, 0x18, 0x3a, 0x9f, 0xa0, 0x71, 0xe8,
                      	0xb4, 0xd9, 0xad, 0xa9, 0xad, 0x7d, 0xed, 0xf4, 
			0xe5, 0xe7, 0x38, 0x76, 0x3f, 0x69, 0x14, 0x5a,
                      	0x57, 0x1b, 0x24, 0x20, 0x12, 0xfb, 0x7a, 0xe0, 
			0x7f, 0xa9, 0xba, 0xac, 0x3d, 0xf1, 0x02, 0xe0,
                      	0x08, 0xb0, 0xe2, 0x79, 0x88, 0x59, 0x88, 0x81, 
			0xd9, 0x20, 0xa9, 0xe6, 0x4f, 0x56, 0x15, 0xcd };
    #elif defined(AES128)
    uint8_t key[] = { 	0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 
			0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    uint8_t in[]  = { 	0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 
			0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                      	0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 
			0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                      	0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 
			0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                      	0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 
			0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7 };
    #endif
    uint8_t iv[]  = { 	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
			0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    uint8_t out[] = { 	0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 
			0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                      	0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 
			0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                      	0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 
			0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                      	0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 
			0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
    //uint8_t buffer[64];
    struct AES_ctx ctx;

    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, in, 64);

    printf("CBC decrypt: ");

    if (0 == memcmp((char*) out, (char*) in, 64)) {
        printf("SUCCESS!\n");
	return(0);
    } else {
        printf("FAILURE!\n");
	return(1);
    }
}

