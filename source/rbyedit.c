#include <stdlib.h>
#include <string.h>
#include "rbyedit.h"
#include "rbychar.c"

static int intsqrt(int n)
{
	if (n == 0 || n == 1) return n;
    
    long long start = 1, end = n / 2;
    long long ans = 0;
    
    while (start <= end) {
        long long mid = (start + end) / 2;
        
        if (mid * mid == n) return mid;
        
        if (mid * mid < n) {
            start = mid + 1;
            ans = mid; // Store potential floor
        } else {
            end = mid - 1;
        }
    }
    return ans;
}

static uint16_t get_int16(uint8_t *save, int address)
{
	uint16_t value = *(uint16_t *)(save + address);
	value = __builtin_bswap16(value);
	return value;
}

static uint32_t get_int24(uint8_t *save, int address)
{
	// TODO: make this and get_int16 safe
	uint32_t value = *(uint32_t *)(save + address);
	value = __builtin_bswap32(value);
	value = value >> 8;
	return value;
}

// TODO: make set_int functions safe
static void set_int16(uint8_t *save, int address, uint16_t value)
{
	save[address] = (value >> 8) & 0xFF;
	save[address + 1] = value & 0xFF;
}

static void set_int24(uint8_t *save, int address, uint32_t value)
{
	save[address] = (value >> 16) & 0xFF;
	save[address + 1] = (value >> 8) & 0xFF;
	save[address + 2] = value & 0xFF;
}

int xp_required_for_level(int level, int id)
{
	PokemonBaseStats base = get_pokemon_base_stats(id);

	uint32_t n = level;
	uint32_t n3 = n * n * n;
	uint32_t n2 = n * n;

	switch(base.growth_rate)
	{
		case FAST:
			return (4 * n3) / 5;

		case MEDIUM_FAST:
			return n3;

		case MEDIUM_SLOW:
			int32_t cube_term = (6 * (int32_t)n3) / 5;
			int32_t square_term = 15 * (int32_t)n2;
			int32_t linear_term = 100 * (int32_t)n;

			int32_t total = cube_term - square_term + linear_term - 140;

			return total;

		case SLOW:
			return (5 * n3) / 4;
	}

	return 0;
}

// There is a bug in the game where the medium slow formula can result in a negative number,
// which overflows the 24 bit value into being very larg.
// this function does not emulate that
// TODO: should this function emulate the underflow bug?
static uint8_t get_level(int xp, GrowthRate rate)
{
	for(int i = 1; i < 100; i++)
	{
		int xp_needed = xp_required_for_level(i, rate);
		if(xp < xp_needed) return i - 1;
	}

	return 100;
}

static int calculate_stat(int base, int iv, int stat_xp, int level)
{
	return ((((base + iv) * 2 + (intsqrt(stat_xp) / 4)) * level) / 100) + 5;
}

static void set_derived_values(Pokemon *pokemon)
{
	PokemonBaseStats base = get_pokemon_base_stats(pokemon->id);
	int level = get_level(pokemon->xp, pokemon->id);
	pokemon->level = level;
	
	pokemon->hp = ((((base.hp + pokemon->hp_iv) * 2 + (intsqrt(pokemon->hp_xp) / 4)) * level) / 100)
	+ level + 10;

	pokemon->attack = calculate_stat(base.attack, pokemon->attack_iv, pokemon->attack_xp, level);
	pokemon->defense = calculate_stat(base.defense, pokemon->defense_iv, pokemon->defense_xp, level);
	pokemon->speed = calculate_stat(base.speed, pokemon->speed_iv, pokemon->speed_xp, level);
	pokemon->special = calculate_stat(base.special, pokemon->special_iv, pokemon->special_xp, level);
}

