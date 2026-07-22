#include "gtk_rbyedit.h"

GlobalWidgets global_widgets;
SaveInformation save;

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
	GtkWidget *prompt_window;
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

	prompt_window = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(prompt_window), "Choose Item");
	gtk_window_set_modal(GTK_WINDOW(prompt_window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(prompt_window),
			GTK_WINDOW(global_widgets.main_window));

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
	item_entry = gtk_widget_get_parent(item_entry);
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

static GtkWidget *create_stat_edit_box(GtkWidget *tab_vbox, char *name, int stat,
		int stat_iv, int stat_xp, GtkWidget **stat_iv_spin, GtkWidget **stat_xp_spin)
{
	GtkWidget *center_box;
	GtkWidget *vbox, *hbox;
	GtkWidget *stat_iv_spin_button;
	GtkWidget *stat_xp_spin_button;
	GtkWidget *label;
	char buffer[16];

	center_box = gtk_center_box_new();

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);

	snprintf(buffer, sizeof(buffer), "%s: %d", name, stat);
	label = gtk_label_new(buffer);
	gtk_box_append(GTK_BOX(vbox), label);

	
	
	if(stat_iv_spin)
	{
		label = gtk_label_new("IV");
		gtk_box_append(GTK_BOX(hbox), label);
		stat_iv_spin_button = gtk_spin_button_new_with_range(0, 15, 1);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(stat_iv_spin_button), stat_iv);
		gtk_box_append(GTK_BOX(hbox), stat_iv_spin_button);

		*stat_iv_spin = stat_iv_spin_button;
	}

	if(stat_xp_spin)
	{
		label = gtk_label_new("XP");
		gtk_box_append(GTK_BOX(hbox), label);
		stat_xp_spin_button = gtk_spin_button_new_with_range(0, 65535, 1);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(stat_xp_spin_button), stat_xp);
		gtk_box_append(GTK_BOX(hbox), stat_xp_spin_button);

		*stat_xp_spin = stat_xp_spin_button;
	}

	gtk_box_append(GTK_BOX(vbox), hbox);
	gtk_center_box_set_center_widget(GTK_CENTER_BOX(center_box), vbox);
	gtk_box_append(GTK_BOX(tab_vbox), center_box);

	return hbox;
}

