#include <gtk/gtk.h>
#include "rbyedit.h"

typedef struct
{
	Pokemon *pokemon;

	GtkWidget *name_entry;
	GtkWidget *level_spin_button;
	GtkWidget *og_trainer_id_entry;
	GtkWidget *og_trainer_name_entry;
	GtkWidget *species_dropdown;


	GtkWidget *hp_iv_spin_button;
	GtkWidget *hp_xp_spin_button;

	GtkWidget *attack_iv_spin_button;
	GtkWidget *attack_xp_spin_button;

	GtkWidget *defense_iv_spin_button;
	GtkWidget *defense_xp_spin_button;

	GtkWidget *speed_iv_spin_button;
	GtkWidget *speed_xp_spin_button;

	GtkWidget *special_iv_spin_button;
	GtkWidget *special_xp_spin_button;


	GtkWidget *move1_dropdown;
	GtkWidget *move2_dropdown;
	GtkWidget *move3_dropdown;
	GtkWidget *move4_dropdown;
} PokemonEditDisplay;

GFile *file;
gsize length;
uint8_t *save;
SaveData save_data;

// TODO: get rid of these
GtkWidget *main_window;
GtkWidget *player_name_entry, *rival_name_entry, *money_entry;
GtkWidget *bag_tab_scrolled, *item_box_tab_scrolled;
GtkWidget *party_tab_scrolled, *pokemon_box_tab_scrolled;

static void update_item_tab(GtkWidget *tab_scrolled, List *item_list);

// there are gaps in the pokemon ids that need to be skipped over
static int get_translated_id(int id)
{
	int translated_id;
	for(translated_id = 1;; translated_id++)
	{
		if(get_pokemon_info(translated_id).name && --id == 0)
				break;
	}

	return translated_id;
}

static void add_prompted_item(GtkButton *button, List *item_list)
{
	GtkWidget *prompt_window = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW);
	GtkWidget *vbox = gtk_window_get_child(GTK_WINDOW(prompt_window));
	GtkWidget *dropdown = gtk_widget_get_first_child(vbox);
	GtkWidget *tab_scrolled = g_object_get_data(G_OBJECT(button), "tab-scrolled");

	int selection_index = gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown));
	selection_index++;
	int id;
	for(id = 1;; id++)
	{
		if(get_item_info(id).name && --selection_index == 0)
		{
			break;
		}
	}

	int index = item_list->count++;
	item_list->entries[index].id = id;
	item_list->entries[index].count = 1;
	
	update_item_tab(tab_scrolled, item_list);
	gtk_window_destroy(GTK_WINDOW(prompt_window));
}

static void prompt_new_item(GtkButton *button, List *item_list)
{
	GtkWidget *prompt_window, *main_window;
	GtkWidget *dropdown, *submit_button;
	GtkWidget *vbox;
	GtkWidget *tab_scrolled;
	GtkStringList *item_strings;

	item_strings = gtk_string_list_new(NULL);
	for(int i = 1; i <= 256; i++)
	{
		Info item = get_item_info(i);
		if(item.name)
		{
			gtk_string_list_append(item_strings, item.name);
		}
	}

	main_window = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW);
	prompt_window = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(prompt_window), "Choose Item");
	gtk_window_set_modal(GTK_WINDOW(prompt_window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(prompt_window), GTK_WINDOW(main_window));

 	dropdown = gtk_drop_down_new(G_LIST_MODEL(item_strings), NULL);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(dropdown), TRUE);

	tab_scrolled = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_SCROLLED_WINDOW);
	submit_button = gtk_button_new_with_label("Submit");
	g_signal_connect(submit_button, "clicked", G_CALLBACK(add_prompted_item), item_list);
	g_object_set_data(G_OBJECT(submit_button), "tab-scrolled", tab_scrolled);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_window_set_child(GTK_WINDOW(prompt_window), vbox);
	gtk_box_append(GTK_BOX(vbox), dropdown);
	gtk_box_append(GTK_BOX(vbox), submit_button);

	gtk_window_present(GTK_WINDOW(prompt_window));
}

