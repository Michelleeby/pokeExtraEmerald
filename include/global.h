#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include <string.h>
#include <limits.h>
#include "config.h" // we need to define config before gba headers as print stuff needs the functions nulled before defines.
#include "gba/gba.h"
#include "constants/global.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/species.h"

// free saveblock 1 defines
//#define FREE_EXTRA_SEEN_FLAGS           //free up extra pokedex seen flags. Frees up 104 bytes
#define FREE_FIELD_3598                 //frees up unused saveblock data. 384 bytes
#define FREE_TRAINER_HILL               //frees up trainer hill data. 28 bytes 
#define FREE_MYSTERY_EVENT_BUFFERS      //frees up mystery event and ramScript. 1880 bytes
#define FREE_MATCH_CALL                 //frees up match call data. 104 bytes
#define FREE_UNION_ROOM_CHAT            //frees up field unk3C88. 210 bytes
#define FREE_ENIGMA_BERRY               //frees up enigma berry. 52 bytes
#define FREE_LINK_BATTLE_RECORDS        //frees link battle record data. 88 bytes
                                        // saveblock1 total: 2850 bytes
//free saveblock 2 defines
#define FREE_BATTLE_TOWER_E_READER      //frees up battle tower e reader trainer data. 188 bytes
#define FREE_POKEMON_JUMP               //frees up pokemon jump data. 16 bytes
#define FREE_RECORD_MIXING_HALL_RECORDS //frees up hall records for record mixing. 1032 bytes
                                        // saveblock2 total: 1236 bytes
                                        
                                        //grand total: 4086

// Prevent cross-jump optimization.
#define BLOCK_CROSS_JUMP asm("");

// to help in decompiling
#define asm_comment(x) asm volatile("@ -- " x " -- ")
#define asm_unified(x) asm(".syntax unified\n" x "\n.syntax divided")
#define NAKED __attribute__((naked))

// IDE support
#if defined (__APPLE__) || defined (__CYGWIN__) || defined (_MSC_VER)
#define _(x) x
#define __(x) x

// Fool CLion IDE
#define INCBIN(x) {0}
#define INCBIN_U8 INCBIN
#define INCBIN_U16 INCBIN
#define INCBIN_U32 INCBIN
#define INCBIN_S8 INCBIN
#define INCBIN_S16 INCBIN
#define INCBIN_S32 INCBIN
#endif // IDE support

#define ARRAY_COUNT(array) (size_t)(sizeof(array) / sizeof((array)[0]))

// GameFreak used a macro called "NELEMS", as evidenced by
// AgbAssert calls.
#define NELEMS(arr) (sizeof(arr)/sizeof(*(arr)))

#define SWAP(a, b, temp)    \
{                           \
    temp = a;               \
    a = b;                  \
    b = temp;               \
}

// useful math macros

// Converts a number to Q8.8 fixed-point format
#define Q_8_8(n) ((s16)((n) * 256))

// Converts a number to Q4.12 fixed-point format
#define Q_4_12(n)  ((s16)((n) * 4096))
#define UQ_4_12(n)  ((u16)((n) * 4096))

// Converts a number to Q24.8 fixed-point format
#define Q_24_8(n)  ((s32)((n) << 8))

// Converts a Q8.8 fixed-point format number to a regular integer
#define Q_8_8_TO_INT(n) ((int)((n) / 256))

// Converts a Q4.12 fixed-point format number to a regular integer
#define Q_4_12_TO_INT(n)  ((int)((n) / 4096))
#define UQ_4_12_TO_INT(n)  ((int)((n) / 4096))

// Converts a Q24.8 fixed-point format number to a regular integer
#define Q_24_8_TO_INT(n) ((int)((n) >> 8))

// Rounding value for Q4.12 fixed-point format
#define Q_4_12_ROUND ((1) << (12 - 1))
#define UQ_4_12_ROUND ((1) << (12 - 1))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))

#if MODERN
#define abs(x) (((x) < 0) ? -(x) : (x))
#endif

// Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

// Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

// There are many quirks in the source code which have overarching behavioral differences from
// a number of other files. For example, diploma.c seems to declare rodata before each use while
// other files declare out of order and must be at the beginning. There are also a number of
// macros which differ from one file to the next due to the method of obtaining the result, such
// as these below. Because of this, there is a theory (Two Team Theory) that states that these
// programming projects had more than 1 "programming team" which utilized different macros for
// each of the files that were worked on.
#define T1_READ_8(ptr)  ((ptr)[0])
#define T1_READ_16(ptr) ((ptr)[0] | ((ptr)[1] << 8))
#define T1_READ_32(ptr) ((ptr)[0] | ((ptr)[1] << 8) | ((ptr)[2] << 16) | ((ptr)[3] << 24))
#define T1_READ_PTR(ptr) (u8*) T1_READ_32(ptr)

// T2_READ_8 is a duplicate to remain consistent with each group.
#define T2_READ_8(ptr)  ((ptr)[0])
#define T2_READ_16(ptr) ((ptr)[0] + ((ptr)[1] << 8))
#define T2_READ_32(ptr) ((ptr)[0] + ((ptr)[1] << 8) + ((ptr)[2] << 16) + ((ptr)[3] << 24))
#define T2_READ_PTR(ptr) (void*) T2_READ_32(ptr)

