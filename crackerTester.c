#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define DATASETHEADERSIZE 0x00000030
#define UNKNOWNDATA 0x00000001
#define ALGO 0x2002

static const char alphabet[] = "ABCDEF"
															 "0123456789";

static const char timestampAlphabet[] = "0123456789";

void *my_malloc(size_t size){
  void *result = malloc(size);
  if(result == NULL){
    printf("malloc failure\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

char* timestampToDate(time_t timestamp){
  struct tm ts;
  char* buf = my_malloc(80 * sizeof(char));
  ts = *localtime(&timestamp);
  strftime(buf, 80 * sizeof(char), "%a %b %d %H:%M:%S %Y", &ts);
  return buf;
}

int dateToTimestamp(int year, int month, int day, int hour, int minute, int second) {
	struct tm t;
  time_t t_of_day;

  t.tm_year = year - 1900;
  t.tm_mon = month - 1; // 0 = January
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;
  t.tm_isdst = -1;  // Is DST on? 1 = yes, 0 = no, -1 = unknown
  t_of_day = mktime(&t);

	return (int)t_of_day;
}

void output(char* datasetGUID, int epochTimestamp, int epochTimestamp2, char* key) {
	printf("----------------------------{ Dataset header }----------------------------\n");
	printf("Dataset size: 0x0000007c (124) bytes (including data)\nUnknown data: 0x%08x (always 0x%08x)\nDataset header size: 0x%08x (always 0x%08x)\nDataset copy size: 0x0000007c (124) bytes)\n", UNKNOWNDATA, UNKNOWNDATA, DATASETHEADERSIZE, DATASETHEADERSIZE);
	printf("Dataset GUID: %.*s-%.*s-%.*s-%.*s-%.*s\n", 8, datasetGUID, 4, datasetGUID + 8, 4, datasetGUID + 12, 4, datasetGUID + 16, 12, datasetGUID + 20);
	printf("Next counter: 1\nEncryption Type: NULL (0)\n");
	printf("Epoch Timestamp: %d sec, that to say %s\n", epochTimestamp, timestampToDate(epochTimestamp));
	printf("--------------------------------------------------------------------------\n");
	printf("Total datum size: 0x004c (76) bytes\nDatum entry type: 6\n`--> ENTRY TYPE UNKNOWN\nDatum value type: 9\n`--> EXTERNAL KEY -- Total size header: 32 -- Nested datum: yes\nStatus: 0x1\n");
	printf("Recovery Key GUID: %.*s-%.*s-%.*s-%.*s-%.*s\n", 8, datasetGUID, 4, datasetGUID + 8, 4, datasetGUID + 12, 4, datasetGUID + 16, 12, datasetGUID + 20);
	printf("Epoch Timestamp: %d sec, being %s\n", epochTimestamp2, timestampToDate(epochTimestamp2));
	printf("------ Nested datum ------\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Total datum size: 0x002c (44) bytes\nDatum entry type: 0\n`--> ENTRY TYPE UNKNOWN 1\nDatum value type: 1\n`--> KEY -- Total size header: 12 -- Nested datum: no\nStatus: 0x1\nUnkown:\n0x00000000 00 00\nAlgo: EXTERN KEY (0x%04x)\nKey:\n", ALGO);
	printf("0x00000000 ");
	int i;
	for(i = 0; i < 15; i+=2) printf("%c%c ", key[i], key[i + 1]);
	printf("- ");
	for(i = 16; i < 31; i+=2) printf("%c%c ", key[i], key[i + 1]);
	printf("\n0x00000010 ");
	for(i = 32; i < 47; i+=2) printf("%c%c ", key[i], key[i + 1]);
	printf("- ");
	for(i = 48; i < 63; i+=2) printf("%c%c ", key[i], key[i + 1]);
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n---------------------------\n");
}

void removeChar(char *s, int c) {
  int j, n = strlen(s);
  for (int i = j = 0; i < n; i++)
  	if (s[i] != c)
    	s[j++] = s[i];
  s[j] = '\0';
}

void usage(char *name) {
	printf("\nUsage: %s -g <GUID> -e <Year Month Day Hour> following this format: <yyyy mm dd hh> and > <2007 01 07 00> -h <Help>\n\n", name);
}

static const int alphabetSize = sizeof(alphabet) - 1;
static const int alphabetSizeT = sizeof(timestampAlphabet) - 1;

unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}

void bruteImplT(char* str, int index, int maxDepth, char* datasetGUID, int year, int month, int day, int hour, char* minute, char* second, char* key) {
	int i, j, epochTimestamp, epochTimestamp2;

		for (i = 0; i < alphabetSizeT; ++i) {
    	str[index] = timestampAlphabet[i];
      if (index == maxDepth - 1){
				memcpy(minute, &str[0], 2);
	      minute[2] = '\0';
	      memcpy(second, &str[2], 2);
	      second[2] = '\0';

				epochTimestamp = dateToTimestamp(year, month, day, hour, atoi(minute), atoi(second));

		    for (j = 0; j <= 10; ++j) {
		      epochTimestamp2 = epochTimestamp - j;
					output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		    }
			}
      else bruteImplT(str, index + 1, maxDepth, datasetGUID, year, month, day, hour, minute, second, key);
    }
}

void bruteImplTT(char* str, int index, int maxDepth, char* epoch, char* datasetGUID, char* key) {
	int i, j, epochTimestamp, epochTimestamp2;

		for (i = 0; i < alphabetSizeT; ++i) {
    	str[index] = timestampAlphabet[i];
      if (index == maxDepth - 1){
	      memcpy(epoch, &str[0], 9);
	      epoch[9] = '\0';

		    epochTimestamp = concatenate(1, atoi(epoch));

		    for (j = 0; j <= 10; ++j) {
		      epochTimestamp2 = epochTimestamp - j;
					output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		    }
			}
      else bruteImplTT(str, index + 1, maxDepth, epoch, datasetGUID, key);
    }
}

void bruteImpl1(char* str, int index, int maxDepth, char* datasetGUID, int year, int month, int day, int hour, char* minute, char* second, char* key) {
  int i;

  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
      memcpy(datasetGUID, &str[0], 32);
      datasetGUID[32] = '\0';
			memcpy(key, &str[32], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			char* buf = my_malloc(4 + 1);
			memset(buf, 0, 4 + 1);
	    bruteImplT(buf, 0, 4, datasetGUID, year, month, day, hour, minute, second, key);
			free(buf);
		}
    else bruteImpl1(str, index + 1, maxDepth, datasetGUID, year, month, day, hour, minute, second, key);
  }
}

void bruteImpl2(char* str, int index, int maxDepth, char* datasetGUID, int year, int month, int day, int hour, char* minute, char* second, char* key) {
  int i;

  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
			memcpy(key, &str[0], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			char* buf = my_malloc(4 + 1);
			memset(buf, 0, 4 + 1);
	    bruteImplT(buf, 0, 4, datasetGUID, year, month, day, hour, minute, second, key);
			free(buf);
		}
    else bruteImpl2(str, index + 1, maxDepth, datasetGUID, year, month, day, hour, minute, second, key);
  }
}

