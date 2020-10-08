#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define WINDOWS_TICK 10000000

static const char alphabet[] = "ABCDEF"
															 "0123456789";

int nFile = 0;
char str[10000];

void *my_malloc(size_t size){
  void *result = malloc(size);
  if(result == NULL){
    printf("malloc failure\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

int dateToTimestamp(int year, int month, int day, int hour) {
	struct tm t;
  time_t t_of_day;

  t.tm_year = year - 1900;
  t.tm_mon = month - 1; // 0 = January
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = 0;
  t.tm_sec = 0;
  t.tm_isdst = -1;  // Is DST on? 1 = yes, 0 = no, -1 = unknown
  t_of_day = mktime(&t);

	return (int)t_of_day;
}

char* original(char* s){
	int j;
	char* output = my_malloc(sizeof(char) * 32);
	output[6] = s[0];
	output[7] = s[1];
	output[4] = s[2];
	output[5] = s[3];
	output[2] = s[4];
	output[3] = s[5];
	output[0] = s[6];
	output[1] = s[7];
	output[10] = s[8];
	output[11] = s[9];
	output[8] = s[10];
	output[9] = s[11];
	output[14] = s[12];
	output[15] = s[13];
	output[12] = s[14];
	output[13] = s[15];

	for(j = 16; j <= 32; j++)
		output[j] = s[j];

	return output;
}

char* removeChar(char *s, int c) {
  int j, n = strlen(s);
  for (int i = j = 0; i < n; i++)
  	if (s[i] != c)
    	s[j++] = s[i];
  s[j] = '\0';

	char* output = my_malloc(sizeof(char) * 32);
	output[0] = s[6];
	output[1] = s[7];
	output[2] = s[4];
	output[3] = s[5];
	output[4] = s[2];
	output[5] = s[3];
	output[6] = s[0];
	output[7] = s[1];
	output[8] = s[10];
	output[9] = s[11];
	output[10] = s[8];
	output[11] = s[9];
	output[12] = s[14];
	output[13] = s[15];
	output[14] = s[12];
	output[15] = s[13];

	for(j = 16; j <= 32; j++)
		output[j] = s[j];

	return output;
}

char* str_reverse(char* str, char* out){
	out[0] = 'A';
	out[1] = 'A';
	out[2] = 'A';
	out[3] = 'A';
	out[4] = 'A';

	out[5] = toupper(str[8]);
	out[6] = toupper(str[6]);
	out[7] = toupper(str[7]);
	out[8] = toupper(str[4]);
	out[9] = toupper(str[5]);
	out[10] = toupper(str[2]);
	out[11] = toupper(str[3]);
	out[12] = toupper(str[0]);
	out[13] = toupper(str[1]);
	out[14] = '\0';
	return out;
}

void output(char* datasetGUID, char* epochTimestamp, char* epochTimestamp2, char* key) {
	unsigned char value;
	int i;
	char* two = my_malloc(2 * sizeof(char));
	two[2] = '\0';
	FILE *fp;
	char* guid = original(datasetGUID);
	sprintf(str, "#%d)%.*s-%.*s-%.*s-%.*s-%.*s.BEK", nFile, 8, guid, 4, guid + 8, 4, guid + 12, 4, guid + 16, 12, guid + 20);
	fp = fopen(str, "wb+");
	value = strtol("7C", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 3; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 3; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("30", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 3; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("7C", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 3; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}

	// GUID

	for(i = 0; i < 32; i += 2){
		two[0] = datasetGUID[i];
		two[1] = datasetGUID[i + 1];
		value = strtol(two, NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}

	//

	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 7; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}

	// epochTimestamp

	for(i = 0; i < 14; i += 2){
		two[0] = epochTimestamp[i];
		two[1] = epochTimestamp[i + 1];
		value = strtol(two, NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);

	//

	value = strtol("4C", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("6", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("9", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);

	// GUID

	for(i = 0; i < 32; i += 2){
		two[0] = datasetGUID[i];
		two[1] = datasetGUID[i + 1];
		value = strtol(two, NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}

	// epochTimestamp2

	for(i = 0; i < 14; i += 2){
		two[0] = epochTimestamp2[i];
		two[1] = epochTimestamp2[i + 1];
		value = strtol(two, NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);

	//

	value = strtol("2C", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 3; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("1", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("0", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("2", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	value = strtol("20", NULL, 16);
	fwrite(&value, sizeof(value), 1, fp);
	for(i = 0; i < 2; i++){
		value = strtol("0", NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}

	// key

	for(i = 0; i < 64; i += 2){
		two[0] = key[i];
		two[1] = key[i + 1];
		value = strtol(two, NULL, 16);
		fwrite(&value, sizeof(value), 1, fp);
	}
	//free(two);

	//

	//free(str);
	fclose(fp);
	nFile++;
}

void usage(char *name) {
	printf("\nUsage: %s -g <GUID> -e <Year Month Day Hour> following this format: <yyyy mm dd hh> and > <2007 01 07 00> -h <Help>\n\n", name);
}

static const int alphabetSize = sizeof(alphabet) - 1;

long long unsigned UnixSecondsToWindowsTick(unsigned unixSeconds){
     return (long long unsigned)(unixSeconds) * WINDOWS_TICK + 116444736000000000;
}

void bruteImpl1(char* str, int index, int maxDepth, char* datasetGUID, char* epochTimestamp, char* epochTimestamp2, char* key) {
  int i;
  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
      memcpy(datasetGUID, &str[0], 32);
      datasetGUID[32] = '\0';
			memcpy(key, &str[32], 64);
			key[64] = '\0';
			memcpy(epochTimestamp, &str[96], 5);
			memcpy(epochTimestamp2, &str[101], 5);

			output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		}
    else bruteImpl1(str, index + 1, maxDepth, datasetGUID, epochTimestamp, epochTimestamp2, key);
  }
}

void bruteImpl2(char* str, int index, int maxDepth, char* datasetGUID, char* epochTimestamp, char* epochTimestamp2, char* key) {
  int i;
  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
			memcpy(key, &str[0], 64);
			key[64] = '\0';
			memcpy(epochTimestamp, &str[64], 5);
			memcpy(epochTimestamp2, &str[69], 5);

			output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		}
    else bruteImpl2(str, index + 1, maxDepth, datasetGUID, epochTimestamp, epochTimestamp2, key);
  }
}

void bruteImpl3(char* str, int index, int maxDepth, char* datasetGUID, char* epochTimestamp, char* epochTimestamp2, char* key) {
  int i;
  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
			memcpy(key, &str[0], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			memcpy(epochTimestamp, &str[64], 14);
			epochTimestamp[14] = '\0';
			memcpy(epochTimestamp2, &str[78], 14);
			epochTimestamp2[14] = '\0';
			//

			output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		}
    else bruteImpl3(str, index + 1, maxDepth, datasetGUID, epochTimestamp, epochTimestamp2, key);
  }
}

void bruteImpl4(char* str, int index, int maxDepth, char* datasetGUID, char* epochTimestamp, char* epochTimestamp2, char* key) {
  int i;
  for (i = 0; i < alphabetSize; ++i) {
    str[index] = alphabet[i];
    if (index == maxDepth - 1) {
      memcpy(datasetGUID, &str[0], 32);
      datasetGUID[32] = '\0';
			memcpy(key, &str[32], 64);
			key[64] = '\0';

			// BRUTEFORCING SUL Timestamp
			memcpy(epochTimestamp, &str[96], 14);
			epochTimestamp[14] = '\0';
			memcpy(epochTimestamp2, &str[110], 14);
			epochTimestamp2[14] = '\0';
			//

			output(datasetGUID, epochTimestamp, epochTimestamp2, key);
		}
    else bruteImpl4(str, index + 1, maxDepth, datasetGUID, epochTimestamp, epochTimestamp2, key);
  }
}

void bruteSequential1(int maxLen, int year, int month, int day, int hour) {
  char* buf = my_malloc(maxLen + 1);
	char* datasetGUID = my_malloc(sizeof(char) * 32);

	char* epochTimestamp = my_malloc(sizeof(char) * 9);
	int epoch = dateToTimestamp(year, month, day, hour);
	long long epochW = UnixSecondsToWindowsTick(epoch);
	sprintf(epochTimestamp, "%llx", epochW & 0xFFFFFFFFFFF00000LL);
	char* correct = my_malloc(sizeof(char) * 9);
	memcpy(correct, &epochTimestamp[1], 9);
	correct[9] = '\0';
	char* out1 = my_malloc(sizeof(char) * 14);
	str_reverse(correct, out1);
	char* out2 = my_malloc(sizeof(char) * 14);
	str_reverse(correct, out2);

	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl1(buf, 0, maxLen, datasetGUID, out1, out2, key);
  free(buf);
}

void bruteSequential2(int maxLen, char* datasetGUID, int year, int month, int day, int hour) {
  char* buf = my_malloc(maxLen + 1);
	char* key = my_malloc(sizeof(char) * 64);

	char* epochTimestamp = my_malloc(sizeof(char) * 9);
	int epoch = dateToTimestamp(year, month, day, hour);
	long long epochW = UnixSecondsToWindowsTick(epoch);
	sprintf(epochTimestamp, "%llx", epochW & 0xFFFFFFFFFFF00000LL);
	char* correct = my_malloc(sizeof(char) * 9);
	memcpy(correct, &epochTimestamp[1], 9);
	correct[9] = '\0';
	char* out1 = my_malloc(sizeof(char) * 14);
	str_reverse(correct, out1);
	char* out2 = my_malloc(sizeof(char) * 14);
	str_reverse(correct, out2);

  memset(buf, 0, maxLen + 1);
  bruteImpl2(buf, 0, maxLen, datasetGUID, out1, out2, key);
  free(buf);
}

void bruteSequential3(int maxLen, char* datasetGUID) {
  char* buf = my_malloc(maxLen + 1);
	char* epochTimestamp = my_malloc(sizeof(char) * 14);
	char* epochTimestamp2 = my_malloc(sizeof(char) * 14);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl3(buf, 0, maxLen, datasetGUID, epochTimestamp, epochTimestamp2, key);
  free(buf);
}

void bruteSequential4(int maxLen) {
  char* buf = my_malloc(maxLen + 1);
	char* datasetGUID = my_malloc(sizeof(char) * 32);
	char* epochTimestamp = my_malloc(sizeof(char) * 14);
	char* epochTimestamp2 = my_malloc(sizeof(char) * 14);
	char* key = my_malloc(sizeof(char) * 64);
  memset(buf, 0, maxLen + 1);
  bruteImpl4(buf, 0, maxLen, datasetGUID, epochTimestamp, epochTimestamp2, key);
  free(buf);
}

int main(int argc, char **argv) {

	void* encryptionType = NULL;
	char* datasetGUID;
  int year; int month; int day; int hour;
	char* yearS = my_malloc(sizeof(char) * 4); char* monthS = my_malloc(sizeof(char) * 2);
  char* dayS = my_malloc(sizeof(char) * 2); char* hourS = my_malloc(sizeof(char) * 2);
  int flagG = 0, flagE = 0;

  // *********************** Options ************************
	int opt;
  while ((opt = getopt(argc, argv, "g:e:h")) != -1) {
    switch (opt) {
			case 'g':
        datasetGUID = removeChar(optarg, '-');
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
				if(dateToTimestamp(year, month, day, hour) < 1170111600){
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
    // 5 bruteforcing (minuti e secondi) +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2
    bruteSequential1(32 + 64 + 5 + 5, year, month, day, hour);

  else if(flagG && flagE)
    // bruteforcing di 5 cifre (minuti e secondi) +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2
		bruteSequential2(64 + 5 + 5, datasetGUID, year, month, day, hour);

  else if(flagG && !flagE)
    // bruteforcing di 14 cifre su epochTimestamp perchè BitLocker nasce per Windows Vista e
    // la sua data di rilascio è 30/01/07 => 1 fisso +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2
		bruteSequential3(64 + 14 + 14, datasetGUID);

  else if(!flagG && !flagE)
    // bruteforcing di 32 cifre +
    // bruteforcing di 14 cifre su epochTimestamp perchè BitLocker nasce per Windows Vista e
    // la sua data di rilascio è 30/01/07 => 1 fisso +
    // bruteforcing puro su 16 x 4 = 64 cifre di chiave +
    // EpochTimestamp2
		bruteSequential4(32 + 64 + 14 + 14);

  seconds -= time(NULL); // fermo il cronometro

	return 0;
}
