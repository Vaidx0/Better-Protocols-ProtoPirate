// protocols/protocol_items.h
#pragma once

#include <lib/subghz/types.h>

// KIA/Hyundai protocols (already in project)
#include "kia_generic.h"
#include "kia_v0.h"
#include "kia_v1.h"
#include "kia_v2.h"
#include "kia_v3_v4.h"
#include "kia_v5.h"
#include "hyundai.h"

// Asian manufacturers
#include "ford_v0.h"
#include "subaru.h"
#include "suzuki.h"
#include "mazda.h"
#include "honda.h"
#include "mitsubishi.h"

// European manufacturers - VAG Group
#include "vw.h"

// European manufacturers - PSA Group
#include "peugeot.h"
#include "citroen.h"

// European manufacturers - BMW/Fiat
#include "bmw.h"
#include "fiat_v0.h"

// American manufacturers
#include "tesla.h"

extern const SubGhzProtocolRegistry protopirate_protocol_registry;