static Pokemon get_pokemon(uint8_t *save, int address)
{
	Pokemon pokemon;
	pokemon.id = save[address];
	pokemon.current_hp = get_int16(save, address + 0x01);
	pokemon.pc_level = save[address + 0x03];
	pokemon.status =  save[address + 0x04];
	pokemon.type1 = save[address + 0x05];
	pokemon.type2 = save[address + 0x06];
	pokemon.catch_rate = save[address + 0x07];
	pokemon.move1_id = save[address + 0x08];
	pokemon.move2_id = save[address + 0x09];
	pokemon.move3_id = save[address + 0x0A];
	pokemon.move4_id = save[address + 0x0B];
	pokemon.og_trainer_id = get_int16(save, address + 0x0C);
	pokemon.xp = get_int24(save, address + 0X0E);
	pokemon.hp_xp = get_int16(save, address + 0x11);
	pokemon.attack_xp = get_int16(save, address + 0x13);
	pokemon.defense_xp = get_int16(save, address + 0x15);
	pokemon.speed_xp = get_int16(save, address + 0x17);
	pokemon.special_xp = get_int16(save, address + 0x19);

	
	//  each iv is four bits in size, and hp iv is composed of the
	//  least significant bit of attack, defense, speed, and special ivs
	//  in that order.
	 
	uint16_t iv_values = get_int16(save, address + 0x1B);
	
	uint8_t attack_iv = (iv_values >> 12) & 0x0F;
	uint8_t defense_iv = (iv_values >> 8) & 0x0F;
	uint8_t speed_iv = (iv_values >> 4) & 0x0F;
	uint8_t special_iv = iv_values & 0x0F;

	pokemon.iv = iv_values;

	pokemon.hp_iv =
		((attack_iv & 0x01) << 3)  |
		((defense_iv & 0x01) << 2) |
		((speed_iv & 0x01) << 1)   |
		(special_iv & 0x01);

	pokemon.attack_iv = attack_iv;
	pokemon.defense_iv = defense_iv;
	pokemon.speed_iv = speed_iv;
	pokemon.special_iv = special_iv;

	pokemon.move1_pp = save[address + 0x1D];
	pokemon.move2_pp = save[address + 0x1E];
	pokemon.move3_pp = save[address + 0x1F];
	pokemon.move4_pp = save[address + 0x20];

	// nickname and trainer name are stored seperatly from the pokemon data,
	// so the caller needs to set them.
	pokemon.nickname = NULL;
	pokemon.og_trainer_name = NULL;

	set_derived_values(&pokemon);

	return pokemon;
}

static List get_list(uint8_t *save, int address, int size)
{
	List list;
	list.count = save[address];
	list.entries = malloc(size * sizeof(ListEntry));

	address++;
	for(int i = 0; i < list.count; i++)
	{
		ListEntry entry;
		entry.id = save[address];
		entry.count = save[address + 1];
		list.entries[i] = entry;
		address += 2;
	}

	return list;
}

static int get_money(uint8_t *save)
{
	uint32_t bcd = get_int24(save, MONEY_ADDR);
	
	int money = 0;
	int multiplier = 1;

	while(bcd > 0)
	{
		money += multiplier * (bcd & 0x0F);
		bcd = bcd >> 4;
		multiplier *= 10;
	}

	return money;
}

static char *get_string(uint8_t *save, int address)
{
	int size;
	for(size = 0; save[address + size] != RBY_CHAR_TERMINATOR; size++);

	char *rby_string = malloc(size + 1);

	int i;
	for(i = 0; i < size; i++)
	{
		rby_string[i] = save[address + i];
	}
	rby_string[i] = RBY_CHAR_TERMINATOR;

	char *ascii_sring = rby_to_ascii(rby_string);
	free(rby_string);
	return ascii_sring;
}

static PokemonParty get_party(uint8_t *save)
{
	PokemonParty party;
	int address;

	party.count = save[PARTY_DATA_ADDR];
	party.pokemon = malloc(6 * sizeof(Pokemon));

	address = PARTY_DATA_ADDR + 0x08;
	for(int i = 0; i < party.count; i++)
	{
		party.pokemon[i] = get_pokemon(save, address);
		address += 44;
	}
	
	address = PARTY_DATA_ADDR + 0x110;
	for(int i = 0; i < party.count; i++)
	{
		party.pokemon[i].og_trainer_name = get_string(save, address);
		address += 11;
	}

	address = PARTY_DATA_ADDR + 0x152;
	for(int i = 0; i < party.count; i++)
	{
		party.pokemon[i].nickname = get_string(save, address);
		address += 11;
	}

	return party;
}

static PokemonBox get_pokemon_box(uint8_t *save, int address)
{
	PokemonBox box;
	box.count = save[address];
	box.pokemon = malloc(20 * sizeof(Pokemon));

	int addr = address + 0x16;
	for(int i = 0; i < box.count; i++)
	{
		box.pokemon[i] = get_pokemon(save, addr);
		addr += 0x21;
	}

	addr = address + 0x2AA;
	for(int i = 0; i < box.count; i++)
	{
		box.pokemon[i].og_trainer_name = get_string(save, addr);
		addr += 0xB;
	}

	addr = address + 0x386;
	for(int i = 0; i < box.count; i++)
	{
		box.pokemon[i].nickname = get_string(save, addr);
		addr += 0xB;
	}

	return box;
}

