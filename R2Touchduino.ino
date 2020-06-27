/////////////////////////////////////////////////////////////////////////
//  Skelmir's R2 Touchduno
//  Written by Mimir Reynisson
//
//  Version 1.0
//
//   Feature complete, but need to move MarcuidnoCommand arrays to SD card.
//   You can disable MY_I2C_ADDRESS and SplashScreen
//
/////////////////////////////////////////////////////////////////////////

// Disable if you don't want a splash screen
#define SPLASH_SCREEN
// Enable if you don't want to be able to send i2c commands directly
//#define MY_I2C_ADDRESS            33
// Serial port used for sending Marcduino commands
#define MARCDUINO_SERIAL            Serial
// Teeces default baud rate is 2400
#define BAUD_RATE                   2400
// JEDI devices run at 9600
//#define BAUD_RATE                 9600

// QRE1113 sensor reading from A3 (disable if you don't want an optical sensor)
#define CARD_SENSOR                 3

#ifdef CARD_SENSOR
// If the value returned by QRE1113 drops below this then trigger event
#define CARD_SENSOR_THRESHOLD       500

// If defined will stop playing the movie if the card is removed
#define CARD_REMOVED_STOPS_MOVIE    1

// #define CARD_MOVIE                  "leia.avi"
// #define CARD_MOVIE                  "plans1.avi"
#define CARD_MOVIE                  "plans2.avi"
#define CARD_MUSIC_COMMAND          "$82\r"
#define CARD_MUSIC_DELAY_MS         4000
#define CARD_OPENPANEL_COMMAND      ":OP05\r"   // open servo panel 5
#define CARD_OPENPANEL_DELAY_MS     1000
#define CARD_STOPPANEL_COMMAND      ":ST05\r"   // stop servo panel 5
#define CARD_CLOSEPANEL_COMMAND     ":CL05\r"   // close servo panel 5
#define CARD_CLOSEPANEL_DELAY_MS    1000
#define CARD_STOPMUSIC_COMMAND      ":SE10\r"
#endif

/////////////////////////////////////////////////////////////////////////

#include "R2UI.h"
#include "r2-assets.h"

/////////////////////////////////////////////////////////////////////////

enum
{
    kSplashScreen = 0,
    kMainScreen = 1,
    kPanelScreen = 2,
    kHoloScreen = 3,
    kLogicScreen = 4,
    kSoundScreen = 5,
    kVideoScreen = 6,
    kKeyboardScreen = 7
};

static MarcduinoCommand PROGMEM sMainCommands[] = {
    { "Close All",      ":CL00" },
    { "Open All",       ":OP00" },
    { "Smirk Wave",     ":SE03" },
    { "Leia",           ":SE08" },
    { "Cantina",        ":SE07" },
    { "Beep Cantina",   ":SE05" },
    { "Disco",          ":SE09" },
    { "Wave",           ":SE02" },
    { "Faint",          ":SE06" },
    { "Scream",         ":SE01" },
    { "Scream Only",    ":SE15" },
    { "Open Wave",      ":SE04" },
    { "Magic Toggle",   "*MF02" },
    { "Magic Scan",     "*MF06" },
    { "Magic Alarm",    "*MF04" },
    { "Magic Life",     "*MF07" },
    { "Magic Collapse", "*MF09" },
    { "Scope Up",       "*PS02" },
    { "Scope Down",     "*PS01" },
    { "Scope Search",   "*PS03" },
    { "Scope Fast",     "*PS04" },
    { "Scope Fwd",      "*PS06" }
};

static MarcduinoCommand PROGMEM sPanelCommands[] = {
    ///////////////////////////////////////////////
    // HARD CODED DO NOT CHANGE (title can change)
    ///////////////////////////////////////////////
    { "None",           "" },
    { "Select Bottom",  "" },
    { "Select Top",     "" },
    { "Select All",     "" },

    ////////////////////////////
    // Feel free to change
    ////////////////////////////
    { "Open",           ":OPxx" },
    { "Close",          ":CLxx" },
    { "Flutter",        ":OFxx" },
    { "",               "" },
    { "",               "" },

    { "All Open",       ":CL00" },
    { "All Close",      ":OP00" },
    { "All Flutter",    ":SE03" },
    { "Scream",         ":SE51" },
    { "Wave",           ":SE52" },
    { "Smirk Wave",     ":SE53" },
    { "Open Wave",      ":SE54" },
    { "Faint",          ":SE56" },
    { "Rythmic",        ":SE57" },
    { "Ants March",     ":SE55" },
};

static MarcduinoCommand PROGMEM sHoloCommands[] = {
    ///////////////////////////////////////////////
    // HARD CODED DO NOT CHANGE (title can change)
    ///////////////////////////////////////////////
    { "None",           "" },
    { "Front",          "" },
    { "Top&Back",       "" },
    { "All",            "" },

    ////////////////////////////
    // Feel free to change
    ////////////////////////////
    { "Holo On",        "*ONxx" },
    { "Holo Off",       "*OFxx" },
    { "Holo Reset",     "*ST00" },
    { "Random Move",    "*RDxx" },
    { "Pulse Random",   "*HPS3xx" },
    { "Rainbow",        "*HPS6xx" },
    { "Wag",            "*HWxx" },
    { "Nod",            "*HNxx" },

    { "Up Left",        "*HP5xx" },
    { "Up",             "*HP2xx" },
    { "Up Right",       "*HP7xx" },
    { "Left",           "*HP4xx" },
    { "Center",         "*HP1xx" },
    { "Right",          "*HP6xx" },
    { "Down Left",      "*HP5xx" },
    { "Down",           "*HP0xx" },
    { "Down Right",     "*HP8xx" },

    { "Radar Cycle",    "*HRS4" },
    { "Radar Pulse",    "*HRS3" },
    { "Radar Red",      "*HRSR" },
    { "Radar Rainbow",  "*HRSR6" },
    { "Radar Off",      "*OF04" },
};

static MarcduinoCommand PROGMEM sLogicCommands[] = {
    ///////////////////////////////////////////////
    // HARD CODED DO NOT CHANGE (title can change)
    ///////////////////////////////////////////////
    { "RESET",          "@0T1" },
    { "All Text",       "" },
    { "Aura Besh",      "" },
    { "Latin",          "" },

    ////////////////////////////
    // Feel free to change
    ////////////////////////////
    { "Off",            "@0T0" },
    { "Normal",         "@0T1" },
    { "Scream",         "@0T5" },
    { "Short Circuit",  "@0T4" },
    { "Flash",          "@0T2" },
    { "Alarm",          "@0T3" },
    { "Leia",           "@0T6" },
    { "March",          "@0T11" },
    { "Disco",          "@0T92" },
    { "Fire",           "@0T93" },
    { "Open XCHG",      ":SE18" },
    { "Close XCHG",     ":SE19" },
    { "",               "" },
    { "",               "" },
    { "",               "" },
    { "Status",         "$998" },
    { "Shutdown",       "$999" },
};

/////////////////////////////////////////////////

