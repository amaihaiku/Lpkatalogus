#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include "../Emulator/keyboard_defs.h"

typedef std::function<void(SpecKeys key, bool down)> OnKeyPress;
typedef std::function<void(SpecKeys key)> OnKeyLongPress;

class AnalogJoystick
{
private:
    OnKeyPress _onKeyPress;
    OnKeyLongPress _onKeyLongPress;
    TaskHandle_t _taskHandle;

    int _pinX;
    int _pinY;
    int _pinA;
    int _pinB;
    int _pinMenu;

    // Joystick state variables
    bool _isUp = false;
    bool _isDown = false;
    bool _isLeft = false;
    bool _isRight = false;

    // Button state variables
    bool _btnA = false;
    bool _btnB = false;
    bool _btnMenu = false;

    void readJoystick();

public:
    AnalogJoystick(OnKeyPress onKeyPress, OnKeyLongPress onKeyLongPress,
                   int pinX, int pinY, int pinA, int pinB, int pinMenu);
    ~AnalogJoystick();
    void start();
    friend void analogJoystickTask(void *pvParameters);
};