//TODO: function seems to segfault if player has never opened a box before
static PokemonBox *get_pokemon_boxes(uint8_t *save)
{
	PokemonBox *boxes = malloc(12 * sizeof(PokemonBox));

	// only the first 7 bits represent the box number, bit 8 checks whether the player
	// has changed boxes before.
	int active_box = save[CURRENT_BOX_ADDR] & 0x7F;

	int address = BANK_2_ADDR;
	for(int i = 0; i < 12; i++)
	{
		// the boxes are split up in different banks and we need to jump
		// to bank 3 when we get to index 6
		if(i == 6) address = BANK_3_ADDR;

		if(i == active_box)
		{
			boxes[i] = get_pokemon_box(save, CURRENT_BOX_DATA_ADDR);
		}
		else
		{
			boxes[i] = get_pokemon_box(save, address);
		}

		address += 0x462;
	}

	return boxes;
}

static void set_checksum(uint8_t *save)
{
	uint8_t checksum = 0;

	for(int addr = BOX_1_CHECKSUM_START_ADDR; addr <= BOX_1_CHECKSUM_END_ADDR; addr++)
	{
		checksum += save[addr];
	}

	save[BOX_1_CHECKSUM_ADDR] = ~checksum;
}

// TODO: update to use set_int24
static void set_money(uint8_t *save, uint32_t amount)
{
	if(amount > 999999) amount = 999999;
	
	uint32_t bcd = 0;
	int shift = 0;
	while(amount > 0)
	{
		bcd |= (amount % 10) << shift;
		amount /= 10;
		shift += 4;
	}

	save[MONEY_ADDR] = bcd >> 16;
	save[MONEY_ADDR + 1] = bcd >> 8;
	save[MONEY_ADDR + 2] = bcd;
}

static void write_list(uint8_t *save, int address, List list)
{
	save[address] = list.count;
	int addr = address + 1;

	if(list.count == 0)
	{
		save[addr] = 0xFF;
	}
	else
	{
		for(int i = 0; i < list.count; i++)
		{
			ListEntry entry = list.entries[i];
			save[addr] = entry.id;
			save[addr + 1] = entry.count;
			addr += 2;
		}
	}

	save[address + (2 * list.count) + 1] = 0XFF;
}

static void write_string(uint8_t *save, int address, char *ascii_string)
{
	char *rby_string = ascii_to_rby(ascii_string);

	int i;
	for(i = 0; rby_string[i] != RBY_CHAR_TERMINATOR; i++)
	{
		save[address + i] = rby_string[i];
	}

	save[address + i] = RBY_CHAR_TERMINATOR;
	free(rby_string);
}

static void write_pokemon33(uint8_t *save, int address, Pokemon pokemon)
{
	save[address] = pokemon.id;
	set_int16(save, address + 0x01, pokemon.current_hp);
	save[address + 0x03] = pokemon.pc_level;
	save[address + 0x04] = pokemon.status;
	save[address + 0x05] = pokemon.type1;
	save[address + 0x06] = pokemon.type2;
	save[address + 0x07] = pokemon.catch_rate;
	save[address + 0x08] = pokemon.move1_id;
	save[address + 0x09] = pokemon.move2_id;
	save[address + 0x0A] = pokemon.move3_id;
	save[address + 0x0B] = pokemon.move4_id;
	set_int16(save, address + 0x0C, pokemon.og_trainer_id);
	set_int24(save, address + 0x0E, pokemon.xp);
	set_int16(save, address + 0x11, pokemon.hp_xp);
	set_int16(save, address + 0x13, pokemon.attack_xp);
	set_int16(save, address + 0x15, pokemon.defense_xp);
	set_int16(save, address + 0x17, pokemon.speed_xp);
	set_int16(save, address + 0x19, pokemon.special_xp);

	uint16_t iv_values = (pokemon.attack_iv << 12) | (pokemon.defense_iv << 8)
			  | (pokemon.speed_iv << 4) | pokemon.special_iv;

	set_int16(save, address + 0x1B, iv_values);
	save[address + 0x1D] = pokemon.move1_pp;
	save[address + 0x1E] = pokemon.move2_pp;
	save[address + 0x1F] = pokemon.move3_pp;
	save[address + 0x20] = pokemon.move4_pp;
}