static MenuItem PROGMEM sSoundCatagories[] = {
    { "Random"   },
    { "General"  },
    { "Chat",    },
    { "Happy",   },
    { "Sad",     },
    { "Whistle", },
    { "Scream",  },
    { "Leia",    },
    { "Music"    }
};

static MarcduinoCommand PROGMEM sGeneralSoundCommands[] = {
    { "Gen-1",      "$11" },
    { "Gen-2",      "$12" },
    { "Gen-3",      "$13" },
    { "Gen-4",      "$14" },
    { "Gen-5",      "$15" },
    { "Gen-6",      "$16" },
    { "Gen-7",      "$17" },
    { "Gen-8",      "$18" },
    { "Gen-9",      "$19" },
    { "Gen-10",     "$110" },
    { "Gen-11",     "$111" },
    { "Gen-12",     "$112" },
    { "Gen-13",     "$113" },
    { "Gen-14",     "$114" },
    { "Overhere",   "$115" },
    { "Proc",       "$116" },
    { "Warn-1",     "$117" },
    { "Warn-2",     "$118" },
    { "Fine-1",     "$119" },
    { "Fine-2",     "$120" },
    { "Fine-3",     "$121" },
    { "ACK-1",      "$122" }
};

static MarcduinoCommand PROGMEM sChatSoundCommands[] = {
    { "Chat-1",     "$21" },
    { "Chat-2",     "$22" },
    { "Chat-3",     "$23" },
    { "Chat-4",     "$24" },
    { "Chat-5",     "$25" },
    { "Chat-6",     "$26" },
    { "Chat-7",     "$27" },
    { "Chat-8",     "$28" },
    { "Chat-9",     "$29" },
    { "Chat-10",    "$210" },
    { "Chat-11",    "$211" },
    { "Chat-12",    "$212" },
    { "Chat-13",    "$213" },
    { "Chat-14",    "$214" },
    { "Chat-15",    "$215" },
    { "Whis-4",     "$216" },
    { "Whis-5",     "$217" },
    { "Whis-6",     "$218" },
    { "Whis-8",     "$219" },
    { "Whis-9",     "$220" },
    { "Whis-10",    "$221" },
    { "Whis-11",    "$222" }
};

static MarcduinoCommand PROGMEM sHappySoundCommands[] = {
    { "Happy-1",    "$31" },
    { "Happy-2",    "$32" },
    { "Happy-3",    "$33" },
    { "Happy-4",    "$34" },
    { "Happy-5",    "$35" },
    { "Happy-6",    "$36" },
    { "Happy-7",    "$37" },
    { "Happy-8",    "$38" },
    { "Happy-9",    "$39" },
    { "Happy-10",   "$310" },
    { "Happy-11",   "$311" },
    { "Happy-12",   "$312" },
    { "Happy-13",   "$313" },
    { "Happy-14",   "$314" },
    { "Happy-15",   "$315" },
    { "Happy-16",   "$316" },
    { "Happy-17",   "$317" },
    { "Happy-18",   "$318" },
    { "Happy-19",   "$319" },
    { "Happy-20",   "$320" },
    { "Happy-21",   "$321" },
    { "Wow",        "$322" }
};

static MarcduinoCommand PROGMEM sSadSoundCommands[] = {
    { "Sad-1",      "$41" },
    { "Sad-2",      "$42" },
    { "Sad-3",      "$43" },
    { "Sad-4",      "$44" },
    { "Sad-5",      "$45" },
    { "Sad-6",      "$46" },
    { "Sad-7",      "$47" },
    { "Sad-8",      "$48" },
    { "Razz-1",     "$49" },
    { "Razz-2",     "$410" },
    { "Razz-3",     "$411" },
    { "Razz-4",     "$412" },
    { "Razz-5",     "$413" },
    { "Razz-6",     "$414" },
    { "Razz-7",     "$415" },
    { "Razz-8",     "$416" },
    { "Razz-9",     "$417" },
    { "Razz-10",    "$418" },
    { "Razz-11",    "$419" },
    { "Razz-12",    "$420" },
    { "Razz-13",    "$421" },
    { "Razz-14",    "$422" }
};

static MarcduinoCommand PROGMEM sWhistleSoundCommands[] = {
    { "Whistle",      "$51" },
    { "Beep Whistle", "$52" },
    { "High Whistle", "$53" },
    { "1",            "$54" },
    { "2",            "$55" },
    { "3",            "$56" },
    { "4",            "$57" },
    { "5",            "$58" },
    { "6",            "$59" },
    { "7",            "$510" },
    { "8",            "$511" },
    { "9",            "$512" },
    { "10",           "$513" },
    { "11",           "$514" },
    { "12",           "$515" },
    { "13",           "$516" },
    { "14",           "$517" },
    { "15",           "$518" },
    { "16",           "$519" },
    { "17",           "$520" },
    { "18",           "$521" },
    { "19",           "$522" }
};

static MarcduinoCommand PROGMEM sScreamSoundCommands[] = {
    { "Scream",       "$61" },
    { "Error",        "$62" },
    { "Short Circuit","$63" },
    { "Alarm-1",      "$64" },
    { "Alarm-2",      "$65" },
    { "Alarm-3",      "$66" },
    { "Alarm-4",      "$67" },
    { "Alarm-5",      "$68" },
    { "Alarm-6",      "$69" },
    { "Alarm-7",      "$610" },
    { "Alarm-8",      "$611" },
    { "Alarm-9",      "$612" },
    { "Alarm-10",     "$613" },
    { "Alarm-11",     "$614" },
    { "Ooh-1",        "$615" },
    { "Ooh-2",        "$616" },
    { "Ooh-3",        "$617" },
    { "Ooh-4",        "$618" },
    { "Ooh-6",        "$619" },
    { "Ooh-7",        "$620" },
    { "Rude",         "$621" },
    { "Cylon",        "$622" }
};

static MarcduinoCommand PROGMEM sLeiaSoundCommands[] = {
    { "Message (clean)",    "$71" },
    { "Help me!",           "$72" },
    { "Message",            "$73" },
    { "R2",                 "$74" },
    { "Short Tropper",      "$75" },
    { "Walking Carpet",     "$76" },
    { "Foul Stench",        "$77" },
    { "Laser Brain",        "$78" },
    { "Mercenary",          "$79" },
    { "Nerf Herder",        "$710" },
    { "Talking About",      "$711" },
    { "Battle station",     "$712" },
    { "Do As I Tell",       "$713" },
    { "Not Over",           "$714" },
    { "Dont Know",          "$715" },
    { "Jabba Flow",         "$716" },
    { "Lack Faith",         "$717" },
    { "Baby Shark",         "$718" },
    { "Destruct",           "$719" },
    { "Yoda Clear",         "$720" },
    { "Yoda Attack",        "$721" },
    { "Yoda Nothing",       "$722" },
};

