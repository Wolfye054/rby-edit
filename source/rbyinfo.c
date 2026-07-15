#include "rbyinfo.h"

static Info items[256] =
{
	[1] = {.name="Master Ball", .filename="master-ball.png"},
	[2] = {.name="Ultra Ball", .filename="ultra-ball.png"},
	[3] = {.name="Great Ball", .filename="great-ball.png"},
	[4] = {.name="Poke Ball", .filename="poke-ball.png"},
	[5] = {.name="Town Map", .filename="town-map.png"},
	[6] = {.name="Bicycle", .filename="bicycle.png"},
	[8] = {.name="Safari Ball", .filename="safari-ball.png"},
	[10] = {.name="Moon Stone", .filename="moon-stone.png"},
	[11] = {.name="Antidote", .filename="antidote.png"},
	[12] = {.name="Burn Heal", .filename="burn-heal.png"},
	[13] = {.name="Ice Heal", .filename="ice-heal.png"},
	[14] = {.name="Awakening", .filename="awakening.png"},
	[15] = {.name="Parlyz Heal", .filename="parlyz-heal.png"},
	[16] = {.name="Full Restore", .filename="full-restore.png"},
	[17] = {.name="Max Potion", .filename="max-potion.png"},
	[18] = {.name="Hyper Potion", .filename="hyper-potion.png"},
	[19] = {.name="Super Potion", .filename="super-potion.png"},
	[20] = {.name="Potion", .filename="potion.png"},
	[29] = {.name="Escape Rope", .filename="escape-rope.png"},
	[30] = {.name="Repel", .filename="repel.png"},
	[31] = {.name="Old Amber", .filename="old-amber.png"},
	[32] = {.name="Fire Stone", .filename="fire-stone.png"},
	[33] = {.name="Thunder stone", .filename="thunder-stone.png"},
	[34] = {.name="Water Stone", .filename="water-stone.png"},
	[35] = {.name="HP Up", .filename="hp-up.png"},
	[36] = {.name="Protein", .filename="protein.png"},
	[37] = {.name="Iron", .filename="iron.png"},
	[38] = {.name="Carbos", .filename="carbos.png"},
	[39] = {.name="Calcium", .filename="calcium.png"},
	[40] = {.name="Rare Candy", .filename="rare-candy.png"},
	[41] = {.name="Dome Fossil", .filename="dome-fossil.png"},
	[42] = {.name="Helix Fossil", .filename="helix-fossil.png"},
	[43] = {.name="Secret Key", .filename="secret-key.png"},
	[45] = {.name="Bike Voucher", .filename="bike-voucher.png"},
	[46] = {.name="X Accuracy", .filename="x-accuracy.png"},
	[47] = {.name="Leaf Stone", .filename="leaf-stone.png"},
	[48] = {.name="Card Key", .filename="card-key.png"},
	[49] = {.name="Nugget", .filename="nugget.png"},
	[51] = {.name="Poke Doll", .filename="poke-doll.png"},
	[52] = {.name="Full Heal", .filename="full-heal.png"},
	[53] = {.name="Revive", .filename="revive.png"},
	[54] = {.name="Max Revive", .filename="max-revive.png"},
	[55] = {.name="Guard Spec.", .filename="guard-spec.png"},
	[56] = {.name="Super Repel", .filename="super-repel.png"},
	[57] = {.name="Max Repel", .filename="max-repel.png"},
	[58] = {.name="Dire Hit", .filename="dire-hit.png"},
	[60] = {.name="Fresh Water", .filename="fresh-water.png"},
	[61] = {.name="Soda Pop", .filename="soda-pop.png"},
	[62] = {.name="Lemonade", .filename="lemonade.png"},
	[63] = {.name="S.S. Ticket", .filename="ss-ticket.png"},
	[64] = {.name="Gold Teeth", .filename="gold-teeth.png"},
	[65] = {.name="X Attack", .filename="x-attack.png"},
	[66] = {.name="X Defend", .filename="x-defend.png"},
	[67] = {.name="X Speed", .filename="x-speed.png"},
	[68] = {.name="X Special", .filename="x-special.png"},
	[69] = {.name="Coin Case", .filename="coin-case.png"},
	[70] = {.name="Oak's Parcel", .filename="oaks-parcel.png"},
	[71] = {.name="Item Finder", .filename="itemfinder.png"},
	[72] = {.name="Silph Scope", .filename="silph-scope.png"},
	[73] = {.name="Poke Flute", .filename="poke-flute.png"},
	[74] = {.name="Lift Key", .filename="lift-key.png"},
	[75] = {.name="Exp. All", .filename="exp-all.png"},
	[76] = {.name="Old Rod", .filename="old-rod.png"},
	[77] = {.name="Good Rod", .filename="good-rod.png"},
	[78] = {.name="Super Rod", .filename="super-rod.png"},
	[79] = {.name="PP Up", .filename="pp-up.png"},
	[80] = {.name="Ether", .filename="ether.png"},
	[81] = {.name="Max Ether", .filename="max-ether.png"},
	[82] = {.name="Elixer", .filename="elixer.png"},
	[83] = {.name="Max Elixer", .filename="max-elixer.png"},
	[196] = {.name="HM01", .filename="normal.png"},
	[197] = {.name="HM02", .filename="flying.png"},
	[198] = {.name="HM03", .filename="water.png"},
	[199] = {.name="HM04", .filename="normal.png"},
	[200] = {.name="HM05", .filename="normal.png"},
	[201] = {.name="TM01", .filename="normal.png"},
	[202] = {.name="TM02", .filename="normal.png"},
	[203] = {.name="TM03", .filename="normal.png"},
	[204] = {.name="TM04", .filename="normal.png"},
	[205] = {.name="TM05", .filename="normal.png"},
	[206] = {.name="TM06", .filename="poison.png"},
	[207] = {.name="TM07", .filename="normal.png"},
	[208] = {.name="TM08", .filename="normal.png"},
	[209] = {.name="TM09", .filename="normal.png"},
	[210] = {.name="TM10", .filename="normal.png"},
	[211] = {.name="TM11", .filename="water.png"},
	[212] = {.name="TM12", .filename="water.png"},
	[213] = {.name="TM13", .filename="ice.png"},
	[214] = {.name="TM14", .filename="ice.png"},
	[215] = {.name="TM15", .filename="normal.png"},
	[216] = {.name="TM16", .filename="normal.png"},
	[217] = {.name="TM17", .filename="fighting.png"},
	[218] = {.name="TM18", .filename="fighting.png"},
	[219] = {.name="TM19", .filename="fighting.png"},
	[220] = {.name="TM20", .filename="normal.png"},
	[221] = {.name="TM21", .filename="grass.png"},
	[222] = {.name="TM22", .filename="grass.png"},
	[223] = {.name="TM23", .filename="dragon.png"},
	[224] = {.name="TM24", .filename="electric.png"},
	[225] = {.name="TM25", .filename="electric.png"},
	[226] = {.name="TM26", .filename="ground.png"},
	[227] = {.name="TM27", .filename="ground.png"},
	[228] = {.name="TM28", .filename="ground.png"},
	[229] = {.name="TM29", .filename="psychic.png"},
	[230] = {.name="TM30", .filename="psychic.png"},
	[231] = {.name="TM31", .filename="normal.png"},
	[232] = {.name="TM32", .filename="normal.png"},
	[233] = {.name="TM33", .filename="psychic.png"},
	[234] = {.name="TM34", .filename="normal.png"},
	[235] = {.name="TM35", .filename="normal.png"},
	[236] = {.name="TM36", .filename="normal.png"},
	[237] = {.name="TM37", .filename="normal.png"},
	[238] = {.name="TM38", .filename="fire.png"},
	[239] = {.name="TM39", .filename="normal.png"},
	[240] = {.name="TM40", .filename="normal.png"},
	[241] = {.name="TM41", .filename="normal.png"},
	[242] = {.name="TM42", .filename="psychic.png"},
	[243] = {.name="TM43", .filename="flying.png"},
	[244] = {.name="TM44", .filename="psychic.png"},
	[245] = {.name="TM45", .filename="electric.png"},
	[246] = {.name="TM46", .filename="psychic.png"},
	[247] = {.name="TM47", .filename="normal.png"},
	[248] = {.name="TM48", .filename="rock.png"},
	[249] = {.name="TM49", .filename="normal.png"},
	[250] = {.name="TM50", .filename="normal.png"},
};

