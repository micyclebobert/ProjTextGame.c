/**
Made for Windows CodeBlocks with GCC GNU Compiler
Tested on Windows 10
Make sure that a.wav is in the same folder as the exe
**/


#pragma execution_character_set( "utf-8" ) //tis depricated but whatever it works

#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<mmsystem.h> //On CodeBlocks go to Settings -> Compiler... -> Linker Settings -> Other Linker Options -> Add "-lwinmm" -> Ok





#define Black            0
#define Blue             1
#define Green            2
#define Aqua             3
#define Red              4
#define Purple           5
#define Yellow           6
#define White            7
#define Gray             8
#define Light_Blue       9
#define Light_Green     10
#define Light_Aqua      11
#define Light_Red       12
#define Light_Purple    13
#define Light_Yellow    14
#define Bright_White    15

#define Default_Color    7


#define Default_Health 100
#define Name_Placeholder '@'

/*
HANDLE is a custom data type from Windows.h */
HANDLE outputHandle; //Console Handler
HANDLE  inputHandle; //Console Handler

/*
CONSOLE_CURSOR_INFO              = { size, boolean }  //Custom data type from Windows.h */
CONSOLE_CURSOR_INFO invisiCursor = {    5,   FALSE }; //Prepares data about an invisible cursor
CONSOLE_CURSOR_INFO   visiCursor = {    5,    TRUE }; //Prepares data about a    visible cursor

int maxHealth;
int health;
int healthBeforeAmulet;
int hasAmulet;
int tempMode;
int defMode=0;
int autoCaps=2;
int highlightName=1;
int width;
char name[1000];






void InitProperties();
void InitNewGame();
void SetColor(int color);
void ShowConsoleCursor();
void HideConsoleCursor();
void ClearInputBuffer();
void ClearOutput();
char InputtedChar();
void Wait(int ms);
void PlayerHealth(int h) ;
void EnemyHealth(int h) ;
void GameOver(int state);
void GradualPrint(char str[]);
void CenteredPrint(char str[]);
void CenteredPrint2(char str[]);
void SayMyName();
void IllegalInput();
void BlinkingProceed();
void FadeOut();
void PlayMusic(char addr[]);
void mainMenu();
void settingsMenu();
void aboutMenu();
void helpMenu();
void gameStart();
void startScene();
void cavernEntrance();
void cavernInside();
void fightDraugr();
void runFromDraugr();
void defeatDraugr(int state);
void furtherCavern(int state);
void amuletRoom();
void takeAmulet();
void leaveAmulet();
void outsideTemple();
void wrongAnswer();
void correctAnswer();
void trollFight(int th, int state);
void midFightTelekinesis(int th);
void endFight(int th);
void defeatTroll();
void insideTemple();
void contentGod();
void angryGod();
void noAmulet();
char InputtedChar();










///Technical Functions
void InitProperties()
{
    SetConsoleOutputCP(65001); //set console text to utf-8 mode (basically utf-8 has more characters than standard char's 7 bits)


    system("mode 8000,8000");
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED); //Fullscreen the console


    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE); //If you imagine the console's text display process as a variable then outputHandle is the pointer to that variable
    inputHandle  = GetStdHandle(STD_INPUT_HANDLE);  //If you imagine the console's text  input  process as a variable then  inputHandle is the pointer to that variable


    HideConsoleCursor();


    SetColor(Default_Color); //sets text color to normal


    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(outputHandle, &screen);
    width = screen.srWindow.Right; //we don't need to -Left coz we maximized the screen
}

void InitNewGame()
{
    ClearOutput();
    maxHealth = Default_Health, health = Default_Health;
    hasAmulet = 0, tempMode = 0;
    gameStart();
}


void SetColor(int color)
{
    SetConsoleTextAttribute(outputHandle, color);
}

void ShowConsoleCursor()
{
    SetConsoleCursorInfo(outputHandle, &  visiCursor); //Replaces the current cursor values to the data we prepared
}

void HideConsoleCursor()
{
    SetConsoleCursorInfo(outputHandle, &invisiCursor); //Replaces the current cursor values to the data we prepared
}


void ClearInputBuffer()
{
    FlushConsoleInputBuffer(inputHandle);
    fflush(stdin);
}

void ClearOutput()
{
    system("cls");
    SetColor(Default_Color);
    tempMode=0;
}