static void update_pokemon(GtkWidget *widget, UpdatePokemonParams *params)
{
	GtkWidget *edit_window = gtk_widget_get_ancestor(widget, GTK_TYPE_WINDOW);
	Pokemon *pokemon = params->pokemon;

	int id = gtk_drop_down_get_selected(GTK_DROP_DOWN(params->species_dropdown));
	id++;
	id = get_pokemon_translated_id(id);
	pokemon->id = id;

	char *nickname = 
		gtk_editable_get_text(GTK_EDITABLE(params->name_entry));
	nickname = g_strdup(nickname);
	pokemon->nickname = nickname;

	char *og_trainer_name =
		gtk_editable_get_text(GTK_EDITABLE(params->og_trainer_name_entry));
	og_trainer_name = g_strdup(og_trainer_name);
	pokemon->og_trainer_name = og_trainer_name;

	pokemon->og_trainer_id = 
		atoi(gtk_editable_get_text(GTK_EDITABLE(params->og_trainer_id_entry)));

	int level = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->level_spin_button));
	pokemon->level = level;
	pokemon->xp = xp_required_for_level(level, pokemon->id);

	pokemon->hp_xp = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->hp_stat_xp));

	pokemon->attack_iv = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->attack_stat_iv));
	pokemon->attack_xp = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->attack_stat_xp));

	pokemon->defense_iv = gtk_spin_button_get_value(
			GTK_SPIN_BUTTON(params->defense_stat_iv));
	pokemon->attack_xp = gtk_spin_button_get_value(
			GTK_SPIN_BUTTON(params->attack_stat_xp));

	pokemon->speed_iv = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->speed_stat_iv));
	pokemon->speed_xp = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->speed_stat_xp));

	pokemon->special_iv = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->special_stat_iv));
	pokemon->special_xp = gtk_spin_button_get_value(GTK_SPIN_BUTTON(params->special_stat_xp));

	int move1_id = gtk_drop_down_get_selected(GTK_DROP_DOWN(params->move1_dropdown));
	move1_id = get_move_translated_id(move1_id);

	int move2_id = gtk_drop_down_get_selected(GTK_DROP_DOWN(params->move2_dropdown));
	move2_id = get_move_translated_id(move2_id);

	int move3_id = gtk_drop_down_get_selected(GTK_DROP_DOWN(params->move3_dropdown));
	move3_id = get_move_translated_id(move3_id);

	int move4_id = gtk_drop_down_get_selected(GTK_DROP_DOWN(params->move4_dropdown));
	move4_id = get_move_translated_id(move4_id);

	pokemon->move1_id = move1_id;
	pokemon->move2_id = move2_id;
	pokemon->move3_id = move3_id;
	pokemon->move4_id = move4_id;
	apply_changes_to_pokemon(pokemon);
	
	update_party_tab();
	update_pokemon_box_tab();

	gtk_window_destroy(GTK_WINDOW(edit_window));
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
	GtkWidget *temp_vbox;
	UpdatePokemonParams *update_pokemon_params = malloc(sizeof(UpdatePokemonParams));

	Info pokemon_info = get_pokemon_info(pokemon->id);
	main_window = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW);
	edit_window = gtk_window_new();
	gtk_window_set_default_size(GTK_WINDOW(edit_window), 600, 400);
	gtk_window_set_title(GTK_WINDOW(edit_window), "Edit Pokemon");
	gtk_window_set_modal(GTK_WINDOW(edit_window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(edit_window), GTK_WINDOW(main_window));

	edits_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_append(GTK_BOX(hbox), edits_vbox);
	gtk_widget_set_margin_top(hbox, 16);
	gtk_widget_set_margin_start(hbox, 16);
	gtk_widget_set_margin_bottom(hbox, 16);
	gtk_window_set_child(GTK_WINDOW(edit_window), hbox);

	gchar *full_path = g_build_filename("/", "rbyedit", "assets", "pokemon", pokemon_info.filename, NULL);
	pokemon_image = gtk_picture_new_for_resource(full_path);
	gtk_picture_set_content_fit(GTK_PICTURE(pokemon_image), GTK_CONTENT_FIT_CONTAIN);
	gtk_widget_set_size_request(pokemon_image, 68, 56);
	gtk_box_append(GTK_BOX(edits_vbox), pokemon_image);
	g_free(full_path);

	temp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	name_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(name_entry), 10);
	gtk_editable_set_text(GTK_EDITABLE(name_entry), pokemon->nickname);
	label = gtk_label_new("Name");
	gtk_box_append(GTK_BOX(temp_vbox), label);
	gtk_box_append(GTK_BOX(temp_vbox), name_entry);
	gtk_box_append(GTK_BOX(edits_vbox), temp_vbox);

	GtkStringList *pokemon_strings = gtk_string_list_new(NULL);

	int pokemon_id = pokemon->id;
	for(int i = 1; i <= POKEMON_MAX_ID; i++)
	{
		Info pokemon = get_pokemon_info(i);
		if(pokemon.name)
		{
			gtk_string_list_append(pokemon_strings, pokemon.name);
			if(i == pokemon_id)
				pokemon_id = g_list_model_get_n_items(G_LIST_MODEL(pokemon_strings));
		}
	}

	temp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	species_dropdown = gtk_drop_down_new(G_LIST_MODEL(pokemon_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(species_dropdown), --pokemon_id);
	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(species_dropdown), TRUE);
	label = gtk_label_new("Species");
	gtk_box_append(GTK_BOX(temp_vbox), label);
	gtk_box_append(GTK_BOX(temp_vbox), species_dropdown);
	gtk_box_append(GTK_BOX(edits_vbox), temp_vbox);

	temp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	level_spin_button = gtk_spin_button_new_with_range(1, 100, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(level_spin_button), pokemon->level);
	label = gtk_label_new("Level");
	gtk_box_append(GTK_BOX(temp_vbox), label);
	gtk_box_append(GTK_BOX(temp_vbox), level_spin_button);
	gtk_box_append(GTK_BOX(edits_vbox), temp_vbox);

	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%d", pokemon->og_trainer_id);
	temp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	og_trainer_id_entry = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(og_trainer_id_entry), buffer);
	label = gtk_label_new("OG Trainer ID");
	gtk_box_append(GTK_BOX(temp_vbox), label);
	gtk_box_append(GTK_BOX(temp_vbox), og_trainer_id_entry);
	gtk_box_append(GTK_BOX(edits_vbox), temp_vbox);

	temp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	og_trainer_name_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(og_trainer_name_entry), 7);
	gtk_editable_set_text(GTK_EDITABLE(og_trainer_name_entry), pokemon->og_trainer_name);
	label = gtk_label_new("OG Trainer Name");
	gtk_box_append(GTK_BOX(temp_vbox), label);
	gtk_box_append(GTK_BOX(temp_vbox), og_trainer_name_entry);
	gtk_box_append(GTK_BOX(edits_vbox), temp_vbox);

	notebook = gtk_notebook_new();
	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);
	gtk_widget_set_margin_start(notebook, 16);
	gtk_widget_set_margin_end(notebook, 16);
	gtk_box_append(GTK_BOX(hbox), notebook);

	GtkWidget *stats_tab_label = gtk_label_new("Stats");
	GtkWidget *stats_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), stats_tab_scrolled, stats_tab_label);

	GtkWidget *stat_tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(stats_tab_scrolled), stat_tab_vbox);

	create_stat_edit_box(stat_tab_vbox, "HP", pokemon->hp,
			pokemon->hp_iv, pokemon->hp_xp, NULL, &update_pokemon_params->hp_stat_xp);

	create_stat_edit_box(stat_tab_vbox, "Attack", pokemon->attack, pokemon->attack_iv,
			pokemon->attack_xp, &update_pokemon_params->attack_stat_iv,
			&update_pokemon_params->attack_stat_xp);

	create_stat_edit_box(stat_tab_vbox, "Defense",
			pokemon->defense, pokemon->defense_iv, pokemon->defense_xp,
			&update_pokemon_params->defense_stat_iv, &update_pokemon_params->defense_stat_xp);

	create_stat_edit_box(stat_tab_vbox, "Speed",
			pokemon->speed, pokemon->speed_iv, pokemon->speed_xp,
			&update_pokemon_params->speed_stat_iv, &update_pokemon_params->speed_stat_xp);

	create_stat_edit_box(stat_tab_vbox, "Special",
			pokemon->special, pokemon->special_iv, pokemon->special_xp,
			&update_pokemon_params->special_stat_iv, &update_pokemon_params->special_stat_xp);

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

	label = gtk_label_new("Move 1");
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move1_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);
	update_pokemon_params->move1_dropdown = move_dropdown;

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move2_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);
	update_pokemon_params->move2_dropdown = move_dropdown;

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move3_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);
	update_pokemon_params->move3_dropdown = move_dropdown;

	g_object_ref(move_strings);
	move_dropdown = gtk_drop_down_new(G_LIST_MODEL(move_strings), NULL);
	gtk_drop_down_set_selected(GTK_DROP_DOWN(move_dropdown), pokemon->move4_id);
 	gtk_drop_down_set_enable_search(GTK_DROP_DOWN(move_dropdown), TRUE);
	gtk_box_append(GTK_BOX(moves_tab_vbox), move_dropdown);
	update_pokemon_params->move4_dropdown = move_dropdown;

	exit_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);	
	gtk_box_append(GTK_BOX(edits_vbox), exit_hbox);
	gtk_widget_set_halign(exit_hbox, GTK_ALIGN_CENTER);

	save_button = gtk_button_new_with_label("Apply");
	g_signal_connect(save_button, "clicked", G_CALLBACK(update_pokemon), update_pokemon_params);
	gtk_box_append(GTK_BOX(exit_hbox), save_button);

	cancel_button = gtk_button_new_with_label("Cancel");
	gtk_box_append(GTK_BOX(exit_hbox), cancel_button);
	g_signal_connect_swapped(cancel_button, "clicked", G_CALLBACK(gtk_window_destroy),
			edit_window);

	update_pokemon_params->pokemon = pokemon;
	update_pokemon_params->name_entry = name_entry;
	update_pokemon_params->level_spin_button = level_spin_button;
	update_pokemon_params->og_trainer_id_entry = og_trainer_id_entry;
	update_pokemon_params->og_trainer_name_entry = og_trainer_name_entry;
	update_pokemon_params->species_dropdown = species_dropdown;

	gtk_window_present(GTK_WINDOW(edit_window));
}

