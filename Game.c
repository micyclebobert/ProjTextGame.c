/*Specific monsters are tied to their functions- players attack first unless, sneak attack
In forest ,if goes deep , faces tiger, otherwise safe outside forest- then cave to fight
Player character attack variable subject to buff or debuff
Series of boolean values linked to reward items that affects attack values
+possible curses affecting attacks

*/
//Due to how I coded it our game only works on Windows CodeBlocks's MinGW (GCC GNU) Compiler

/*Opponent health red blocks [████████ ██]
/*Player health green blocks [██████ ████]

/console clear
/choice of equiping any 1 amulet
//potential sound
/draugr fight chest has 1 potion
/Main menu
/Directly capture input Use Get function
Check fgets get ch etc cplusplus.com/forum/articles/19975/ stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar-for-reading-a-single-character-only
Keep options for inventory and equipments with Bool inventory and bool equipments to switch when we want
*/
//conio.h->getch() is windows specific nonstandard function
//On CodeBlocks go to Settings -> Compiler... -> Linker Settings -> Other Linker Options -> Add "-lwinmm" -> Ok
#pragma execution_character_set( "utf-8" ) //tis depricated but whatever it works

#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<mmsystem.h>




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

#define Default_Health 100
#define Name_Placeholder '@'


HANDLE outputHandle; //Console Handler
HANDLE inputHandle; //Console Handler

int health;
int maxHealth;
int hasAmulet;
int tempMode;
int defMode=0;
int width;
char name[];




///Technical Functions
InitProperties()
{
    SetConsoleOutputCP(65001); //set console text to utf-8 mode (basically utf-8 has more characters than standard char's 7 bits)

    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED); //Fullscreen the console //system("mode x,y") doesn't seem to put console to top-left corner

    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE); //the console we're using
    inputHandle = GetStdHandle(STD_INPUT_HANDLE); //the console we're using

    SetColorToDefault(); //sets text color to normal

    CONSOLE_CURSOR_INFO cursor; //Prepares data about an invisible cursor
    cursor.dwSize = 100;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(outputHandle, &cursor); //Changes the current cursor values to the data we prepared

    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(outputHandle, &screen);
    width = screen.srWindow.Right; //we don't need to -Left coz we maximized the screen
}

InitNewGame()
{
    ClearOutput();
    maxHealth = Default_Health, health = Default_Health;
    hasAmulet=0,tempMode=0;
    gameStart();
}


SetColor(int color)
{
    SetConsoleTextAttribute(outputHandle, color);
}

SetColorToDefault()
{
    SetConsoleTextAttribute(outputHandle, White);
}


ClearInputBuffer()
{
    FlushConsoleInputBuffer(inputHandle);
    fflush(stdin);
}

ClearOutput()
{
    system("cls");
    SetColorToDefault();
    tempMode=0;
}


char InputtedChar()
{
    ClearInputBuffer();
    return toupper(getch());

}


unsigned int Decide(unsigned int x,unsigned int y)
{
    if(tempMode||defMode) return y;
    return x;
}


Wait(unsigned int ms)
{
    if(ms>100) Sleep(ms); //Sleep is not too accurate, but its fine to be inaccurate in big waits (10000ms vs 10100ms is meh)
    else
    {
        clock_t x=ms+clock(); //But 5ms vs 105ms is a huge difference
        while (x>clock());
    }
}


PlayerHealth(int h) // █ ▓ ▒ ░ ■ ▬ -
{
    printf("\t\t\tPlayer Health: \t");
    int x=h/10;
    SetColor(Light_Green);
    for(int i=0; i<x; i++) printf("█ ");
    int y=h%10;
    if(y!=0)
    {
        x++; if(y>6) printf("▓ "); //7 8 9
        else if(y>3) printf("▒ "); //4 5 6
        else         printf("░ "); //1 2 3
    }
    for(int i=x; i<10; i++) printf("▬ ");
    SetColorToDefault();
    printf("\n\n");
}

EnemyHealth(int h) // █ ▓ ▒ ░ ■ ▬ - //Separate troll and draugr HP display?
{
    printf("\t\t\tEnemy Health:  \t");
    int x=h/10;
    SetColor(Red);
    for(int i=0; i<x; i++) printf("█ ");
    int y=h%10;
    if(y!=0)
    {
        x++; if(y>6) printf("▓ "); //7 8 9
        else if(y>3) printf("▒ "); //4 5 6
        else         printf("░ "); //1 2 3
    }
    for(int i=x; i<10; i++) printf("▬ ");
    SetColorToDefault();
    printf("\n\n");
}