char InputtedChar()
{
    ClearInputBuffer();
    int c=getch();
    if(c==0||c==224)
    {
        getch();
        c=0;
    }
    ClearInputBuffer();
    return toupper(c);
}


void Wait(int ms)
{
    if(ms>100) Sleep(ms); //Sleep is not too accurate, but its fine to be inaccurate in big waits (10000ms vs 10100ms is meh)
    else
    {
        clock_t x=ms+clock(); //But 5ms vs 105ms is a huge difference
        while (x>clock());
    }
}


void PlayerHealth(int h) // █ ▓ ▒ ░ ■ ▬ -
{
    printf("\t\t\tPlayer Health: \t");
    int x=h/10;
    SetColor(Light_Green);
    for(int i=0; i<x; i++) printf("█ ");
    int y=h%10;
    if(y!=0)
    {
        x++;
        if(y>6) printf("▓ "); //7 8 9
        else if(y>3) printf("▒ "); //4 5 6
        else         printf("░ "); //1 2 3
    }
    for(int i=x; i<10; i++) printf("▬ ");
    SetColor(Default_Color);
    printf("\n\n");
}

void EnemyHealth(int h) // █ ▓ ▒ ░ ■ ▬ - //Separate troll and draugr HP display?
{
    printf("\t\t\tEnemy Health:  \t");
    SetColor(Red);
    int x=h/10,y=h%10;
    for(int i=0; i<x; i++) printf("█ ");
    if(y!=0)
    {
        x++;
        if(y>6) printf("▓ "); //7 8 9
        else if(y>3) printf("▒ "); //4 5 6
        else         printf("░ "); //1 2 3
    }
    for(int i=x; i<10; i++) printf("▬ ");
    SetColor(Default_Color);
    printf("\n\n");
}