static MarcduinoCommand PROGMEM sMusicSoundCommands[] = {
    { "Cantina Beep",      "$81" },
    { "Star Wars",         "$82" },
    { "March",             "$83" },
    { "Mahna Mahna",       "$84" },
    { "Addams Family",     "$85" },
    { "Stayin' Alive",     "$86" },
    { "Birthday",          "$87" },
    { "In Love",           "$88" },
    { "Macho Man",         "$89" },
    { "Peanut",            "$810" },
    { "Uptown Funk",       "$811" },
    { "Celebrate",         "$812" },
    { "Dance",             "$813" },
    { "Dance (Long)",      "$814" },
    { "Harlem Shake",      "$815" },
    { "Voyager",           "$816" },
    { "SWCantina",         "$817" },
    { "SWMarch",           "$818" },
    { "Rick Roll",         "$819" },
    { "Star Trek",         "$820" },
    { "Girl On Fire",      "$821" },
    { "Pacman",            "$822" },
};

///////////////////////////////////////////////////////////////////////////////

static void PopupKeyboardEdit(char* msg, unsigned len);
static bool PlayMovie(const char* fileName, bool sensorTriggered = false);

///////////////////////////////////////////////////////////////////////////////

class SplashScreen : public CommandScreen
{
public:
    SplashScreen() :
        CommandScreen(kSplashScreen)
    {}

    virtual void init()
    {
        GD.cmd_loadimage(0, 0);
        GD.load("Splash.jpg");        
    }

    virtual void render()
    {
        if (elapsed() >= 10000)
        {
            switchToScreen(kMainScreen);
            blankScreen();
            return;
        }
        GD.Begin(BITMAPS);
        GD.Vertex2ii(0, 0);

        GD.Begin(RECTS);
        GD.ColorRGB(0x000000);
        GD.Vertex2ii(0, 270);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);
    }
};

///////////////////////////////////////////////////////////////////////////////

class MainScreen : public CommandScreen
{
public:
    MainScreen() :
        CommandScreen(kMainScreen, sMainCommands, SizeOfArray(sMainCommands))
    {}

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        uint8_t sel = currentSelection();
        GD.ColorRGB(0x000000);

        GD.Begin(RECTS);
        GD.Vertex2ii(0, 0);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);

        GD.Begin(BITMAPS);
        GD.BlendFunc(SRC_ALPHA, ONE);
        GD.Vertex2ii(0, 0, R2D2_IMAGE_SMALL2_HANDLE);
        GD.BlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);

        GD.Tag(kPanelScreenText);
        GD.ColorRGB((sel == kPanelScreenText) ? 0x0000FF : 0xFFFFFF);
        GD.cmd_text(85, 30, 31, OPT_CENTER, "Panel");

        GD.Tag(kHoloScreenText);
        GD.ColorRGB((sel == kHoloScreenText) ? 0x0000FF : 0xFFFFFF);
        GD.cmd_text(85, 80, 31, OPT_CENTER, "Holo");

        GD.Tag(kLogicScreenText);
        GD.ColorRGB((sel == kLogicScreenText) ? 0x0000FF : 0xFFFFFF);
        GD.cmd_text(85, 130, 31, OPT_CENTER, "Logic");

        GD.Tag(kSoundScreenText);
        GD.ColorRGB((sel == kSoundScreenText) ? 0x0000FF : 0xFFFFFF);
        GD.cmd_text(85, 180, 31, OPT_CENTER, "Sound");

        GD.Tag(kVideoScreenText);
        GD.ColorRGB((sel == kVideoScreenText) ? 0x0000FF : 0xFFFFFF);
        GD.cmd_text(85, 230, 31, OPT_CENTER, "Video");
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kPanelScreenText:
                switchToScreen(kPanelScreen);
                break;
            case kHoloScreenText:
                switchToScreen(kHoloScreen);
                break;
            case kLogicScreenText:
                switchToScreen(kLogicScreen);
                break;
            case kSoundScreenText:
                switchToScreen(kSoundScreen);
                break;
            case kVideoScreenText:
            {
                const char* movies[] = { "leia.avi", "plans1.avi", "plans2.avi" };
                PlayMovie(movies[random(0,3)]);
                break;
            }
        }
    }
protected:
    enum
    {
        kPanelScreenText = 100,
        kHoloScreenText  = 101,
        kLogicScreenText = 102,
        kSoundScreenText = 103,
        kVideoScreenText = 104
    };
};

///////////////////////////////////////////////////////////////////////////////

class PanelScreen : public CommandScreen
{
public:
    PanelScreen() :
        CommandScreen(kPanelScreen, sPanelCommands, SizeOfArray(sPanelCommands)),
        fBottomPieMask(0),
        fTopPieMask(0)
    {}

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        GD.ColorRGB(0x000000);

        GD.Begin(RECTS);
        GD.Vertex2ii(0, 0);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);

        unsigned pieCount;
        const uint16_t* pies = getBottomPies(pieCount);
        drawActivePies(90, 138, 65, 70, 140, fBottomPieMask, pies, pieCount);

        pies = getTopPies(pieCount);
        drawActivePies(90, 138, 20, 40, 40, fTopPieMask, pies, pieCount);

        GD.ColorRGB(0xffffff);
        GD.SaveContext();
        {
            GD.Begin(BITMAPS);
            GD.BlendFunc(SRC_ALPHA, ONE);
            GD.Vertex2ii(0, 25, DOME_HANDLE);
            GD.Tag(kR2Home);
            GD.Vertex2ii(0, 0, R2_HANDLE);
        }
        GD.RestoreContext();

        drawBottomPieLabel(108, 34, "5", kDomePanel5, 140, 80, 140, 35, 128, 35);
        drawBottomPieLabel(160, 70, "4", kDomePanel4, 170, 80, 170, 140, 165, 140);
        drawBottomPieLabel(164, 188, "3", kDomePanel3, 160, 165, 165, 165, 165, 172);
        drawBottomPieLabel(160, 238, "2", kDomePanel2, 146, 188, 146, 220, 155, 230);
        drawBottomPieLabel(100, 238, "1", kDomePanel1, 135, 198, 135, 220, 110, 230);
        drawBottomPieLabel(20, 238, "6", kDomePanel6, 20, 170, 20, 220, 20, 220);

        drawPieLabel(120, 138, "10", kDomePanel10, 160);
        drawPieLabel(105, 165, "9", kDomePanel9, 160);
        drawPieLabel(75, 165, "8", kDomePanel8, 160);
        drawPieLabel(60, 138, "7", kDomePanel7, 160);
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kR2Home:
                switchToScreen(kMainScreen);
                break;
            case kNone:
                fBottomPieMask = 0;
                fTopPieMask = 0;
                break;
            case kSelectBottom:
                fBottomPieMask = (kPanel6|kPanel5|kPanel4|kPanel3|kPanel2|kPanel1);
                break;
            case kSelectTop:
                fTopPieMask = (kPanel8|kPanel7|kPanel9|kPanel10);
                break;
            case kSelectAll:
                fBottomPieMask = (kPanel6|kPanel5|kPanel4|kPanel3|kPanel2|kPanel1);
                fTopPieMask = (kPanel8|kPanel7|kPanel9|kPanel10);
                break;
            case kDomePanel1:
                toggleMaskBit(fBottomPieMask, kPanel1);
                break;
            case kDomePanel2:
                toggleMaskBit(fBottomPieMask, kPanel2);
                break;
            case kDomePanel3:
                toggleMaskBit(fBottomPieMask, kPanel3);
                break;
            case kDomePanel4:
                toggleMaskBit(fBottomPieMask, kPanel4);
                break;
            case kDomePanel5:
                toggleMaskBit(fBottomPieMask, kPanel5);
                break;
            case kDomePanel6:
                toggleMaskBit(fBottomPieMask, kPanel6);
                break;
            case kDomePanel7:
                toggleMaskBit(fTopPieMask, kPanel7);
                break;
            case kDomePanel8:
                toggleMaskBit(fTopPieMask, kPanel8);
                break;
            case kDomePanel9:
                toggleMaskBit(fTopPieMask, kPanel9);
                break;
            case kDomePanel10:
                toggleMaskBit(fTopPieMask, kPanel10);
                break;
        }
    }

    virtual void sendMarcduinoCommand(const char* cmd)
    {
        int len = strlen(cmd);
        if (len >= 3 && cmd[len-1] == '\r' && cmd[len-2] == 'x' && cmd[len-3] == 'x')
        {
            // Command ends with 'xx'
            char buf[MARCDUINO_BUFFER_SIZE];
            strcpy(buf, cmd);
            if ((fBottomPieMask & kPanel1) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '1';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fBottomPieMask & kPanel2) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '2';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fBottomPieMask & kPanel3) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '3';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fBottomPieMask & kPanel4) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '4';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fBottomPieMask & kPanel5) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '5';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fBottomPieMask & kPanel6) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '6';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fTopPieMask & kPanel7) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '7';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fTopPieMask & kPanel8) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '8';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fTopPieMask & kPanel9) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '9';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fTopPieMask & kPanel10) != 0)
            {
                buf[len-3] = '1';
                buf[len-2] = '0';
                CommandScreen::sendMarcduinoCommand(buf);
            }
        }
        else
        {
            CommandScreen::sendMarcduinoCommand(cmd);
        }
    }