// Macros for checking the joypad
#define TEST_BUTTON(field, button) ({(field) & (button);})
#define JOY_NEW(button) TEST_BUTTON(gMain.newKeys,  button)
#define JOY_HELD(button)  TEST_BUTTON(gMain.heldKeys, button)
#define JOY_HELD_RAW(button) TEST_BUTTON(gMain.heldKeysRaw, button)
#define JOY_REPEAT(button) TEST_BUTTON(gMain.newAndRepeatedKeys, button)

#define S16TOPOSFLOAT(val)   \
({                           \
    s16 v = (val);           \
    float f = (float)v;      \
    if(v < 0) f += 65536.0f; \
    f;                       \
})

// Branch defines: Used by other branches to detect each other.
// Each define must be here for each of RHH's branch you have pulled.
// e.g. If you have both the battle_engine and pokemon_expansion branch,
//      then both BATTLE_ENGINE and POKEMON_EXPANSION must be defined here.
#define BATTLE_ENGINE
#define ITEM_EXPANSION
#define POKEMON_EXPANSION


#define ROUND_BITS_TO_BYTES(numBits)(((numBits) / 8) + (((numBits) % 8) ? 1 : 0))

#define DEX_FLAGS_NO (ROUND_BITS_TO_BYTES(POKEMON_SLOTS_NUMBER))
#define NUM_FLAG_BYTES (ROUND_BITS_TO_BYTES(FLAGS_COUNT))

struct Coords8
{
    s8 x;
    s8 y;
};

struct UCoords8
{
    u8 x;
    u8 y;
};

struct Coords16
{
    s16 x;
    s16 y;
};

struct UCoords16
{
    u16 x;
    u16 y;
};

struct Coords32
{
    s32 x;
    s32 y;
};

struct UCoords32
{
    u32 x;
    u32 y;
};

struct Time
{
    /*0x00*/ s16 days;
    /*0x02*/ s8 hours;
    /*0x03*/ s8 minutes;
    /*0x04*/ s8 seconds;
    /*0x05*/ s8 dayOfWeek;
};

struct Pokedex
{
    /*0x00*/ u8 order;
    /*0x01*/ u8 mode;
    /*0x02*/ u8 nationalMagic; // must equal 0xDA in order to have National mode
    /*0x03*/ u8 unknown2;
    /*0x04*/ u32 unownPersonality; // set when you first see Unown
    /*0x08*/ u32 spindaPersonality; // set when you first see Spinda
    /*0x0C*/ u32 unknown3;
    /*0x10*/ u8 filler[0x68]; // Previously Dex Flags, feel free to remove.
};

struct PokemonJumpResults
{
    u16 jumpsInRow;
    u16 field2;
    u16 excellentsInRow;
    u16 field6;
    u32 field8;
    u32 bestJumpScore;
};

struct BerryPickingResults
{
    u32 bestScore;
    u16 berriesPicked;
    u16 berriesPickedInRow;
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    u8 field_F;
};

// two arrays for lvl50 and open level
struct PyramidBag
{
    u16 itemId[2][PYRAMID_BAG_ITEMS_COUNT];
    u8 quantity[2][PYRAMID_BAG_ITEMS_COUNT];
};

struct BerryCrush
{
    u16 berryCrushResults[4];
    u32 berryPowderAmount;
    u32 unk;
};

struct ApprenticeMon
{
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u16 item;
};

// This is for past players Apprentices or Apprentices received via Record Mix.
// For the current Apprentice, see struct PlayersApprentice
struct Apprentice
{
    u8 id:5;
    u8 lvlMode:2; // + 1
    u8 numQuestions;
    u8 number;
    struct ApprenticeMon party[MULTI_PARTY_SIZE];
    u16 speechWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    u8 playerId[TRAINER_ID_LENGTH];
    u8 playerName[PLAYER_NAME_LENGTH];
    u8 language;
    u32 checksum;
};

struct BattleTowerPokemon
{
    u16 species;
    u16 heldItem;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 ppBonuses;
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u32 otId;
    u32 hpIV:5;
    u32 attackIV:5;
    u32 defenseIV:5;
    u32 speedIV:5;
    u32 spAttackIV:5;
    u32 spDefenseIV:5;
    u32 gap:1;
    u32 abilityNum:1;
    u32 personality;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 friendship;
};

#define NULL_BATTLE_TOWER_POKEMON { .nickname = __("$$$$$$$$$$$") }