static void create_pokemon_tab_entry(GtkWidget *tab_vbox, Pokemon *pokemon_group, int index,
	Pokemon *pokemon)
{
	GtkWidget *entry_center_box;
	GtkWidget *center_box_left_hbox, *center_box_right_hbox;
	GtkWidget *pokemon_image, *name_label;
	GtkWidget *edit_button;

	Info pokemon_info = get_pokemon_info(pokemon->id);

	entry_center_box = gtk_center_box_new();
	gtk_widget_set_hexpand(entry_center_box, TRUE);

	center_box_left_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	center_box_right_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	g_object_set(center_box_right_hbox, "margin-end", 8, NULL);

	gtk_center_box_set_start_widget(GTK_CENTER_BOX(entry_center_box), center_box_left_hbox);
	gtk_center_box_set_end_widget(GTK_CENTER_BOX(entry_center_box), center_box_right_hbox);

	gchar *full_path = g_build_filename("/", "rbyedit", "assets", "pokemon", pokemon_info.filename, NULL);
	pokemon_image = gtk_picture_new_for_resource(full_path);
	gtk_picture_set_content_fit(GTK_PICTURE(pokemon_image), GTK_CONTENT_FIT_CONTAIN);
	gtk_widget_set_size_request(pokemon_image, 68, 56);
	gtk_box_append(GTK_BOX(center_box_left_hbox), pokemon_image);
	g_free(full_path);

	name_label = gtk_label_new(pokemon->nickname);
	gtk_box_append(GTK_BOX(center_box_left_hbox), name_label);

	edit_button = gtk_button_new_from_icon_name("document-edit-symbolic");
	gtk_widget_set_valign(edit_button, GTK_ALIGN_CENTER);
	g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(display_pokemon_edit_window),
			pokemon);
	gtk_box_append(GTK_BOX(center_box_right_hbox), edit_button);

	gtk_box_append(GTK_BOX(tab_vbox), entry_center_box);
}

