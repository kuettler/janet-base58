#include <janet.h>
#include <openssl/sha.h>

#include "libbase58.h"

static bool sha256(void *dst, const void *src, size_t size) {
    SHA256(src, size, dst);
    return true;
}

JANET_FN(cfun_base58_decode,
         "(base58/decode message)",
         "Base58 decode message.") {
    janet_fixarity(argc, 1);
    const uint8_t* message = janet_getstring(argv, 0);

    size_t length = janet_string_length(message);
    if (length == 0)
        return janet_wrap_nil();

    uint8_t* bin = (uint8_t*)janet_smalloc(length);
    size_t bin_sz = length;
    bool result = b58tobin(bin, &bin_sz, message, length);
    if (!result) janet_panic("Failed to base58 decode message.");
    const uint8_t* msg = janet_string(bin + length - bin_sz, bin_sz);
    janet_sfree(bin);
    return janet_wrap_string(msg);
}

JANET_FN(cfun_base58_checked_decode,
         "(base58/checked_decode message)",
         "Base58 decode message with checksum and version byte.") {
    janet_fixarity(argc, 1);
    const uint8_t* message = janet_getstring(argv, 0);

    b58_sha256_impl = sha256;

    size_t length = janet_string_length(message);
    if (length == 0)
        return janet_wrap_nil();

    uint8_t* bin = (uint8_t*)janet_smalloc(length);
    size_t bin_sz = length;
    bool result = b58tobin(bin, &bin_sz, message, length);
    if (!result) janet_panic("Failed to base58 decode message.");
    int version = b58check(bin + length - bin_sz, bin_sz, message, length);
    if (version < 0) janet_panic("Failed to verify checksum.");
    const uint8_t* msg = janet_string(bin + length - bin_sz + 1, bin_sz - 5);
    janet_sfree(bin);
    const Janet values[2] = {
        janet_wrap_string(msg),
        janet_wrap_number(version)
    };
    return janet_wrap_tuple(janet_tuple_n(values, 2));
}

JANET_FN(cfun_base58_encode,
         "(base58/encode message)",
         "Base58 encode message.") {
    janet_fixarity(argc, 1);
    const uint8_t* message = janet_getstring(argv, 0);

    size_t length = janet_string_length(message);
    if (length == 0)
        return janet_wrap_nil();
    size_t b58c_sz = 2*length;
    char* b58c = (char*)janet_smalloc(b58c_sz);
    bool result = b58enc(b58c, &b58c_sz, message, janet_string_length(message));
    if (!result) janet_panicf("Failed to base58 encode message. %d bytes needed.", b58c_sz);
    const uint8_t* md = janet_string(b58c, b58c_sz - 1);
    janet_sfree(b58c);
    return janet_wrap_string(md);
}

JANET_FN(cfun_base58_checked_encode,
         "(base58/checked_encode message version)",
         "Base58 encode message with checksum and version byte.") {
    janet_arity(argc, 1, 2);
    const uint8_t* message = janet_getstring(argv, 0);
    uint8_t ver = janet_optnat(argv, argc, 1, 0);

    b58_sha256_impl = sha256;

    size_t length = janet_string_length(message);
    if (length == 0)
        return janet_wrap_nil();
    size_t b58c_sz = 2*length + 9;
    char* b58c = (char*)janet_smalloc(b58c_sz);
    bool result = b58check_enc(b58c, &b58c_sz, ver, message, janet_string_length(message));
    if (!result) janet_panicf("Failed to base58 encode message. %d bytes needed.", b58c_sz);
    const uint8_t* md = janet_string(b58c, b58c_sz - 1);
    janet_sfree(b58c);
    return janet_wrap_string(md);
}

JANET_MODULE_ENTRY(JanetTable *env) {
    JanetRegExt cfuns[] = {
        JANET_REG("decode", cfun_base58_decode),
        JANET_REG("checked_decode", cfun_base58_checked_decode),
        JANET_REG("encode", cfun_base58_encode),
        JANET_REG("checked_encode", cfun_base58_checked_encode),
        JANET_REG_END
    };
    janet_cfuns_ext(env, "base58", cfuns);
}
