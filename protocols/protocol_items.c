#include "protocol_items.h"

const SubGhzProtocol* protopirate_protocol_registry_items[] = {
    // KIA/Hyundai family
    &kia_protocol_v0,
    &kia_protocol_v1,
    &kia_protocol_v2,
    &kia_protocol_v3_v4,
    &kia_protocol_v5,
    &hyundai_protocol,
    
    // Asian manufacturers
    &ford_protocol_v0,
    &subaru_protocol,
    &suzuki_protocol,
    &mazda_protocol,
    &honda_protocol,
    &mitsubishi_protocol,
    
    // European VAG Group
    &vw_protocol,
    
    // European PSA Group
    &peugeot_protocol,
    &citroen_protocol,
    
    // European BMW/Fiat
    &bmw_protocol,
    &fiat_protocol_v0,
    
    // American
    &tesla_protocol,
};

const SubGhzProtocolRegistry protopirate_protocol_registry = {
    .items = protopirate_protocol_registry_items,
    .size = COUNT_OF(protopirate_protocol_registry_items),
};