void bruteImpl3(char* str, int index, int maxDepth, char* datasetGUID, char* epoch, char* key) {
  int i;

  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
			memcpy(key, &str[0], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			char* buf = my_malloc(9 + 1);
			memset(buf, 0, 9 + 1);
	    bruteImplTT(buf, 0, 9, epoch, datasetGUID, key);
			free(buf);
		}
    else bruteImpl3(str, index + 1, maxDepth, datasetGUID, epoch, key);
  }
}

void bruteImpl4(char* str, int index, int maxDepth, char* datasetGUID, char* epoch, char* key) {
  int i, j, epochTimestamp, epochTimestamp2;

  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
      memcpy(datasetGUID, &str[0], 32);
      datasetGUID[32] = '\0';
			memcpy(key, &str[32], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			char* buf = my_malloc(9 + 1);
			memset(buf, 0, 9 + 1);
	    bruteImplTT(buf, 0, 9, epoch, datasetGUID, key);
			free(buf);
		}
    else bruteImpl4(str, index + 1, maxDepth, datasetGUID, epoch, key);
  }
}

void bruteSequential1(int maxLen, int year, int month, int day, int hour) {
  char* buf = my_malloc(maxLen + 1);
	char* datasetGUID = my_malloc(sizeof(char) * 32);
	char* second = my_malloc(sizeof(char) * 2); char* minute = my_malloc(sizeof(char) * 2);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl1(buf, 0, maxLen, datasetGUID, year, month, day, hour, minute, second, key);
  free(buf);
}