Info pokemon[] = 
{
	[1] = {.name="Rhydon", .filename="rhydon.png"},
	[2] = {.name="Kangaskhan", .filename="kangaskhan.png"},
	[3] = {.name="Nidoran M", .filename="nidoran-m.png"},	
	[4] = {.name="Clefairy", .filename="clefairy.png"},
	[5] = {.name="Spearow", .filename="spearow.png"},
	[6] = {.name="Voltorb", .filename="voltorb.png"},
	[7] = {.name="Nidoking", .filename="nidoking.png"},
	[8] = {.name="Slowbro", .filename="slowbro.png"},
	[9] = {.name="Ivysaur", .filename="ivysaur.png"},
	[10] = {.name="Exeggutor", .filename="exeggutor.png"},
	[11] = {.name="Lickitung", .filename="lickitung.png"},
	[12] = {.name="Exeggcute", .filename="exeggcute.png"},
	[13] = {.name="Grimer", .filename="grimer.png"},
	[14] = {.name="Gengar", .filename="gengar.png"},
	[15] = {.name="Nidoran F", .filename="nidoran-f.png"},
	[16] = {.name="Nidoqueen", .filename="nidoqueen.png"},
	[17] = {.name="Cubone", .filename="cubone.png"},
	[18] = {.name="Rhyhorn", .filename="rhyhorn.png"},
	[19] = {.name="Lapras", .filename="lapras.png"},
	[20] = {.name="Arcanine", .filename="arcanine.png"},
	[21] = {.name="Mew", .filename="mew.png"},
	[22] = {.name="Gyarados", .filename="gyarados.png"},
	[23] = {.name="Shellder", .filename="shellder.png"},
	[24] = {.name="Tentacool", .filename="tentacool.png"},
	[25] = {.name="Gastly", .filename="gastly.png"},
	[26] = {.name="Scyther", .filename="scyther.png"},
	[27] = {.name="Staryu", .filename="staryu.png"},
	[28] = {.name="Blastoise", .filename="blastoise.png"},
	[29] = {.name="Pinsir", .filename="pinsir.png"},
	[30] = {.name="Tangela", .filename="tangela.png"},
	[33] = {.name="Growlithe", .filename="growlithe.png"},
	[34] = {.name="Onix", .filename="onix.png"},
	[35] = {.name="Fearow", .filename="fearow.png"},
	[36] = {.name="Pidgey", .filename="pidgey.png"},
	[37] = {.name="Slowpoke", .filename="slowpoke.png"},
	[38] = {.name="Kadabra", .filename="kadabra.png"},
	[39] = {.name="Graveler", .filename="graveler.png"},
	[40] = {.name="Chansey", .filename="chansey.png"},
	[41] = {.name="Machoke", .filename="machoke.png"},
	[42] = {.name="Mr. Mime", .filename="mr-mime.png"},
	[43] = {.name="Hitmonlee", .filename="hitmonlee.png"},
	[44] = {.name="Hitmonchan", .filename="hitmonchand.png"},
	[45] = {.name="Arbok", .filename="arbok.png"},
	[46] = {.name="Parasect", .filename="parasect.png"},
	[47] = {.name="Psyduck", .filename="psyduck.png"},
	[48] = {.name="Drowzee", .filename="drowzee.png"},
	[49] = {.name="Golem", .filename="golem.png"},
	[51] = {.name="Magmar", .filename="magmar.png"},
	[53] = {.name="Electabuzz", .filename="electabuzz.png"},
	[54] = {.name="Magneton", .filename="magneton.png"},
	[55] = {.name="Koffing", .filename="koffing.png"},
	[57] = {.name="Mankey", .filename="mankey.png"},
	[58] = {.name="Seel", .filename="seel.png"},
	[59] = {.name="Diglett", .filename="diglett.png"},
	[60] = {.name="Tauros", .filename="tauros.png"},
	[64] = {.name="Farfetch'd", .filename="farfetchd.png"},
	[65] = {.name="Venonat", .filename="venonat.png"},
	[66] = {.name="Dragonite", .filename="dragonite.png"},
	[70] = {.name="Doduo", .filename="doduo.png"},
	[71] = {.name="Poliwag", .filename="poliwag.png"},
	[72] = {.name="Jynx", .filename="jynx.png"},
	[73] = {.name="Moltres", .filename="moltres.png"},
	[74] = {.name="Articuno", .filename="articuno.png"},
	[75] = {.name="Zapdos", .filename="zapdos.png"},
	[76] = {.name="Ditto", .filename="ditto.png"},
	[77] = {.name="Meowth", .filename="meowth.png"},
	[78] = {.name="Krabby", .filename="krabby.png"},
	[82] = {.name="Vulpix", .filename="vulpix.png"},
	[83] = {.name="Ninetales", .filename="ninetales.png"},
	[84] = {.name="Pikachu", .filename="pikachu.png"},
	[85] = {.name="Raichu", .filename="raichu.png"},
	[88] = {.name="Dratini", .filename="dratini.png"},
	[89] = {.name="Dragonair", .filename="dragonair.png"},
	[90] = {.name="Kabuto", .filename="kabuto.png"},
	[91] = {.name="Kabutops", .filename="kabutops.png"},
	[92] = {.name="Horsea", .filename="horsea.png"},
	[93] = {.name="Seadra", .filename="seadra.png"},
	[96] = {.name="Sandshrew", .filename="sandshrew.png"},
	[97] = {.name="Sandslash", .filename="sandslash.png"},
	[98] = {.name="Omanyte", .filename="omanyte.png"},
	[99] = {.name="Omastar", .filename="omastar.png"},
	[100] = {.name="Jigglypuff", .filename="jugglypuff.png"},
	[101] = {.name="Wigglytuff", .filename="wigglytuff.png"},
	[102] = {.name="Eevee", .filename="eevee.png"},
	[103] = {.name="Flareon", .filename="flareon.png"},
	[104] = {.name="Jolteon", .filename="jolteon.png"},
	[105] = {.name="Vaporeon", .filename="vaporeon.png"},
	[106] = {.name="Machop", .filename="machop.png"},
	[107] = {.name="Zubat", .filename="zubat.png"},
	[108] = {.name="Ekans", .filename="ekans.png"},
	[109] = {.name="Paras", .filename="paras.png"},
	[110] = {.name="Poliwhirl", .filename="poliwhirl.png"},
	[111] = {.name="Poliwrath", .filename="poliwrath.png"},
	[112] = {.name="Weedle", .filename="weedle.png"},
	[113] = {.name="Kakuna", .filename="kakuna.png"},
	[114] = {.name="Beedrill", .filename="beedrill.png"},
	[116] = {.name="Dodrio", .filename="dodrio.png"},
	[117] = {.name="Primeape", .filename="primeape.png"},
	[118] = {.name="Dugtrio", .filename="dugtrio.png"},
	[119] = {.name="Venomoth", .filename="venomoth.png"},
	[120] = {.name="Dewgong", .filename="dewgong.png"},
	[123] = {.name="Caterpie", .filename="caterpie.png"},
	[124] = {.name="Metapod", .filename="Metapod.png"},
	[125] = {.name="Butterfree", .filename="butterfree.png"},
	[126] = {.name="Machamp", .filename="machamp.png"},
	[128] = {.name="Golduck", .filename="golduck.png"},
	[129] = {.name="Hypno", .filename="hypno.png"},
	[130] = {.name="Golbat", .filename="golbat.png"},
	[131] = {.name="Mewtwo", .filename="mewtwo.png"},
	[132] = {.name="Snorlax", .filename="snorlax.png"},
	[133] = {.name="Magikarp", .filename="magikarp.png"},
	[136] = {.name="Muk", .filename="muk.png"},
	[138] = {.name="Kingler", .filename="kingler.png"},
	[139] = {.name="Cloyster", .filename="cloyster.png"},
	[141] = {.name="Electrode", .filename="electrode.png"},
	[142] = {.name="Clefable", .filename="clefable.png"},
	[143] = {.name="Weezing", .filename="weezing.png"},
	[144] = {.name="Persian", .filename="persian.png"},
	[145] = {.name="Marowak", .filename="marowak.png"},
	[147] = {.name="Haunter", .filename="haunter.png"},
	[148] = {.name="Abra", .filename="abra.png"},
	[149] = {.name="Alakazam", .filename="alakazam.png"},
	[150] = {.name="Pidgeotto", .filename="pidgeotto.png"},
	[151] = {.name="Pidgeot", .filename="pidgeot.png"},
	[152] = {.name="Starmie", .filename="starmie.png"},
	[153] = {.name="Bulbasaur", .filename="bulbasaur.png"},
	[154] = {.name="Venusaur", .filename="venusaur.png"},
	[155] = {.name="Tentacruel", .filename="tentacruel.png"},
	[157] = {.name="Goldeen", .filename="goldeen.png"},
	[158] = {.name="Seaking", .filename="seaking.png"},
	[163] = {.name="Ponyta", .filename="ponyta.png"},
	[164] = {.name="Rapidash", .filename="rapidash.png"},
	[165] = {.name="Rattata", .filename="rattata.png"},
	[166] = {.name="Raticate", .filename="raticate.png"},
	[167] = {.name="Nidorino", .filename="nidorino.png"},
	[168] = {.name="Nidorina", .filename="nidorina.png"},
	[169] = {.name="Geodude", .filename="geodude.png"},
	[170] = {.name="Porygon", .filename="porygon.png"},
	[171] = {.name="Aerodactyl", .filename="aerodactyl.png"},
	[173] = {.name="Magnemite", .filename="magnemite.png"},
	[176] = {.name="Charmander", .filename="charmander.png"},
	[177] = {.name="Squirtle", .filename="squirtle.png"},
	[178] = {.name="Charmeleon", .filename="charmeleon.png"},
	[179] = {.name="Wartortle", .filename="wartortle.png"},
	[180] = {.name="Charizard", .filename="charizard.png"},
	[185] = {.name="Oddish", .filename="oddish.png"},
	[186] = {.name="Gloom", .filename="gloom.png"},
	[187] = {.name="Vileplume", .filename="vileplume.png"},
	[188] = {.name="Bellsprout", .filename="bellsprout.png"},
	[189] = {.name="Weepinbell", .filename="weepinbell.png"},
	[190] = {.name="Victreebel", .filename="victreebel.png"},
};

