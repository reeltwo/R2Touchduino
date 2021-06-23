#include <SPI.h>
#include <GD2.h>
#include <Wire.h>

// Specify SD chip select pins here that are not specified in GD2.h
#ifdef ESP32
#define USE_SD_PIN 15
#define USE_DEFAULT_CS 5
#else
#define USE_SD_PIN SD_PIN
#define USE_DEFAULT_CS DEFAULT_CS
#endif
#define GD_INIT() GD.begin((GD_CALIBRATE | GD_TRIM | GD_STORAGE), USE_DEFAULT_CS, USE_SD_PIN)

const typedef struct MenuItem_t {
    char label[10];
} MenuItem;

const typedef struct MarcduinoCommand_t {
    char label[15];
    char cmd[9];
} MarcduinoCommand;

#ifndef SizeOfArray
#define SizeOfArray(x) (sizeof(x)/sizeof(x[0]))
#endif
#ifndef UNUSED_ARG
 #define UNUSED_ARG(arg) (void)arg;
#endif

#define MARCDUINO_BUFFER_SIZE sizeof(((MarcduinoCommand_t*)0)->cmd)

typedef unsigned ScreenID;

void SendMarcduinoCommand(const char* cmd)
{
#ifdef MY_I2C_ADDRESS
    if (cmd[0] == '&')
    {
        uint8_t addr = 0;
        uint8_t i2c_buffer[64];
        uint8_t* i2c_end = i2c_buffer + SizeOfArray(i2c_buffer) - 1;
        uint8_t* i2c_ptr = i2c_buffer;
        *i2c_ptr = '\0';

        const uint8_t* c = (uint8_t*)cmd+1;
        while (*c >= '0' && *c <= '9')
        {
            addr = addr * 10 + *c - '0';
            c++;
        }
        if (*c != '\0')
        {
            int val = 0;
            while (*c++ == ',')
            {
                if (*c == 'x' || *c == 'X')
                {
                    // Hexidecimal
                    c++;
                    if (*c >= '0' && *c <= '9')
                        val = *c - '0';
                    else if (*c >= 'a' && *c <= 'f')
                        val = 10 + *c - 'a';
                    else if (*c >= 'A' && *c <= 'F')
                        val = 10 + *c - 'A';
                    else
                        break;
                    c++;
                    if (*c >= '0' && *c <= '9')
                        val = val * 10 + *c - '0';
                    else if (*c >= 'a' && *c <= 'f')
                        val = val * 10 + 10 + *c - 'a';
                    else if (*c >= 'A' && *c <= 'F')
                        val = val * 10 + 10 + *c - 'A';
                    else
                        break;
                    c++;
                }
                else if (*c == '\'')
                {
                    // Single character
                    val = *++c;
                }
                else if (*c >= '0' && *c <= '9')
                {
                    // Decimal
                    while (*c >= '0' && *c <= '9')
                    {
                        val = val * 10 + *c - '0';
                        c++;
                    }
                }
                else if (*c == '"')
                {
                    // Send string
                    c++;
                    while (*c != '\0' || *c != ',')
                    {
                        if (i2c_ptr+1 < i2c_end)
                            *i2c_ptr++ = *c;
                        else
                            break;
                        c++;
                    }
                }
                else
                {
                    break;
                }
                if (i2c_ptr+1 < i2c_end)
                    *i2c_ptr++ = val;
                else
                    break;
            }
            if (*i2c_buffer != '\0')
            {
                Wire.beginTransmission(addr);
                Wire.write(i2c_buffer, i2c_ptr - i2c_buffer);
                Wire.endTransmission();
            }
        }
    }
    else
#endif
    {
#ifdef ESP32
        Serial.print(cmd);
        Serial.flush();
        strcat(client_command, cmd);
        strcat(client_command, "\r");
        client.write(client_command, strlen(client_command));
        client_command[0] = '\0';
#endif
#ifdef MARCDUINO_SERIAL
        MARCDUINO_SERIAL.print(cmd);
        MARCDUINO_SERIAL.flush();
#endif
#ifdef XBEE_SOFT_SERIAL
        XBee.print(cmd);
        XBee.flush();
#endif
    }
}

