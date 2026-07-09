#include <stdint.h>
#include "rbyinfo.h"

#define RBY_CHAR_TERMINATOR 0x50
#define RBY_CHAR_UNKNOWN 0xE6

typedef struct
{
	uint8_t id;
	uint16_t current_hp;
	uint8_t pc_level;
	uint8_t status;
	uint8_t type1;
	uint8_t type2;
	uint8_t catch_rate;
	uint8_t move1_id;
	uint8_t move2_id;
	uint8_t move3_id;
	uint8_t move4_id;
	uint16_t og_trainer_id;
	uint32_t xp;
	uint16_t hp_xp;
	uint16_t attack_xp;
	uint16_t defense_xp;
	uint16_t speed_xp;
	uint16_t special_xp;
	uint8_t iv;
	uint8_t hp_iv;
	uint8_t attack_iv;
	uint8_t defense_iv;
	uint8_t speed_iv;
	uint8_t special_iv;
	uint8_t move1_pp;
	uint8_t move2_pp;
	uint8_t move3_pp;
	uint8_t move4_pp;
	char *nickname;
	char *og_trainer_name;

	// derived stats
	uint8_t level;
	uint16_t hp;
	uint16_t attack;
	uint16_t defense;
	uint16_t speed;
	uint16_t special;
} Pokemon;

typedef struct
{
	int count;
	Pokemon *pokemon;
} PokemonParty;

typedef struct
{
	int count;
	Pokemon *pokemon;
} PokemonBox;

typedef struct
{
	int id;
	int count;
} ListEntry;

typedef struct
{
	int count;
	ListEntry *entries;
} List;

typedef struct
{
	char *player_name;
	char *rival_name;
	int money;
	List bag;
	List box_items;
	PokemonParty party;
	PokemonBox *pokemon_boxes;
} SaveData;

SaveData get_save_data(uint8_t *save);
void update_save(uint8_t *save, SaveData save_data);
int xp_required_for_level(int level, int id);