static void create_item_tab_entry(GtkWidget *tab_vbox, List *item_list, int index,
		int id, int count)
{
	GtkWidget *entry_center_box;
	GtkWidget *center_box_left_hbox, *center_box_right_hbox;
	GtkWidget *item_image, *name_label;
	GtkWidget *count_spin_button, *remove_button;

	Info item = get_item_info(id);

	entry_center_box = gtk_center_box_new();
	gtk_widget_set_hexpand(entry_center_box, TRUE);

	center_box_left_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	center_box_right_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	g_object_set(center_box_right_hbox, "margin-end", 8, NULL);

	gtk_center_box_set_start_widget(GTK_CENTER_BOX(entry_center_box), center_box_left_hbox);
	gtk_center_box_set_end_widget(GTK_CENTER_BOX(entry_center_box), center_box_right_hbox);

	gchar *full_path = g_build_filename("/", "rbyedit", "assets" ,"item", item.filename, NULL);
	item_image = gtk_picture_new_for_resource(full_path);
	gtk_picture_set_content_fit(GTK_PICTURE(item_image), GTK_CONTENT_FIT_CONTAIN);
	gtk_widget_set_size_request(item_image, 32, 32);
	gtk_box_append(GTK_BOX(center_box_left_hbox), item_image);
	g_free(full_path);

	name_label = gtk_label_new(item.name);
	gtk_box_append(GTK_BOX(center_box_left_hbox), name_label);

	count_spin_button = gtk_spin_button_new_with_range(1, 99, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(count_spin_button), count);
	g_signal_connect(count_spin_button, "value-changed", G_CALLBACK(edit_item_count), item_list);
	g_object_set_data(G_OBJECT(count_spin_button), "item-index", GINT_TO_POINTER(index));
	gtk_box_append(GTK_BOX(center_box_right_hbox), count_spin_button);
	g_object_set_data(G_OBJECT(entry_center_box), "spin-button", count_spin_button);
	gtk_widget_set_valign(count_spin_button, GTK_ALIGN_CENTER);

	remove_button = gtk_button_new_from_icon_name("window-close-symbolic");
	g_signal_connect(remove_button, "clicked", G_CALLBACK(delete_item), item_list);
	gtk_box_append(GTK_BOX(center_box_right_hbox), remove_button);
	gtk_widget_set_valign(remove_button, GTK_ALIGN_CENTER);

	gtk_box_append(GTK_BOX(tab_vbox), entry_center_box);
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
		new_item_button = gtk_button_new();
		gtk_button_set_label(GTK_BUTTON(new_item_button), "Add New Item");
		g_signal_connect(new_item_button, "clicked", G_CALLBACK(prompt_new_item), item_list);
		gtk_widget_set_halign(new_item_button, GTK_ALIGN_CENTER);
		gtk_box_append(GTK_BOX(tab_vbox), new_item_button);
	}
}

