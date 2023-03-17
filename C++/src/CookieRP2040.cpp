#include "CookieRP2040.hpp"

extern "C"
{
#include "ws2812.pio.h"
}

InputEventQueue gInputEventQueue;

const float CookieRP240::DEFAULT_BRIGHTNESS = 1.0f;

CookieRP240::CookieRP240() : mDataIndex(0), mMessageDirection(MessageDirection::UP)
{
    InitDMA();
    InitPIO();
    InitLED();
    InitButtons();
}

void CookieRP240::SetMessageColors(uint32_t backgroundColor, uint32_t charColor)
{
    mBackgroundColor = backgroundColor;
    mCharColor = charColor;
}
    
void CookieRP240::SetMessageDirection(MessageDirection direction) { mMessageDirection = direction; }

void CookieRP240::ShowMessage(const std::string &message, uint32_t timeMS, enum MessageDirection)
{
    mMessage = message;

    mBuffer.clear();
    mBuffer.resize((message.size() + 2) * WORDS_PER_CHAR);
 
    switch (mMessageDirection)
    {
        case MessageDirection::UP:  

            for (int i = 0; i < message.size(); i++)
            {
                const uint8_t (&character)[CHAR_WIDTH] = glyphs[message[i] - 32];
                //const uint8_t *character = glyphs[message[i] - 32];  

                for (int row = 0; row < CHAR_HEIGHT; row++)
                    for (int col = 0; col < CHAR_WIDTH; col++)
                    {
                        uint8_t characterCol = character[col]; 
                        uint32_t color = characterCol & 1 << (CHAR_HEIGHT - 1) - row ? mCharColor : mBackgroundColor;
                        mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                    }      
            }

            mDataIndex = 0;

            break;

        case MessageDirection::DOWN:
        {
            std::string reversedMessage = ReverseMessage(message);
                
            for (int i = 0; i < reversedMessage.size(); i++)
            {
                const uint8_t (&character)[CHAR_WIDTH] = glyphs[reversedMessage[i] - 32];

                for (int row = 0; row < CHAR_HEIGHT; row++)
                    for (int col = 0; col < CHAR_WIDTH; col++)
                    {
                        uint8_t characterCol = character[col]; 
                        uint32_t color = characterCol & 1 << (CHAR_HEIGHT - 1) - row ? mCharColor : mBackgroundColor;
                        mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                    } 
            }
                
            mDataIndex = (reversedMessage.size() + 1) * WORDS_PER_CHAR;

            break;
        }    
                
        case MessageDirection::LEFT:

            for (int i = 0; i < message.size(); i++)
            {
                const uint8_t (&character)[CHAR_WIDTH] = glyphs[message[i] - 32];

                for (int row = 0; row < CHAR_HEIGHT; row++)
                    for (int col = 0; col < CHAR_WIDTH; col++)
                    {
                        uint8_t characterCol = character[row]; 
                        uint32_t color = characterCol & 1 << col ? mCharColor : mBackgroundColor;
                        mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                    } 
            }

            mFrameRow = 0;

            break;

        case MessageDirection::RIGHT:
        {
            std::string reversedMessage = ReverseMessage(message);

            for (int i = 0; i < reversedMessage.size(); i++)
            {
                const uint8_t (&character)[CHAR_WIDTH] = glyphs[reversedMessage[i] - 32];
                    
                for (int row = 0; row < CHAR_HEIGHT; row++)
                    for (int col = 0; col < CHAR_WIDTH; col++)
                    {
                        uint8_t characterCol = character[row]; 
                        uint32_t color = characterCol & 1 << col ? mCharColor : mBackgroundColor;
                        mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                    } 
            }

            mFrameRow = (reversedMessage.size() + 1) * CHAR_WIDTH; 

            break;
        }
    }

    // fill first and last empty character
    for (int row = 0; row < CHAR_HEIGHT; row++)
        for (int col = 0; col < CHAR_WIDTH; col++)
        {
            mBuffer[row * CHAR_WIDTH + col] = mBackgroundColor;
            mBuffer[row * CHAR_WIDTH + col + (mMessage.size() + 1) * WORDS_PER_CHAR] = mBackgroundColor;
        } 
 
    mIsShowingMessage = true;
    bIsTimerDone = false;
    SetupAndStartTimer(timeMS);
}

void CookieRP240::StopMessage()
{
    mIsShowingMessage = false;
}

void CookieRP240::ClearDisplay(uint8_t red, uint8_t green, uint8_t blue, float brightness)
{
    for (uint8_t i = 0; i < WORDS_PER_CHAR; i++)
        mFrame[i] = FormatColor(red, green, blue, brightness);
}