GameOver(int state)
{
    if(state) SetColor(Red);
    else      SetColor(Aqua);
    GradualPrint("{Game Over}\n\n");
    SetColorToDefault();
}

GradualPrint(char str[])
{
    ClearInputBuffer();
    int len=strlen(str), i=0;
    if(tempMode==0&& defMode==0)
    {
        for(; i<len; i++)
        {
            if(kbhit())
            {
                getch();
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

CenteredPrint(char str[])
{
    int pad=(width-strlen(str))/2;
    for(int i=0;i<pad;i++) printf(" ");
    printf("%s\n",str);
}

SayMyName()
{
    SetColor(Aqua);
    GradualPrint(name);
    SetColorToDefault();
}

IllegalInput()
{
    printf("\nIllegal Input!!\n");
}

BlinkingProceed()
{
    printf("\n");
    SetColor(Gray);
    printf("\rPress any key to continue");
    Wait(75);
    SetColorToDefault();
    int k=500;
    ClearInputBuffer();
    while(!kbhit())
    {
        printf("\rPress any key to continue");
        Wait(k+100);
        printf("\r                         ");
        Wait(k);
    }
    getch();
    tempMode=0;
}

FadeOut()
{
    system("color 8");
    Wait(50);
    ClearOutput();
    Wait(700);
    system("color 7");
}


PlayMusic(char addr[])
{
    PlaySound(addr, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}




///Story Functions
/*
templateFunction()
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


mainMenu()
{
    ClearOutput();
    CenteredPrint("The Lost Library: A Cavernous Challenge\n\n\n");
    CenteredPrint("(P)lay Game\n");
    CenteredPrint("(E)xit\n");
    CenteredPrint("(A)bout\n");
    CenteredPrint("(H)elp\n");
    CenteredPrint("(S)ettings\n");
    int flag=1;
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
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

settingsMenu()
{
    ClearOutput();
    printf("\tFast (C)utscenes: ");
    if(defMode==0) printf("OFF\n");
    else printf("ON\n");
    printf("\n\t\tReturn to (M)ain Menu");
    int flag=1;
    while(1)
    {
        switch(InputtedChar())
        {
        case 'C':
            defMode=!defMode;
            return settingsMenu();
            break;
        case 'M':
            return mainMenu();
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

aboutMenu()
{
    ClearOutput();
    printf("A text based adventure game created by Group #1\n\n\n");
    printf("Press any key to return to the Main Menu");
    InputtedChar();
    return mainMenu();
}

helpMenu()
{
    ClearOutput();
    printf("Press the button corresponding to the single letter encapsulated in brackets to navigate that option.\n\n");
    printf("First letter of the user - inputted name is automatically capitalized.\n\n");
    printf("Every letter in the answer of the riddle is automatically capitalized.\n");
    printf("BackSpace is disabled for the riddle so answers must be given without mistakes.\n\n\n\n");
    printf("Press any key to return to the Main Menu");
    return mainMenu();
}


gameStart()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("Insert Player Name: ");
    gets(name);
    name[0]=toupper(name[0]);
    tempMode=0;
    GradualPrint("Welcome, @, to The Lost Library: A Cavernous Challenge\n");
    Wait(1000);
    FadeOut();
    return startScene();
}

startScene()
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

cavernEntrance()
{
    ClearOutput();
    GradualPrint("You descend into the cavern entrance, the sun's light fading behind you.\n");
    GradualPrint("The air grows damp and cool, the silence broken only by the dripping of water.\n");
    GradualPrint("Your torch flickers, casting eerie shadows on the cavern walls.\n");
    GradualPrint("You explore deeper and find your path blocked by a fallen rock.\n");
    GradualPrint("What do you do?\n\n");
    tempMode=2;
    GradualPrint("\tUse (E)xplosive Fireball Spell\n");
    GradualPrint("\tGo Retrun to (M)ain Menu\n\n");
    int flag=1;
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
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

cavernInside()
{
    ClearOutput();
    GradualPrint("The rock crumbles into pieces revealing the path.\n");
    GradualPrint("The cave walls have various carvings of ancient men performing rituals and sacrifices to what seems to the Time Lord.\n");
    GradualPrint("You remember Professor Chronos mentioning him as an ancient God worshiped by mortals in the 4th Era.\n");
    GradualPrint("As you walk through, you enter a chamber full of coffins and burial urns.\n");
    GradualPrint("As you move one step, you hear a *click\n");
    tempMode=2;
    GradualPrint("A coffin bursts open and a Draugr Death Overlord steps out.\n");
    GradualPrint("It jumps at you and with a slash of its sword lands a hit on you.\n\n");
    health-=24;
    PlayerHealth(health);
    GradualPrint("\t(F)ight Back, use Chain Lightning\n");
    GradualPrint("\t(R)un Away\n\n");
    int flag=1;
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
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

fightDraugr()
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

runFromDraugr()
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

defeatDraugr(int state)
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

furtherCavern(int state)
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
    return amuletRoom();
}

amuletRoom()
{
    GradualPrint("The room is full of old shelves full of books and scrolls.\n");
    GradualPrint("There is a study table in the corner of the room with a skeleton sitting in front of it.\n");
    GradualPrint("The skeleton has a gold neck amulet with a red ruby on it.\n");
    GradualPrint("\"Stealing from the dead cannot be good, the amulet might be cursed.\n");
    GradualPrint("Should I....\"\n\n");
    tempMode=2;
    GradualPrint("\t(T)ake the amulet\n");
    GradualPrint("\t(L)eave the amulet\n\n");
    int flag=1;
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
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

takeAmulet()
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

leaveAmulet()
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

outsideTemple()
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
    for(char c=InputtedChar(); c!='\r'; c=InputtedChar())
    {
        printf("%c",c);
        if(i>=4 || c!=a[i++]) wrong=1;
    }
    if(i!=4) wrong=1;
    //enter ends input && disabled backspace (must answer without mistakes)
    if(wrong) return wrongAnswer();
    else return correctAnswer();
}

wrongAnswer()
{
    ClearOutput();
    GradualPrint("\"YOU HAVE CHOSEN POORLY\"\n");
    tempMode=2;
    GradualPrint("You hear a click... thousands of poison darts shoot out of various holes surrounding the door\n");
    GradualPrint("@ meets a gruesome end, falling to the ground bleeding to death \n\n");
    tempMode=0;
    return GameOver(1);
    //Do you wish to go back to the last checkpoint
    //Make sure player health is reset back to what it was during amulet room
    //ClearOutput();
    //return amuletRoom();
}

correctAnswer()
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
    int flag=1;
    while(1)
    {
        switch(InputtedChar())
        {
        case 'E':
            return trollFight(70,0);
            break;
        case 'F':
            return trollFight(80,0);
            break;
        case 'S':
            return trollFight(90,1);
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

trollFight(int th, int state)
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
    int flag=1;
    while(1)
    {
        switch(InputtedChar())
        {
        case 'T':
            return midFightTelekinesis(th-20);
            break;
        case 'F':
        case 'C':
            return endFight(th-35);
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

midFightTelekinesis(int th)
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ throws boulders using telekinesis.\n");
    th-=20;
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
            return endFight(th-40);
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

endFight(int th)
{
    ClearOutput();
    tempMode=2;
    GradualPrint("The troll rips off its own arm and throws it at @.\n\n");
    GradualPrint("\t(D)odge\n");
    GradualPrint("\t(C)onjure Magical Shield\n");
    GradualPrint("\t(U)se Healing Potion\n");
    int flag=1;
    while(1)
    {
        switch(InputtedChar())
        {
        case 'D':
        case 'C':
            th-=35;
            EnemyHealth(th);
            return defeatTroll();
            break;
        case 'U':
            health=maxHealth;
            GradualPrint("The potion completely restores @'s health\n");
            EnemyHealth(th);
            PlayerHealth(health);
            GradualPrint("Then the attack hits @.\n");
            health-=25;
            PlayerHealth(health);
            return defeatTroll();
            break;
        default:
            if(--flag == 0) IllegalInput();
            break;
        }
    }
}

defeatTroll()
{
    ClearOutput();
    tempMode=2;
    GradualPrint("@ gets up, and says, \"Its time to end this!!\"\n\n");
    GradualPrint("\tTake a (G)amble\n\n");
    int flag=1;
    while(InputtedChar()!='G')
    {
        if(--flag==0) IllegalInput();
    }
    GradualPrint("@ focuses more than he ever did in his entire life, maybe more that he ever thought he could\n");
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

insideTemple()
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
        int flag=1;
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
                if(--flag == 0) IllegalInput();
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

contentGod()
{
    ClearOutput();
    GradualPrint("\"A NOBLE CHOICE, THIS BOOK CONTAINS ALL YOU NEED TO KNOW ABOUT TIME, FROM THE BEGINNING TO THE END\n");
    GradualPrint("ALL THAT IS COMPREHENDABLE TO YOUR MORTAL LIMITS\"\n");
    GradualPrint("A glowing golden book emerges in thin air and falls to the adventurer’s hand.\n");
    GradualPrint("The air hums with ancient energy.\n");
    GradualPrint("You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.\n\n");
    return GameOver(0);
}

angryGod()
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

noAmulet()
{
    ClearOutput();
    GradualPrint("Examining the platform, @ finds a golden book.\n");
    GradualPrint("Titled 'Eternal', the book contains old writings on time itself.\n");
    GradualPrint("This could help deepen the understanding of time for the Chronological Institute!!\n");
    GradualPrint("The air hums with ancient energy.\n");
    GradualPrint("You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.\n\n");
    return GameOver(0);
}




main()
{
    PlayMusic("a.wav");
    InitProperties();
    mainMenu();
}
























/*
Player Input: Name? *Game Starts
The Lost Library: A Cavernous Challenge GAME OUTLINE
Mention Story: With a pounding heart, you grip the map, the worn parchment crackling in your hands. The librarian, Professor Chronos, has entrusted you with this perilous mission. "This library," he rasped, his voice laden with years, "holds knowledge that can reshape the very fabric of time. But be warned, young researcher, the journey will be fraught with danger."
You descend into the cavern entrance, the sun's light fading behind you. The air grows damp and cool, the silence broken only by the dripping of water. Your torch flickers, casting eerie shadows on the cavern walls.
You explore deeper and find your path blocked by a fallen rock. What do you do? *Use Explosive Fireball Spell *Go back(Quit Game)
If Explosive Fireball used: The rock crumbles into pieces revealing the path. The cave walls have various carvings of ancient men performing rituals and sacrifices to what seems to the Time Lord. You remember Professor Chronos mentioning him as an ancient God worshiped by mortals in the 4th Era.
As you walk through, you enter a chamber full of coffins and burial urns. As you move one step, you hear a *click. A coffin bursts open and a Draugr Death Overlord steps out. It jumps at you and with a slash of its sword lands a hit on you.
*Health -24
if*Fight Back= *Use Chain lightning spell= Draugr Health -28
= Draugr hits back and misses= *Use Fireball Spell= Draugr Explodes into pieces. {Display Current Health after a fight scene ends}
else*Run Away= tries to run but Draugr lands another fatal blow chopping [Insert Player Name]’s arm off. {Health -52} [Insert Player Name] falls to the ground. The Draugr is about to land another fatal blow:

if*Use Explosive Fireball = Draugr Explodes into pieces= Player crawls and searches a chest nearby. Player luckily finds a health regeneration potion and drinks it. {Health +60} {Display Current health}
else*Do nothing= Draugr chops [name] head off. {Game Over}
[Insert Player Name] continues on his path. “The cave is ridden with traps and monsters! I should be more cautious...”
Player rummages through the chest and finds:
-Health Regeneration Potions 2X {Reset Health to 100}
-Spell Tome on Explosive Chain Lightning{Explosive Chain Lightning Unlocked}
-Gold Coins 25X , Ruby 2X, Garnet 1X, Amulet of Enhanced Spell Casting {Player Auto-equips for 10% damage buff- just mention this as printf statement}
[Insert Player Name] continues on to a deeper chamber. The room is full of old shelves full of books and scrolls. There is a study table in the corner of the room with a skeleton sitting in front of it. The skeleton has a gold neck amulet with a red ruby on it. “Stealing from the dead cannot be good, the amulet might be cursed. Sould I....”
*Take the amulet *Leave it
If *take the amulet: [Name] screams in agony as his left arm withers away, *Health-30 *Damage Debuff 30%
Else *Leave it: Player notices a book open on the desk. It reads: “Time...It has been time all along...”
You walk over to a shelf and go through the books.
“Ancient scriptures and studies on the Time Lord..interesting...I should collect some for the professor.”
You take the book and brush off the age-old dust.

“Hmm..the book tells of timeline altering ritual altar in this cave. It says they worshiped the Lord and brought him gold and ornaments in exchange for wishes.”
[Name]puts the book in his satchel and walks to a giant stone door at the end of the chamber. After pulling a lever sitting alongside the door, a loud voice growled across the room..
“I HAVE NO VOICE, YET I SPEAK TO YOU EVERY HOUR. I HAVE NO HANDS YET I POINT THE WAY TO POWER. I HAVE NO EYES YET I WITNESS ALL THAT’S DONE. I HAVE NO LEGS YET I OUTRUN EVERYONE......WHAT AM I?”
Scanf: correct answer is “time” uppercase or lower case
If incorrect: “YOU HAVE CHOSEN POORLY” you hear a click... thousands of poison darts shoot out of various holes surrounding the door, piercing [name] all over.
[name] falls to the ground bleeding to death {Game Over}{Start Over from skeleton sitting in front of desk}
If correct: “YOU HAVE CHOSEN WISELY”
The door roars and slowly slides apart. [Name] finds themselves in a massive room. The room is broad and oval in shape accentuating the sheer size. It has a podium like structure with seats all around the room facing a giant altar in the center. As soon as the player steps into the room, giant pillars surrounding the podium carrying lanterns lit up showing the grand room.
As the player steps down the stairs leading to the altar, large boulders surrounding the altar start whirring around and piling up. [Name] watched as the boulders combined to form a gigantic stone troll. The troll roars “Intruder!!!” and starts to stomp around [Name]. As [Name] dodged those attacks , the troll has to be dealt with..
*Cast Explosive Chain Lightning *Cast Fireball *Conjure a magical shield If *Cast Explosive Chain Lightning = Troll health-30.
Troll kicks [Name]. [Name] gets flung onto a chair .{Health -20}
If *Cast Fireball = Troll health -20

Troll kicks [Name]. [Name] gets flung onto a chair .{Health -20}
If *Conjure a magical shield= Troll breaks shield {Troll health-10} and kicks [Name]. [Name] gets flung onto a chair .{Health -20}{Display current health}
Player gets back up..
*Use Telekinesis {Throw rocks} *Use Explosive Fireball *Use Explosive Chain Lightning
If *Telekinesis= [Name] throws boulders using telekinesis. Troll health -20
Troll has been stunned, attack combo allowed *Explosive Chain lightning *Explosive Fireball
For either use = Troll health -40
If *Explosive Fireball / Explosive Chain Lightning = troll health – 35
Troll rips off its own arm and throws at the adventurer.
*Dodge *Conjure Magical Shield *Use Healing Potion
If Dodge/Magical Shield = Attack blocked
If healing potion = Player Health reset to 100, Boulder hits player Health – 25 {Display current health}
Player gets up,
“Its time to end this!!”
*Cast Explosive Lightning + Explosive Fireball {Cost: Player health -5}
[Name] combines the power of lightning and fireball. The fusion crashes down onto the troll blasting it to pieces.

After taking out the troll, [Name] walks into the altar. On the altar stands tall the statue of the Time Lord. The stone sculpture is decorated with gold jewelries, diamonds , amethysts and rubies. He is depicted as standing in front of a ring of fire showing the endless cycle of life and rebirth, the flow of time itself.
There is a wide platform where sacrifices and offerings were made to the God.
{If the player took the golden necklace from the skeleton in the previous level they can offer that now to the god and get a wish fulfilled}
If player has amulet =
*Offer amulet = The player places the amulet on the platform. Instantly, the amulet catches on fire. The lanterns on the columns glow emerald green as the God’s statue speaks “WHAT IS IT THAT YOU DESIRE MY CHILD?”
*I want to know all about time to deepen our institute’s understanding. *I want to control time so I can rule over mankind.
If option1: “A NOBLE CHOICE, THIS BOOK CONTAINS ALL YOU NEED TO KNOW ABOUT TIME, FROM THE BEGINNING TO THE END, ALL THAT IS COMPREHENDABLE TO YOUR MORTAL LIMITS”, A glowing golden book emerges in thin air and falls to the adventurer’s hand. The air hums with ancient energy. You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.{Game Over}
If option 2: “FOOLISH CHILD! YOUR DESIRES ARE DARK AND TWISTED!! YOU DARE DESCECRATE MY TEMPLE WITH YOUR FILTH!! LEAVE NOW BEFORE I LOSE MY PATIENCE!!” The player is thrown away with a great force. The place starts to crumble and break apart. [Name] runs as the temple collapses, sealing the ancient history within it permanently. {Game Over}
If no amulet: Examining the platform, [Name] finds a golden book. Titled ‘Eternal’ the book contains old writings on time itself. This could help deepen the understanding of time for the Chronological Institute!! The air hums with ancient

energy. You feel a surge of excitement as you delve into the forbidden knowledge, uncovering the secrets of time manipulation.{Game Over}
*/