protected:
    uint16_t fBottomPieMask;
    uint16_t fTopPieMask;

    enum
    {
        kNone = 1,
        kSelectBottom = 2,
        kSelectTop = 3,
        kSelectAll = 4,
        kR2Home = 200,
        kDomePanel1 = 201,
        kDomePanel2 = 202,
        kDomePanel3 = 203,
        kDomePanel4 = 204,
        kDomePanel5 = 205,
        kDomePanel6 = 206,
        kDomePanel7 = 207,
        kDomePanel8 = 208,
        kDomePanel9 = 209,
        kDomePanel10= 210
    };

    enum {
        kBottomBlank1 = 1<<0,
        kPanel6       = 1<<1,
        kBottomBlank2 = 1<<2,
        kPanel5       = 1<<3,
        kBottomBlank3 = 1<<4,
        kPanel4       = 1<<5,
        kPanel3       = 1<<6,
        kPanel2       = 1<<7,
        kPanel1       = 1<<8
    };

    enum {
        kTopBlank1 = 1<<0,
        kPanel8    = 1<<1,
        kTopBlank2 = 1<<2,
        kPanel7    = 1<<3,
        kTopBlank3 = 1<<4,
        kPeriscope = 1<<5,
        kTopBlank4 = 1<<6,
        kTopHolo   = 1<<7,
        kTopBlank5 = 1<<8,
        kPanel10   = 1<<9,
        kTopBlank6 = 1<<10,
        kPanel9    = 1<<11
    };

    void drawPieLabel(int x, int y, const char* label, uint8_t tag, uint8_t ra)
    {
        GD.SaveContext();
        GD.ColorRGB(0xf7931e);
        GD.Begin(POINTS);
        GD.PointSize(ra);
        xy circle = { x*16, y*16 };
        circle.draw();

        if (getSelected() != tag)
        {
            GD.ColorRGB(0x000000);
            GD.PointSize(ra-10);
            circle.draw();
        }

        GD.Begin(RECTS);
        GD.ColorA(0xFF);
        GD.ColorRGB((getSelected() == tag) ? 0xFFFFFF : 0xf7931e);
        // GD.ColorRGB(0xf7931e);
        GD.Tag(tag);
        GD.cmd_text(x, y, 20, OPT_CENTER, label);
        GD.RestoreContext();
    }

    void drawBottomPieLabel(int x, int y, const char* label, uint8_t tag,
                            uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
    {
        drawArrowLine(x1, y1, x2, y2, x3, y3);
        drawPieLabel(x, y, label, tag, 210);
    }

    static void drawArrowLine(int x1, int y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
    {
        GD.SaveContext();
        GD.Begin(LINE_STRIP);
        GD.ColorRGB(0xf7931e);
        GD.Vertex2ii(x1, y1);
        GD.Vertex2ii(x2, y2);
        GD.Vertex2ii(x3, y3);
        GD.RestoreContext();
    }

    static const uint16_t* getBottomPies(unsigned& outSize)
    {
        static const uint16_t PROGMEM sBottomPie[] = {
            0, 42,      // Blank
            42, 90,     // Panel 6
            90, 215,    // Blank
            215, 243,   // Panel 5
            243, 261,   // Blank
            261, 287,   // Panel 4
            287, 300,   // Panel 3
            300, 312,   // Panel 2
            312, 325,   // Panel 1
            325, 360
        };
        outSize = SizeOfArray(sBottomPie)/2;
        return sBottomPie;
    }

    static const uint16_t* getTopPies(unsigned& outSize)
    {
        static const uint16_t PROGMEM sTopPie[] = {
            0, 2,       // Blank
            2, 58,      // Panel 8
            58, 62,     // Blank
            62, 118,    // Panel 7
            118, 122,   // Blank
            122, 178,   // Periscope
            178, 182,   // Blank
            182, 238,   // Top Holo
            238, 242,   // Blank
            242, 298,   // Panel 10
            298, 302,   // Blank
            302, 358,   // Panel 9
            358, 360    // Blank
        };
        outSize = SizeOfArray(sTopPie)/2;
        return sTopPie;
    }

    static void drawActivePies(int x, int y, uint16_t r0, uint16_t r1, uint16_t ra,
                                unsigned pieMask, const uint16_t* pieCoords, unsigned pieCount)
    {
        x *= 16;
        y *= 16;
        r0 *= 16;
        r1 *= 16;
        ra *= 16;
        GD.SaveContext();
        GD.ColorRGB(0x00ff00);
        GD.Begin(POINTS);
        // Paint alpha
        GD.BlendFunc(ONE, ONE_MINUS_SRC_ALPHA);
        GD.PointSize(r1);
        GD.Vertex2f(x, y);

        // Clear alpha
        GD.BlendFunc(ZERO, ONE_MINUS_SRC_ALPHA);
        GD.PointSize(r0);
        GD.Vertex2f(x, y);

        ra += (ra >> 1);
        for (unsigned i = 0; i < pieCount; i++)
        {
            if ((pieMask & 1<<i) == 0)
            {
                uint16_t th0 = DEGREES(pgm_read_word_near(&pieCoords[i*2]));
                uint16_t th1 = DEGREES(pgm_read_word_near(&pieCoords[i*2+1]));
                Poly po;
                po.begin();
                po.v(x, y);

                int rx, ry;
                GD.polar(rx, ry, ra, th1 + (((uint32_t)(uint16_t)(th0 - th1))));
                po.v(x + rx, y + ry);

                GD.polar(rx, ry, ra, th1);
                po.v(x + rx, y + ry);

                po.draw();
            }
        }
        GD.RestoreContext();
    }

    static void arc(int x, int y, uint16_t th0, uint16_t th1, uint16_t r0, uint16_t r1)
    {
        // Need to use sub-pixel coordinates
        x *= 16;
        y *= 16;
        th0 = DEGREES(th0); // Convert to Furmans
        th1 = DEGREES(th1); // Convert to Furmans
        r0 *= 16;
        r1 *= 16;
        GD.SaveContext();

        GD.Begin(POINTS);
        // Paint alpha
        GD.BlendFunc(ONE, ONE_MINUS_SRC_ALPHA);
        GD.PointSize(r1);
        GD.Vertex2f(x, y);

        // Clear alpha
        GD.BlendFunc(ZERO, ONE_MINUS_SRC_ALPHA);
        GD.PointSize(r0);
        GD.Vertex2f(x, y);

        Poly po;
        po.begin();
        po.v(x, y);
        int ra = r1 + (r1 >> 1);
        for (uint8_t d = 0; d <= 4; d++)
        {
            int rx, ry;
            GD.polar(rx, ry, ra, th1 + (((uint32_t)(uint16_t)(th0 - th1) * d) >> 2));
            po.v(x + rx, y + ry);
        }
        po.draw();
        GD.RestoreContext();
    }
};

///////////////////////////////////////////////////////////////////////////////

class HoloScreen : public CommandScreen
{
public:
    HoloScreen() :
        CommandScreen(kHoloScreen, sHoloCommands, SizeOfArray(sHoloCommands)),
        fHoloMask(0)
    {}

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        GD.ColorRGB(0x000000);

        GD.Begin(RECTS);

        GD.Vertex2ii(0, 0);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);

        GD.SaveContext();
        {
            GD.Begin(BITMAPS);
            GD.BlendFunc(SRC_ALPHA, ONE);
            GD.Vertex2ii(0, 25, DOME_HANDLE);
            GD.Tag(kR2Home);
            GD.Vertex2ii(0, 0, R2_HANDLE);
        }
        GD.RestoreContext();

        drawHolo(getRearHolo(), "2", kRearHolo, kRearHoloMask);
        drawHolo(getTopHolo(), "3", kTopHolo, kTopHoloMask);
        drawHolo(getFrontHolo(), "1", kFrontHolo, kFrontHoloMask);
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kR2Home:
                switchToScreen(kMainScreen);
                break;
            case kNoneButton:
                fHoloMask = 0;
                break;
            case kFrontButton:
                fHoloMask = (kFrontHoloMask);
                break;
            case kTopAndBackButton:
                fHoloMask = (kRearHoloMask|kTopHoloMask);
                break;
            case kAllButton:
                fHoloMask = (kFrontHoloMask|kRearHoloMask|kTopHoloMask);
                break;
            case kRearHolo:
                toggleMaskBit(fHoloMask, kRearHoloMask);
                break;
            case kTopHolo:
                toggleMaskBit(fHoloMask, kTopHoloMask);
                break;
            case kFrontHolo:
                toggleMaskBit(fHoloMask, kFrontHoloMask);
                break;
        }
    }

    virtual void sendMarcduinoCommand(const char* cmd)
    {
        int len = strlen(cmd);
        if (len >= 3 && cmd[len-1] == '\r' && cmd[len-2] == 'x' && cmd[len-3] == 'x')
        {
            // Command ends with 'xx'
            char buf[MARCDUINO_BUFFER_SIZE];
            strcpy(buf, cmd);
            if ((fHoloMask & kFrontHoloMask) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '1';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fHoloMask & kRearHoloMask) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '2';
                CommandScreen::sendMarcduinoCommand(buf);
            }
            if ((fHoloMask & kTopHoloMask) != 0)
            {
                buf[len-3] = '0';
                buf[len-2] = '3';
                CommandScreen::sendMarcduinoCommand(buf);
            }
        }
        else
        {
            CommandScreen::sendMarcduinoCommand(cmd);
        }
    }