static PokemonBaseStats pokemon_stats[] =
{
	[1] = {.hp=105, .attack=130, .defense=120, .speed=40, .special=45, .growth_rate=SLOW}, // Rhydon
	[2] = {.hp=105, .attack=95, .defense=80, .speed=90, .special=40, .growth_rate=MEDIUM_FAST}, // Kangaskhan
	[3] = {.hp=46, .attack=57, .defense=40, .speed=50, .special=40, .growth_rate=MEDIUM_SLOW}, // Nidoran M
	[4] = {.hp=70, .attack=45, .defense=48, .speed=35, .special=60, .growth_rate=FAST}, // Clefairy
	[5] = {.hp=40, .attack=60, .defense=30, .speed=70, .special=31, .growth_rate=MEDIUM_FAST}, // Spearow
	[6] = {.hp=40, .attack=30, .defense=50, .speed=100, .special=55, .growth_rate=MEDIUM_FAST}, // Voltorb
	[7] = {.hp=81, .attack=92, .defense=77, .speed=85, .special=75, .growth_rate=MEDIUM_SLOW}, // Nidoking
	[8] = {.hp=95, .attack=75, .defense=110, .speed=30, .special=80, .growth_rate=MEDIUM_FAST}, // Slowbro
	[9] = {.hp=60, .attack=62, .defense=63, .speed=60, .special=80, .growth_rate=MEDIUM_SLOW}, // Ivysaur
	[10] = {.hp=95, .attack=95, .defense=85, .speed=55, .special=125, .growth_rate=SLOW}, // Exeggutor
	[11] = {.hp=90, .attack=55, .defense=75, .speed=30, .special=60, .growth_rate=MEDIUM_FAST}, // Lickitung
	[12] = {.hp=60, .attack=40, .defense=80, .speed=40, .special=60, .growth_rate=SLOW}, // Exeggcute
	[13] = {.hp=80, .attack=80, .defense=50, .speed=25, .special=40, .growth_rate=MEDIUM_FAST}, // Grimer
	[14] = {.hp=60, .attack=65, .defense=60, .speed=110, .special=130, .growth_rate=MEDIUM_SLOW}, // Gengar
	[15] = {.hp=55, .attack=47, .defense=52, .speed=41, .special=40, .growth_rate=MEDIUM_SLOW}, // Nidoran F
	[16] = {.hp=90, .attack=82, .defense=87, .speed=76, .special=75, .growth_rate=MEDIUM_SLOW}, // Nidoqueen
	[17] = {.hp=50, .attack=50, .defense=95, .speed=35, .special=40, .growth_rate=MEDIUM_FAST}, // Cubone
	[18] = {.hp=80, .attack=85, .defense=95, .speed=25, .special=30, .growth_rate=SLOW}, // Rhyhorn
	[19] = {.hp=130, .attack=85, .defense=80, .speed=60, .special=95, .growth_rate=SLOW}, // Lapras
	[20] = {.hp=90, .attack=110, .defense=80, .speed=95, .special=80, .growth_rate=SLOW}, // Arcanine
	[21] = {.hp=100, .attack=100, .defense=100, .speed=100, .special=100, .growth_rate=SLOW}, // Mew
	[22] = {.hp=95, .attack=125, .defense=79, .speed=81, .special=100, .growth_rate=SLOW}, // Gyarados
	[23] = {.hp=30, .attack=65, .defense=100, .speed=40, .special=45, .growth_rate=SLOW}, // Shellder
	[24] = {.hp=40, .attack=40, .defense=35, .speed=70, .special=100, .growth_rate=SLOW}, // Tentacool
	[25] = {.hp=30, .attack=35, .defense=30, .speed=80, .special=100, .growth_rate=MEDIUM_SLOW}, // Gastly
	[26] = {.hp=70, .attack=110, .defense=80, .speed=105, .special=55, .growth_rate=MEDIUM_FAST}, // Scyther
	[27] = {.hp=30, .attack=45, .defense=55, .speed=85, .special=70, .growth_rate=SLOW}, // Staryu
	[28] = {.hp=79, .attack=83, .defense=100, .speed=78, .special=85, .growth_rate=MEDIUM_SLOW}, // Blastoise
	[29] = {.hp=65, .attack=125, .defense=100, .speed=85, .special=55, .growth_rate=SLOW}, // Pinsir
	[30] = {.hp=65, .attack=55, .defense=115, .speed=60, .special=100, .growth_rate=MEDIUM_FAST}, // Tangela
	[33] = {.hp=55, .attack=70, .defense=45, .speed=60, .special=50, .growth_rate=SLOW}, // Growlithe
	[34] = {.hp=35, .attack=45, .defense=160, .speed=70, .special=30, .growth_rate=MEDIUM_FAST}, // Onix
	[35] = {.hp=65, .attack=90, .defense=65, .speed=100, .special=61, .growth_rate=MEDIUM_FAST}, // Fearow
	[36] = {.hp=40, .attack=45, .defense=40, .speed=56, .special=35, .growth_rate=MEDIUM_SLOW}, // Pidgey
	[37] = {.hp=90, .attack=65, .defense=65, .speed=15, .special=40, .growth_rate=MEDIUM_FAST}, // Slowpoke
	[38] = {.hp=40, .attack=35, .defense=30, .speed=105, .special=120, .growth_rate=MEDIUM_SLOW}, // Kadabra
	[39] = {.hp=55, .attack=95, .defense=115, .speed=35, .special=45, .growth_rate=MEDIUM_SLOW}, // Graveler
	[40] = {.hp=250, .attack=5, .defense=5, .speed=50, .special=105, .growth_rate=FAST}, // Chansey
	[41] = {.hp=80, .attack=100, .defense=70, .speed=45, .special=50, .growth_rate=MEDIUM_SLOW}, // Machoke
	[42] = {.hp=40, .attack=45, .defense=65, .speed=90, .special=100, .growth_rate=MEDIUM_FAST}, // Mr. Mime
	[43] = {.hp=50, .attack=120, .defense=53, .speed=87, .special=35, .growth_rate=MEDIUM_FAST}, // Hitmonlee
	[44] = {.hp=50, .attack=105, .defense=79, .speed=76, .special=35, .growth_rate=MEDIUM_FAST}, // Hitmonchan
	[45] = {.hp=60, .attack=85, .defense=69, .speed=80, .special=65, .growth_rate=MEDIUM_FAST}, // Arbok
	[46] = {.hp=60, .attack=95, .defense=80, .speed=30, .special=80, .growth_rate=MEDIUM_FAST}, // Parasect
	[47] = {.hp=50, .attack=52, .defense=48, .speed=55, .special=50, .growth_rate=MEDIUM_FAST}, // Psyduck
	[48] = {.hp=60, .attack=48, .defense=45, .speed=42, .special=90, .growth_rate=MEDIUM_FAST}, // Drowzee
	[49] = {.hp=80, .attack=110, .defense=130, .speed=45, .special=55, .growth_rate=MEDIUM_SLOW}, // Golem
	[51] = {.hp=65, .attack=95, .defense=57, .speed=93, .special=85, .growth_rate=MEDIUM_FAST}, // Magmar
	[53] = {.hp=65, .attack=83, .defense=57, .speed=105, .special=85, .growth_rate=MEDIUM_FAST}, // Electabuzz
	[54] = {.hp=50, .attack=60, .defense=95, .speed=70, .special=120, .growth_rate=MEDIUM_FAST}, // Magneton
	[55] = {.hp=40, .attack=65, .defense=95, .speed=35, .special=60, .growth_rate=MEDIUM_FAST}, // Koffing
	[57] = {.hp=40, .attack=80, .defense=35, .speed=70, .special=35, .growth_rate=MEDIUM_FAST}, // Mankey
	[58] = {.hp=65, .attack=45, .defense=55, .speed=45, .special=70, .growth_rate=MEDIUM_FAST}, // Seel
	[59] = {.hp=10, .attack=55, .defense=25, .speed=95, .special=45, .growth_rate=MEDIUM_FAST}, // Diglett
	[60] = {.hp=75, .attack=100, .defense=95, .speed=110, .special=70, .growth_rate=SLOW}, // Tauros
	[64] = {.hp=52, .attack=65, .defense=55, .speed=60, .special=58, .growth_rate=MEDIUM_FAST}, // Farfetch'd
	[65] = {.hp=60, .attack=55, .defense=50, .speed=45, .special=40, .growth_rate=MEDIUM_FAST}, // Venonat
	[66] = {.hp=91, .attack=134, .defense=95, .speed=80, .special=100, .growth_rate=SLOW}, // Dragonite
	[70] = {.hp=35, .attack=85, .defense=45, .speed=75, .special=35, .growth_rate=MEDIUM_FAST}, // Doduo
	[71] = {.hp=40, .attack=50, .defense=40, .speed=90, .special=40, .growth_rate=MEDIUM_SLOW}, // Poliwag
	[72] = {.hp=65, .attack=50, .defense=35, .speed=95, .special=95, .growth_rate=MEDIUM_FAST}, // Jynx
	[73] = {.hp=90, .attack=100, .defense=90, .speed=90, .special=125, .growth_rate=SLOW}, // Moltres
	[74] = {.hp=90, .attack=85, .defense=100, .speed=85, .special=125, .growth_rate=SLOW}, // Articuno
	[75] = {.hp=90, .attack=90, .defense=85, .speed=100, .special=125, .growth_rate=SLOW}, // Zapdos
	[76] = {.hp=48, .attack=48, .defense=48, .speed=48, .special=48, .growth_rate=MEDIUM_FAST}, // Ditto
	[77] = {.hp=40, .attack=45, .defense=35, .speed=90, .special=40, .growth_rate=MEDIUM_FAST}, // Meowth
	[78] = {.hp=30, .attack=105, .defense=90, .speed=50, .special=25, .growth_rate=MEDIUM_FAST}, // Krabby
	[82] = {.hp=38, .attack=41, .defense=40, .speed=65, .special=65, .growth_rate=MEDIUM_FAST}, // Vulpix
	[83] = {.hp=73, .attack=76, .defense=75, .speed=100, .special=100, .growth_rate=MEDIUM_FAST}, // Ninetales
	[84] = {.hp=35, .attack=55, .defense=30, .speed=90, .special=50, .growth_rate=MEDIUM_FAST}, // Pikachu
	[85] = {.hp=60, .attack=90, .defense=55, .speed=100, .special=90, .growth_rate=MEDIUM_FAST}, // Raichu
	[88] = {.hp=41, .attack=64, .defense=45, .speed=50, .special=50, .growth_rate=SLOW}, // Dratini
	[89] = {.hp=61, .attack=84, .defense=65, .speed=70, .special=70, .growth_rate=SLOW}, // Dragonair
	[90] = {.hp=30, .attack=80, .defense=90, .speed=55, .special=45, .growth_rate=MEDIUM_FAST}, // Kabuto
	[91] = {.hp=60, .attack=115, .defense=105, .speed=80, .special=70, .growth_rate=MEDIUM_FAST}, // Kabutops
	[92] = {.hp=30, .attack=40, .defense=70, .speed=60, .special=70, .growth_rate=MEDIUM_FAST}, // Horsea
	[93] = {.hp=55, .attack=65, .defense=95, .speed=85, .special=95, .growth_rate=MEDIUM_FAST}, // Seadra
	[96] = {.hp=50, .attack=75, .defense=85, .speed=40, .special=30, .growth_rate=MEDIUM_FAST}, // Sandshrew
	[97] = {.hp=75, .attack=100, .defense=110, .speed=65, .special=55, .growth_rate=MEDIUM_FAST}, // Sandslash
	[98] = {.hp=35, .attack=40, .defense=100, .speed=35, .special=90, .growth_rate=MEDIUM_FAST}, // Omanyte
	[99] = {.hp=70, .attack=60, .defense=125, .speed=55, .special=115, .growth_rate=MEDIUM_FAST}, // Omastar
	[100] = {.hp=115, .attack=45, .defense=20, .speed=20, .special=25, .growth_rate=FAST}, // Jigglypuff
	[101] = {.hp=140, .attack=70, .defense=45, .speed=45, .special=50, .growth_rate=FAST}, // Wigglytuff
	[102] = {.hp=55, .attack=55, .defense=50, .speed=55, .special=65, .growth_rate=MEDIUM_FAST}, // Eevee
	[103] = {.hp=65, .attack=130, .defense=60, .speed=65, .special=110, .growth_rate=MEDIUM_FAST}, // Flareon
	[104] = {.hp=65, .attack=65, .defense=60, .speed=130, .special=110, .growth_rate=MEDIUM_FAST}, // Jolteon
	[105] = {.hp=130, .attack=65, .defense=60, .speed=65, .special=110, .growth_rate=MEDIUM_FAST}, // Vaporeon
	[106] = {.hp=70, .attack=80, .defense=50, .speed=35, .special=35, .growth_rate=MEDIUM_SLOW}, // Machop
	[107] = {.hp=40, .attack=45, .defense=35, .speed=55, .special=40, .growth_rate=MEDIUM_FAST}, // Zubat
	[108] = {.hp=35, .attack=60, .defense=44, .speed=55, .special=40, .growth_rate=MEDIUM_FAST}, // Ekans
	[109] = {.hp=35, .attack=70, .defense=55, .speed=25, .special=55, .growth_rate=MEDIUM_FAST}, // Paras
	[110] = {.hp=65, .attack=65, .defense=65, .speed=90, .special=50, .growth_rate=MEDIUM_SLOW}, // Poliwhirl
	[111] = {.hp=90, .attack=85, .defense=95, .speed=70, .special=70, .growth_rate=MEDIUM_SLOW}, // Poliwrath
	[112] = {.hp=40, .attack=35, .defense=30, .speed=50, .special=20, .growth_rate=MEDIUM_FAST}, // Weedle
	[113] = {.hp=45, .attack=25, .defense=50, .speed=35, .special=25, .growth_rate=MEDIUM_FAST}, // Kakuna
	[114] = {.hp=65, .attack=80, .defense=40, .speed=75, .special=45, .growth_rate=MEDIUM_FAST}, // Beedrill
	[116] = {.hp=60, .attack=110, .defense=70, .speed=100, .special=60, .growth_rate=MEDIUM_FAST}, // Dodrio
	[117] = {.hp=65, .attack=105, .defense=60, .speed=95, .special=60, .growth_rate=MEDIUM_FAST}, // Primeape
	[118] = {.hp=35, .attack=80, .defense=50, .speed=120, .special=70, .growth_rate=MEDIUM_FAST}, // Dugtrio
	[119] = {.hp=70, .attack=65, .defense=60, .speed=90, .special=90, .growth_rate=MEDIUM_FAST}, // Venomoth
	[120] = {.hp=90, .attack=70, .defense=80, .speed=70, .special=95, .growth_rate=MEDIUM_FAST}, // Dewgong
	[123] = {.hp=45, .attack=30, .defense=35, .speed=45, .special=20, .growth_rate=MEDIUM_FAST}, // Caterpie
	[124] = {.hp=50, .attack=20, .defense=55, .speed=30, .special=25, .growth_rate=MEDIUM_FAST}, // Metapod
	[125] = {.hp=60, .attack=45, .defense=50, .speed=70, .special=80, .growth_rate=MEDIUM_FAST}, // Butterfree
	[126] = {.hp=90, .attack=130, .defense=80, .speed=55, .special=65, .growth_rate=MEDIUM_SLOW}, // Machamp
	[128] = {.hp=80, .attack=82, .defense=78, .speed=85, .special=80, .growth_rate=MEDIUM_FAST}, // Golduck
	[129] = {.hp=85, .attack=73, .defense=70, .speed=67, .special=115, .growth_rate=MEDIUM_FAST}, // Hypno
	[130] = {.hp=75, .attack=80, .defense=70, .speed=90, .special=75, .growth_rate=MEDIUM_FAST}, // Golbat
	[131] = {.hp=106, .attack=110, .defense=90, .speed=130, .special=154, .growth_rate=SLOW}, // Mewtwo
	[132] = {.hp=160, .attack=110, .defense=65, .speed=30, .special=65, .growth_rate=SLOW}, // Snorlax
	[133] = {.hp=20, .attack=10, .defense=55, .speed=80, .special=20, .growth_rate=SLOW}, // Magikarp
	[136] = {.hp=105, .attack=105, .defense=75, .speed=50, .special=65, .growth_rate=MEDIUM_FAST}, // Muk
	[138] = {.hp=55, .attack=130, .defense=115, .speed=75, .special=50, .growth_rate=MEDIUM_FAST}, // Kingler
	[139] = {.hp=50, .attack=95, .defense=180, .speed=70, .special=85, .growth_rate=SLOW}, // Cloyster
	[141] = {.hp=60, .attack=50, .defense=70, .speed=140, .special=80, .growth_rate=MEDIUM_FAST}, // Electrode
	[142] = {.hp=95, .attack=70, .defense=73, .speed=60, .special=85, .growth_rate=FAST}, // Clefable
	[143] = {.hp=65, .attack=90, .defense=120, .speed=60, .special=85, .growth_rate=MEDIUM_FAST}, // Weezing
	[144] = {.hp=65, .attack=70, .defense=60, .speed=115, .special=65, .growth_rate=MEDIUM_FAST}, // Persian
	[145] = {.hp=60, .attack=80, .defense=110, .speed=45, .special=50, .growth_rate=MEDIUM_FAST}, // Marowak
	[147] = {.hp=45, .attack=50, .defense=45, .speed=95, .special=115, .growth_rate=MEDIUM_SLOW}, // Haunter
	[148] = {.hp=25, .attack=20, .defense=15, .speed=90, .special=105, .growth_rate=MEDIUM_SLOW}, // Abra
	[149] = {.hp=55, .attack=50, .defense=45, .speed=120, .special=135, .growth_rate=MEDIUM_SLOW}, // Alakazam
	[150] = {.hp=63, .attack=60, .defense=55, .speed=71, .special=50, .growth_rate=MEDIUM_SLOW}, // Pidgeotto
	[151] = {.hp=83, .attack=80, .defense=75, .speed=91, .special=70, .growth_rate=MEDIUM_SLOW}, // Pidgeot
	[152] = {.hp=60, .attack=75, .defense=85, .speed=115, .special=100, .growth_rate=SLOW}, // Starmie
	[153] = {.hp=45, .attack=49, .defense=49, .speed=45, .special=65, .growth_rate=MEDIUM_SLOW}, // Bulbasaur
	[154] = {.hp=80, .attack=82, .defense=83, .speed=80, .special=100, .growth_rate=MEDIUM_SLOW}, // Venusaur
	[155] = {.hp=80, .attack=70, .defense=65, .speed=100, .special=120, .growth_rate=SLOW}, // Tentacruel
	[157] = {.hp=45, .attack=67, .defense=60, .speed=63, .special=50, .growth_rate=MEDIUM_FAST}, // Goldeen
	[158] = {.hp=80, .attack=92, .defense=65, .speed=68, .special=80, .growth_rate=MEDIUM_FAST}, // Seaking
	[163] = {.hp=50, .attack=85, .defense=55, .speed=90, .special=65, .growth_rate=MEDIUM_FAST}, // Ponyta
	[164] = {.hp=65, .attack=100, .defense=70, .speed=105, .special=80, .growth_rate=MEDIUM_FAST}, // Rapidash
	[165] = {.hp=30, .attack=56, .defense=35, .speed=72, .special=25, .growth_rate=MEDIUM_FAST}, // Rattata
	[166] = {.hp=55, .attack=81, .defense=60, .speed=97, .special=50, .growth_rate=MEDIUM_FAST}, // Raticate
	[167] = {.hp=61, .attack=72, .defense=57, .speed=65, .special=55, .growth_rate=MEDIUM_SLOW}, // Nidorino
	[168] = {.hp=70, .attack=62, .defense=67, .speed=56, .special=55, .growth_rate=MEDIUM_SLOW}, // Nidorina
	[169] = {.hp=40, .attack=80, .defense=100, .speed=20, .special=30, .growth_rate=MEDIUM_SLOW}, // Geodude
	[170] = {.hp=65, .attack=60, .defense=70, .speed=40, .special=75, .growth_rate=MEDIUM_FAST}, // Porygon
	[171] = {.hp=80, .attack=105, .defense=65, .speed=130, .special=60, .growth_rate=SLOW}, // Aerodactyl
	[173] = {.hp=25, .attack=35, .defense=70, .speed=45, .special=95, .growth_rate=MEDIUM_FAST}, // Magnemite
	[176] = {.hp=39, .attack=52, .defense=43, .speed=65, .special=50, .growth_rate=MEDIUM_SLOW}, // Charmander
	[177] = {.hp=44, .attack=48, .defense=65, .speed=43, .special=50, .growth_rate=MEDIUM_SLOW}, // Squirtle
	[178] = {.hp=58, .attack=64, .defense=58, .speed=80, .special=65, .growth_rate=MEDIUM_SLOW}, // Charmeleon
	[179] = {.hp=59, .attack=63, .defense=80, .speed=58, .special=65, .growth_rate=MEDIUM_SLOW}, // Wartortle
	[180] = {.hp=78, .attack=84, .defense=78, .speed=100, .special=85, .growth_rate=MEDIUM_SLOW}, // Charizard
	[185] = {.hp=45, .attack=50, .defense=55, .speed=30, .special=75, .growth_rate=MEDIUM_SLOW}, // Oddish
	[186] = {.hp=60, .attack=65, .defense=70, .speed=40, .special=85, .growth_rate=MEDIUM_SLOW}, // Gloom
	[187] = {.hp=75, .attack=80, .defense=85, .speed=50, .special=100, .growth_rate=MEDIUM_SLOW}, // Vileplume
	[188] = {.hp=50, .attack=75, .defense=35, .speed=40, .special=70, .growth_rate=MEDIUM_SLOW}, // Bellsprout
	[189] = {.hp=65, .attack=90, .defense=50, .speed=55, .special=85, .growth_rate=MEDIUM_SLOW}, // Weepinbell
	[190] = {.hp=80, .attack=105, .defense=65, .speed=70, .special=100, .growth_rate=MEDIUM_SLOW}, // Victreebel
};