void CookieRP240::SetPixel(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue, float brightness)
{
    mFrame[y * 5 + x] = FormatColor(red, green, blue, brightness);
}

void CookieRP240::ShowDisplay()
{
    while (!bIsTimerDone)
        ;

    dma_channel_set_read_addr(mDMAChannel, mFrame, true);
}

bool CookieRP240::IsButtonPressed(Button button)  
{
    return GetButtonState(button) == ButtonState::JUST_PRESSED;
}

void CookieRP240::SetLED(bool on)
{
    gpio_put(LED_PIN, on);
}

void CookieRP240::InitDMA()
{
    mDMAChannel = dma_claim_unused_channel(true);
    dma_channel_config channel_config = dma_channel_get_default_config(mDMAChannel);
    channel_config_set_transfer_data_size(&channel_config, DMA_SIZE_32);
    channel_config_set_read_increment(&channel_config, true);
    channel_config_set_write_increment(&channel_config, false);
    channel_config_set_dreq(&channel_config, DREQ_PIO0_TX0);

    uint sm = 0;
       
    dma_channel_configure(
        mDMAChannel,
        &channel_config,
        &pio0_hw->txf[sm],
        NULL,
        25,
        false);
}

void CookieRP240::InitPIO()
{
    mPIO = pio0;
    mSM = pio_claim_unused_sm(mPIO, true);
    uint offset = pio_add_program(mPIO, &ws2812_program);
    ws2812_program_init(mPIO, mSM, offset, WS2812_PIN_BASE, 800000);
}

void CookieRP240::InitLED()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void CookieRP240::InitButtons()
{
    gpio_init(GPIO_BUTTON_A);
    gpio_set_dir(GPIO_BUTTON_A, GPIO_IN);
        
    gpio_init(GPIO_BUTTON_B);
    gpio_set_dir(GPIO_BUTTON_B, GPIO_IN);       
}

