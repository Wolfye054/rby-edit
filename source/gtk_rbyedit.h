#include <gtk/gtk.h>
#include "rbyedit.h"

typedef struct
{
	GtkWidget *main_window,
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
} UpdatePokemonParams;

static void update_item_tab(GtkWidget *tab_scrolled, List *item_list);
static void update_party_tab(void);
static void update_pokemon_box_tab(void);
