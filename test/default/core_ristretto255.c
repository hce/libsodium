#define TEST_NAME "core_ristretto255"
#include "cmptest.h"

static void
tv1(void)
{
    static const char *bad_encodings_hex[] = {
        /* Non-canonical field encodings */
        "00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f",
        "f3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f",
        "edffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f",

        /* Negative field elements */
        "0100000000000000000000000000000000000000000000000000000000000000",
        "01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f",
        "ed57ffd8c914fb201471d1c3d245ce3c746fcbe63a3679d51b6a516ebebe0e20",
        "c34c4e1826e5d403b78e246e88aa051c36ccf0aafebffe137d148a2bf9104562",
        "c940e5a4404157cfb1628b108db051a8d439e1a421394ec4ebccb9ec92a8ac78",
        "47cfc5497c53dc8e61c91d17fd626ffb1c49e2bca94eed052281b510b1117a24",
        "f1c6165d33367351b0da8f6e4511010c68174a03b6581212c71c0e1d026c3c72",
        "87260f7a2f12495118360f02c26a470f450dadf34a413d21042b43b9d93e1309",

        /* Non-square x^2 */
        "26948d35ca62e643e26a83177332e6b6afeb9d08e4268b650f1f5bbd8d81d371",
        "4eac077a713c57b4f4397629a4145982c661f48044dd3f96427d40b147d9742f",
        "de6a7b00deadc788eb6b6c8d20c0ae96c2f2019078fa604fee5b87d6e989ad7b",
        "bcab477be20861e01e4a0e295284146a510150d9817763caf1a6f4b422d67042",
        "2a292df7e32cababbd9de088d1d1abec9fc0440f637ed2fba145094dc14bea08",
        "f4a9e534fc0d216c44b218fa0c42d99635a0127ee2e53c712f70609649fdff22",
        "8268436f8c4126196cf64b3c7ddbda90746a378625f9813dd9b8457077256731",
        "2810e5cbc2cc4d4eece54f61c6f69758e289aa7ab440b3cbeaa21995c2f4232b",

        /* Negative xy value */
        "3eb858e78f5a7254d8c9731174a94f76755fd3941c0ac93735c07ba14579630e",
        "a45fdc55c76448c049a1ab33f17023edfb2be3581e9c7aade8a6125215e04220",
        "d483fe813c6ba647ebbfd3ec41adca1c6130c2beeee9d9bf065c8d151c5f396e",
        "8a2e1d30050198c65a54483123960ccc38aef6848e1ec8f5f780e8523769ba32",
        "32888462f8b486c68ad7dd9610be5192bbeaf3b443951ac1a8118419d9fa097b",
        "227142501b9d4355ccba290404bde41575b037693cef1f438c47f8fbf35d1165",
        "5c37cc491da847cfeb9281d407efc41e15144c876e0170b499a96a22ed31e01e",
        "445425117cb8c90edcbc7c1cc0e74f747f2c1efa5630a967c64f287792a48a4b",

        /* s = -1, which causes y = 0 */
        "ecffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f"
    };
    unsigned char *s;
    size_t         i;

    s = (unsigned char *) sodium_malloc(crypto_core_ristretto255_BYTES);
    for (i = 0; i < sizeof bad_encodings_hex / sizeof bad_encodings_hex[0]; i++) {
        sodium_hex2bin(s, crypto_core_ristretto255_BYTES, bad_encodings_hex[i],
                       crypto_core_ristretto255_BYTES * 2 + 1,
                       NULL, NULL, NULL);
        if (crypto_core_ristretto255_is_valid_point(s)) {
            printf("[%s] was not rejected\n", bad_encodings_hex[i]);
        }
    }
    sodium_free(s);
};

static void
tv2(void)
{
    static const char *uniform_hex[] = {
        "5d1be09e3d0c82fc538112490e35701979d99e06ca3e2b5b54bffe8b4dc772c1"
        "4d98b696a1bbfb5ca32c436cc61c16563790306c79eaca7705668b47dffe5bb6",

        "f116b34b8f17ceb56e8732a60d913dd10cce47a6d53bee9204be8b44f6678b27"
        "0102a56902e2488c46120e9276cfe54638286b9e4b3cdb470b542d46c2068d38",

        "8422e1bbdaab52938b81fd602effb6f89110e1e57208ad12d9ad767e2e25510c"
        "27140775f9337088b982d83d7fcf0b2fa1edffe51952cbe7365e95c86eaf325c",

        "ac22415129b61427bf464e17baee8db65940c233b98afce8d17c57beeb7876c2"
        "150d15af1cb1fb824bbd14955f2b57d08d388aab431a391cfc33d5bafb5dbbaf",

        "165d697a1ef3d5cf3c38565beefcf88c0f282b8e7dbd28544c483432f1cec767"
        "5debea8ebb4e5fe7d6f6e5db15f15587ac4d4d4a1de7191e0c1ca6664abcc413",

        "a836e6c9a9ca9f1e8d486273ad56a78c70cf18f0ce10abb1c7172ddd605d7fd2"
        "979854f47ae1ccf204a33102095b4200e5befc0465accc263175485f0e17ea5c",

        "2cdc11eaeb95daf01189417cdddbf95952993aa9cb9c640eb5058d09702c7462"
        "2c9965a697a3b345ec24ee56335b556e677b30e6f90ac77d781064f866a3c982"
    };
    unsigned char *s;
    unsigned char *u;
    char          *hex;
    size_t         i;

    s = (unsigned char *) sodium_malloc(crypto_core_ristretto255_BYTES);
    u = (unsigned char *) sodium_malloc(crypto_core_ristretto255_UNIFORMBYTES);
    hex = (char *) sodium_malloc(crypto_core_ristretto255_BYTES * 2 + 1);
    for (i = 0; i < sizeof uniform_hex / sizeof uniform_hex[0]; i++) {
        sodium_hex2bin(u, crypto_core_ristretto255_UNIFORMBYTES, uniform_hex[i],
                       crypto_core_ristretto255_UNIFORMBYTES * 2 + 1,
                       NULL, NULL, NULL);
        crypto_core_ristretto255_from_uniform(s, u);
        sodium_bin2hex(hex, crypto_core_ristretto255_BYTES * 2 + 1,
                       s, crypto_core_ristretto255_BYTES);
        printf("%s\n", hex);
    }
    sodium_free(hex);
    sodium_free(u);
    sodium_free(s);
}

int
main(void)
{
    tv1();
    tv2();
    printf("OK\n");

    return 0;
}