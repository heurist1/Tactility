#pragma once

#include "hal/SdCard.h"

#define SDCARD_SPI_HOST SPI3_HOST

using namespace tt::hal;

std::shared_ptr<SdCard> createYellowResSdCard();