static void delete_item(GtkButton *button, List *item_list)
{
	GtkWidget *item_entry = gtk_widget_get_parent(GTK_WIDGET(button));
	GtkWidget *item_spin_button = g_object_get_data(G_OBJECT(item_entry), "spin-button");

	GtkWidget *tab_scrolled = gtk_widget_get_ancestor(item_entry, GTK_TYPE_SCROLLED_WINDOW);
	int index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(item_spin_button), "item-index"));

	for(int i = index; i < item_list->count - 1; i++)
	{
		item_list->entries[i] = item_list->entries[i + 1];
	}
	item_list->count--;

	update_item_tab(tab_scrolled, item_list);
}

static void edit_item_count(GtkEditable *item_spin_button, List *item_list) 
{
	int index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(item_spin_button), "item-index"));
	int count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(item_spin_button));

	item_list->entries[index].count = count;
}

static void create_stat_edit_hbox(GtkWidget *tab_vbox, char *name, int stat,
		int stat_iv, int stat_xp)
{
	GtkWidget *hbox;
	GtkWidget *stat_spin_button;
	GtkWidget *stat_iv_spin_button;
	GtkWidget *stat_xp_spin_button;
	GtkWidget *label;
	char buffer[16];

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	
	label = gtk_label_new(name);
	gtk_box_append(GTK_BOX(hbox), label);
	stat_spin_button = gtk_spin_button_new_with_range(1, 1000, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(stat_spin_button), stat);
	gtk_box_append(GTK_BOX(hbox), stat_spin_button);

	snprintf(buffer, sizeof(buffer), "%s IV", name);
	label = gtk_label_new(buffer);
	gtk_box_append(GTK_BOX(hbox), label);
	stat_iv_spin_button = gtk_spin_button_new_with_range(0, 15, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(stat_iv_spin_button), stat_iv);
	gtk_box_append(GTK_BOX(hbox), stat_iv_spin_button);

	snprintf(buffer, sizeof(buffer), "%s XP", name);
	label = gtk_label_new(buffer);
	gtk_box_append(GTK_BOX(hbox), label);
	stat_xp_spin_button = gtk_spin_button_new_with_range(0, 65535, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(stat_xp_spin_button), stat_xp);
	gtk_box_append(GTK_BOX(hbox), stat_xp_spin_button);

	gtk_box_append(GTK_BOX(tab_vbox), hbox);
}

static void update_pokemon(GtkWidget *widget, PokemonEditDisplay *pokemon_edits)
{
	// TODO: free memory

	GtkWidget *edit_window = gtk_widget_get_ancestor(widget, GTK_TYPE_WINDOW);

	Pokemon *pokemon = pokemon_edits->pokemon;

	int id = gtk_drop_down_get_selected(GTK_DROP_DOWN(pokemon_edits->species_dropdown));
	id++;
	id = get_translated_id(id);
	pokemon->id = id;

	char *nickname = 
		gtk_editable_get_text(GTK_EDITABLE(pokemon_edits->name_entry));
	nickname = g_strdup(nickname);
	pokemon->nickname = nickname;

	char *og_trainer_name =
		gtk_editable_get_text(GTK_EDITABLE(pokemon_edits->og_trainer_name_entry));
	og_trainer_name = g_strdup(og_trainer_name);
	pokemon->og_trainer_name = og_trainer_name;

	pokemon->og_trainer_id = 
		atoi(gtk_editable_get_text(GTK_EDITABLE(pokemon_edits->og_trainer_id_entry)));


	int level = gtk_spin_button_get_value(GTK_SPIN_BUTTON(pokemon_edits->level_spin_button));
	pokemon->level = level;
	pokemon->xp = xp_required_for_level(level, pokemon->id);
	
	gtk_window_destroy(GTK_WINDOW(edit_window));
	//TODO pokemon stats and moves
}