protected:
    uint8_t fHoloMask;
    enum
    {
        kFrontHoloMask = 0x1,
        kRearHoloMask = 0x2,
        kTopHoloMask = 0x4
    };
    enum
    {
        kNoneButton = 1,
        kFrontButton = 2,
        kTopAndBackButton = 3,
        kAllButton = 4,
        kR2Home = 202,
        kRearHolo = 203,
        kTopHolo = 204,
        kFrontHolo = 205
    };
    const typedef struct Holo_t {
        uint8_t line[6];
        uint8_t circle[2];   // sub-pixel coordinates (1/16)
        uint8_t holomask[2]; // sub-pixel coordinates (1/16)
        uint8_t text[2];
    } Holo;

    static Holo* getRearHolo()
    {
        static Holo PROGMEM sRearHolo = {
            // Line
            { 78, 50, 78, 35, 90, 35 },
            // Circle
            { 1725 / 16, 550 / 16 },
            // Holo mask
            { 1225 / 16, 1050 / 16 },
            // Label
            { 107, 34 }
        };
        return &sRearHolo;
    }

    static Holo* getTopHolo()
    {
        static Holo PROGMEM sTopHolo = {
            // Line
            { 110, 110, 157, 90, 157, 50 },
            // Circle
            { 2500 / 16, 950 / 16 },
            // Holo mask
            { 1700 / 16, 1800 / 16 },
            // Label
            { 157, 59 }
        };
        return &sTopHolo;
    }

    static Holo* getFrontHolo()
    {
        static Holo PROGMEM sFrontHolo = {
            // Line
            { 125, 220, 125, 230, 50, 230 },
            // Circle
            { 625 / 16, 3650 / 16 },
            // Holo mask
            { 1950 / 16, 3300 / 16 },
            // Label
            { 40, 227 }
        };
        return &sFrontHolo;
    }

    void drawHolo(Holo* holo, const char* label, uint8_t tag, uint8_t mask)
    {
        GD.SaveContext();
        {
            GD.Begin(LINE_STRIP);
            GD.ColorRGB(0xf7931e);
            GD.Vertex2ii(pgm_read_byte_near(&holo->line[0]), pgm_read_byte_near(&holo->line[1]));
            GD.Vertex2ii(pgm_read_byte_near(&holo->line[2]), pgm_read_byte_near(&holo->line[3]));
            GD.Vertex2ii(pgm_read_byte_near(&holo->line[4]), pgm_read_byte_near(&holo->line[5]));

            GD.SaveContext();
            GD.Begin(POINTS);
            GD.PointSize(250);
            xy circle = { pgm_read_byte_near(&holo->circle[0])*16, pgm_read_byte_near(&holo->circle[1])*16 };
            circle.draw();

            if (getSelected() != tag)
            {
                GD.ColorRGB(0x000000);
                GD.PointSize(240);
                circle.draw();
            }

            if ((fHoloMask & mask) != 0)
            {
                GD.ColorA(0x80);
                GD.ColorRGB(0x00f700);
                GD.PointSize(150);
                xy rearHolo = { pgm_read_byte_near(&holo->holomask[0])*16, pgm_read_byte_near(&holo->holomask[1])*16 };
                rearHolo.draw();
            }
            GD.RestoreContext();

            GD.Begin(RECTS);
            GD.ColorA(0xFF);
            GD.ColorRGB((getSelected() == tag) ? 0xFFFFFF : 0xf7931e);
            GD.Tag(tag);
            GD.cmd_text(pgm_read_byte_near(&holo->text[0]), pgm_read_byte_near(&holo->text[1]), 28, OPT_CENTER, label);
        }
        GD.RestoreContext();
    }
};

