#include "hyundai.h"

#define TAG "HyundaiProtocol"

static const SubGhzBlockConst subghz_protocol_hyundai_const = {
    .te_short = 250,
    .te_long = 500,
    .te_delta = 100,
    .min_count_bit_for_found = 61,
};

struct SubGhzProtocolDecoderHyundai
{
    SubGhzProtocolDecoderBase base;
    SubGhzBlockDecoder decoder;
    SubGhzBlockGeneric generic;
    uint16_t header_count;
};

struct SubGhzProtocolEncoderHyundai
{
    SubGhzProtocolEncoderBase base;
    SubGhzProtocolBlockEncoder encoder;
    SubGhzBlockGeneric generic;
};

typedef enum
{
    HyundaiDecoderStepReset = 0,
    HyundaiDecoderStepCheckPreambula,
    HyundaiDecoderStepSaveDuration,
    HyundaiDecoderStepCheckDuration,
} HyundaiDecoderStep;

const SubGhzProtocolDecoder subghz_protocol_hyundai_decoder = {
    .alloc = subghz_protocol_decoder_hyundai_alloc,
    .free = subghz_protocol_decoder_hyundai_free,
    .feed = subghz_protocol_decoder_hyundai_feed,
    .reset = subghz_protocol_decoder_hyundai_reset,
    .get_hash_data = subghz_protocol_decoder_hyundai_get_hash_data,
    .serialize = subghz_protocol_decoder_hyundai_serialize,
    .deserialize = subghz_protocol_decoder_hyundai_deserialize,
    .get_string = subghz_protocol_decoder_hyundai_get_string,
};

const SubGhzProtocolEncoder subghz_protocol_hyundai_encoder = {
    .alloc = NULL,
    .free = NULL,
    .deserialize = NULL,
    .stop = NULL,
    .yield = NULL,
};

const SubGhzProtocol hyundai_protocol = {
    .name = HYUNDAI_PROTOCOL_NAME,
    .type = SubGhzProtocolTypeDynamic,
    .flag = SubGhzProtocolFlag_433 | SubGhzProtocolFlag_FM | SubGhzProtocolFlag_Decodable,
    .decoder = &subghz_protocol_hyundai_decoder,
    .encoder = &subghz_protocol_hyundai_encoder,
};

void *subghz_protocol_decoder_hyundai_alloc(SubGhzEnvironment *environment)
{
    UNUSED(environment);
    SubGhzProtocolDecoderHyundai *instance = malloc(sizeof(SubGhzProtocolDecoderHyundai));
    instance->base.protocol = &hyundai_protocol;
    instance->generic.protocol_name = instance->base.protocol->name;
    return instance;
}

void subghz_protocol_decoder_hyundai_free(void *context)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;
    free(instance);
}

void subghz_protocol_decoder_hyundai_reset(void *context)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;
    instance->decoder.parser_step = HyundaiDecoderStepReset;
}