void SendPartialMarcduinoCommand(const char* cmd, const char* param)
{
#ifdef ESP32
    Serial.print(cmd);
    Serial.print(param);
    Serial.println();
    strcat(client_command, cmd);
    strcat(client_command, param);
    strcat(client_command, "\r");
    client.write(client_command, strlen(client_command));
    client_command[0] = '\0';
#endif
#ifdef MARCDUINO_SERIAL
    MARCDUINO_SERIAL.print(cmd);
    MARCDUINO_SERIAL.print(param);
#endif
#ifdef XBEE_SOFT_SERIAL
    XBee.print(cmd);
    XBee.print(param);
#endif
}

class CommandMenu
{
public:
    CommandMenu(MarcduinoCommand* cmds, uint8_t siz) :
        fCommands(cmds),
        fSize(siz),
        fValue(0)
    {
    }

    MarcduinoCommand* getMarcduino(int index)
    {
        return &(fCommands[index % fSize]);
    }
    const char* getButtonLabel(int index, char* buf, size_t maxSize)
    {
        strncpy_P(buf, getMarcduino(index)->label, maxSize);
        if (strlen(buf) > 9)
        {
            char *ix = buf;
            int n = 0;
            while ((ix = strchr(ix, ' ')) != NULL)
            {
                *ix++ = '\n';
                n++;
            }
        }
        return buf;
    }
    const char* getCommand(int index, char* buf, size_t maxSize)
    {
        strncpy_P(buf, getMarcduino(index)->cmd, maxSize);
        size_t len = strlen(buf);
        if (len > 0)
        {
            maxSize = (len + 1 < maxSize) ? len : maxSize-2;
            buf[maxSize] = '\r';
            buf[maxSize+1] = '\0';
        }
        return buf;
    }

    uint8_t getSize()
    {
        return fSize;
    }

    void draw(int x, int y)
    {
        if (fSize == 0)
            return;
        if ((GD.inputs.track_tag & 0xff) == kTAG_SLIDER)
            fValue = GD.inputs.track_val;

        GD.SaveContext();

        // Setup slider and track
        GD.Tag(kTAG_SLIDER);
        GD.cmd_slider(455, 20, 15, 220, 0, fValue, 0xFFFF);
        GD.cmd_track(455, 20, 15, 220, kTAG_SLIDER);

        // Draw menu buttons
        char buf[32];
        int lineSize = (0xFFFF / (fSize / 3)) * 2;
        for (int gridy = 0; gridy < 5; gridy++)
        {
            for (int gridx = 0; gridx < 3; gridx++)
            {
                int tag = (gridy + fValue / lineSize) * 3 + gridx;
                if (tag >= fSize)
                    break;
                getButtonLabel(tag, buf, sizeof(buf));
                if (buf[0] == '\0')
                    continue;
                GD.Tag(tag+1);
                GD.ColorRGB(0xf7931e);
                GD.cmd_fgcolor((GD.inputs.tag == tag+1) ? 0x75715e : 0x171812);
                GD.cmd_button(x + (90*gridx), y + (50*gridy), 80, 40, 16, 0, buf);
            }
        }
        GD.RestoreContext();
    }

    inline uint8_t getSelected()
    {
        return fSelected;
    }

    bool handleSelection(uint8_t selection)
    {
        if (selection > 0 && selection < fSize)
        {
            fSelected = selection-1;
            return true;
        }
        return false;
    }

    void changeCommands(MarcduinoCommand* newCommands, uint8_t newSize)
    {
        fCommands = newCommands;
        fSize = newSize;
        fValue = 0;
    }

private:
    enum
    {
        kTAG_SLIDER = 254
    };

    MarcduinoCommand *fCommands;
    uint8_t fSize;
    uint16_t fValue;
    uint8_t fSelected;
};

class SelectionMenu
{
public:
    SelectionMenu(MenuItem* items, uint8_t siz, unsigned baseTag, uint8_t fontSize) :
        fItems(items),
        fSize(siz),
        fSelected(0),
        fBaseTag(baseTag),
        fFontSize(fontSize)
    {
    }

    MenuItem* getItem(int index)
    {
        return &(fItems[index % fSize]);
    }
    const char* getTitle(int index, char* buf, size_t maxSize)
    {
        strncpy_P(buf, getItem(index)->label, maxSize);
        if (strlen(buf) > 9)
        {
            char *ix = buf;
            int n = 0;
            while ((ix = strchr(ix, ' ')) != NULL)
            {
                *ix++ = '\n';
                n++;
            }
        }
        return buf;
    }