static void update_party_tab(void)
{
	GtkWidget *tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(
			GTK_SCROLLED_WINDOW(global_widgets.party_tab_scrolled),
			tab_vbox);

	PokemonParty *party = &save.save_data.party;
	for(int i = 0; i < party->count; i++)
	{
		Pokemon *pokemon = party->pokemon + i;
		create_pokemon_tab_entry(tab_vbox, party->pokemon, i, pokemon);
	}
}

static void update_pokemon_box_tab(void)
{
	GtkWidget *tab_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_scrolled_window_set_child(
			GTK_SCROLLED_WINDOW(global_widgets.pokemon_box_tab_scrolled),
			tab_vbox);

	PokemonBox *boxes = save.save_data.pokemon_boxes;
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
	if(!save.save_mem) return;

	save.save_data.player_name = gtk_editable_get_text(
			GTK_EDITABLE(global_widgets.player_name_entry));
	save.save_data.rival_name = gtk_editable_get_text(
			GTK_EDITABLE(global_widgets.rival_name_entry));
	save.save_data.money = atoi(gtk_editable_get_text(
				GTK_EDITABLE(global_widgets.money_entry)));

	update_save(save.save_mem, save.save_data);
	g_file_replace_contents(save.file, (char *)save.save_mem, save.length, NULL,
			0, G_FILE_CREATE_REPLACE_DESTINATION, NULL, NULL,
			NULL);
}