///////////////////////////////////////////////////////////////////////////////

class LogicScreen : public CommandScreen
{
public:
    LogicScreen() :
        CommandScreen(kLogicScreen, sLogicCommands, SizeOfArray(sLogicCommands))
    {
        strcpy(fTopText, "HELLO");
        strcpy(fBotText, "   WORLD");
        strcpy(fRearText, "YOU ROCK");
    }

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        GD.ColorRGB(0x000000);

        GD.Begin(RECTS);
        GD.Vertex2ii(0, 0);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);

        GD.Begin(BITMAPS);
        GD.BlendFunc(SRC_ALPHA, ONE);
        GD.Tag(kR2Home);
        GD.Vertex2ii(0, 0, R2_HANDLE);
        GD.BlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);

        renderFrontLogic(80, 10, kTopFLDText, kTopFLD, kTopFLDTextField, fTopText);

        renderFrontLogic(80, 90, kBottomFLDText, kBottomFLD, kBottomFLDTextField, fBotText);

        renderRearLogic(15, 176, kRearLDText, kRearLD, kRearLDTextField, fRearText);
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kR2Home:
                switchToScreen(kMainScreen);
                break;
            case kReset:
                fTextMask &= ~(kTopFLDText|kBottomFLDText|kRearLDText);
                SendMarcduinoCommand("@0T1\r");
                break;
            case kAllText:
                fTextMask |= (kTopFLDText|kBottomFLDText|kRearLDText);
                SendPartialMarcduinoCommand("@1M");
                SendPartialMarcduinoCommand(fTopText);
                SendPartialMarcduinoCommand("\r");
                SendPartialMarcduinoCommand("@1M");
                SendPartialMarcduinoCommand(fBotText);
                SendPartialMarcduinoCommand("\r");
                SendPartialMarcduinoCommand("@1M");
                SendPartialMarcduinoCommand(fRearText);
                SendPartialMarcduinoCommand("\r");
                SendMarcduinoCommand("@0T100\r");
                break;
            case kAuraBesh:
                if ((fTextMask & kTopFLDText) != 0)
                    SendMarcduinoCommand("@1P61\r");
                if ((fTextMask & kBottomFLDText) != 0)
                    SendMarcduinoCommand("@1P61\r");
                if ((fTextMask & kRearLDText) != 0)
                    SendMarcduinoCommand("@1P61\r");
                break;
            case kLatin:
                if ((fTextMask & kTopFLDText) != 0)
                    SendMarcduinoCommand("@1P60\r");
                if ((fTextMask & kBottomFLDText) != 0)
                    SendMarcduinoCommand("@1P60\r");
                if ((fTextMask & kRearLDText) != 0)
                    SendMarcduinoCommand("@1P60\r");
                break;
            case kTopFLD:
                toggleMaskBit(fTextMask, kTopFLDText);
                if ((fTextMask & kTopFLDText) != 0)
                {
                    SendPartialMarcduinoCommand("@1M");
                    SendPartialMarcduinoCommand(fTopText);
                    SendPartialMarcduinoCommand("\r");
                    SendMarcduinoCommand("@1T100\r");
                }
                else
                {
                    SendMarcduinoCommand("@1T1\r");
                }
                break;
            case kBottomFLD:
                toggleMaskBit(fTextMask, kBottomFLDText);
                if ((fTextMask & kBottomFLDText) != 0)
                {
                    SendPartialMarcduinoCommand("@2M");
                    SendPartialMarcduinoCommand(fBotText);
                    SendPartialMarcduinoCommand("\r");
                    SendMarcduinoCommand("@2T100\r");
                }
                else
                {
                    SendMarcduinoCommand("@2T1\r");
                }
                break;
            case kRearLD:
                toggleMaskBit(fTextMask, kRearLDText);
                if ((fTextMask & kRearLDText) != 0)
                {
                    SendPartialMarcduinoCommand("@3M");
                    SendPartialMarcduinoCommand(fRearText);
                    SendPartialMarcduinoCommand("\r");
                    SendMarcduinoCommand("@3T100\r");
                }
                else
                {
                    SendMarcduinoCommand("@3T1\r");
                }
                break;
            case kTopFLDTextField:
                PopupKeyboardEdit(fTopText, sizeof(fTopText));
                break;
            case kBottomFLDTextField:
                PopupKeyboardEdit(fBotText, sizeof(fBotText));
                break;
            case kRearLDTextField:
                PopupKeyboardEdit(fRearText, sizeof(fRearText));
                break;
        }
    }