// TODO multi word names are broken
static char *moves[] =
{
	[1] = "Pound",
	[2] = "Karate",
	[3] = "Double",
	[4] = "Comet",
	[5] = "Mega",
	[6] = "Pay",
	[7] = "Fire",
	[8] = "Ice",
	[9] = "Thunder",
	[10] = "Scratch",
	[11] = "Vice",
	[12] = "Guillotine",
	[13] = "Razor",
	[14] = "Swords",
	[15] = "Cut",
	[16] = "Gust",
	[17] = "Wing",
	[18] = "Whirlwind",
	[19] = "Fly",
	[20] = "Bind",
	[21] = "Slam",
	[22] = "Vine",
	[23] = "Stomp",
	[24] = "Double",
	[25] = "Mega",
	[26] = "Jump",
	[27] = "Rolling",
	[28] = "Sand-Attack",
	[29] = "Headbutt",
	[30] = "Horn",
	[31] = "Fury",
	[32] = "Horn",
	[33] = "Tackle",
	[34] = "Body",
	[35] = "Wrap",
	[36] = "Take",
	[37] = "Thrash",
	[38] = "Double-Edge",
	[39] = "Tail",
	[40] = "Poison",
	[41] = "Twineedle",
	[42] = "Pin",
	[43] = "Leer",
	[44] = "Bite",
	[45] = "Growl",
	[46] = "Roar",
	[47] = "Sing",
	[48] = "Supersonic",
	[49] = "Sonic",
	[50] = "Disable",
	[51] = "Acid",
	[52] = "Ember",
	[53] = "Flamethrower",
	[54] = "Mist",
	[55] = "Water",
	[56] = "Hydro",
	[57] = "Surf",
	[58] = "Ice",
	[59] = "Blizzard",
	[60] = "Psybeam",
	[61] = "Bubble",
	[62] = "Aurora",
	[63] = "Hyper",
	[64] = "Peck",
	[65] = "Drill",
	[66] = "Submission",
	[67] = "Low",
	[68] = "Counter",
	[69] = "Seismic",
	[70] = "Strength",
	[71] = "Absorb",
	[72] = "Mega",
	[73] = "Leech",
	[74] = "Growth",
	[75] = "Razor",
	[76] = "Solar",
	[77] = "Poison",
	[78] = "Stun",
	[79] = "Sleep",
	[80] = "Petal",
	[81] = "String",
	[82] = "Dragon",
	[83] = "Fire",
	[84] = "Thunder",
	[85] = "Thunderbolt",
	[86] = "Thunder",
	[87] = "Thunder",
	[88] = "Rock",
	[89] = "Earthquake",
	[90] = "Fissure",
	[91] = "Dig",
	[92] = "Toxic",
	[93] = "Confusion",
	[94] = "Psychic",
	[95] = "Hypnosis",
	[96] = "Meditate",
	[97] = "Agility",
	[98] = "Quick",
	[99] = "Rage",
	[100] = "Teleport",
	[101] = "Night",
	[102] = "Mimic",
	[103] = "Screech",
	[104] = "Double",
	[105] = "Recover",
	[106] = "Harden",
	[107] = "Minimize",
	[108] = "Smokescreen",
	[109] = "Confuse",
	[110] = "Withdraw",
	[111] = "Defense",
	[112] = "Barrier",
	[113] = "Light",
	[114] = "Haze",
	[115] = "Reflect",
	[116] = "Focus",
	[117] = "Bide",
	[118] = "Metronome",
	[119] = "Mirror",
	[120] = "Self-Destruct",
	[121] = "Egg",
	[122] = "Lick",
	[123] = "Smog",
	[124] = "Sludge",
	[125] = "Bone",
	[126] = "Fire",
	[127] = "Waterfall",
	[128] = "Clamp",
	[129] = "Swift",
	[130] = "Skull",
	[131] = "Spike",
	[132] = "Constrict",
	[133] = "Amnesia",
	[134] = "Kinesis",
	[135] = "Soft-Boiled",
	[136] = "Hi",
	[137] = "Glare",
	[138] = "Dream",
	[139] = "Poison",
	[140] = "Barrage",
	[141] = "Leech",
	[142] = "Lovely",
	[143] = "Sky",
	[144] = "Transform",
	[145] = "Bubble",
	[146] = "Dizzy",
	[147] = "Spore",
	[148] = "Flash",
	[149] = "Psywave",
	[150] = "Splash",
	[151] = "Acid",
	[152] = "Crabhammer",
	[153] = "Explosion",
	[154] = "Fury",
	[155] = "Bonemerang",
	[156] = "Rest",
	[157] = "Rock",
	[158] = "Hyper",
	[159] = "Sharpen",
	[160] = "Conversion",
	[161] = "Tri",
	[162] = "Super",
	[163] = "Slash",
	[164] = "Substitute",
	[165] = "Struggle",
};

Info get_item_info(int id)
{
	return items[id];
}

Info get_pokemon_info(int id)
{
	return pokemon[id];
}

PokemonBaseStats get_pokemon_base_stats(int id)
{
	return pokemon_stats[id];
}

char *get_move_name(int id)
{
	return moves[id];
}

// there are gaps in the pokemon ids that need to be skipped over
int get_pokemon_translated_id(int id)
{
	int translated_id;
	for(translated_id = 1;; translated_id++)
	{
		if(get_pokemon_info(translated_id).name && --id == 0)
				break;
	}

	return translated_id;
}

int get_move_translated_id(int id)
{
	if(id == 0)
		return id;

	int translated_id;
	for(translated_id = 1;; translated_id++)
	{
		if(get_move_name(translated_id) && --id == 0)
			break;
	}

	return translated_id;
}
