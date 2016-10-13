// Basic constants

#ifndef CONSTS_H
#define CONSTS_H

#pragma once

#include "typedef.h"

const size_t LIMB_SIZE = sizeof (limb_t);
const size_t LIMB_BITS = LIMB_SIZE * 8;

const limb_t LIMB_MAX = limb_t (-1);

#endif