protected:
    enum{
        kTopFLDText = 1,
        kBottomFLDText = 2,
        kRearLDText = 4
    };
    uint8_t fTextMask;
    char fTopText[64];
    char fBotText[64];
    char fRearText[64];
    enum
    {
        kReset = 1,
        kAllText = 2,
        kAuraBesh = 3,
        kLatin = 4,
        kR2Home = 200,
        kTopFLD = 201,
        kTopFLDTextField = 202,
        kBottomFLD = 203,
        kBottomFLDTextField = 204,
        kRearLD = 205,
        kRearLDTextField = 206
    };

    void renderFrontLogic(int x, int y, uint8_t mask, uint8_t fld_id, uint8_t text_id, const char* text)
    {
        GD.Begin(RECTS);
        GD.Tag(fld_id);
        GD.ColorRGB(0x000000);
        GD.Vertex2ii(x-5, y-5);
        GD.Vertex2ii(x+48, y+35);

        GD.ColorRGB(0x00FF00);
        GD.Begin(POINTS);
        GD.PointSize(3*16);
        for (int gy = 0; gy < 5; gy++)
        {
            for (int gx = 0; gx < 8; gx++)
            {
                if ((fTextMask & mask) != 0)
                    GD.ColorRGB(GD.random(255), GD.random(255), GD.random(255));
                else
                    GD.ColorRGB(0x171812);
                xy circle = { x*16+gx*6*16, y*16+gy*6*16 };
                circle.draw();
            }
        }

        GD.Begin(RECTS);
        GD.ColorRGB(((fTextMask & mask) != 0) ? 0xf7f31e : 0xf7931e);
        GD.Vertex2ii(x-75, y+40);
        GD.Vertex2ii(x+90, y+60);

        GD.Tag(text_id);
        GD.ColorRGB(0x000000);
        GD.Vertex2ii(x-74, y+41);
        GD.Vertex2ii(x+89, y+59);

        GD.SaveContext();
        GD.ScissorXY(x-74, y+41);
        GD.ScissorSize(x+89, y+59);
        GD.ColorRGB(0xf7931e);
        GD.cmd_text(x-72, y+50, 20, OPT_CENTERY, text);
        GD.RestoreContext();
    }

    void renderRearLogic(int x, int y, uint8_t mask, uint8_t fld_id, uint8_t text_id, const char* text)
    {
        GD.Begin(RECTS);
        GD.Tag(fld_id);
        GD.ColorRGB(0x000000);
        GD.Vertex2ii(x-5, y-5);
        GD.Vertex2ii(x+135, y+29);

        GD.ColorRGB(0x00FF00);
        GD.Begin(POINTS);
        GD.PointSize(3*16);
        for (int gy = 0; gy < 4; gy++)
        {
            for (int gx = 0; gx < 25; gx++)
            {
                int offs = (gy&1)*-32;
                if ((fTextMask & mask) != 0)
                    GD.ColorRGB(GD.random(255), GD.random(255), GD.random(255));
                else
                    GD.ColorRGB(0x171812);
                xy circle = { x*16+gx*6*16+offs, y*16+gy*6*16 };
                circle.draw();
            }
        }

        GD.Begin(RECTS);
        GD.ColorRGB(((fTextMask & mask) != 0) ? 0xf7f31e : 0xf7931e);
        GD.Vertex2ii(x-10, y+34);
        GD.Vertex2ii(x+155, y+54);

        GD.Tag(text_id);
        GD.ColorRGB(0x000000);
        GD.Vertex2ii(x-9, y+35);
        GD.Vertex2ii(x+154, y+53);

        GD.SaveContext();
        GD.ScissorXY(x-9, y+35);
        GD.ScissorSize(x+154, y+53);
        GD.ColorRGB(0xf7931e);
        GD.cmd_text(x-7, y+44, 20, OPT_CENTERY, text);
        GD.RestoreContext();
    }
};

///////////////////////////////////////////////////////////////////////////////

class SoundScreen : public CommandScreen
{
public:
    SoundScreen() :
        CommandScreen(kSoundScreen, sGeneralSoundCommands, SizeOfArray(sGeneralSoundCommands)),
        fCatagories(sSoundCatagories, SizeOfArray(sSoundCatagories), kCatagoryMenu, 26),
        fVolumeControl(kVolumeControl)
    {}

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        GD.cmd_gradient(0, 0, 0x000000, 480, 480, 0x404044);

        GD.ColorRGB(0xFFFFFF);
        GD.Begin(RECTS);

        GD.Begin(BITMAPS);
        GD.Tag(kR2Home);
        GD.BlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        GD.Vertex2ii(0, 0, R2_HANDLE);

        fCatagories.draw(120, 20);

        fVolumeControl.draw(5, 60);
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kR2Home:
                switchToScreen(kMainScreen);
                break;
            default:
                if (fVolumeControl.handleSelection(selection))
                {
                    const char* cmdString[] = {
                        "$s\r",
                        "$-\r",
                        "$m\r",
                        "$+\r"
                    };
                    sendMarcduinoCommand(cmdString[fVolumeControl.getSelected()]);
                }
                if (fCatagories.handleSelection(selection))
                {
                    switch (fCatagories.getSelected())
                    {
                        case 0:
                        case 1:
                            fMenu.changeCommands(sGeneralSoundCommands, SizeOfArray(sGeneralSoundCommands));
                            break;
                        case 2:
                            fMenu.changeCommands(sChatSoundCommands, SizeOfArray(sChatSoundCommands));
                            break;
                        case 3:
                            fMenu.changeCommands(sHappySoundCommands, SizeOfArray(sHappySoundCommands));
                            break;
                        case 4:
                            fMenu.changeCommands(sSadSoundCommands, SizeOfArray(sSadSoundCommands));
                            break;
                        case 5:
                            fMenu.changeCommands(sWhistleSoundCommands, SizeOfArray(sWhistleSoundCommands));
                            break;
                        case 6:
                            fMenu.changeCommands(sScreamSoundCommands, SizeOfArray(sScreamSoundCommands));
                            break;
                        case 7:
                            fMenu.changeCommands(sLeiaSoundCommands, SizeOfArray(sLeiaSoundCommands));
                            break;
                        case 8:
                            fMenu.changeCommands(sMusicSoundCommands, SizeOfArray(sMusicSoundCommands));
                            break;
                    }
                }
        }
    }
protected:
    enum
    {
        kVolumeControl = 100,
        kR2Home = 202,
        kVolume = 203,
        kCatagoryMenu = 204
    };
    SelectionMenu fCatagories;
    VerticalVolumeControl fVolumeControl;
};

///////////////////////////////////////////////////////////////////////////////

class VideoScreen : public CommandScreen
{
public:
    VideoScreen() :
        CommandScreen(kVideoScreen),
        fReturnScreen(0)
    {}

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        int res;

    #ifdef CARD_SENSOR
        if (fSensorTriggered)
        {
            blankScreen();
            GD.Clear();
            GD.swap();
            sendMarcduinoCommand(CARD_MUSIC_COMMAND); // music
            delay(CARD_MUSIC_DELAY_MS);
            sendMarcduinoCommand(CARD_OPENPANEL_COMMAND); // open panel 5
            delay(CARD_OPENPANEL_DELAY_MS);
            sendMarcduinoCommand(CARD_STOPPANEL_COMMAND);
            restoreScreen();
        }
    #endif

        GD.cmd_mediafifo(fBase, fSize);
        GD.cmd_regwrite(REG_MEDIAFIFO_WRITE, fPtr);
        GD.finish();

        GD.cmd_playvideo(OPT_MEDIAFIFO | OPT_FULLSCREEN);
        GD.flush();
        do
        {
            res = service();
        }
        while (res > 0);
        if (res == 0)
        {
            GD.cmd_memcpy(0, 0, 4);
            GD.finish();
        }
        if (res <= 0)
        {
            // video is over
        #ifdef CARD_SENSOR
            if (fSensorTriggered)
            {
                sendMarcduinoCommand(CARD_CLOSEPANEL_COMMAND);
                delay(CARD_CLOSEPANEL_DELAY_MS);
                sendMarcduinoCommand(CARD_STOPMUSIC_COMMAND);
            }
        #endif
            switchToScreen(kMainScreen);
        #ifdef CARD_SENSOR
            if (fSensorTriggered)
            {
                blankScreen();
            }
        #endif
        }
    }

    bool playMovie(const char* fileName, ScreenID returnScreen, bool sensorTriggered)
    {
        bool found = false;
        fSensorTriggered = sensorTriggered;
        fReturnScreen = returnScreen;
        fSize = 0x40000UL;
        fBase = 0x100000UL - fSize;
        GD.__end();
        found = (fReader.openfile(fileName) != 0);
        GD.resume();
        if (found)
        {
            fPtr = 0;
            while (fPtr < (fSize - SECTOR_SIZE))
            {
                loadsector();
            }
            switchToScreen(kVideoScreen);
        }
        return found;
    }