static void display_pokemon_edit_window(GtkButton *button, Pokemon *pokemon)
{
	GtkWidget *edit_window, *main_window;
	GtkWidget *hbox, *exit_hbox, *edits_vbox;
	GtkWidget *pokemon_image;
	GtkWidget *name_entry;
	GtkWidget *level_spin_button;
	GtkWidget *label;
	GtkWidget *og_trainer_id_entry, *og_trainer_name_entry;
	GtkWidget *notebook;
	GtkWidget *species_dropdown;
	GtkWidget *save_button, *cancel_button;

	Info pokemon_info = get_pokemon_info(pokemon->id);
	main_window = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW);
	edit_window = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(edit_window), "Edit Pokemon");
	gtk_window_set_modal(GTK_WINDOW(edit_window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(edit_window), GTK_WINDOW(main_window));

	edits_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_window_set_child(GTK_WINDOW(edit_window), hbox);
	gtk_box_append(GTK_BOX(hbox), edits_vbox);

	gchar *full_path = g_build_filename("..", "assets", "pokemon", pokemon_info.filename, NULL);
	GFile *file = g_file_new_for_path(full_path);
	pokemon_image = gtk_picture_new_for_file(file);
	gtk_picture_set_content_fit(GTK_PICTURE(pokemon_image), GTK_CONTENT_FIT_CONTAIN);
	gtk_widget_set_size_request(pokemon_image, 68, 56);
	gtk_box_append(GTK_BOX(edits_vbox), pokemon_image);
	g_free(full_path);

	name_entry = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(name_entry), pokemon->nickname);
	label = gtk_label_new("Name");
	gtk_box_append(GTK_BOX(edits_vbox), label);
	gtk_box_append(GTK_BOX(edits_vbox), name_entry);

	GtkStringList *pokemon_strings = gtk_string_list_new(NULL);

	int pokemon_id = pokemon->id;
	for(int i = 1; i <= 256; i++)
	{
		Info pokemon = get_pokemon_info(i);
		if(pokemon.name)
		{
			gtk_string_list_append(pokemon_strings, pokemon.name);
			if(i == pokemon_id)
				pokemon_id = g_list_model_get_n_items(G_LIST_MODEL(pokemon_strings));
		}
	}

	species_dropdown = gtk_drop_down_new(G_LIST_MODEL(pokemon_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(species_dropdown), --pokemon_id);
	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(species_dropdown), TRUE);
	label = gtk_label_new("Species");
	gtk_box_append(GTK_BOX(edits_vbox), label);
	gtk_box_append(GTK_BOX(edits_vbox), species_dropdown);

	level_spin_button = gtk_spin_button_new_with_range(1, 100, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(level_spin_button), pokemon->level);
	label = gtk_label_new("Level");
	gtk_box_append(GTK_BOX(edits_vbox), label);
	gtk_box_append(GTK_BOX(edits_vbox), level_spin_button);

	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%d", pokemon->og_trainer_id);
	og_trainer_id_entry = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(og_trainer_id_entry), buffer);
	label = gtk_label_new("OG Trainer ID");
	gtk_box_append(GTK_BOX(edits_vbox), label);
	gtk_box_append(GTK_BOX(edits_vbox), og_trainer_id_entry);

	og_trainer_name_entry = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(og_trainer_name_entry), pokemon->og_trainer_name);
	label = gtk_label_new("OG Trainer Name");
	gtk_box_append(GTK_BOX(edits_vbox), label);
	gtk_box_append(GTK_BOX(edits_vbox), og_trainer_name_entry);

	notebook = gtk_notebook_new();
	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);
	gtk_widget_set_margin_bottom(notebook, 16);
	gtk_widget_set_margin_start(notebook, 16);
	gtk_widget_set_margin_end(notebook, 16);
	gtk_box_append(GTK_BOX(hbox), notebook);

	GtkWidget *stats_tab_label = gtk_label_new("Stats");
	GtkWidget *stats_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), stats_tab_scrolled, stats_tab_label);

	GtkWidget *stat_tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(stats_tab_scrolled), stat_tab_vbox);

	create_stat_edit_hbox(stat_tab_vbox, "HP", pokemon->hp,
			pokemon->hp_iv, pokemon->hp_xp);
	create_stat_edit_hbox(stat_tab_vbox, "Attack", pokemon->attack,
			pokemon->attack_iv, pokemon->attack_xp);
	create_stat_edit_hbox(stat_tab_vbox, "Defense", pokemon->defense,
			pokemon->defense_iv, pokemon->defense_xp);
	create_stat_edit_hbox(stat_tab_vbox, "Speed", pokemon->speed,
			pokemon->speed_iv, pokemon->speed_xp);
	create_stat_edit_hbox(stat_tab_vbox, "Special", pokemon->special,
			pokemon->special_iv, pokemon->special_xp);

	GtkWidget *moves_tab_label = gtk_label_new("Moves");
	GtkWidget *moves_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), moves_tab_scrolled, moves_tab_label);

	GtkStringList *move_strings = gtk_string_list_new(NULL);
	gtk_string_list_append(move_strings, "None");
	for(int i = 1; i <= 165; i++)
	{
		gtk_string_list_append(move_strings, get_move_name(i));
	}

	GtkWidget *moves_tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(moves_tab_scrolled), moves_tab_vbox);

	GtkWidget *move_dropdown;

	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move1_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move2_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
		gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move3_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move4_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);

	exit_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);	
	gtk_box_append(GTK_BOX(edits_vbox), exit_hbox);

	PokemonEditDisplay *pokemon_edit = malloc(sizeof(PokemonEditDisplay));
	pokemon_edit->pokemon = pokemon;
	pokemon_edit->name_entry = name_entry;
	pokemon_edit->level_spin_button = level_spin_button;
	pokemon_edit->og_trainer_id_entry = og_trainer_id_entry;
	pokemon_edit->og_trainer_name_entry = og_trainer_name_entry;
	pokemon_edit->species_dropdown = species_dropdown;

	save_button = gtk_button_new_with_label("Apply Changes");
	g_signal_connect(save_button, "clicked", G_CALLBACK(update_pokemon), pokemon_edit);
	gtk_box_append(GTK_BOX(exit_hbox), save_button);

	cancel_button = gtk_button_new_with_label("Cancel");
	gtk_box_append(GTK_BOX(exit_hbox), cancel_button);
	g_signal_connect_swapped(cancel_button, "clicked", G_CALLBACK(gtk_window_destroy),
			edit_window);
gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);

	gtk_window_present(GTK_WINDOW(edit_window));
}

static void create_pokemon_tab_entry(GtkWidget *tab_vbox, Pokemon *pokemon_group,
	int index, Pokemon *pokemon)
{
	GtkWidget *entry_hbox;
	GtkWidget *pokemon_image, *name_label;
	GtkWidget *edit_button;

	Info pokemon_info = get_pokemon_info(pokemon->id);

	entry_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_hexpand(entry_hbox, TRUE);

	gchar *full_path = g_build_filename("..", "assets", "pokemon", pokemon_info.filename, NULL);
	GFile *file = g_file_new_for_path(full_path);
	pokemon_image = gtk_picture_new_for_file(file);
	gtk_picture_set_content_fit(GTK_PICTURE(pokemon_image), GTK_CONTENT_FIT_CONTAIN);
	gtk_widget_set_size_request(pokemon_image, 68, 56);
	gtk_box_append(GTK_BOX(entry_hbox), pokemon_image);
	g_free(full_path);

	name_label = gtk_label_new(pokemon->nickname);
	gtk_box_append(GTK_BOX(entry_hbox), name_label);

	edit_button = gtk_button_new_from_icon_name("document-edit-symbolic");
	g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(display_pokemon_edit_window),
			pokemon);
	gtk_box_append(GTK_BOX(entry_hbox), edit_button);

	gtk_box_append(GTK_BOX(tab_vbox), entry_hbox);
}

