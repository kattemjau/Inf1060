#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define CNRM "\x1b[0m"
#define CRED "\x1b[31m"
#define CGRN "\x1b[32m"

int stringsum(char *s);
void stringsum2(char *s, int *res);
int distance_between(char *s, char c);
char *string_between(char *s, char c);
char **split(char *s);

static int test_num = 1;

static void logger(int passed, char *s)
{
	char *res;
	char *color;

	if (passed) {
		res = "PASS";
		color = CGRN;
	} else {
		res = "FAIL";
		color = CRED;
	}
	printf("[Test %d][%s%s%s] %s\n", test_num++, color, res, CNRM, s);
}

static void test_split(char *str, char **correct)
{
	int i, pass = 1;
	char buf[256] = { 0 };
	char *str_cpy = strdup(str); // Allow mutation of original string
	char **res = split(str_cpy);

	if (!res || !res[0]) {
		pass = 0;
		sprintf(buf, "split() returned NULL or an empty array");
		logger(pass, buf);
		return;
	}

	sprintf(buf, "\n%-16s%-16s\n", "Returned", "Expected");

	for (i = 0; res[i]; i++) {
		char tmp[256] = { 0 };
		sprintf(tmp, "%-16s%-16s\n", res[i], correct[i]);
		strcat(buf, tmp);
		if (strcmp(res[i], correct[i])) {
			pass = 0;
			logger(pass, buf);
			return;
		}
	}

	logger(pass, buf);
	free(str_cpy);
}

int main(void)
{
	char buf[256] = { 0 };
	int test;
	char *res_char;
	int res_int = 0;

	printf("Testing stringsum()\n");

	test = stringsum("abcd");
	sprintf(buf, "Returned: %d, Expected: %d", test, 10);
	logger(test == 10, buf);

	test = stringsum("a!");
	sprintf(buf, "Returned: %d, Expected: %d", test, -1);
	logger(test == -1, buf);

	test = stringsum("aAzZ");
	sprintf(buf, "Returned: %d, Expected: %d", test, 54);
	logger(test == 54, buf);

	test = stringsum("ababcDcabcddAbcDaBcabcABCddabCddabcabcddABCabcDd");
	sprintf(buf, "Returned: %d, Expected: %d", test, 120);
	logger(test == 120, buf);

	test = stringsum("");
	sprintf(buf, "Returned: %d, Expected: %d", test, 0);
	logger(test == 0, buf);

	test_num = 1;

	printf("Testing distance_between()\n");

	test = distance_between("a1234a", 'a');
	sprintf(buf, "Returned: %d, Expected: %d", test, 5);
	logger(test == 5, buf);

	test = distance_between("a1234", 'a');
	sprintf(buf, "Returned: %d, Expected: %d", test, -1);
	logger(test == -1, buf);

	test = distance_between("123456a12334a123a", 'a');
	sprintf(buf, "Returned: %d, Expected: %d", test, 6 );
	logger(test == 6, buf);

	test = distance_between("", 'a');
	sprintf(buf, "Returned: %d, Expected: %d", test, -1);
	logger(test == -1, buf);

	test_num = 1;

	printf("Testing string_between()\n");

	res_char = string_between("a1234a", 'a');
	sprintf(buf, "Returned: %s, Expected: %s", res_char, "1234");
	if (res_char) {
		logger(!strcmp(res_char, "1234"), buf);
		free(res_char);
	} else {
		logger(0, buf);
	}

	res_char = string_between("a1234", 'a');
	if (res_char) {
		sprintf(buf, "Returned: %s, Expected: %p", res_char, NULL);
	} else {
		sprintf(buf, "Returned: %p, Expected: %p", res_char, NULL);
	}
	logger(res_char == NULL, buf);
	if (res_char)
		free(res_char);

	res_char = string_between("A123adette er svaretaasd2qd3asd12", 'a');
	sprintf(buf, "Returned: %s, Expected: %s", res_char, "dette er sv");
	if (res_char) {
		logger(!strcmp(res_char, "dette er sv"), buf);
		free(res_char);
	} else {
		logger(0, buf);
	}

	res_char = string_between("", 'a');
	if (res_char) {
		sprintf(buf, "Returned: %s, Expected: %p", res_char, NULL);
	} else {
		sprintf(buf, "Returned: %p, Expected: %p", res_char, NULL);
	}
	logger(res_char == NULL, buf);
	if (res_char)
		free(res_char);

	test_num = 1;

	printf("Testing stringsum2()\n");

	stringsum2("abcd", &res_int);
	sprintf(buf, "Returned: %d, Expected: %d", res_int, 10);
	logger(res_int == 10, buf);

	stringsum2("abcd!", &res_int);
	sprintf(buf, "Returned: %d, Expected: %d", res_int, -1);
	logger(res_int == -1, buf);

	stringsum2("bbbdbbbbbdbbdbbbbbddbbbbbdbbdbbbbdbd", &res_int);
	sprintf(buf, "Returned: %d, Expected: %d", res_int, 90);
	logger(res_int == 90, buf);

	stringsum2("", &res_int);
	sprintf(buf, "Returned: %d, Expected: %d", res_int, 0);
	logger(res_int == 0, buf);

	test_num = 1;

	printf("Testing split()\n");

	test_split("abcd", (char *[]){ "abcd", NULL });
	test_split("Hei du", (char *[]){ "Hei", "du", NULL });
	test_split("Dette er mange ord", (char *[]){ "Dette", "er", "mange", "ord", NULL });

	return 0;
}