void GameOver(int state)
{
    if(state) SetColor(Red);
    else      SetColor(Aqua);
    GradualPrint("{Game Over}\n\n\n");
    SetColor(Default_Color);
    /*mainMenu*/ printf("\t\tReturn to (M)ain Menu\n");
    if(state==2) printf("\t\tReturn to Last (C)heckpoint\n");
    printf("(E)xit\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'M':
            return mainMenu();
            break;
        case 'E':
            return;
            break;
        case 'C':
            if(state==2)
            {
                ClearOutput();
                health=healthBeforeAmulet;
                return amuletRoom();
            }
            else
            {
                IllegalInput();
            }
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void GradualPrint(char str[])
{
    ClearInputBuffer();
    int len=strlen(str), i=0;
    if(tempMode==0&& defMode==0)
    {
        for(; i<len; i++)
        {
            if(kbhit())
            {
                int c=getch();
                if(c==0||c==224) getch();
                tempMode=1;
                break;
            }
            if(str[i]==Name_Placeholder) SayMyName();
            else
            {
                printf("%c",str[i]);
                Wait(35);
            }
        }
    }
    if(tempMode==1 || defMode==1)
    {
        for(; i<len; i++)
        {
            if(str[i]==Name_Placeholder) SayMyName();
            else
            {
                printf("%c",str[i]);
                Wait(5);
            }
        }
    }
    if(tempMode==2)
    {
        for(; i<len; i++)
        {
            if(str[i]==Name_Placeholder) SayMyName();
            else
            {
                printf("%c",str[i]);
                Wait(1);
            }
        }
    }
}

void CenteredPrint(char str[])
{
    int pad=(width-strlen(str))/2;
    for(int i=0; i<pad; i++) printf(" ");
    printf("%s\n",str);
}

void CenteredPrint2(char str[])
{
    int pad=(width-77)/2;
    for(int i=0; i<pad; i++) printf(" ");
    printf("%s\n",str);
}

void SayMyName()
{
    if(highlightName==1) SetColor(Aqua);
    GradualPrint(name);
    if(highlightName==1) SetColor(Default_Color);
}

void IllegalInput()
{
    printf("\rIllegal Input!!");
}

void BlinkingProceed()
{
    printf("\n");
    SetColor(Gray);
    printf("\rPress any key to continue");
    Wait(75);
    SetColor(Default_Color);
    int k=500;
    ClearInputBuffer();
    while(!kbhit())
    {
        printf("\rPress any key to continue");
        Wait(k+100);
        printf("\r                         ");
        Wait(k);
    }

    int c=getch();
    if(c==0||c==224) getch();
    tempMode=0;
}

void FadeOut()
{
    system("color 8");
    Wait(50);
    ClearOutput();
    Wait(700);
    system("color 7");
}


void PlayMusic(char addr[])
{
    PlaySound(addr, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}




///Story Functions
/*
void templateFunction()
{
    ClearOutput();
    GradualPrint("Opening line with options for doing\n\n"
           "\t(S)omething\n"
           "\tSomething (E)lse\n"
           "\tS(O)mething else entirely\n\n");
    switch(InputtedChar())
    {
        case 'S': //Option: Something
            return somethingFunction();
            break;
        case 'E': //Option: Something else
            return somethingElseFunction();
            break;
        case 'O': //Option: Something else entirely
            return somethingElseEntirelyFunction();
            break;
        default: //Illegal input
            return templateFunction(); //restart function
            break;
    }
}
*/


void mainMenu()
{
    ClearOutput();
    Wait(1000);
    CenteredPrint("\n");
    CenteredPrint("The Lost Library: A Cavernous Challenge\n\n\n");
    CenteredPrint2("┌───────────────────────────────────────────────────────────────────────────┐");
    CenteredPrint2("│                                                                           │");
    CenteredPrint2("│ ┌─────────────────╥─────────────────╥─────────────────╥─────────────────┐ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │▄▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ ╞═════════════════╬═════════════════╬═════════════════╬═════════════════╡ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │█▐▌█▐║▄▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ ╞═════════════════╬═════════════════╬═════════════════╬═════════════════╡ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║▄▐▌█▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ ╞═════════════════╬═════════════════╬═════════════════╬═════════════════╡ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║▄▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ ╞═════════════════╬═════════════════╬═════════════════╬═════════════════╡ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║▄▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ ╞═════════════════╬═════════════════╬═════════════════╬═════════════════╡ │");
    CenteredPrint2("│ │                 ║                 ║                 ║                 │ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║▄▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌▄▐║█▐▌█▐║█▐▌█▐║▄▐▌█▐│ │");
    CenteredPrint2("│ │█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐║█▐▌█▐│ │");
    CenteredPrint2("│ └─────────────────╨─────────────────╨─────────────────╨─────────────────┘ │");
    CenteredPrint2("│                                                                           │");
    CenteredPrint2("└───────────────────────────────────────────────────────────────────────────┘");
    CenteredPrint("\n\n");
    CenteredPrint("(P)lay Game\n");
    CenteredPrint("(E)xit\n");
    CenteredPrint("(A)bout\n");
    CenteredPrint("(H)elp\n");
    CenteredPrint("(S)ettings\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'P':
            return InitNewGame();
            break;
        case 'E':
            return;
            break;
        case 'A':
            return aboutMenu();
            break;
        case 'H':
            return helpMenu();
            break;
        case 'S':
            return settingsMenu();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void settingsMenu()
{
    ClearOutput();
    printf("\tFast (C)utscenes: ");
    if(defMode==0)       printf("OFF\n");
    else                 printf("ON\n");
    printf("\t(A)uto Capitalize Name: ");
         if(autoCaps==0) printf("OFF\n");
    else if(autoCaps==1) printf("First\n");
    else                 printf("All\n");
    printf("\t(H)ighlight Names: ");
    if(highlightName==0) printf("OFF\n");
    else                 printf("ON\n");
    printf("\n\n\n\t\tReturn to (M)ain Menu\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'C':
            defMode=!defMode;
            return settingsMenu();
            break;
        case 'A':
            autoCaps++;
            if(autoCaps==3) autoCaps=0;
            return settingsMenu();
            break;
        case 'H':
            highlightName=!highlightName;
            return settingsMenu();
            break;
        case 'M':
            return mainMenu();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void aboutMenu()
{
    ClearOutput();
    printf("A text based adventure game created by Group #1\n\n\n");
    printf("\t             Name                 ID    \n");
    printf("\t      Aoutul Nabi Purna       2412826042\n");
    printf("\t         Shishir Dhar         2412876042\n");
    printf("\t     Hasibur Rahman Hemal     2413800042\n");
    printf("\t        Zareen Tasnim         2412912042\n");
    printf("\nPress any key to return to the Main Menu\n");
    InputtedChar();
    return mainMenu();
}

void helpMenu()
{
    ClearOutput();
    printf("Press the button corresponding to the single letter encapsulated in brackets to navigate that option.\n\n");
    printf("First letter of every word in the names are automatically capitalized.\n\n");
    printf("Every letter in the answer of the riddle is automatically capitalized.\n");
    printf("BackSpace is disabled for the riddle so answers must be given without mistakes.\n\n");
    printf("Press any key during a scene to fast forward it.\n");
    printf("Some quality of life features can be adjusted in the settings.\n\n\n\n");
    printf("Press any key to return to the Main Menu");
    InputtedChar();
    return mainMenu();
}


void gameStart()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("Insert Player Name: ");
    ClearInputBuffer();
    ShowConsoleCursor();
    fgets(name,1000,stdin);
    HideConsoleCursor();
    if(autoCaps>=1) name[0]    =     toupper(name[0]);
    if(autoCaps==2)
    {
        for(int i=0; name[i]!=0; i++)
        {
            if(name[i]=='\n') name[i]=0;
            if(i!=0&&name[i+1]==' ') toupper(name[i]);
        }
    }
    tempMode=0;
    GradualPrint("Welcome, @, to The Lost Library: A Cavernous Challenge\n");
    Wait(1000);
    FadeOut();
    return startScene();
}

void startScene()
{
    ClearOutput();
    GradualPrint("With a pounding heart, you grip the map, the worn parchment crackling in your hands.\n");
    GradualPrint("The librarian, Professor Chronos, has entrusted you with this perilous mission.\n");
    Wait(300);
    GradualPrint("\"This library,\" he rasped, his voice laden with years, ");
    Wait(350);
    GradualPrint("\"holds knowledge that can reshape the very fabric of time.\"\n");
    Wait(500);
    GradualPrint("\"But be warned, young researcher, the journey will be fraught with danger\", he adds.\n\n\n");
    BlinkingProceed();
    FadeOut();
    return cavernEntrance();
}

void cavernEntrance()
{
    ClearOutput();
    GradualPrint("You descend into the cavern entrance, the sun's light fading behind you.\n");
    GradualPrint("The air grows damp and cool, the silence broken only by the dripping of water.\n");
    GradualPrint("Your torch flickers, casting eerie shadows on the cavern walls.\n");
    GradualPrint("You explore deeper and find your path blocked by a fallen rock.\n");
    GradualPrint("What do you do?\n\n");
    tempMode=2;
    GradualPrint("\tUse (E)xplosive Fireball Spell\n");
    GradualPrint("\tReturn to (M)ain Menu\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'E':
            return cavernInside();
            break;
        case 'M':
            return mainMenu();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void cavernInside()
{
    ClearOutput();
    GradualPrint("The rock crumbles into pieces revealing the path.\n");
    GradualPrint("The cave walls have various carvings of ancient men performing rituals and sacrifices to what seems to the Time Lord.\n");
    GradualPrint("You remember Professor Chronos mentioning him as an ancient God worshiped by mortals in the 4th Era.\n");
    GradualPrint("As you walk through, you enter a chamber full of coffins and burial urns.\n");
    GradualPrint("As you move one step, you hear a *click\n");
    tempMode=2;
    GradualPrint("Suddenly, a coffin bursts open and a Draugr Death Overlord steps out.\n");
    GradualPrint("It jumps at you and with a slash of its sword lands a hit on you.\n\n");
    health-=24;
    PlayerHealth(health);
    GradualPrint("\t(F)ight Back, use Chain Lightning\n");
    GradualPrint("\t(R)un Away\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'F':
            return fightDraugr();
            break;
        case 'R':
            return runFromDraugr();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void fightDraugr()
{
    ClearOutput();
    PlayerHealth(health);
    EnemyHealth(76);
    tempMode=2;
    GradualPrint("Draugr hits back and misses\n\n");
    GradualPrint("\tUse Explosive (F)ireball spell\n\n");
    if(InputtedChar() == 'F') return defeatDraugr(0);
    GradualPrint("You miss and the Draugr chops manages to land a fatal blow on you.\n\n");
    return GameOver(1);
}

void runFromDraugr()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ tries to run but Draugr lands another fatal blow chopping @'s arm off\n");
    health-=52;
    GradualPrint("@'s arm falls to the ground. The Draugr is about to land another fatal blow\n\n");
    PlayerHealth(health);
    EnemyHealth(76);
    tempMode=2;
    GradualPrint("\tUse Explosive (F)ireball spell\n\n");
    if(InputtedChar() == 'F') return defeatDraugr(1);
    else GradualPrint("Draugr chops @'s head off. \n\n");
    tempMode=0;
    return GameOver(1);
}

void defeatDraugr(int state)
{
    ClearOutput();
    tempMode=2;
    GradualPrint("Draugr explodes into pieces.\n");
    PlayerHealth(health);
    EnemyHealth(0);
    if(state)
    {
        GradualPrint("@ crawls and searches a chest nearby.\n");
        GradualPrint("@ luckily finds a health regeneration potion and drinks it.\n");
        health+=60;
        PlayerHealth(health);
    }
    BlinkingProceed();
    return furtherCavern(state);
}

void furtherCavern(int state)
{
    ClearOutput();
    GradualPrint("@ continues on his path.\n");
    GradualPrint("\"The cave is ridden with traps and monsters! I should be more cautious...\"\n");
    if(!state) GradualPrint("@ finds a treasure chest\n");
    GradualPrint("@ rummages through the chest and finds:\n");
    GradualPrint("\tHealth Regeneration Potions 2X {Reset Health to 100}\n");
    GradualPrint("\tSpell Tome on Explosive Chain Lightning {Explosive Chain Lightning Unlocked}\n");
    GradualPrint("\tGold Coins 25X \n");
    GradualPrint("\tRuby 2X, Garnet 1X\n");
    GradualPrint("\tAmulet of Enhanced Spell Casting {10% damage buff}\n\n");
    BlinkingProceed();
    ClearOutput();
    GradualPrint("@ continues on to a deeper chamber.\n");
    healthBeforeAmulet=health;
    return amuletRoom();
}

void amuletRoom()
{
    GradualPrint("The room is full of old shelves full of books and scrolls.\n");
    GradualPrint("There is a study table in the corner of the room with a skeleton sitting in front of it.\n");
    GradualPrint("The skeleton has a gold neck amulet with a red ruby on it.\n");
    GradualPrint("\"Stealing from the dead cannot be good, the amulet might be cursed.\n");
    GradualPrint("Should I....\"\n\n");
    tempMode=2;
    GradualPrint("\t(T)ake the amulet\n");
    GradualPrint("\t(L)eave the amulet\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'T':
            return takeAmulet();
            break;
        case 'L':
            return leaveAmulet();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void takeAmulet()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ screams in agony as his left arm withers away\n");
    health-=30;
    hasAmulet=1;
    PlayerHealth(health);
    GradualPrint("\nYou pass out from the pain.\n\n");
    BlinkingProceed();
    ClearOutput();
    GradualPrint("After an unknown amount of time passes, you wake up, finding yourself in a weakened state, unable to muster your full strength.\n");
    GradualPrint("You decide to quickly leave this godforsaken room.\n");
    GradualPrint("You start looking for any available way to exit this place.\n\n");
    BlinkingProceed();
    return outsideTemple();
}

void leaveAmulet()
{
    ClearOutput();
    GradualPrint("@ notices a book open on the desk. It reads: \"Time...It has been time all along...\"");
    GradualPrint("You walk over to a shelf and go through the books.\n");
    GradualPrint("\"Ancient scriptures and studies on the Time Lord ... Interesting ... I should collect some for the professor.\"\n");
    GradualPrint("You take the book and brush off the age-old dust.\n\n");
    GradualPrint("\"Hmm... the book tells of timeline altering ritual altar in this cave.\n");
    GradualPrint("It says they worshiped the Lord and brought him gold and ornaments in exchange for wishes.\"n");
    GradualPrint("@ puts the book in his satchel and looks around.\n\n");
    BlinkingProceed();
    return outsideTemple();
}

void outsideTemple()
{
    ClearOutput();
    GradualPrint("You notice a giant stone door at the end of the chamber.\n");
    GradualPrint("The door is full of intricate patterns that introduce a feeling of mystery and awe on whoever laid their eyes on it.\n");
    GradualPrint("After unknowingly marveling at the door for a long time you notice a seemingly ordinary lever sitting alongside the door.\n");
    GradualPrint("After pulling the lever, a loud voice growled across the room...\n");
    GradualPrint("\"I HAVE NO VOICE, YET I SPEAK TO YOU EVERY HOUR.\n");
    GradualPrint("I HAVE NO HANDS YET I POINT THE WAY TO POWER.\n");
    GradualPrint("I HAVE NO EYES YET I WITNESS ALL THAT’S DONE.\n");
    GradualPrint("I HAVE NO LEGS YET I OUTRUN EVERYONE......WHAT AM I?\"\n\n");
    int wrong=0,i=0;
    char a[]="TIME";
    ShowConsoleCursor();
    for(char c=InputtedChar(); c!='\r'; c=InputtedChar())
    {
        if(c>='A'&&c<='Z')
        {
            printf("%c",c);
            if(i>=4 || c!=a[i++]) wrong=1;
        }
    }
    HideConsoleCursor();
    if(i!=4) wrong=1;
    //enter ends input && disabled backspace (must answer without mistakes)
    if(wrong) return wrongAnswer();
    else return correctAnswer();
}

void wrongAnswer()
{
    ClearOutput();
    GradualPrint("\"YOU HAVE CHOSEN POORLY\"\n");
    tempMode=2;
    GradualPrint("You hear a click... thousands of poison darts shoot out of various holes surrounding the door\n");
    GradualPrint("@ meets a gruesome end, falling to the ground bleeding to death \n\n");
    tempMode=0;
    return GameOver(2);
}

void correctAnswer()
{
    ClearOutput();
    GradualPrint("\"YOU HAVE CHOSEN WISELY\"\n");
    GradualPrint("The door roars and slowly slides apart.\n");
    GradualPrint("@ finds themselves in a massive room.\n");
    GradualPrint("The room is broad and oval in shape accentuating the sheer size.\n");
    GradualPrint("It has a podium like structure with seats all around the room facing a giant altar in the center.\n");
    GradualPrint("As soon as the player steps into the room, giant pillars surrounding the podium carrying lanterns lit up showing the grand room.\n");
    GradualPrint("As the player steps down the stairs leading to the altar, large boulders surrounding the altar start whirring around and piling up.\n");
    GradualPrint("@ watches as the boulders combined to form a gigantic stone troll.\n");
    GradualPrint("The troll roars \"Intruder!!!\" and starts to stomp around @.\n");
    tempMode=2;
    GradualPrint("As @ dodged those attacks , the troll has to be dealt with...\n\n");
    GradualPrint("\tCast (E)xplosive Chain Lightning\n");
    GradualPrint("\tCast (F)ireball\n");
    GradualPrint("\tConjure a magical (S)hield\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'E':
            return trollFight(80,0);
            break;
        case 'F':
            return trollFight(85,0);
            break;
        case 'S':
            return trollFight(90,1);
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void trollFight(int th, int state)
{
    ClearOutput();
    tempMode=2;
    if(state)
    {
        GradualPrint("Troll breaks the shield\n");

    }
    GradualPrint("Troll kicks @ and @ gets flung onto a chair\n");
    health-=20;
    PlayerHealth(health);
    EnemyHealth(th);
    GradualPrint("@ gets back up\n\n");
    GradualPrint("\tUse (T)elekinesis {Throw rocks}\n");
    GradualPrint("\tUse Explosive (F)ireball\n");
    GradualPrint("\tUse Explosive (C)hain Lightning\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'T':
            return midFightTelekinesis(th-10);
            break;
        case 'F':
            return endFight(th-15);
            break;
        case 'C':
            return endFight(th-20);
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void midFightTelekinesis(int th)
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ throws boulders using telekinesis.\n");
    EnemyHealth(th);
    GradualPrint("The troll has been stunned\n\n");
    GradualPrint("\tCast Explosive Chain (L)ightning\n");
    GradualPrint("\tCast Explosive Chain (F)ireball\n\n");
    int flag=1;
    while(1)
    {
        switch(InputtedChar())
        {
        case 'L':
        case 'F':
            return endFight(th-20);
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

void endFight(int th)
{
    ClearOutput();
    tempMode=2;
    GradualPrint("The troll rips off its own arm and throws it at @.\n\n");
    GradualPrint("\t(D)odge\n");
    GradualPrint("\t(C)onjure Magical Shield\n");
    GradualPrint("\t(U)se Healing Potion\n\n");
    while(1)
    {
        switch(InputtedChar())
        {
        case 'D':
        case 'C':
            EnemyHealth(th);
            BlinkingProceed();
            return defeatTroll();
            break;
        case 'U':
            health=maxHealth;
            GradualPrint("The potion completely restores @'s health\n");
            PlayerHealth(health);
            EnemyHealth(th);
            GradualPrint("Then the attack hits @.\n");
            health-=25;
            PlayerHealth(health);
            BlinkingProceed();
            return defeatTroll();
            break;
        default:
            IllegalInput();
            break;
        }
    }
}

void defeatTroll()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ gets up, and says, \"Its time to end this!!\"\n\n");
    GradualPrint("\tTake a (G)amble\n\n");
    while(InputtedChar()!='G')
    {
        IllegalInput();
    }
    GradualPrint("@ focuses more than he ever did in his entire life, maybe more than what anyone ever could\n");
    GradualPrint("@ combines the power of Explosive Lightning and Explosive Fireball\n");
    GradualPrint("@ coughs up blood at the raw power of the new spell, but he still perseveres.\n");
    health-=5;
    PlayerHealth(health);
    GradualPrint("\"HAAAAAHHHH...\"\n");
    GradualPrint("The fusion spell crashes down onto the troll evaporating it.\n");
    EnemyHealth(0);
    BlinkingProceed();
    return insideTemple();
}

void insideTemple()
{
    ClearOutput();
    GradualPrint("After taking out the troll, @ walks into the altar.\n");
    GradualPrint("On the altar stands tall the statue of the Time Lord.\n");
    GradualPrint("The stone sculpture is decorated with gold jewelries, diamonds , amethysts and rubies.\n");
    GradualPrint("He is depicted as standing in front of a ring of fire showing the endless cycle of life and rebirth, the flow of time itself.\n");
    GradualPrint("There is a wide platform where sacrifices and offerings were made to the God.\n");
    if(hasAmulet)
    {
        Wait(700);
        GradualPrint("\n@ places the amulet on the platform.\n");
        GradualPrint("Instantly, the amulet catches on fire.\n");
        GradualPrint("The lanterns on the columns glow emerald green as the God’s statue speaks");
        GradualPrint("\"WHAT IS IT THAT YOU DESIRE MY CHILD?\"\n\n");
        GradualPrint("\tI want to (K)now all about time to deepen our institute’s understanding.\n");
        GradualPrint("\tI want to (C)ontrol time so I can rule over mankind.\n\n");
        while(1)
        {
            switch(InputtedChar())
            {
            case 'K':
                return contentGod();
                break;
            case 'C':
                return angryGod();
                break;
            default:
                IllegalInput();
                break;
            }
        }
    }
    else
    {
        BlinkingProceed();
        return noAmulet();
    }
}

void contentGod()
{
    ClearOutput();
    GradualPrint("\"A NOBLE CHOICE, THIS BOOK CONTAINS ALL YOU NEED TO KNOW ABOUT TIME, FROM THE BEGINNING TO THE END\n");
    GradualPrint("ALL THAT IS COMPREHENDABLE TO YOUR MORTAL LIMITS\"\n");
    GradualPrint("A glowing golden book emerges in thin air and falls to the adventurer’s hand.\n");
    GradualPrint("The air hums with ancient energy.\n");
    GradualPrint("You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.\n\n");
    return GameOver(0);
}

void angryGod()
{
    ClearOutput();
    GradualPrint("\"FOOLISH CHILD!\n");
    GradualPrint("YOUR DESIRES ARE DARK AND TWISTED!!\n");
    GradualPrint("YOU DARE DESCECRATE MY TEMPLE WITH YOUR FILTH!!\n");
    GradualPrint("LEAVE NOW BEFORE I LOSE MY PATIENCE!!\"\n\n");
    GradualPrint("@ is thrown away with a great force.\n");
    GradualPrint("The place starts to crumble and break apart.\n");
    GradualPrint("@ runs as the temple collapses, sealing the ancient history within it permanently.\n");
    return GameOver(0);
}

void noAmulet()
{
    ClearOutput();
    GradualPrint("Examining the platform, @ finds a golden book.\n");
    GradualPrint("Titled 'Eternal', the book contains old writings on time itself.\n");
    GradualPrint("This could help deepen the understanding of time for the Chronological Institute!!\n");
    GradualPrint("The air hums with ancient energy.\n");
    GradualPrint("You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.\n\n");
    return GameOver(0);
}




int main()
{
    PlayMusic("a.wav");
    InitProperties();
    mainMenu();
}