private:
    ScreenID fReturnScreen;
    uint32_t fSize;
    uint32_t fBase;
    uint32_t fPtr;
    bool fSensorTriggered;
    Reader fReader;
    static constexpr int SECTOR_SIZE = 512;

    int service()
    {
        if (fReader.eof())
            return 0;

        uint32_t fullness = (fPtr - GD.rd32(REG_MEDIAFIFO_READ)) & (fSize - 1);
        while (fullness < (fSize - SECTOR_SIZE))
        {
            loadsector();
            fullness += SECTOR_SIZE;
            GD.get_inputs();
            GD.wr32(REG_MEDIAFIFO_WRITE, fPtr);

            bool stopPlaying = false;
        #if CARD_REMOVED_STOPS_MOVIE
            // If we are not currently playing a video then check the optical sensor
            if (fSensorTriggered && analogRead(CARD_SENSOR) > CARD_SENSOR_THRESHOLD)
                stopPlaying = true;
        #endif
            if (GD.inputs.touching != 0)
                stopPlaying = true;
            if (stopPlaying)
            {
                GD.begin();
                return -1;
            }
        }
        return 1;
    }

    void loadsector()
    {
        byte buf[SECTOR_SIZE];
        GD.__end();
        fReader.readsector(buf);
        GD.resume();
        GD.wr_n(fBase + fPtr, buf, SECTOR_SIZE);
        fPtr = (fPtr + SECTOR_SIZE) & (fSize - 1);
    }
};

///////////////////////////////////////////////////////////////////////////////

class KeyboardScreen : public CommandScreen
{
public:
    KeyboardScreen() :
        CommandScreen(kKeyboardScreen),
        fCaps(false),
        fMsg(NULL),
        fMaxLen(0),
        fReturnScreen(0)
    {
    }

    void editMessage(char* msg, unsigned len, ScreenID returnScreen)
    {
        fReturnScreen = returnScreen;
        fMsg = msg;
        fMaxLen = len;
        switchToScreen(kKeyboardScreen);
    }

    virtual void init()
    {
        LOAD_ASSETS();        
    }

    virtual void render()
    {
        GD.ColorRGB(0x000000);

        GD.Begin(RECTS);

        GD.Vertex2ii(0, 0);
        GD.Vertex2ii(480, 272);
        GD.ColorRGB(0xffffff);

        GD.SaveContext();
        {
            GD.Begin(BITMAPS);
            GD.BlendFunc(SRC_ALPHA, ONE);
            GD.Tag(kR2Home);
            GD.Vertex2ii(0, 0, R2_HANDLE);
        }
        GD.RestoreContext();

        byte key = GD.inputs.tag;
        GD.cmd_keys(70, 80 + (44*0), 320, 40, 31, OPT_FLAT | OPT_CENTER | key, (fCaps) ? "QWERTYUIOP" : "qwertyuiop");
        GD.cmd_keys(70, 80 + (44*1), 320, 40, 31, OPT_FLAT | OPT_CENTER | key, (fCaps) ? "ASDFGHJKL"  : "asdfghjkl");
        GD.cmd_keys(70, 80 + (44*2), 320, 40, 31, OPT_FLAT | OPT_CENTER | key, (fCaps) ? ",ZXCVBNM."  : ",zxcvbnm.");
        GD.Tag(kCapsKey);
        GD.cmd_button(36, 80 + (44*3), 80, 40, 28, OPT_FLAT,  (fCaps) ? "CAPS" : "caps");
        GD.Tag(' ');
        GD.cmd_button(120, 80 + (44*3), 220, 40, 28, OPT_FLAT, "space");
        GD.Tag(kDelKey);
        GD.cmd_button(344, 80 + (44*3), 80, 40, 28, OPT_FLAT, "del");

        if (fMsg != NULL)
        {
            GD.ColorRGB(0xf7931e);
            const char* text = fMsg;
            int len = strlen(text);
            if (len > 24)
                text = text + len - 20;
            GD.cmd_text(60, 20, 30, 0, text);
        }
    }

    virtual void handleSelection(uint8_t selection)
    {
        switch (selection)
        {
            case kR2Home:
                switchToScreen(fReturnScreen);
                break;
            case kCapsKey:
                fCaps = !fCaps;
                break;
            case kDelKey:
            {
                int len = strlen(fMsg);
                if (len >= 0)
                    fMsg[len-1] = '\0';
                break;
            }
            default:
                if (selection >= ' ' && selection < 0x7f)
                {
                    unsigned len = strlen(fMsg);
                    if (len < fMaxLen)
                    {
                        fMsg[len] = selection;
                        fMsg[len+1] = '\0';
                    }
                }
                break;
        }
    }

protected:
    bool fCaps;
    char* fMsg;
    unsigned fMaxLen;
    ScreenID fReturnScreen;
    enum
    {
        kR2Home = 200,
        kCapsKey = 201,
        kDelKey = 202
    };
};

static void PopupKeyboardEdit(char* msg, unsigned len)
{
    KeyboardScreen* scr = (KeyboardScreen*)CommandScreen::findScreen(kKeyboardScreen);
    if (scr != NULL)
    {
        scr->editMessage(msg, len, CommandScreen::currentID());
    }
}

static bool PlayMovie(const char* fileName, bool sensorTriggered)
{
    VideoScreen* scr = (VideoScreen*)CommandScreen::findScreen(kVideoScreen);
    return (scr != NULL) ? scr->playMovie(fileName, CommandScreen::currentID(), sensorTriggered) : false;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef SPLASH_SCREEN
SplashScreen sSplashScreen;
#endif
MainScreen sMainScreen;
PanelScreen sPanelScreen;
HoloScreen sHoloScreen;
LogicScreen sLogicScreen;
SoundScreen sSoundScreen;
VideoScreen sVideoScreen;
KeyboardScreen sKeyboardScreen;

///////////////////////////////////////////////////////////////////////////////

void setup()
{
    MARCDUINO_SERIAL.begin(BAUD_RATE);
#ifdef MY_I2C_ADDRESS
    Wire.begin(MY_I2C_ADDRESS);
#endif
    GD.begin();
}

void loop()
{
    CommandScreen::processLoop();
    // If we are not currently playing a video then check the optical sensor
#ifdef CARD_SENSOR
    if (CommandScreen::currentID() != kVideoScreen &&
        CommandScreen::currentID() != kSplashScreen &&
        analogRead(CARD_SENSOR) <= CARD_SENSOR_THRESHOLD)
    {
        PlayMovie(CARD_MOVIE, true);
    }
#endif
}