void subghz_protocol_decoder_hyundai_feed(void *context, bool level, uint32_t duration)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;

    switch (instance->decoder.parser_step)
    {
    case HyundaiDecoderStepReset:
        if ((level) && (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta))
        {
            instance->decoder.parser_step = HyundaiDecoderStepCheckPreambula;
            instance->decoder.te_last = duration;
            instance->header_count = 0;
        }
        break;
    case HyundaiDecoderStepCheckPreambula:
        if (level)
        {
            if ((DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta) ||
                (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_long) < subghz_protocol_hyundai_const.te_delta))
            {
                instance->decoder.te_last = duration;
            }
            else
            {
                instance->decoder.parser_step = HyundaiDecoderStepReset;
            }
        }
        else if (
            (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta) &&
            (DURATION_DIFF(instance->decoder.te_last, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta))
        {
            instance->header_count++;
            break;
        }
        else if (
            (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_long) < subghz_protocol_hyundai_const.te_delta) &&
            (DURATION_DIFF(instance->decoder.te_last, subghz_protocol_hyundai_const.te_long) < subghz_protocol_hyundai_const.te_delta))
        {
            if (instance->header_count > 15)
            {
                instance->decoder.parser_step = HyundaiDecoderStepSaveDuration;
                instance->decoder.decode_data = 0;
                instance->decoder.decode_count_bit = 1;
                subghz_protocol_blocks_add_bit(&instance->decoder, 1);
            }
            else
            {
                instance->decoder.parser_step = HyundaiDecoderStepReset;
            }
        }
        else
        {
            instance->decoder.parser_step = HyundaiDecoderStepReset;
        }
        break;
    case HyundaiDecoderStepSaveDuration:
        if (level)
        {
            if (duration >=
                (subghz_protocol_hyundai_const.te_long + subghz_protocol_hyundai_const.te_delta * 2UL))
            {
                instance->decoder.parser_step = HyundaiDecoderStepReset;
                if (instance->decoder.decode_count_bit ==
                    subghz_protocol_hyundai_const.min_count_bit_for_found)
                {
                    instance->generic.data = instance->decoder.decode_data;
                    instance->generic.data_count_bit = instance->decoder.decode_count_bit;
                    if (instance->base.callback)
                        instance->base.callback(&instance->base, instance->base.context);
                }
                instance->decoder.decode_data = 0;
                instance->decoder.decode_count_bit = 0;
                break;
            }
            else
            {
                instance->decoder.te_last = duration;
                instance->decoder.parser_step = HyundaiDecoderStepCheckDuration;
            }
        }
        else
        {
            instance->decoder.parser_step = HyundaiDecoderStepReset;
        }
        break;
    case HyundaiDecoderStepCheckDuration:
        if (!level)
        {
            if ((DURATION_DIFF(instance->decoder.te_last, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta) &&
                (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_short) < subghz_protocol_hyundai_const.te_delta))
            {
                subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                instance->decoder.parser_step = HyundaiDecoderStepSaveDuration;
            }
            else if (
                (DURATION_DIFF(instance->decoder.te_last, subghz_protocol_hyundai_const.te_long) < subghz_protocol_hyundai_const.te_delta) &&
                (DURATION_DIFF(duration, subghz_protocol_hyundai_const.te_long) < subghz_protocol_hyundai_const.te_delta))
            {
                subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                instance->decoder.parser_step = HyundaiDecoderStepSaveDuration;
            }
            else
            {
                instance->decoder.parser_step = HyundaiDecoderStepReset;
            }
        }
        else
        {
            instance->decoder.parser_step = HyundaiDecoderStepReset;
        }
        break;
    }
}

static void subghz_protocol_hyundai_check_remote_controller(SubGhzBlockGeneric *instance)
{
    instance->serial = (uint32_t)((instance->data >> 12) & 0x0FFFFFFF);
    instance->btn = (instance->data >> 8) & 0x0F;
    instance->cnt = (instance->data >> 40) & 0xFFFF;
}

uint8_t subghz_protocol_decoder_hyundai_get_hash_data(void *context)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;
    return subghz_protocol_blocks_get_hash_data(
        &instance->decoder, (instance->decoder.decode_count_bit / 8) + 1);
}

SubGhzProtocolStatus subghz_protocol_decoder_hyundai_serialize(
    void *context,
    FlipperFormat *flipper_format,
    SubGhzRadioPreset *preset)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;
    
    subghz_protocol_hyundai_check_remote_controller(&instance->generic);
    return subghz_block_generic_serialize(&instance->generic, flipper_format, preset);
}

SubGhzProtocolStatus
subghz_protocol_decoder_hyundai_deserialize(void *context, FlipperFormat *flipper_format)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;
    return subghz_block_generic_deserialize_check_count_bit(
        &instance->generic, flipper_format, subghz_protocol_hyundai_const.min_count_bit_for_found);
}

void subghz_protocol_decoder_hyundai_get_string(void *context, FuriString *output)
{
    furi_assert(context);
    SubGhzProtocolDecoderHyundai *instance = context;

    subghz_protocol_hyundai_check_remote_controller(&instance->generic);
    uint32_t code_found_hi = instance->generic.data >> 32;
    uint32_t code_found_lo = instance->generic.data & 0x00000000ffffffff;

    furi_string_cat_printf(
        output,
        "%s %dbit\r\n"
        "Key:%08lX%08lX\r\n"
        "Sn:%07lX Btn:%X Cnt:%04lX\r\n",
        instance->generic.protocol_name,
        instance->generic.data_count_bit,
        code_found_hi,
        code_found_lo,
        instance->generic.serial,
        instance->generic.btn,
        instance->generic.cnt);
}