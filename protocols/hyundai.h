#pragma once

#include "kia_generic.h"

#define HYUNDAI_PROTOCOL_NAME "Hyundai"

typedef struct SubGhzProtocolDecoderHyundai SubGhzProtocolDecoderHyundai;
typedef struct SubGhzProtocolEncoderHyundai SubGhzProtocolEncoderHyundai;

extern const SubGhzProtocolDecoder subghz_protocol_hyundai_decoder;
extern const SubGhzProtocolEncoder subghz_protocol_hyundai_encoder;
extern const SubGhzProtocol hyundai_protocol;

void* subghz_protocol_decoder_hyundai_alloc(SubGhzEnvironment* environment);
void subghz_protocol_decoder_hyundai_free(void* context);
void subghz_protocol_decoder_hyundai_reset(void* context);
void subghz_protocol_decoder_hyundai_feed(void* context, bool level, uint32_t duration);
uint8_t subghz_protocol_decoder_hyundai_get_hash_data(void* context);
SubGhzProtocolStatus subghz_protocol_decoder_hyundai_serialize(
    void* context,
    FlipperFormat* flipper_format,
    SubGhzRadioPreset* preset);
SubGhzProtocolStatus subghz_protocol_decoder_hyundai_deserialize(void* context, FlipperFormat* flipper_format);
void subghz_protocol_decoder_hyundai_get_string(void* context, FuriString* output);