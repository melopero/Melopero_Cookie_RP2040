#ifndef COOKIERP2040_H
#define COOKIERP2040_H

extern "C"
{
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <pico/time.h>
}

#include "font.h"
#include <vector>
#include <string>

static const uint8_t GPIO_BUTTON_A = 10;
static const uint8_t GPIO_BUTTON_B = 11;

bool repeating_timer_callback(repeating_timer_t *rt);
void gpio_callback(uint gpio, uint32_t event_mask);

enum class InputEvent : uint8_t
{
    BUTTON_A_JUST_RELEASED, BUTTON_A_JUST_PRESSED,
    BUTTON_B_JUST_RELEASED, BUTTON_B_JUST_PRESSED,
    NULL_EVENT,
};

struct InputEventQueue  // ring buffer
{
    static const uint8_t MAX_NUM_INPUT_EVENTS = 10;
    InputEvent mQueue[MAX_NUM_INPUT_EVENTS];
    int8_t mHead = 0;
    int8_t mTail = 0;

    void Insert(InputEvent event)
    {
        if (!IsFull())
        {
            mQueue[mTail++] = event;
            mTail %= MAX_NUM_INPUT_EVENTS;
        }
    }

    InputEvent Get()
    {
        if (!IsEmpty())
        {   
            InputEvent event = mQueue[mHead++];
            mHead %= MAX_NUM_INPUT_EVENTS;

            return event;
        }
        else
            return InputEvent::NULL_EVENT;
    }

    bool IsEmpty() const
    {
        return mHead == mTail;
    }

    bool IsFull() const
    {
        return (mTail + 1) % MAX_NUM_INPUT_EVENTS == mHead;
    }

};

extern InputEventQueue gInputEventQueue;

class CookieRP240
{
friend bool repeating_timer_callback(repeating_timer_t *rt);

public:
    CookieRP240();

    void InitButtonsWithEvents();

    void SetMessageColors(uint32_t backgroundColor, uint32_t charColor);

    enum class MessageDirection : uint8_t
    {
        UP, DOWN, LEFT, RIGHT,
    };
    
    void SetMessageDirection(MessageDirection direction);

    void ShowMessage(const std::string &message, uint32_t timeMS = 200, enum MessageDirection = MessageDirection::LEFT);

    void StopMessage();

    void SetPixel(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue, float brightness = DEFAULT_BRIGHTNESS);

    void ClearDisplay(uint8_t red, uint8_t green, uint8_t blue, float brightness = DEFAULT_BRIGHTNESS);

    void ShowDisplay();

    enum class Button
    {
        A, B,
    };

    bool IsButtonPressed(Button button);

    void SetLED(bool on);
private:
    static const uint8_t WORDS_PER_CHAR = 25;
    static const uint8_t CHAR_WIDTH = 5;
    static const uint8_t CHAR_HEIGHT = 5;

    bool mIsShowingMessage = false;
    bool bIsTimerDone = true;

    std::vector<uint32_t> mBuffer;
    std::vector<uint32_t>::size_type mDataIndex;
    
    uint32_t mFrame[WORDS_PER_CHAR];
    uint32_t mFrameRow; 

    // PIO's sm shifts OSR to the left
    // WS128B protocol GRB color sequence (MSB first)
    uint32_t mBackgroundColor = 0x00100000;
    uint32_t mCharColor = 0x10101000;
    static const float DEFAULT_BRIGHTNESS;

    uint mDMAChannel;
    PIO mPIO;
    uint8_t mSM;

    repeating_timer_t mTimer;

    std::string mMessage;
    MessageDirection mMessageDirection;

    void InitDMA();

    static const uint8_t WS2812_PIN_BASE = 17;

    void InitPIO();

    void InitButtons();

    static const uint8_t LED_PIN = 21;

    void InitLED();

    void SetupAndStartTimer(uint32_t timeMS);

    void RemapBuffer();

    std::string const ReverseMessage(const std::string &message) const;

    uint32_t FormatColor(uint8_t red, uint8_t green, uint8_t blue, float brightness);

    enum class ButtonState
    {
        JUST_RELEASED, RELEASED, JUST_PRESSED, PRESSED,
    } mButtonStateA = ButtonState::RELEASED, mButtonStateB = ButtonState::RELEASED;

    ButtonState GetButtonState(Button button);
};

bool repeating_timer_callback(repeating_timer_t *rt);
void gpio_callback(uint gpio, uint32_t event_mask);

#endif  // COOKIERP2040_H