static void create_item_tab_entry(GtkWidget *tab_vbox, List *item_list, int index,
		int id, int count)
{
	GtkWidget *entry_hbox;
	GtkWidget *item_image, *name_label;
	GtkWidget *count_spin_button, *remove_button;

	Info item = get_item_info(id);

	entry_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_hexpand(entry_hbox, TRUE);

	gchar *full_path = g_build_filename("..", "assets", "item", item.filename, NULL);
	item_image = gtk_image_new_from_file(full_path);
	gtk_image_set_pixel_size(GTK_IMAGE(item_image), 64);
	gtk_box_append(GTK_BOX(entry_hbox), item_image);
	g_free(full_path);

	name_label = gtk_label_new(item.name);
	gtk_box_append(GTK_BOX(entry_hbox), name_label);

	count_spin_button = gtk_spin_button_new_with_range(1, 99, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(count_spin_button), count);
	g_signal_connect(count_spin_button, "value-changed", G_CALLBACK(edit_item_count), item_list);
	g_object_set_data(G_OBJECT(count_spin_button), "item-index", GINT_TO_POINTER(index));
	gtk_box_append(GTK_BOX(entry_hbox), count_spin_button);
	g_object_set_data(G_OBJECT(entry_hbox), "spin-button", count_spin_button);

	remove_button = gtk_button_new_from_icon_name("window-close-symbolic");
	g_signal_connect(remove_button, "clicked", G_CALLBACK(delete_item), item_list);
	gtk_box_append(GTK_BOX(entry_hbox), remove_button);

	gtk_box_append(GTK_BOX(tab_vbox), entry_hbox);
}

static void update_item_tab(GtkWidget *tab_scrolled, List *item_list)
{
	GtkWidget *tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(tab_scrolled), tab_vbox);

	for(int i = 0; i < item_list->count; i++)
	{
		ListEntry item = item_list->entries[i];
		create_item_tab_entry(tab_vbox, item_list, i, item.id, item.count);
	}

	if(item_list->count < 20)
	{
		GtkWidget *new_item_button;
		new_item_button = gtk_button_new_from_icon_name("list-add-symbolic");
		g_signal_connect(new_item_button, "clicked", G_CALLBACK(prompt_new_item), item_list);
		gtk_box_append(GTK_BOX(tab_vbox), new_item_button);
	}
}

static void update_party_tab(GtkWidget *tab_scrolled, PokemonParty *party)
{
	GtkWidget *tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(tab_scrolled), tab_vbox);

	for(int i = 0; i < party->count; i++)
	{
		Pokemon *pokemon = party->pokemon + i;
		create_pokemon_tab_entry(tab_vbox, party->pokemon, i, pokemon);
	}
}

static void update_pokemon_box_tab(GtkWidget *tab_scrolled, PokemonBox *boxes)
{
	GtkWidget *tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(tab_scrolled), tab_vbox);

	char buffer[10];
	for(int i = 0; i < 12; i++)
	{
		if(boxes[i].count > 0)
		{
			snprintf(buffer, sizeof(buffer), "Box %d", i + 1);
			GtkWidget *box_label = gtk_label_new(buffer);
			gtk_box_append(GTK_BOX(tab_vbox), box_label);

			for(int j = 0; j < boxes[i].count; j++)
			{
				Pokemon *pokemon = boxes[i].pokemon + j;
				create_pokemon_tab_entry(tab_vbox, boxes[i].pokemon, i, pokemon);
			}
		}
	}
}

static GtkWidget *create_save_edit_entry(GtkWidget *save_edits_vbox, char *name)
{
	GtkWidget *save_edit_vbox, *label, *entry;
	
	save_edit_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	label = gtk_label_new(name);
	entry = gtk_entry_new();
	gtk_widget_set_name(entry, name);
	gtk_box_append(GTK_BOX(save_edit_vbox), label);
	gtk_box_append(GTK_BOX(save_edit_vbox), entry);
	gtk_box_append(GTK_BOX(save_edits_vbox), save_edit_vbox);

	return entry;
}

static void save_file()
{
	if(!save) return;

	// TODO: make sure player name is not longer than the game allows
	save_data.player_name = gtk_editable_get_text(GTK_EDITABLE(player_name_entry));
	save_data.rival_name = gtk_editable_get_text(GTK_EDITABLE(rival_name_entry));
	save_data.money = atoi(gtk_editable_get_text(GTK_EDITABLE(money_entry)));

	update_save(save, save_data);
	g_file_replace_contents(file, (char *)save, length, NULL,
			0, G_FILE_CREATE_REPLACE_DESTINATION, NULL, NULL,
			NULL);
}