static void load_file(GObject *file_dialog, GAsyncResult *result, gpointer window)
{
	if(save.save_mem)
	{
		g_free(save.save_mem);
		save.save_mem = NULL;
	}
	if(save.file)
	{
		g_object_unref(save.file);
		save.file = NULL;
	}

	GError *error = NULL;
	char str[12];

	save.file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(file_dialog), result, &error);

	if(!save.file)
	{
		return;
	}

	g_file_load_contents(save.file, NULL, (char **)&save.save_mem, &save.length, NULL, &error); 

	if(error)
	{
		GtkAlertDialog *dialog = gtk_alert_dialog_new("An unexpected error occurred");
		gtk_alert_dialog_set_detail (dialog, error->message);
		gtk_alert_dialog_show (dialog, GTK_WINDOW (window));
		g_object_unref (dialog);
	}

	g_object_unref(file_dialog);

	save.save_data = get_save_data(save.save_mem);

	gtk_editable_set_text(
			GTK_EDITABLE(global_widgets.player_name_entry),
			save.save_data.player_name);
	gtk_editable_set_text(
			GTK_EDITABLE(global_widgets.rival_name_entry),
			save.save_data.rival_name);

	snprintf(str, sizeof(str), "%d", save.save_data.money);
	gtk_editable_set_text(GTK_EDITABLE(global_widgets.money_entry), str);

	update_item_tab(global_widgets.bag_tab_scrolled, &save.save_data.bag);
	update_item_tab(global_widgets.item_box_tab_scrolled, &save.save_data.box_items);
	update_party_tab();
	update_pokemon_box_tab();

	gtk_window_set_child(GTK_WINDOW(window), global_widgets.main_vbox);
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
	global_widgets.main_window = window;

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
	global_widgets.main_vbox = vbox;

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

	global_widgets.player_name_entry = create_save_edit_entry(save_edits_vbox, "Player Name");
	gtk_entry_set_max_length(GTK_ENTRY(global_widgets.player_name_entry), 7);

	global_widgets.rival_name_entry = create_save_edit_entry(save_edits_vbox, "Rival Name");
	gtk_entry_set_max_length(GTK_ENTRY(global_widgets.rival_name_entry), 7);

	// money edit needs to be spin button
	GtkWidget *money_vbox, *money_label;
	
	money_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	money_label = gtk_label_new("Money");
	global_widgets.money_entry  = gtk_spin_button_new_with_range(0, 999999, 10);
	gtk_box_append(GTK_BOX(money_vbox), money_label);
	gtk_box_append(GTK_BOX(money_vbox), global_widgets.money_entry);
	gtk_box_append(GTK_BOX(save_edits_vbox), money_vbox);

	notebook = gtk_notebook_new();
	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);
	gtk_widget_set_margin_bottom(notebook, 16);
	gtk_widget_set_margin_start(notebook, 16);
	gtk_widget_set_margin_end(notebook, 16);
	gtk_box_append(GTK_BOX(hbox), notebook);

	bag_tab_label = gtk_label_new("Bag Items");
	global_widgets.bag_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), global_widgets.bag_tab_scrolled,
			bag_tab_label);

	item_box_tab_label = gtk_label_new("Pc Items");
	global_widgets.item_box_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), global_widgets.item_box_tab_scrolled,
			item_box_tab_label);

	party_tab_label = gtk_label_new("Party");
	global_widgets.party_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), global_widgets.party_tab_scrolled,
			party_tab_label);

	pokemon_box_tab_label = gtk_label_new("Pokemon Boxes");
	global_widgets.pokemon_box_tab_scrolled = gtk_scrolled_window_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), global_widgets.pokemon_box_tab_scrolled,
			pokemon_box_tab_label);

	GtkWidget *first_vbox = gtk_center_box_new();
	GtkWidget *open_file_button = gtk_button_new_with_label("Open File");
	gtk_widget_set_valign(open_file_button, GTK_ALIGN_CENTER);
	gtk_center_box_set_center_widget(GTK_CENTER_BOX(first_vbox), open_file_button);
	g_signal_connect_swapped(open_file_button, "clicked", G_CALLBACK(open_file), window);
	gtk_window_set_child(GTK_WINDOW(window), first_vbox);

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