    inline uint8_t getSize()
    {
        return fSize;
    }

    inline uint8_t getSelected()
    {
        return fSelected;
    }

    void draw(int x, int y)
    {
        char title[10];
        uint8_t sel = fSelected;
        for (uint8_t i = 0; i < fSize; i++)
        {
            GD.Tag(fBaseTag+i);
            GD.SaveContext();
            GD.ScissorSize(105, fFontSize-2);
            GD.ScissorXY(x-50, (y-12)+(fFontSize*i));
            if (sel == i)
            {
                GD.cmd_gradient(35, 20+(fFontSize*i), 0x171812, 480, 272, 0xf7931e);
                GD.ColorRGB(0x0000FF);
            }
            else
            {
                GD.ColorRGB(0xFFFFFF);
            }
            GD.cmd_text(x, y+(fFontSize*i), fFontSize, OPT_CENTER, getTitle(i, title, sizeof(title)));
            GD.RestoreContext();
        }
    }

    bool handleSelection(uint8_t selection)
    {
        if (selection >= fBaseTag && selection < fBaseTag+fSize)
        {
            fSelected = selection - fBaseTag;
            return true;
        }
        return false;
    }

private:
    MenuItem *fItems;
    uint8_t fSize;
    uint8_t fSelected;
    uint8_t fBaseTag;
    uint8_t fFontSize;
};

class VerticalVolumeControl
{
public:
    VerticalVolumeControl(uint8_t baseTag) :
        fBaseTag(baseTag)
    {}

    void draw(int x, int y)
    {
        const char* title[] = { "OFF", "-", "Mid", "+" };
        for (unsigned i = 0; i < SizeOfArray(title); i++)
        {
            GD.Tag(fBaseTag + i);
            GD.ColorRGB(0xf7931e);
            GD.cmd_fgcolor((GD.inputs.tag == fBaseTag + i) ? 0x75715e : 0x171812);
            GD.cmd_button(x, y + (50*i), 60, 30, 30, OPT_FLAT, title[i]);
        }
    }

    inline uint8_t getSelected()
    {
        return fSelected;
    }

    bool handleSelection(uint8_t selection)
    {
        if (selection >= fBaseTag && selection <= fBaseTag+kLAST)
        {
            fSelected = selection - fBaseTag;
            return true;
        }
        return false;
    }

    enum
    {
        kVolumeOff = 0,
        kVolumeDown = 1,
        kVolumeMid = 2,
        kVolumeUp = 3,
    };

private:
    uint8_t fBaseTag;
    uint8_t fSelected;

    enum
    {
        kLAST = kVolumeUp
    };
};


class CommandScreen
{
public:
    CommandScreen(ScreenID id) :
        fID(id),
        fMenu(NULL, 0),
        fLastTag(0),
        fNext(NULL)
    {
        append();
    }

    CommandScreen(ScreenID id, MarcduinoCommand* cmds, uint8_t siz) :
        fID(id),
        fMenu(cmds, siz),
        fLastTag(0),
        fNext(NULL)
    {
        append();
    }

    static void processLoop(uint32_t screenBlankDelay = 15000)
    {
        static uint32_t lastMillis;
        static CommandScreen* lastScreen;
        GD.Clear();
        GD.get_inputs();
        CommandScreen* current = currentScreen();
        if (lastScreen != current)
        {
            // Screen was changed
            if (current != NULL)
            {
                current->fLastTag = 0;
                current->init();
            }
            screenStartMillis() = lastMillis = millis();
        }
        if ((screenState() & 0x1) == 0 && screenBlankDelay > 0 && lastMillis + screenBlankDelay < millis())
        {
            blankScreen();
        }
        if (current != NULL && (screenState() & 0x1) == 0)
        {
            if (GD.inputs.touching)
                lastMillis = millis();

            GD.SaveContext();
            current->render();
            GD.RestoreContext();

            current->fMenu.draw(180, 10);
        }
        else
        {
            static bool sScreenTouched;
            // Blank screen
            GD.Begin(RECTS);
            GD.ColorRGB(0x000000);
            GD.Vertex2ii(0, 0);
            GD.Vertex2ii(480, 272);
            if (GD.inputs.touching != 0)
            {
                if (!sScreenTouched)
                    sScreenTouched = true;
            }
            else if (sScreenTouched)
            {
                sScreenTouched = false;
                lastMillis = millis();
                restoreScreen();
            }
        }
        GD.swap();
        if (GD.inputs.tag == 0 && current->fLastTag != 0)
        {
            current->handleSelection(current->fLastTag);
            if (current->fLastTag > 0)
            {
                if (current->fMenu.handleSelection(current->fLastTag))
                {
                    char buf[MARCDUINO_BUFFER_SIZE];
                    const char* cmd = current->fMenu.getCommand(
                        current->fMenu.getSelected(), buf, sizeof(buf));
                    if (*cmd != '\0')
                        current->sendMarcduinoCommand(cmd);
                }
            }
            current->fLastTag = 0;
        }
        else
        {
            if (current->fLastTag != 0 && GD.inputs.touching)
            {
                // ignore roaming fingers
            }
            else
            {
                current->fLastTag = GD.inputs.tag;
            }
        }
        lastScreen = current;
    }