static void load_file(GObject *file_dialog, GAsyncResult *result, gpointer window)
{
	if(save)
	{
		g_free(save);
		save = NULL;
	}
	if(file)
	{
		g_object_unref(file);
		file = NULL;
	}

	GError *error = NULL;
	char str[12];

	file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(file_dialog), result, &error);

	if(!file)
	{
		return;
	}

	g_file_load_contents(file, NULL, (char **)&save, &length, NULL, &error); 

	if(error)
	{
		GtkAlertDialog *dialog = gtk_alert_dialog_new("An unexpected error occurred");
		gtk_alert_dialog_set_detail (dialog, error->message);
		gtk_alert_dialog_show (dialog, GTK_WINDOW (window));
		g_object_unref (dialog);
	}

	g_object_unref(file_dialog);

	save_data = get_save_data(save);

	gtk_editable_set_text(GTK_EDITABLE(player_name_entry), save_data.player_name);
	gtk_editable_set_text(GTK_EDITABLE(rival_name_entry), save_data.rival_name);

	snprintf(str, sizeof(str), "%d", save_data.money);
	gtk_editable_set_text(GTK_EDITABLE(money_entry), str);

	update_item_tab(bag_tab_scrolled, &save_data.bag);
	update_item_tab(item_box_tab_scrolled, &save_data.box_items);
	update_party_tab(party_tab_scrolled, &save_data.party);
	update_pokemon_box_tab(pokemon_box_tab_scrolled, save_data.pokemon_boxes);
}

static void open_file(GtkWindow *window)
{
	GtkFileDialog *file_dialog;

	file_dialog = gtk_file_dialog_new();
	gtk_file_dialog_open(file_dialog, window, 0, load_file, window);
}

static void app_activate(GApplication *app)
{
	GtkWidget *window;
	GtkWidget *vbox, *hbox;
	GtkWidget *save_edits_vbox;
	GtkWidget *toolbar;
	GtkWidget *openf_button, *savef_button;
	GtkWidget *notebook;
	GtkWidget *bag_tab_label, *item_box_tab_label;
	GtkWidget *party_tab_label, *pokemon_box_tab_label;

	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "rby edit");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
	main_window = window;

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
	gtk_window_set_child(GTK_WINDOW(main_window), vbox);

	toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	gtk_widget_add_css_class(toolbar, "toolbar");
	gtk_box_append(GTK_BOX(vbox), toolbar);

	openf_button = gtk_button_new_from_icon_name("document-open");
	g_signal_connect_swapped(openf_button, "clicked", G_CALLBACK(open_file), window);
	gtk_box_append(GTK_BOX(toolbar), openf_button);

	savef_button = gtk_button_new_from_icon_name("document-save");
	g_signal_connect(savef_button, "clicked", G_CALLBACK(save_file), NULL);
	gtk_box_append(GTK_BOX(toolbar), savef_button);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_hexpand(hbox, TRUE);
	gtk_widget_set_vexpand(hbox, TRUE);
	gtk_box_append(GTK_BOX(vbox), hbox);

	save_edits_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_widget_set_margin_start(save_edits_vbox, 16);
	gtk_box_append(GTK_BOX(hbox), save_edits_vbox);

	player_name_entry = create_save_edit_entry(save_edits_vbox, "Player Name");
	rival_name_entry = create_save_edit_entry(save_edits_vbox, "Rival Name");
	money_entry = create_save_edit_entry(save_edits_vbox, "Money");

	notebook = gtk_notebook_new();
	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);
	gtk_widget_set_margin_bottom(notebook, 16);
	gtk_widget_set_margin_start(notebook, 16);
	gtk_widget_set_margin_end(notebook, 16);
	gtk_box_append(GTK_BOX(hbox), notebook);

	bag_tab_label = gtk_label_new("Bag Items");
	bag_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), bag_tab_scrolled, bag_tab_label);

	item_box_tab_label = gtk_label_new("Pc Items");
	item_box_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), item_box_tab_scrolled, item_box_tab_label);

	party_tab_label = gtk_label_new("Party");
	party_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), party_tab_scrolled, party_tab_label);

	pokemon_box_tab_label = gtk_label_new("Pokemon Boxes");
	pokemon_box_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), pokemon_box_tab_scrolled,
			pokemon_box_tab_label);

	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **args)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.github.Wolfye054.rbyedit", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, args);

	return status;
}
