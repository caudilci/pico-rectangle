#include "gpio_to_button_sets/F1.hpp"

#include "pico/stdlib.h"

#include "persistence/pages/runtime_remapping.hpp"
#include "persistence/functions.hpp"

#include "global.hpp"

namespace GpioToButtonSets {
namespace F1 {

struct PinMapping {
    uint8_t pin;
    bool ButtonSet::* ptrToMember;
};

const PinMapping pinMappings[] = {
    #if !USE_UART0
    { 0, &ButtonSet::start },
    #endif
    { 27, &ButtonSet::right },
    { 26, &ButtonSet::down },
    { 22, &ButtonSet::left },
    { 9, &ButtonSet::l },
    { 16, &ButtonSet::mx },
    { 18, &ButtonSet::my },
    { 10, &ButtonSet::cUp },
    { 11, &ButtonSet::cLeft },
    { 13, &ButtonSet::a },
    { 12, &ButtonSet::cDown },
    { 14, &ButtonSet::cRight },
    { 21, &ButtonSet::up },
    { 8, &ButtonSet::ms },
    { 3, &ButtonSet::z },
    { 2, &ButtonSet::ls },
    { 4, &ButtonSet::x },
    { 7, &ButtonSet::y },
    { 6, &ButtonSet::b },
    { 5, &ButtonSet::r },
};

bool remapped;

const PinMapping remappedPinMappings[] = {
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.startPin, &ButtonSet::start },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.rightPin, &ButtonSet::right },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.downPin, &ButtonSet::down },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.leftPin, &ButtonSet::left },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.lPin, &ButtonSet::l },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.mxPin, &ButtonSet::mx },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.myPin, &ButtonSet::my },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.cUpPin, &ButtonSet::cUp },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.cLeftPin, &ButtonSet::cLeft },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.aPin, &ButtonSet::a },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.cDownPin, &ButtonSet::cDown },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.cRightPin, &ButtonSet::cRight },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.upPin, &ButtonSet::up },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.msPin, &ButtonSet::ms },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.zPin, &ButtonSet::z },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.lsPin, &ButtonSet::ls },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.xPin, &ButtonSet::x },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.yPin, &ButtonSet::y },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.bPin, &ButtonSet::b },
    { Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.rPin, &ButtonSet::r }
};

bool init = false;

void initDefaultConversion() {
    remapped = Persistence::isnt0xFF(Persistence::read<Persistence::Pages::RuntimeRemapping>()->f1GpioToButtonSetRemapping.configured);
    for (PinMapping pinMapping : remapped ? remappedPinMappings : pinMappings) {
        gpio_init(pinMapping.pin);
        gpio_set_dir(pinMapping.pin, GPIO_IN);
        gpio_pull_up(pinMapping.pin);
    }
    init = true;
}

ButtonSet defaultConversion() {

    if (!init) initDefaultConversion();

    if (remapped) gpio_put(LED_PIN, 1);
    
    ButtonSet f1ButtonSet;

    uint32_t inputSnapshot = sio_hw->gpio_in;

    for (PinMapping pinMapping : remapped ? remappedPinMappings : pinMappings) {
        f1ButtonSet.*(pinMapping.ptrToMember) = !(inputSnapshot & (1 << (pinMapping.pin)));
    }

    return f1ButtonSet;
}

}
}