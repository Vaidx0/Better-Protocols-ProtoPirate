#pragma once

#include <furi.h>
#include <lib/subghz/protocols/base.h>
#include <lib/subghz/types.h>
#include <lib/subghz/blocks/const.h>
#include <lib/subghz/blocks/decoder.h>
#include <lib/subghz/blocks/encoder.h>
#include <lib/subghz/blocks/generic.h>
#include <lib/subghz/blocks/math.h>
#include <flipper_format/flipper_format.h>

#define CITROEN_PROTOCOL_NAME "Citroen"

extern const SubGhzProtocol citroen_protocol;

void* subghz_protocol_decoder_citroen_alloc(SubGhzEnvironment* environment);
void subghz_protocol_decoder_citroen_free(void* context);
void subghz_protocol_decoder_citroen_reset(void* context);
void subghz_protocol_decoder_citroen_feed(void* context, bool level, uint32_t duration);
uint8_t subghz_protocol_decoder_citroen_get_hash_data(void* context);
SubGhzProtocolStatus subghz_protocol_decoder_citroen_serialize(
    void* context,
    FlipperFormat* flipper_format,
    SubGhzRadioPreset* preset);
SubGhzProtocolStatus subghz_protocol_decoder_citroen_deserialize(
    void* context,
    FlipperFormat* flipper_format);
void subghz_protocol_decoder_citroen_get_string(void* context, FuriString* output);