void bruteSequential2(int maxLen, char* datasetGUID, int year, int month, int day, int hour) {
  char* buf = my_malloc(maxLen + 1);
	char* second = my_malloc(sizeof(char) * 2); char* minute = my_malloc(sizeof(char) * 2);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl2(buf, 0, maxLen, datasetGUID, year, month, day, hour, minute, second, key);
  free(buf);
}

void bruteSequential3(int maxLen, char* datasetGUID) {
  char* buf = my_malloc(maxLen + 1);
	char* epoch = my_malloc(sizeof(char) * 9);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl3(buf, 0, maxLen, datasetGUID, epoch, key);
  free(buf);
}

void bruteSequential4(int maxLen) {
  char* buf = my_malloc(maxLen + 1);
	char* datasetGUID = my_malloc(sizeof(char) * 32);
	char* epoch = my_malloc(sizeof(char) * 9);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl4(buf, 0, maxLen, datasetGUID, epoch, key);
  free(buf);
}

int main(int argc, char **argv) {

  int datasetCopySize = 0x7c;
	int nextCounter = 1;
	void* encryptionType = NULL;
	char* datasetGUID = my_malloc(sizeof(char) * 32);
	int totalDatumSize = 0x4c;
  int datumEntryType = 6;
  int datumValueType = 9;
  int status = 0x1;
	int totalDatumSize2 = 0x2c;
  int datumEntryType2 = 0;
  int datumValueType2 = 1;
  int status2 = 0x1;
  int year; int month; int day; int hour;
	char* yearS = my_malloc(sizeof(char) * 4); char* monthS = my_malloc(sizeof(char) * 2);
  char* dayS = my_malloc(sizeof(char) * 2); char* hourS = my_malloc(sizeof(char) * 2);
  int flagG = 0, flagE = 0;

  // *********************** Options ************************
	int opt;
  while ((opt = getopt(argc, argv, "g:e:h")) != -1) {
    switch (opt) {
			case 'g':
        removeChar(optarg, '-');
        strcpy(datasetGUID, optarg);
        flagG = 1;
				break;
      case 'e':
        memcpy(yearS, &optarg[0], 4);
        yearS[4] = '\0';
				year = atoi(yearS);
        memcpy(monthS, &optarg[5], 2);
        monthS[2] = '\0';
				month = atoi(monthS);
        memcpy(dayS, &optarg[8], 2);
        dayS[2] = '\0';
				day = atoi(dayS);
        memcpy(hourS, &optarg[11], 13);
        hourS[2] = '\0';
				hour = atoi(hourS);
				if(dateToTimestamp(year, month, day, hour, 0, 0) < 1170111600){
					printf("Error!\n");
	        usage(argv[0]);
	        exit(EXIT_FAILURE);
				}
        flagE = 1;
				break;
      case 'h':
        usage(argv[0]);
        exit(EXIT_FAILURE);
      default:
        printf("Error!\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

	//

	int seconds = time(NULL); // faccio scattare il cronometro

  if(!flagG && flagE)
    // bruteforcing di 32 cifre +
    // 4 bruteforcing (minuti e secondi) +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2 := EpochTimestamp - [0, 10]
    bruteSequential1(32 + 64, year, month, day, hour);

  else if(flagG && flagE)
    // bruteforcing di 4 cifre (minuti e secondi) +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2 := EpochTimestamp - [0, 10]
		bruteSequential2(64, datasetGUID, year, month, day, hour);

  else if(flagG && !flagE)
    // bruteforcing di 9 cifre su epochTimestamp perchè BitLocker nasce per Windows Vista e
    // la sua data di rilascio è 30/01/07 => 1 fisso +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2 := EpochTimestamp - [0, 10]
		bruteSequential3(64, datasetGUID);

  else if(!flagG && !flagE)
    // bruteforcing di 32 cifre +
    // bruteforcing di 9 cifre su epochTimestamp perchè BitLocker nasce per Windows Vista e
    // la sua data di rilascio è 30/01/07 => 1 fisso +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2 := EpochTimestamp - [0, 10]
		bruteSequential4(32 + 64);

  seconds -= time(NULL); // fermo il cronometro
  printf("\n\n*** Execution time = %d ***\n\n", seconds);

	return 0;
}
