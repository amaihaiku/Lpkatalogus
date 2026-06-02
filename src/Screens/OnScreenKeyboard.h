#pragma once
#include "Screen.h"
#include <Arduino.h>
#include <functional>
#include "../Emulator/keyboard_defs.h"
#include "../TFT/Display.h"
#include "../TFT/HDMIDisplay.h"
#include "../AudioOutput/AudioOutput.h"
#include "../Files/Files.h"

typedef std::function<void(SpecKeys key, bool down)> OnKeyPress;

class OnScreenKeyboard : public Screen
{
private:
    OnKeyPress _onKeyPress;
    std::function<void()> _onClose;
    int _cursorX = 0;
    int _cursorY = 0;
    bool _isClosing = false;
    unsigned long _lastMove = 0;

    static const int ROWS = 4;
    static const int COLS = 10;
    SpecKeys _keys[ROWS][COLS] = {
        {SPECKEY_1, SPECKEY_2, SPECKEY_3, SPECKEY_4, SPECKEY_5, SPECKEY_6, SPECKEY_7, SPECKEY_8, SPECKEY_9, SPECKEY_0},
        {SPECKEY_Q, SPECKEY_W, SPECKEY_E, SPECKEY_R, SPECKEY_T, SPECKEY_Y, SPECKEY_U, SPECKEY_I, SPECKEY_O, SPECKEY_P},
        {SPECKEY_A, SPECKEY_S, SPECKEY_D, SPECKEY_F, SPECKEY_G, SPECKEY_H, SPECKEY_J, SPECKEY_K, SPECKEY_L, SPECKEY_ENTER},
        {SPECKEY_SHIFT, SPECKEY_Z, SPECKEY_X, SPECKEY_C, SPECKEY_V, SPECKEY_B, SPECKEY_N, SPECKEY_M, SPECKEY_SYMB, SPECKEY_SPACE}
    };

public:
    OnScreenKeyboard(Display &display, HDMIDisplay *hdmiDisplay, AudioOutput *audioOutput, IFiles *files, OnKeyPress onKeyPress, std::function<void()> onClose)
        : Screen(display, hdmiDisplay, audioOutput, files), _onKeyPress(onKeyPress), _onClose(onClose)
    {
    }

    void didAppear() override
    {
        _isClosing = false;
        _cursorX = 0;
        _cursorY = 0;
        _lastMove = millis();
        // Just print basic stuff to console or rely on basic clear
        m_tft.fillRect(0, 0, m_tft.width(), m_tft.height(), TFT_BLACK);
        drawKeyboard();
    }

    void drawKeyboard()
    {
        int cellW = 30;
        int cellH = 30;
        int startX = 10;
        int startY = 60;

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                int px = startX + x * cellW;
                int py = startY + y * cellH;

                uint16_t color = TFT_WHITE;
                uint16_t bgColor = TFT_BLACK;

                if (x == _cursorX && y == _cursorY) {
                    color = TFT_BLACK;
                    bgColor = TFT_WHITE;
                }

                m_tft.fillRect(px, py, cellW-2, cellH-2, bgColor);
            }
        }
    }

    bool loop()
    {
        return !_isClosing;
    }

    void updateKey(SpecKeys key, uint8_t down)
    {
        if (!down) return;

        if (millis() - _lastMove < 150) return; // debounce

        bool moved = false;
        if (key == JOYK_UP || key == JOYK_UP) { _cursorY--; moved = true; }
        if (key == JOYK_DOWN || key == JOYK_DOWN) { _cursorY++; moved = true; }
        if (key == JOYK_LEFT || key == JOYK_LEFT) { _cursorX--; moved = true; }
        if (key == JOYK_RIGHT || key == JOYK_RIGHT) { _cursorX++; moved = true; }

        if (moved) {
            if (_cursorX < 0) _cursorX = COLS - 1;
            if (_cursorX >= COLS) _cursorX = 0;
            if (_cursorY < 0) _cursorY = ROWS - 1;
            if (_cursorY >= ROWS) _cursorY = 0;

            drawKeyboard();
            _lastMove = millis();
            return;
        }

        if (key == SPECKEY_ENTER || key == JOYK_FIRE || key == SPECKEY_SPACE) {
             // Fire button pressed
             _onKeyPress(_keys[_cursorY][_cursorX], true);
             delay(50);
             _onKeyPress(_keys[_cursorY][_cursorX], false);
             _isClosing = true;
             _onClose();
        }

        if (key == SPECKEY_MENU) {
            _isClosing = true;
            _onClose();
        }
    }
};
