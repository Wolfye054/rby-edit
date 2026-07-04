/*
 * currently we do not handle conversion between all characters,
 * only the characters that the player can enter when naming
 * themselves/pokemon.
 *
 * not all characters in rby have a counter part in ascii (gender and PkMn symbols),
 * so for both converting to and from we use $x, where x is a char that maps to a
 * specific symbol.
 *
 * any characters we cant convert to/from will be substituted with the character sets
 * respective "?".
 *
 * A full breakdown of the character encoding can be found at:
 * https://bulbapedia.bulbagarden.net/wiki/Character_encoding_(Generation_I)
 */

#define RBY_CHAR_TERMINATOR 0x50
#define RBY_CHAR_UNKNOWN 0xE6

static char rby_character_set[] =
{
	// uppercase alphabet
	[0x80] = 'A',
	[0x81] = 'B',
	[0x82] = 'C',
	[0x83] = 'D',
	[0x84] = 'E',
	[0x85] = 'F',
	[0x86] = 'G',
	[0x87] = 'H',
	[0x88] = 'I',
	[0x89] = 'J',
	[0x8A] = 'K',
	[0x8B] = 'L',
	[0x8C] = 'M',
	[0x8D] = 'N',
	[0x8E] = 'O',
	[0x8F] = 'P',
	[0x90] = 'Q',
	[0x91] = 'R',
	[0x92] = 'S',
	[0x93] = 'T',
	[0x94] = 'U',
	[0x95] = 'V',
	[0x96] = 'W',
	[0x97] = 'X',
	[0x98] = 'Y',
	[0x99] = 'Z',

	// lowercase alphabet
	[0xA0] = 'a',
	[0xA1] = 'b',
	[0xA2] = 'c',
	[0xA3] = 'd',
	[0xA4] = 'e',
	[0xA5] = 'f',
	[0xA6] = 'g',
	[0xA7] = 'h',
	[0xA8] = 'i',
	[0xA9] = 'j',
	[0xAA] = 'k',
	[0xAB] = 'l',
	[0xAC] = 'm',
	[0xAD] = 'n',
	[0xAE] = 'o',
	[0xAF] = 'p',
	[0xB0] = 'q',
	[0xB1] = 'r',
	[0xB2] = 's',
	[0xB3] = 't',
	[0xB4] = 'u',
	[0xB5] = 'v',
	[0xB6] = 'w',
	[0xB7] = 'x',
	[0xB8] = 'y',
	[0xB9] = 'z',

	// punctuation
	[0x7F] = ' ',
	[0x9A] = '(',
	[0x9B] = ')',
	[0x9C] = ':',
	[0x9D] = ';',
	[0x9E] = '[',
	[0x9F] = ']',
	[0xF1] = '*',
	[0xE3] = '-',
	[0xE6] = '?',
	[0xE7] = '!',
	[0xF3] = '/',
	[0xF2] = '.',
	[0xF4] = ',',
	
	// return '%' so rby_to_ascii knows to check escaped_rby
	[0xE1] = '$',
	[0xE2] = '$',
	[0xEF] = '$',
	[0xF5] = '$',
};

static char escaped_rby[] =
{
	[0xE1] = 'p',
	[0xE2] = 'n',
	[0xEF] = 'm',
	[0xF5] = 'f',
};

static char escaped_ascii[] = 
{
	['p'] = 0xE1,
	['n'] = 0xE2,
	['m'] = 0xEF,
	['f'] = 0xF5,
};

char *rby_to_ascii(char *rby_string)
{
	int size = 0;
	char *ascii_string;

	for(int i = 0; rby_string[i] != RBY_CHAR_TERMINATOR; size++, i++)
		if(rby_character_set[(uint8_t)rby_string[i]] == '$') size++;


	ascii_string = malloc(size + 1);

	int j = 0;
	for(int i = 0; rby_string[i] != RBY_CHAR_TERMINATOR; i++, j++)
	{
		char ascii_char = rby_character_set[(uint8_t)rby_string[i]];

		if(ascii_char == 0)
		{
			ascii_string[j] = '?';
		}
		else if(ascii_char == '$')
		{
			char esc = escaped_rby[(uint8_t)rby_string[i]];

			if(esc == '\0')
			{
				ascii_string[j] = '?';
			}
			else
			{
				ascii_string[j] = '$';
				ascii_string[++j] =  esc;
			}
		}
		else
		{
			ascii_string[j] = ascii_char;
		}
	}

	ascii_string[j] = '\0';
	return ascii_string;
}

char *ascii_to_rby(char *ascii_string)
{
	int size = 0;
	char *rby_string;

	for(int i = 0; ascii_string[i] != '\0'; size++, i++)
		if(ascii_string[i] == '$') --size;

	rby_string = malloc(size + 1);

	int j = 0;	
	for(int i = 0; ascii_string[i] != '\0'; i++, j++)
	{
		if(ascii_string[i] == '$')
		{
			char esc = escaped_ascii[(uint8_t)ascii_string[++i]];

			if(esc == '\0')
				rby_string[j] = RBY_CHAR_UNKNOWN;
			else
				rby_string[j] = esc;

			continue;
		}

		rby_string[j] = RBY_CHAR_UNKNOWN;
		for(int i2 = 0; i2 < (uint8_t)(sizeof(rby_character_set) / sizeof(char)); i2++)
		{
			if(rby_character_set[i2] == ascii_string[i])
			{
				rby_string[j] = i2;
			}
		}
	}

	rby_string[j] = RBY_CHAR_TERMINATOR;
	return rby_string;
}
