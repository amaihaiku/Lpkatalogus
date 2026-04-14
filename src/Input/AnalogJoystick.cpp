#include "AnalogJoystick.h"

void analogJoystickTask(void *pvParameters)
{
    AnalogJoystick *joystick = (AnalogJoystick *)pvParameters;
    while (true)
    {
        joystick->readJoystick();
        vTaskDelay(pdMS_TO_TICKS(20)); // Poll every 20ms
    }
}

AnalogJoystick::AnalogJoystick(OnKeyPress onKeyPress, OnKeyLongPress onKeyLongPress,
                               int pinX, int pinY, int pinA, int pinB, int pinMenu)
    : _onKeyPress(onKeyPress), _onKeyLongPress(onKeyLongPress),
      _pinX(pinX), _pinY(pinY), _pinA(pinA), _pinB(pinB), _pinMenu(pinMenu)
{
    pinMode(_pinX, INPUT);
    pinMode(_pinY, INPUT);
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_pinMenu, INPUT_PULLUP);
}

AnalogJoystick::~AnalogJoystick()
{
    if (_taskHandle)
    {
        vTaskDelete(_taskHandle);
    }
}

void AnalogJoystick::start()
{
    xTaskCreatePinnedToCore(
        analogJoystickTask,
        "AnalogJoystick",
        4096,
        this,
        1,
        &_taskHandle,
        1);
}

void AnalogJoystick::readJoystick()
{
    int x = analogRead(_pinX);
    int y = analogRead(_pinY);

    // Joystick thresholds (0-4095)
    bool newUp = y < 1000;
    bool newDown = y > 3000;
    bool newLeft = x < 1000;
    bool newRight = x > 3000;

    bool newBtnA = digitalRead(_pinA) == LOW;
    bool newBtnB = digitalRead(_pinB) == LOW;
    bool newBtnMenu = digitalRead(_pinMenu) == LOW;

    if (newUp != _isUp) {
        _isUp = newUp;
        _onKeyPress(JOYK_UP, _isUp);
    }
    if (newDown != _isDown) {
        _isDown = newDown;
        _onKeyPress(JOYK_DOWN, _isDown);
    }
    if (newLeft != _isLeft) {
        _isLeft = newLeft;
        _onKeyPress(JOYK_LEFT, _isLeft);
    }
    if (newRight != _isRight) {
        _isRight = newRight;
        _onKeyPress(JOYK_RIGHT, _isRight);
    }

    // Button A -> Fire / Enter
    if (newBtnA != _btnA) {
        _btnA = newBtnA;
        _onKeyPress(SPECKEY_ENTER, _btnA);
    }

    // Button B -> Space and long press check
    static unsigned long last_btn_b_press = 0;
    static bool btn_b_long_pressed = false;

    if (newBtnB != _btnB) {
        _btnB = newBtnB;
        if (_btnB) {
            last_btn_b_press = millis();
            btn_b_long_pressed = false;
            _onKeyPress(SPECKEY_SPACE, true);
        } else {
            if (!btn_b_long_pressed) {
                _onKeyPress(SPECKEY_SPACE, false);
            }
        }
    }

    // Check long press while held
    if (_btnB && !btn_b_long_pressed && (millis() - last_btn_b_press > 800)) {
        btn_b_long_pressed = true;
        _onKeyPress(SPECKEY_SPACE, false); // Release the normal key
        _onKeyLongPress(SPECKEY_SPACE);    // Trigger the long press for the UI!
    }

    // Menu button
    static unsigned long last_btn_menu_press = 0;
    static bool btn_menu_long_pressed = false;

    if (newBtnMenu != _btnMenu) {
        _btnMenu = newBtnMenu;
        if (_btnMenu) {
            last_btn_menu_press = millis();
            btn_menu_long_pressed = false;
            _onKeyPress(SPECKEY_MENU, true);
        } else {
            if (!btn_menu_long_pressed) {
                 _onKeyPress(SPECKEY_MENU, false);
            }
        }
    }

    if (_btnMenu && !btn_menu_long_pressed && (millis() - last_btn_menu_press > 800)) {
        btn_menu_long_pressed = true;
        _onKeyPress(SPECKEY_MENU, false);
        _onKeyLongPress(SPECKEY_MENU);
    }
}
