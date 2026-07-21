#include <gtk/gtk.h>
#include "rbyedit.h"

typedef struct
{
	GtkWidget *main_window, *main_vbox,
			  *player_name_entry, *rival_name_entry, *money_entry,
	          *bag_tab_scrolled, *item_box_tab_scrolled,
			  *party_tab_scrolled, *pokemon_box_tab_scrolled;
} GlobalWidgets;

typedef struct
{
	GFile *file;
	gsize length;
	uint8_t *save_mem;
	SaveData save_data;
} SaveInformation;

typedef struct
{
	Pokemon *pokemon;
	GtkWidget *name_entry;
	GtkWidget *level_spin_button;
	GtkWidget *og_trainer_id_entry;
	GtkWidget *og_trainer_name_entry;
	GtkWidget *species_dropdown;
	GtkWidget *hp_stat_xp;
	GtkWidget *attack_stat_iv;
	GtkWidget *attack_stat_xp;
	GtkWidget *defense_stat_iv;
	GtkWidget *defense_stat_xp;
	GtkWidget *speed_stat_iv;
	GtkWidget *speed_stat_xp;
	GtkWidget *special_stat_iv;
	GtkWidget *special_stat_xp;
	GtkWidget *move1_dropdown;
	GtkWidget *move2_dropdown;
	GtkWidget *move3_dropdown;
	GtkWidget *move4_dropdown;
} UpdatePokemonParams;

static void update_item_tab(GtkWidget *tab_scrolled, List *item_list);
static void update_party_tab(void);
static void update_pokemon_box_tab(void);