void CookieRP240::InitButtonsWithEvents()
{
    InitButtons();

    gpio_set_irq_enabled(GPIO_BUTTON_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(GPIO_BUTTON_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_set_irq_callback(&gpio_callback);

    irq_set_enabled(IO_IRQ_BANK0, true);
}

void CookieRP240::SetupAndStartTimer(uint32_t timeMS)
{
    add_repeating_timer_ms(timeMS, repeating_timer_callback, this, &mTimer);
}

void CookieRP240::RemapBuffer()
{
    for (uint row = 0; row < CHAR_HEIGHT; row++)
        for (uint col = 0; col < CHAR_WIDTH; col++)
            mFrame[row * CHAR_WIDTH + col] = mBuffer[(mFrameRow + col) * CHAR_HEIGHT + (CHAR_HEIGHT - 1) - row];
}

std::string const CookieRP240::ReverseMessage(const std::string &message) const 
{
    std::string reversed;
    reversed.resize(message.size());
    uint32_t i = 0;
    for (auto rit = message.rbegin(); rit!= message.rend(); ++rit)
        reversed[i++] = *rit;

    return reversed;
}

uint32_t CookieRP240::FormatColor(uint8_t red, uint8_t green, uint8_t blue, float brightness)
{
    // uint8_t invertedRed = 0, invertedGreen = 0, invertedBlue = 0;
    // for (int8_t i = 0; i < 8; i++)
    // {
    //     invertedRed |= (red & 1 << i) >> i << 7 - i; 
    //     invertedGreen |= (green & 1 << i) >> i << 7 - i;
    //     invertedBlue |= (blue & 1 << i) >> i << 7 - i;
    // }

    // return (uint32_t)invertedBlue << 16 | (uint32_t)invertedRed << 8 | invertedGreen;

    green *= brightness;
    red *= brightness;
    blue *= brightness;
    return (uint32_t)green << 24 | (uint32_t)red << 16 | blue << 8;
}

CookieRP240::ButtonState CookieRP240::GetButtonState(Button button)
{
    switch (button)
    {
        case Button::A:
            mButtonStateA = gpio_get(GPIO_BUTTON_A) ? (mButtonStateA == ButtonState::RELEASED ? ButtonState::JUST_PRESSED : ButtonState::PRESSED) : (mButtonStateA == ButtonState::PRESSED ? ButtonState::JUST_RELEASED : ButtonState::RELEASED);
            return mButtonStateA;
            break;

        case Button::B:
            mButtonStateB = gpio_get(GPIO_BUTTON_B) ? (mButtonStateB == ButtonState::RELEASED ? ButtonState::JUST_PRESSED : ButtonState::PRESSED) : (mButtonStateB == ButtonState::PRESSED ? ButtonState::JUST_RELEASED : ButtonState::RELEASED);
            return mButtonStateB;
            break;

        default:
            return ButtonState::RELEASED;
            break;
    }
}

/**** callbacks ****/

bool repeating_timer_callback(repeating_timer_t *rt)
{
    if (!((CookieRP240*)(rt->user_data))->mIsShowingMessage)
    {
        ((CookieRP240*)(rt->user_data))->bIsTimerDone = true;
        return false;
    }

    switch (((CookieRP240*)(rt->user_data))->mMessageDirection)
    {
        case CookieRP240::MessageDirection::UP:

            if (((CookieRP240*)(rt->user_data))->mDataIndex >= (((CookieRP240 *)(rt->user_data))->mMessage.size() + 1) * ((CookieRP240*)(rt->user_data))->WORDS_PER_CHAR)  
                ((CookieRP240*)(rt->user_data))->mDataIndex = 0;

            dma_channel_set_read_addr(((CookieRP240*)(rt->user_data))->mDMAChannel, ((CookieRP240*)(rt->user_data))->mBuffer.data() + ((CookieRP240*)(rt->user_data))->mDataIndex, true);
            
            ((CookieRP240*)(rt->user_data))->mDataIndex += ((CookieRP240*)(rt->user_data))->CHAR_WIDTH;

            break;

        case CookieRP240::MessageDirection::DOWN:

            if (((CookieRP240*)(rt->user_data))->mDataIndex <= 0)
                ((CookieRP240*)(rt->user_data))->mDataIndex = (((CookieRP240 *)(rt->user_data))->mMessage.size() + 1) * ((CookieRP240*)(rt->user_data))->WORDS_PER_CHAR; 

            dma_channel_set_read_addr(((CookieRP240*)(rt->user_data))->mDMAChannel, ((CookieRP240*)(rt->user_data))->mBuffer.data() + ((CookieRP240*)(rt->user_data))->mDataIndex, true);
            
            ((CookieRP240 *)(rt->user_data))->mDataIndex -= ((CookieRP240*)(rt->user_data))->CHAR_WIDTH;
            
            break;

        case CookieRP240::MessageDirection::LEFT:

            if (((CookieRP240*)(rt->user_data))->mFrameRow >= (((CookieRP240*)(rt->user_data))->mMessage.size() + 1) * ((CookieRP240*)(rt->user_data))->CHAR_WIDTH)
                ((CookieRP240*)(rt->user_data))->mFrameRow = 0;

            ((CookieRP240*)(rt->user_data))->RemapBuffer();
            dma_channel_set_read_addr(((CookieRP240*)(rt->user_data))->mDMAChannel, ((CookieRP240*)(rt->user_data))->mFrame, true);
            
            ((CookieRP240*)(rt->user_data))->mFrameRow++;
            
            break;

        case CookieRP240::MessageDirection::RIGHT:

            if (((CookieRP240*)(rt->user_data))->mFrameRow <= 0)
                ((CookieRP240*)(rt->user_data))->mFrameRow = (((CookieRP240*)(rt->user_data))->mMessage.size() + 1) * ((CookieRP240*)(rt->user_data))->CHAR_WIDTH;

            ((CookieRP240*)(rt->user_data))->RemapBuffer();
            dma_channel_set_read_addr(((CookieRP240*)(rt->user_data))->mDMAChannel, ((CookieRP240*)(rt->user_data))->mFrame, true);
            
            ((CookieRP240*)(rt->user_data))->mFrameRow--;
            
            break;
    }

    return true;
}

void gpio_callback(uint gpio, uint32_t event_mask)
{
    switch (gpio)
    {
        case GPIO_BUTTON_A:
            if (event_mask & GPIO_IRQ_EDGE_RISE)
                gInputEventQueue.Insert(InputEvent::BUTTON_A_JUST_PRESSED);
            else if (event_mask & GPIO_IRQ_EDGE_FALL)
                gInputEventQueue.Insert(InputEvent::BUTTON_A_JUST_RELEASED);
            break;

        case GPIO_BUTTON_B:
            if (event_mask & GPIO_IRQ_EDGE_RISE)
                gInputEventQueue.Insert(InputEvent::BUTTON_B_JUST_PRESSED);
            else if (event_mask & GPIO_IRQ_EDGE_FALL)
                gInputEventQueue.Insert(InputEvent::BUTTON_B_JUST_RELEASED);
            break;

        //gpio_acknowledge_irq(gpio, event_mask);  // called automatically
    }
}