    inline uint8_t getSelected()
    {
        return fLastTag;
    }

    static CommandScreen* current()
    {
        return currentScreen();
    }
    static ScreenID currentID()
    {
        CommandScreen* scr = current();
        return (scr != NULL) ? scr->fID : 0;
    }

    static CommandScreen* findScreen(ScreenID id)
    {
        for (CommandScreen* scr = *head(); scr != NULL; scr = scr->fNext)
        {
            if (scr->fID == id)
                return scr;
        }
        return NULL;
    }

    static void switchToScreen(ScreenID id)
    {
        CommandScreen* scr = findScreen(id);
        if (scr != NULL)
        {
            currentScreen() = scr;
            restoreScreen();
        }
    }

    static void blankScreen()
    {
        if ((screenState() & 0x1) == 0)
        {
            screenState() |= 0x1;
            GD.wr(REG_PWM_DUTY, 0);
            GD.wr(REG_GPIO, 0);
            GD.wr(REG_PCLK, 0);
        }
    }

    static void restoreScreen()
    {
        if ((screenState() & 0x1) == 0x1)
        {
            screenState() &= ~0x1;
            CommandScreen* current = currentScreen();
            GD.wr(REG_GPIO, 0x80);
            GD.wr(REG_PCLK, 5);
            if (current != NULL)
                current->init();
            GD.wr(REG_PWM_DUTY, 128);
        }
    }
    virtual void init() {}
    virtual void handleSelection(uint8_t selection) { UNUSED_ARG(selection) }
    virtual void render() = 0;
    virtual void sendMarcduinoCommand(const char* cmd)
    {
//        if (*cmd == '@')
//            cmd++;
        // Check for empty command and then do nothing
        if (*cmd != '\0')
        {
            SendMarcduinoCommand(cmd);
        }
    }
    static uint32_t elapsed()
    {
        return millis() - screenStartMillis();
    }
protected:
    static void toggleMaskBit(uint8_t &mask, uint8_t bit)
    {
        if ((mask & bit) == 0)
            mask |= bit;
        else
            mask &= ~bit;
    }
    static void toggleMaskBit(uint16_t &mask, uint16_t bit)
    {
        if ((mask & bit) == 0)
            mask |= bit;
        else
            mask &= ~bit;
    }
    void clearSelection()
    {
        fLastTag = 0;
    }
    inline uint8_t currentSelection()
    {
        return GD.inputs.tag;
    }
    inline bool hasMenu()
    {
        return (fMenu.getSize() != 0);
    }
    ScreenID fID;
    CommandMenu fMenu;
    uint8_t fLastTag;

private:
    CommandScreen* fNext;

    void append()
    {
        if (*head() == NULL)
            *head() = this;
        if (*tail() != NULL)
            (*tail())->fNext = this;
        *tail() = this;
        if (currentScreen() == NULL)
            currentScreen() = this;
    }

    static CommandScreen** head()
    {
        static CommandScreen* sHead;
        return &sHead;
    }

    static CommandScreen** tail()
    {
        static CommandScreen* sTail;
        return &sTail;
    }

    static CommandScreen*& currentScreen()
    {
        static CommandScreen* sCurrentScreen;
        return sCurrentScreen;
    }

    static uint8_t& screenState()
    {
        static uint8_t sState;
        return sState;
    }

    static uint32_t& screenStartMillis()
    {
        static uint32_t sMillis;
        return sMillis;
    }
};