struct EmeraldBattleTowerRecord
{
    /*0x00*/ u8 lvlMode; // 0 = level 50, 1 = level 100
    /*0x01*/ u8 facilityClass;
    /*0x02*/ u16 winStreak;
    /*0x04*/ u8 name[PLAYER_NAME_LENGTH + 1];
    /*0x0C*/ u8 trainerId[TRAINER_ID_LENGTH];
    /*0x10*/ u16 greeting[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x1C*/ u16 speechWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x28*/ u16 speechLost[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x34*/ struct BattleTowerPokemon party[MAX_FRONTIER_PARTY_SIZE];
    /*0xE4*/ u8 language;
    /*0xE8*/ u32 checksum;
};

struct BattleTowerInterview
{
    u16 playerSpecies;
    u16 opponentSpecies;
    u8 opponentName[PLAYER_NAME_LENGTH + 1];
    u8 opponentMonNickname[POKEMON_NAME_LENGTH + 1];
    u8 opponentLanguage;
};

struct BattleTowerEReaderTrainer
{
    /*0x00*/ u8 unk0;
    /*0x01*/ u8 facilityClass;
    /*0x02*/ u16 winStreak;
    /*0x04*/ u8 name[PLAYER_NAME_LENGTH + 1];
    /*0x0C*/ u8 trainerId[TRAINER_ID_LENGTH];
    /*0x10*/ u16 greeting[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x1C*/ u16 farewellPlayerLost[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x28*/ u16 farewellPlayerWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x34*/ struct BattleTowerPokemon party[FRONTIER_PARTY_SIZE];
    /*0xB8*/ u32 checksum;
};

// For displaying party information on the player's Battle Dome tourney page
struct DomeMonData
{
    u16 moves[MAX_MON_MOVES];
    u8 evs[NUM_STATS];
    u8 nature;
};

struct RentalMon
{
    u16 monId;
    u32 personality;
    u8 ivs;
    u8 abilityNum;
};

struct BattleDomeTrainer
{
    u16 trainerId:10;
    u16 isEliminated:1;
    u16 eliminatedAt:2;
    u16 forfeited:3;
};

#define DOME_TOURNAMENT_TRAINERS_COUNT 16
#define BATTLE_TOWER_RECORD_COUNT 5

struct BattleFrontier
{
    /*0x64C*/ struct EmeraldBattleTowerRecord towerPlayer;
    /*0x738*/ struct EmeraldBattleTowerRecord towerRecords[BATTLE_TOWER_RECORD_COUNT]; // From record mixing.
    /*0xBEB*/ struct BattleTowerInterview towerInterview;
    #ifndef FREE_BATTLE_TOWER_E_READER
    /*0xBEC*/ struct BattleTowerEReaderTrainer ereaderTrainer;  //188 bytes
    #endif
    /*0xCA8*/ u8 challengeStatus;
    /*0xCA9*/ u8 lvlMode:2;
    /*0xCA9*/ u8 challengePaused:1;
    /*0xCA9*/ u8 disableRecordBattle:1;
    /*0xCAA*/ u16 selectedPartyMons[MAX_FRONTIER_PARTY_SIZE];
    /*0xCB2*/ u16 curChallengeBattleNum; // Battle number / room number (Pike) / floor number (Pyramid)
    /*0xCB4*/ u16 trainerIds[20];
    /*0xCDC*/ u32 winStreakActiveFlags;
    /*0xCE0*/ u16 towerWinStreaks[4][2];
    /*0xCF0*/ u16 towerRecordWinStreaks[4][2];
    /*0xD00*/ u16 battledBrainFlags;
    /*0xD02*/ u16 towerSinglesStreak; // Never read
    /*0xD04*/ u16 towerNumWins; // Increments to MAX_STREAK but never read otherwise
    /*0xD06*/ u8 towerBattleOutcome;
    /*0xD07*/ u8 towerLvlMode;
    /*0xD08*/ u8 domeAttemptedSingles50:1;
    /*0xD08*/ u8 domeAttemptedSinglesOpen:1;
    /*0xD08*/ u8 domeHasWonSingles50:1;
    /*0xD08*/ u8 domeHasWonSinglesOpen:1;
    /*0xD08*/ u8 domeAttemptedDoubles50:1;
    /*0xD08*/ u8 domeAttemptedDoublesOpen:1;
    /*0xD08*/ u8 domeHasWonDoubles50:1;
    /*0xD08*/ u8 domeHasWonDoublesOpen:1;
    /*0xD09*/ u8 domeUnused;
    /*0xD0A*/ u8 domeLvlMode;
    /*0xD0B*/ u8 domeBattleMode;
    /*0xD0C*/ u16 domeWinStreaks[2][2];
    /*0xD14*/ u16 domeRecordWinStreaks[2][2];
    /*0xD1C*/ u16 domeTotalChampionships[2][2];
    /*0xD24*/ struct BattleDomeTrainer domeTrainers[DOME_TOURNAMENT_TRAINERS_COUNT];
    /*0xD64*/ u16 domeMonIds[DOME_TOURNAMENT_TRAINERS_COUNT][FRONTIER_PARTY_SIZE];
    /*0xDC4*/ u16 unused_DC4;
    /*0xDC6*/ u16 palacePrize;
    /*0xDC8*/ u16 palaceWinStreaks[2][2];
    /*0xDD0*/ u16 palaceRecordWinStreaks[2][2];
    /*0xDD8*/ u16 arenaPrize;
    /*0xDDA*/ u16 arenaWinStreaks[2];
    /*0xDDE*/ u16 arenaRecordStreaks[2];
    /*0xDE2*/ u16 factoryWinStreaks[2][2];
    /*0xDEA*/ u16 factoryRecordWinStreaks[2][2];
    /*0xDF6*/ u16 factoryRentsCount[2][2];
    /*0xDFA*/ u16 factoryRecordRentsCount[2][2];
    /*0xE02*/ u16 pikePrize;
    /*0xE04*/ u16 pikeWinStreaks[2];
    /*0xE08*/ u16 pikeRecordStreaks[2];
    /*0xE0C*/ u16 pikeTotalStreaks[2];
    /*0xE10*/ u8 pikeHintedRoomIndex:3;
    /*0xE10*/ u8 pikeHintedRoomType:4;
    /*0xE10*/ u8 pikeHealingRoomsDisabled:1;
    /*0xE12*/ u16 pikeHeldItemsBackup[FRONTIER_PARTY_SIZE];
    /*0xE18*/ u16 pyramidPrize;
    /*0xE1A*/ u16 pyramidWinStreaks[2];
    /*0xE1E*/ u16 pyramidRecordStreaks[2];
    /*0xE22*/ u16 pyramidRandoms[4];
    /*0xE2A*/ u8 pyramidTrainerFlags;
    /*0xE2C*/ struct PyramidBag pyramidBag;
    /*0xE68*/ u8 pyramidLightRadius;
    /*0xE6A*/ u16 verdanturfTentPrize;
    /*0xE6C*/ u16 fallarborTentPrize;
    /*0xE6E*/ u16 slateportTentPrize;
    /*0xE70*/ struct RentalMon rentalMons[FRONTIER_PARTY_SIZE * 2];
    /*0xEB8*/ u16 battlePoints;
    /*0xEBA*/ u16 cardBattlePoints;
    /*0xEBC*/ u32 battlesCount;
    /*0xEC0*/ u16 domeWinningMoves[DOME_TOURNAMENT_TRAINERS_COUNT];
    /*0xEE0*/ u8 trainerFlags;
    /*0xEE1*/ u8 opponentNames[2][PLAYER_NAME_LENGTH + 1];
    /*0xEF1*/ u8 opponentTrainerIds[2][TRAINER_ID_LENGTH];
    /*0xEF9*/ u8 unk_EF9:7; // Never read
    /*0xEF9*/ u8 savedGame:1;
    /*0xEFA*/ u8 unused_EFA;
    /*0xEFB*/ u8 unused_EFB;
    /*0xEFC*/ struct DomeMonData domePlayerPartyData[FRONTIER_PARTY_SIZE];
};

struct ApprenticeQuestion
{
    u8 questionId:2;
    u8 monId:2;
    u8 moveSlot:2;
    u8 suggestedChange:2; // TRUE if told to use held item or second move, FALSE if told to use no item or first move
    u16 data; // used both as an itemId and a moveId
};

struct PlayersApprentice
{
    /*0xB0*/ u8 id;
    /*0xB1*/ u8 lvlMode:2;  //0: Unassigned, 1: Lv 50, 2: Open Lv
    /*0xB1*/ u8 questionsAnswered:4;
    /*0xB1*/ u8 leadMonId:2;
    /*0xB2*/ u8 party:3;
    /*0xB2*/ u8 saveId:2;
    /*0xB3*/ u8 unused;
    /*0xB4*/ u8 speciesIds[MULTI_PARTY_SIZE];
    /*0xB8*/ struct ApprenticeQuestion questions[APPRENTICE_MAX_QUESTIONS];
};

struct RankingHall1P
{
    u8 id[TRAINER_ID_LENGTH];
    u16 winStreak;
    u8 name[PLAYER_NAME_LENGTH + 1];
    u8 language;
};

struct RankingHall2P
{
    u8 id1[TRAINER_ID_LENGTH];
    u8 id2[TRAINER_ID_LENGTH];
    u16 winStreak;
    u8 name1[PLAYER_NAME_LENGTH + 1];
    u8 name2[PLAYER_NAME_LENGTH + 1];
    u8 language;
};

struct SaveBlock2
{
    /*0x00*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x08*/ u8 playerGender; // MALE, FEMALE
    /*0x09*/ u8 specialSaveWarpFlags;
    /*0x0A*/ u8 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x0E*/ u16 playTimeHours;
    /*0x10*/ u8 playTimeMinutes;
    /*0x11*/ u8 playTimeSeconds;
    /*0x12*/ u8 playTimeVBlanks;
    /*0x13*/ u8 optionsButtonMode;  // OPTIONS_BUTTON_MODE_[NORMAL/LR/L_EQUALS_A]
    /*0x14*/ u16 optionsTextSpeed:3; // OPTIONS_TEXT_SPEED_[SLOW/MID/FAST]
             u16 optionsWindowFrameType:5; // Specifies one of the 20 decorative borders for text boxes
             u16 optionsSound:1; // OPTIONS_SOUND_[MONO/STEREO]
             u16 optionsBattleStyle:1; // OPTIONS_BATTLE_STYLE_[SHIFT/SET]
             u16 optionsBattleSceneOff:1; // whether battle animations are disabled
             u16 regionMapZoom:1; // whether the map is zoomed in
             u16 expShare:1;
    /*0x18*/ struct Pokedex pokedex;
    /*0x90*/ u8 filler_90[0x8];
    /*0x98*/ struct Time localTimeOffset;
    /*0xA0*/ struct Time lastBerryTreeUpdate;
    /*0xA8*/ u32 gcnLinkFlags; // Read by Pokemon Colosseum/XD
    /*0xAC*/ u32 encryptionKey;
    /*0xB0*/ struct PlayersApprentice playerApprentice;
    /*0xDC*/ struct Apprentice apprentices[APPRENTICE_COUNT];   //272 bytes
    /*0x1EC*/ struct BerryCrush berryCrush;
    #ifndef FREE_POKEMON_JUMP
    /*0x1FC*/ struct PokemonJumpResults pokeJump;   //16 bytes
    #endif
    /*0x20C*/ struct BerryPickingResults berryPick;
    #ifndef FREE_RECORD_MIXING_HALL_RECORDS
    /*0x21C*/ struct RankingHall1P hallRecords1P[HALL_FACILITIES_COUNT][2][3]; // From record mixing.
    /*0x57C*/ struct RankingHall2P hallRecords2P[2][3]; // From record mixing.
    #endif
    /*0x624*/ u16 contestLinkResults[CONTEST_CATEGORIES_COUNT][CONTESTANT_COUNT];
    /*0x64C*/ struct BattleFrontier frontier;
}; // sizeof=0xF2C

extern struct SaveBlock2 *gSaveBlock2Ptr;

struct SecretBaseParty
{
    u32 personality[PARTY_SIZE];
    u16 moves[PARTY_SIZE * MAX_MON_MOVES];
    u16 species[PARTY_SIZE];
    u16 heldItems[PARTY_SIZE];
    u8 levels[PARTY_SIZE];
    u8 EVs[PARTY_SIZE];
};

struct SecretBase
{
    /*0x1A9C*/ u8 secretBaseId;
    /*0x1A9D*/ u8 sbr_field_1_0:4;
    /*0x1A9D*/ u8 gender:1;
    /*0x1A9D*/ u8 battledOwnerToday:1;
    /*0x1A9D*/ u8 registryStatus:2;
    /*0x1A9E*/ u8 trainerName[PLAYER_NAME_LENGTH];
    /*0x1AA5*/ u8 trainerId[TRAINER_ID_LENGTH]; // byte 0 is used for determining trainer class
    /*0x1AA9*/ u8 language;
    /*0x1AAA*/ u16 numSecretBasesReceived;
    /*0x1AAC*/ u8 numTimesEntered;
    /*0x1AAD*/ u8 sbr_field_11;
    /*0x1AAE*/ u8 decorations[DECOR_MAX_SECRET_BASE];
    /*0x1ABE*/ u8 decorationPositions[DECOR_MAX_SECRET_BASE];
    /*0x1AD0*/ struct SecretBaseParty party;
};

#include "constants/game_stat.h"
#include "global.fieldmap.h"
#include "global.berry.h"
#include "global.tv.h"
#include "pokemon.h"

struct WarpData
{
    s8 mapGroup;
    s8 mapNum;
    s8 warpId;
    s16 x, y;
};

struct ItemSlot
{
    u16 itemId;
    u16 quantity;
};

struct Pokeblock
{
    u8 color;
    u8 spicy;
    u8 dry;
    u8 sweet;
    u8 bitter;
    u8 sour;
    u8 feel;
};

struct Roamer
{
    /*0x00*/ u32 ivs;
    /*0x04*/ u32 personality;
    /*0x08*/ u16 species;
    /*0x0A*/ u16 hp;
    /*0x0C*/ u8 level;
    /*0x0D*/ u8 status;
    /*0x0E*/ u8 cool;
    /*0x0F*/ u8 beauty;
    /*0x10*/ u8 cute;
    /*0x11*/ u8 smart;
    /*0x12*/ u8 tough;
    /*0x13*/ bool8 active;
    /*0x14*/ u8 filler[0x8];
};

struct RamScriptData
{
    u8 magic;
    u8 mapGroup;
    u8 mapNum;
    u8 objectId;
    u8 script[995];
};

struct RamScript
{
    u32 checksum;
    struct RamScriptData data;
};

struct EasyChatPair
{
    u16 unk0_0:7;
    u16 unk0_7:7;
    u16 unk1_6:1;
    u16 unk2;
    u16 words[2];
}; /*size = 0x8*/

struct MailStruct
{
    /*0x00*/ u16 words[MAIL_WORDS_COUNT];
    /*0x12*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x1A*/ u8 trainerId[TRAINER_ID_LENGTH];
    /*0x1E*/ u16 species;
    /*0x20*/ u16 itemId;
};

struct MauvilleManCommon
{
    u8 id;
};

struct MauvilleManBard
{
    /*0x00*/ u8 id;
    /*0x02*/ u16 songLyrics[BARD_SONG_LENGTH];
    /*0x0E*/ u16 temporaryLyrics[BARD_SONG_LENGTH];
    /*0x1A*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x22*/ u8 filler_2DB6[0x3];
    /*0x25*/ u8 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x29*/ bool8 hasChangedSong;
    /*0x2A*/ u8 language;
}; /*size = 0x2C*/

struct MauvilleManStoryteller
{
    u8 id;
    bool8 alreadyRecorded;
    u8 filler2[2];
    u8 gameStatIDs[NUM_STORYTELLER_TALES];
    u8 trainerNames[NUM_STORYTELLER_TALES][PLAYER_NAME_LENGTH];
    u8 statValues[NUM_STORYTELLER_TALES][4];
    u8 language[NUM_STORYTELLER_TALES];
};

struct MauvilleManGiddy
{
    /*0x00*/ u8 id;
    /*0x01*/ u8 taleCounter;
    /*0x02*/ u8 questionNum;
    /*0x04*/ u16 randomWords[10];
    /*0x18*/ u8 questionList[8];
    /*0x20*/ u8 language;
}; /*size = 0x2C*/

struct MauvilleManHipster
{
    u8 id;
    bool8 alreadySpoken;
    u8 language;
};

struct MauvilleOldManTrader
{
    u8 id;
    u8 decorations[NUM_TRADER_ITEMS];
    u8 playerNames[NUM_TRADER_ITEMS][11];
    u8 alreadyTraded;
    u8 language[NUM_TRADER_ITEMS];
};

typedef union OldMan
{
    struct MauvilleManCommon common;
    struct MauvilleManBard bard;
    struct MauvilleManGiddy giddy;
    struct MauvilleManHipster hipster;
    struct MauvilleOldManTrader trader;
    struct MauvilleManStoryteller storyteller;
    u8 filler[0x40];
} OldMan;

struct RecordMixing_UnknownStructSub
{
    u32 unk0;
    u8 data[0x34];
    //u8 data[0x38];
};

struct RecordMixing_UnknownStruct
{
    struct RecordMixing_UnknownStructSub data[2];
    u32 unk70;
    u16 unk74[0x2];
};

#define LINK_B_RECORDS_COUNT 5

struct LinkBattleRecord
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    u16 trainerId;
    u16 wins;
    u16 losses;
    u16 draws;
};

struct LinkBattleRecords
{
    struct LinkBattleRecord entries[LINK_B_RECORDS_COUNT];
    u8 languages[LINK_B_RECORDS_COUNT];
};

struct RecordMixingGiftData
{
    u8 unk0;
    u8 quantity;
    u16 itemId;
    u8 filler4[8];
};

struct RecordMixingGift
{
    int checksum;
    struct RecordMixingGiftData data;
};

struct ContestWinner
{
    u32 personality;
    u32 trainerId;
    u16 species;
    u8 contestCategory;
    u8 monName[POKEMON_NAME_LENGTH + 1];
    u8 trainerName[PLAYER_NAME_LENGTH + 1];
    u8 contestRank;
};

struct DayCareMail
{
    struct MailStruct message;
    u8 OT_name[PLAYER_NAME_LENGTH + 1];
    u8 monName[POKEMON_NAME_LENGTH + 1];
    u8 gameLanguage:4;
    u8 monLanguage:4;
};

struct DaycareMon
{
    struct BoxPokemon mon;
    struct DayCareMail mail;
    u32 steps;
};

struct DayCare
{
    struct DaycareMon mons[DAYCARE_MON_COUNT];
    u32 offspringPersonality;
    u8 stepCounter;
};

struct RecordMixingDayCareMail
{
    struct DayCareMail mail[DAYCARE_MON_COUNT];
    u32 numDaycareMons;
    bool16 holdsItem[DAYCARE_MON_COUNT];
};

struct LilycoveLadyQuiz
{
    /*0x000*/ u8 id;
    /*0x001*/ u8 state;
    /*0x002*/ u16 question[9];
    /*0x014*/ u16 correctAnswer;
    /*0x016*/ u16 playerAnswer;
    /*0x018*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x020*/ u16 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x028*/ u16 prize;
    /*0x02a*/ bool8 waitingForChallenger;
    /*0x02b*/ u8 questionId;
    /*0x02c*/ u8 prevQuestionId;
    /*0x02d*/ u8 language;
};

struct LilycoveLadyFavor
{
    /*0x000*/ u8 id;
    /*0x001*/ u8 state;
    /*0x002*/ bool8 likedItem;
    /*0x003*/ u8 numItemsGiven;
    /*0x004*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x00c*/ u8 favorId;
    /*0x00e*/ u16 itemId;
    /*0x010*/ u16 bestItem;
    /*0x012*/ u8 language;
};

struct LilycoveLadyContest
{
    /*0x000*/ u8 id;
    /*0x001*/ bool8 givenPokeblock;
    /*0x002*/ u8 numGoodPokeblocksGiven;
    /*0x003*/ u8 numOtherPokeblocksGiven;
    /*0x004*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x00c*/ u8 maxSheen;
    /*0x00d*/ u8 category;
    /*0x00e*/ u8 language;
};

typedef union // 3b58
{
    struct LilycoveLadyQuiz quiz;
    struct LilycoveLadyFavor favor;
    struct LilycoveLadyContest contest;
    u8 id;
    u8 pad[0x40];
} LilycoveLady;

struct WaldaPhrase
{
    u16 colors[2]; // Background, foreground.
    u8 text[16];
    u8 iconId;
    u8 patternId;
    bool8 patternUnlocked;
};

struct TrainerNameRecord
{
    u32 trainerId;
    u8 trainerName[PLAYER_NAME_LENGTH + 1];
};

struct SaveTrainerHill
{
    /*0x3D64*/ u32 timer;
    /*0x3D68*/ u32 bestTime;
    /*0x3D6C*/ u8 unk_3D6C;
    /*0x3D6D*/ u8 unused;
    /*0x3D6E*/ u16 receivedPrize:1;
    /*0x3D6E*/ u16 checkedFinalTime:1;
    /*0x3D6E*/ u16 spokeToOwner:1;
    /*0x3D6E*/ u16 hasLost:1;
    /*0x3D6E*/ u16 maybeECardScanDuringChallenge:1;
    /*0x3D6E*/ u16 field_3D6E_0f:1;
    /*0x3D6E*/ u16 tag:2;
};

struct MysteryEventStruct
{
    u8 unk_0_0:2;
    u8 unk_0_2:3;
    u8 unk_0_5:3;
    u8 unk_1;
};

 struct WonderNews
{
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04[40];
    u8 unk_2C[10][40];
};

 struct WonderNewsSaveStruct
{
    u32 crc;
    struct WonderNews data;
};

 struct WonderCard
{
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08_0:2;
    u8 unk_08_2:4;
    u8 unk_08_6:2;
    u8 unk_09;
    u8 unk_0A[40];
    u8 unk_32[40];
    u8 unk_5A[4][40];
    u8 unk_FA[40];
    u8 unk_122[40];
};

 struct WonderCardSaveStruct
{
    u32 crc;
    struct WonderCard data;
};

 struct MEventBuffer_3430_Sub
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08[2][7];
};

 struct MEventBuffer_3430
{
    u32 crc;
    struct MEventBuffer_3430_Sub data;
};

 struct MEventBuffers
{
    /*0x000 0x322C*/ struct WonderNewsSaveStruct wonderNews;
    /*0x1c0 0x33EC*/ struct WonderCardSaveStruct wonderCard;
    /*0x310 0x353C*/ struct MEventBuffer_3430 buffer_310;
    /*0x338 0x3564*/ u16 unk_338[4];
    /*0x340 0x356C*/ struct MysteryEventStruct unk_340;
    /*0x344 0x3570*/ u32 unk_344[2][5];
}; // 0x36C 0x3598

struct SaveBlock1
{
    /*0x00*/ struct Coords16 pos;
    /*0x04*/ struct WarpData location;
    /*0x0C*/ struct WarpData continueGameWarp;
    /*0x14*/ struct WarpData dynamicWarp;
    /*0x1C*/ struct WarpData lastHealLocation; // used by white-out and teleport
    /*0x24*/ struct WarpData escapeWarp; // used by Dig and Escape Rope
    /*0x2C*/ u16 savedMusic;
    /*0x2E*/ u8 weather;
    /*0x2F*/ u8 weatherCycleStage;
    /*0x30*/ u8 flashLevel;
    /*0x32*/ u16 mapLayoutId;
    /*0x34*/ u16 mapView[0x100];
    /*0x234*/ u8 playerPartyCount;
    /*0x238*/ struct Pokemon playerParty[PARTY_SIZE];
    /*0x490*/ u32 money;
    /*0x494*/ u16 coins;
    /*0x496*/ u16 registeredItem; // registered for use with SELECT button
    /*0x498*/ struct ItemSlot pcItems[PC_ITEMS_COUNT];
    /*0x560*/ struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
    /*0x5D8*/ struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
    /*0x650*/ struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
    /*0x690*/ struct ItemSlot bagPocket_TMHM[BAG_TMHM_COUNT];
    /*0x790*/ struct ItemSlot bagPocket_Berries[BAG_BERRIES_COUNT];
              struct ItemSlot bagPocket_Medicine[BAG_MEDICINE_COUNT];
              struct ItemSlot bagPocket_BattleItems[BAG_BATTLEITEMS_COUNT];
              struct ItemSlot bagPocket_PowerUp[BAG_POWERUP_COUNT];
    /*0x848*/ struct Pokeblock pokeblocks[POKEBLOCKS_COUNT];

    /*0x9BC*/ u16 berryBlenderRecords[3];
    /*0x9C2*/ u8 field_9C2[6];
    #ifndef FREE_MATCH_CALL
    /*0x9C8*/ u16 trainerRematchStepCounter;    //104 bytes
    /*0x9CA*/ u8 trainerRematches[MAX_REMATCH_ENTRIES];
    #endif
    /*0xA30*/ struct ObjectEvent objectEvents[OBJECT_EVENTS_COUNT];
    /*0xC70*/ struct ObjectEventTemplate objectEventTemplates[OBJECT_EVENT_TEMPLATES_COUNT];
    /*0x1270*/ u8 flags[NUM_FLAG_BYTES];
    /*0x139C*/ u16 vars[VARS_COUNT];
    /*0x159C*/ u32 gameStats[NUM_GAME_STATS];
    /*0x169C*/ struct BerryTree berryTrees[BERRY_TREES_COUNT];
    /*0x1A9C*/ struct SecretBase secretBases[SECRET_BASES_COUNT];
    /*0x271C*/ u8 playerRoomDecorations[DECOR_MAX_PLAYERS_HOUSE];
    /*0x2728*/ u8 playerRoomDecorationPositions[DECOR_MAX_PLAYERS_HOUSE];
    /*0x2734*/ u8 decorationDesks[10];
    /*0x273E*/ u8 decorationChairs[10];
    /*0x2748*/ u8 decorationPlants[10];
    /*0x2752*/ u8 decorationOrnaments[30];
    /*0x2770*/ u8 decorationMats[30];
    /*0x278E*/ u8 decorationPosters[10];
    /*0x2798*/ u8 decorationDolls[40];
    /*0x27C0*/ u8 decorationCushions[10];
    /*0x27CA*/ u8 padding_27CA[2];
    /*0x27CC*/ TVShow tvShows[TV_SHOWS_COUNT];  //900 bytes
    /*0x2B50*/ PokeNews pokeNews[POKE_NEWS_COUNT];
    /*0x2B90*/ u16 outbreakPokemonSpecies;
    /*0x2B92*/ u8 outbreakLocationMapNum;
    /*0x2B93*/ u8 outbreakLocationMapGroup;
    /*0x2B94*/ u8 outbreakPokemonLevel;
    /*0x2B95*/ u8 outbreakUnk1;
    /*0x2B96*/ u16 outbreakUnk2;
    /*0x2B98*/ u16 outbreakPokemonMoves[MAX_MON_MOVES];
    /*0x2BA0*/ u8 outbreakUnk4;
    /*0x2BA1*/ u8 outbreakPokemonProbability;
    /*0x2BA2*/ u16 outbreakDaysLeft;
    /*0x2BA4*/ struct GabbyAndTyData gabbyAndTyData;
    /*0x2BB0*/ u16 easyChatProfile[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BBC*/ u16 easyChatBattleStart[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BC8*/ u16 easyChatBattleWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BD4*/ u16 easyChatBattleLost[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BE0*/ struct MailStruct mail[MAIL_COUNT];
    /*0x2E20*/ u8 additionalPhrases[8]; // bitfield for 33 additional phrases in easy chat system
    /*0x2E28*/ OldMan oldMan;
    /*0x2e64*/ struct EasyChatPair easyChatPairs[5]; //Dewford trend [0] and some other stuff
    /*0x2e90*/ struct ContestWinner contestWinners[NUM_CONTEST_WINNERS]; // see CONTEST_WINNER_*
    /*0x3030*/ struct DayCare daycare;
    #ifndef FREE_LINK_BATTLE_RECORDS
    /*0x3150*/ struct LinkBattleRecords linkBattleRecords;  //88 bytes
    #endif
    /*0x31A8*/ u8 giftRibbons[52];
    /*0x31DC*/ struct Roamer roamer;
    #ifndef FREE_ENIGMA_BERRY
    /*0x31F8*/ struct EnigmaBerry enigmaBerry;  //52 bytes
    #endif
    #ifndef FREE_MYSTERY_EVENT_BUFFERS
    /*0x322C*/ struct MEventBuffers unk_322C;   //876 bytes
    #endif
    #ifndef FREE_FIELD_3598
    /*0x3598*/ u8 field_3598[0x180];    //384 bytes
    #endif
    #ifndef FREE_TRAINER_HILL
    /*0x3718*/ u32 trainerHillTimes[4]; //16 bytes
    #endif
    #ifndef FREE_MYSTERY_EVENT_BUFFERS
    /*0x3728*/ struct RamScript ramScript;
    #endif
    /*0x3B14*/ struct RecordMixingGift recordMixingGift;
    /*0x3B58*/ LilycoveLady lilycoveLady;
    /*0x3B98*/ struct TrainerNameRecord trainerNameRecords[20];
    #ifndef FREE_UNION_ROOM_CHAT
    /*0x3C88*/ u8 registeredTexts[UNION_ROOM_KB_ROW_COUNT][21];      //210 bytes
    #endif
    /*0x3D5A*/ u8 filler3D5A[0xA];    //10 bytes
    /*0x3???*/ u8 dexSeen[DEX_FLAGS_NO];
    /*0x3???*/ u8 dexCaught[DEX_FLAGS_NO];
    #ifndef FREE_TRAINER_HILL
    /*0x3D64*/ struct SaveTrainerHill trainerHill;  //12 bytes
    #endif
    /*0x3D70*/ struct WaldaPhrase waldaPhrase;
               u8 dexNavSearchLevels[NUM_SPECIES];
               u8 dexNavChain;
    // sizeof: 0x3D88
};

extern struct SaveBlock1* gSaveBlock1Ptr;

struct MapPosition
{
    s16 x;
    s16 y;
    s8 height;
};

struct TradeRoomPlayer
{
    u8 playerId;
    u8 isLocalPlayer;
    u8 c;
    u8 facing;
    struct MapPosition pos;
    u16 field_C;
};

#endif // GUARD_GLOBAL_H
