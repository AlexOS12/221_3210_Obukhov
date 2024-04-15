#include <stdio.h>
#include <string>

#define BUFFER_SIZE 128

// Показывает меню-подсказку
void helpMenu() {
	printf("Usage:\n");
	printf("stdReadWrite [arguments]\n");
	printf("\nParametrs:\n");
	printf("h        - shows this help menu\n");
	printf("r <file> - reads from file (default, if not specified) \n");
	printf("w <file> - write to file\n");
	printf("\n Examples:\n");
	printf("'stdReadWrite a.txt' - reads text from 'a.txt'\n");
	printf("'stdReadWrite r a.txt' - same as previous\n");
	printf("'stdReadWrite w a.txt' - writes to 'a.txt' \n");
	printf("'stdReadWrite h' - shows this menu \n");
}

// Читает содержимое файла, указанного в path
void readFile(char path[]) {
	printf("Reading file %s\n", path);

	FILE* file = fopen(path, "r");

	if (!file) {
		printf("Could not access file %s\n", path);
		return;
	}

	char content[BUFFER_SIZE];

	size_t charsCount = fread(content, 1, 128, file);

	printf("read %zu chars of %s:\n", charsCount, path);

	if (charsCount == BUFFER_SIZE) {
		printf("%s \n", content);
	}
	else {
		if (feof(file)) {
			printf("Unexpected end of file\n");
			printf("Chars read: %zu\n", charsCount);
			for (int i = 0; i < charsCount; i++) {
				printf("%c", content[i]);
			}
			printf("\n");
		}
		else if (ferror(file)) {
			printf("Error ocured while reading file\n");
		}
	}

	fclose(file);

	return;
}


// Пишет 128 байт в файл, указанный в path
void writeFile(char path[]) {
	printf("Writing 128 bytes to %s", path);

	char content[128] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut dapibus nibh. Nunc varius augue sit amet ex dignissim nec";
	size_t bytesCount = 128;

	FILE* file = fopen(path, "w");

	if (!file) {
		printf("Could not access file %s\n", path);
		return;
	}

	size_t bytesWritten = fwrite(content, 1, 128, file);

	if (128 == bytesWritten) {
		printf("Successfuly written 128 bytes to %s\n", path);
	}
	else {
		printf("Could not fully write to %s\n", path);
		printf("Written only %zu bytes of 128\n", bytesWritten);
	}
}


int main(int argc, char* argv[]) {
	if (argc == 2) {
		if (!strcmp(argv[1], "h")) {
			helpMenu();
		}
		else {
			readFile(argv[1]);
		}
	}
	else if (argc == 3) {
		if (!strcmp(argv[1], "r")) {
			readFile(argv[2]);
		}
		else if (!strcmp(argv[1], "w")) {
			writeFile(argv[2]);
		}
	}
	else {
		printf("Wrong arguments!\n");
		printf("Run 'stdReadWrite h' to get help\n");
	}
	return 0;
}