static void write_pokemon44(uint8_t *save, int address, Pokemon pokemon)
{
	// TODO maybe we should be setting derived values when editing pokemon
	set_derived_values(&pokemon);

	write_pokemon33(save, address, pokemon);
	save[address + 0x21] = pokemon.level;
	set_int16(save, address + 0x22, pokemon.hp);
	set_int16(save, address + 0x24, pokemon.attack);
	set_int16(save, address + 0x26, pokemon.defense);
	set_int16(save, address + 0x28, pokemon.speed);
	set_int16(save, address + 0x2A, pokemon.special);
}

static void set_party(uint8_t *save, PokemonParty party)
{
	int address = PARTY_DATA_ADDR;
	save[address++] = party.count;

	int i;
	for(i = 0; i < party.count; i++, address++)
	{
		save[address] = party.pokemon[i].id;
	}
	save[address] = 0xFF;

	address = PARTY_DATA_ADDR + 0x8;
	for(i = 0; i < party.count; i++)
	{
		write_pokemon44(save, address, party.pokemon[i]);
		address += 0x2C;
	}
	
	address = PARTY_DATA_ADDR + 0x110;
	for(i = 0; i < party.count; i++)
	{
		write_string(save, address, party.pokemon[i].og_trainer_name);
		address += 0xB;
	}

	address = PARTY_DATA_ADDR + 0x152;
	for(i = 0; i < party.count; i++)
	{
		write_string(save, address, party.pokemon[i].nickname);
		address += 0xB;
	}
}

static void write_box(uint8_t *save, int box_address, PokemonBox box)
{
	int address = box_address;
	save[address++] = box.count;

	int i;
	for(i = 0; i < box.count; i++, address++)
	{
		save[address] = box.pokemon[i].id;
	}
	save[address] = 0xFF;

	address = box_address + 0x16;
	for(i = 0; i < box.count; i++)
	{
		write_pokemon33(save, address, box.pokemon[i]);
		address += 0x21;
	}

	address = box_address + 0x2AA;
	for(i = 0; i < box.count; i++)
	{
		write_string(save, address, box.pokemon[i].og_trainer_name);
		address += 0xB;
	}
	
	address = box_address + 0x386;
	for(i = 0; i < box.count; i++)
	{
		write_string(save, address, box.pokemon[i].nickname);
		address += 0xB;
	}
}

static void set_boxes(uint8_t *save, PokemonBox *boxes)
{
	int active_box = save[CURRENT_BOX_ADDR] & 0x7F;

	int address = BANK_2_ADDR;
	for(int i = 0; i < 12; i++)
	{
		// the boxes are split up in different banks and we need to jump
		// to bank 3 when we get to index 6
		if(i == 6) address = BANK_3_ADDR;

		if(i == active_box)
		{
			write_box(save, CURRENT_BOX_DATA_ADDR, boxes[i]);		
		}
		else
		{
			write_box(save, address, boxes[i]);
		}

		address += 0x462;
	}
}

void update_save(uint8_t *save, SaveData save_data)
{
	set_money(save, save_data.money);
	write_string(save, PLAYER_NAME_ADDR, save_data.player_name);
	write_string(save, RIVAL_NAME_ADDR, save_data.rival_name);
	write_list(save, BAG_ITEMS_ADDR, save_data.bag);
	write_list(save, BOX_ITEMS_ADDR, save_data.box_items);
	set_party(save, save_data.party);
	set_boxes(save, save_data.pokemon_boxes);
	set_checksum(save);
}

SaveData get_save_data(uint8_t *save)
{
	// TODO: check if the passed save is a valid rby save file.
	// TODO: free old save_data memory
	SaveData save_data;
	
	save_data.rival_name = get_string(save, RIVAL_NAME_ADDR);
	save_data.player_name = get_string(save, PLAYER_NAME_ADDR);
	save_data.money = get_money(save);
	save_data.bag = get_list(save, BAG_ITEMS_ADDR, 20);
	save_data.box_items = get_list(save, BOX_ITEMS_ADDR, 50);
	save_data.party = get_party(save);
	save_data.pokemon_boxes = get_pokemon_boxes(save);

	return save_data;
}
