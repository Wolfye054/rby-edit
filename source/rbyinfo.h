#include <stdint.h>

/* 
 * Addresses for various values and data structures in the North American
 * Pokemon Red, Blue, and Yellow games.
 *
 * A full breakdown of the save data structure can be can be found at:
 * https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_I)
 */
#define BANK_0_ADDR 0x0


#define BANK_1_ADDR 0x2000
#define PLAYER_NAME_ADDR 0x2598
#define BAG_ITEMS_ADDR 0x25C9
#define MONEY_ADDR 0x25F3
#define RIVAL_NAME_ADDR 0x25F6
#define BOX_ITEMS_ADDR 0x27E6
#define CURRENT_BOX_ADDR 0x284C
#define PARTY_DATA_ADDR 0x2F2C
#define CURRENT_BOX_DATA_ADDR 0x30C0
#define BOX_1_CHECKSUM_ADDR 0x3523
#define BOX_1_CHECKSUM_START_ADDR 0x2598
#define BOX_1_CHECKSUM_END_ADDR 0x3522


#define BANK_2_ADDR 0x4000


#define BANK_3_ADDR 0x6000

typedef enum 
{
	SLOW,
	MEDIUM_SLOW,
	MEDIUM_FAST,
	FAST
} GrowthRate;

typedef struct
{
	char *name;
	char *filename;
} Info;

typedef struct
{
	uint8_t hp;
	uint8_t attack;
	uint8_t defense;
	uint8_t speed;
	uint8_t special;
	GrowthRate growth_rate;
} PokemonBaseStats;

Info get_item_info(int id);
Info get_pokemon_info(int id);
PokemonBaseStats get_pokemon_base_stats(int id);
char *get_move_name(int id);
