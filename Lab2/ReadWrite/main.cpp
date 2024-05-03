#include <iostream>
#include <string>
#include <fstream>

void helpMenu() {
	std::cout << "Enter filename and operation to do:\n";
	std::cout << "ReadWrite [r/w/h] [fileName] [text to write]\n";
	std::cout << "r - to read the file\n";
	std::cout << "w - to write some text to the file\n";
	std::cout << "h - to see this text\n";
	std::cout << "When writting to file, you can also type text to write to the file, but not more than 1024 chars!\n\n";

	std::cout << "Examples:\n";
	std::cout << "ReadWrite h               - shows this text\n";
	std::cout << "ReadWrite file.txt        - displays the content of file.txt\n";
	std::cout << "ReadWrite r file.txt      - same as the previous one\n";
	std::cout << "ReadWrite w file.txt      - writes fish-text to file.txt\n";
}

bool readFile(char path[]) {
	std::cout << "Reading text from file: " << path << "\n";

	std::ifstream fIn;

	fIn.open(path);

	if (!fIn) {
		std::cout << "Unable to read file " << path << "\n";
		std::cout << "Aboring...\n";
		fIn.close();
		return false;
	}
	else {
		std::string line;
		while (std::getline(fIn, line)) {
			std::cout << line << "\n";
		}
		std::cout << "\nSuccessful!\n";
	}

	fIn.close();

	return true;
}

bool writeToFile(char path[], char text[]) {
	std::cout << "Writing text to file: " << path << "\n";

	std::ofstream fOut;

	fOut.open(path);

	if (!fOut) {
		std::cout << "Unable to write to file " << path << "\n";
		std::cout << "Aborting...\n";
		fOut.close();
		return false;
	}
	else {
		fOut << text;
		fOut.close();
	}


	return true;
}

bool writeToFile(char path[]) {
	char text[] = "Eel archerfish goldfish river stingray; Quillfish Australian lungfish four \
 eyed fish; electric ray gibberfish mouthbrooder Manta Ray ruffe, rock beauty.Scaleless black dragonfish\
 sandbar shark, frigate mackerel Hammerjaw, slimehead,  squarehead catfish sea bream barracudina.Pygmy sunfish \
basslet kahawai, shortnose greeneye pigfish moonfish gombessa peacock flounder, scorpionfish sharksucker\
 lancetfish delta smelt.Rohu gombessa catalufa bighead carp, pencilfish dace cow shark sturgeon tonguefish,\
 conger eel leaffish elephantnose fish threadtail mudminnow peladillo Blacksmelt, platyfish.Sea bream plownose\
 chimaera sleeper skate white croaker platyfish ups\ide - down catfish sawfish.Tube - eye goldeye Canthigaster r\
ostrata triplefin blenny velvet - belly shark leaffish frogmouth catfish dragonfish daggertooth pike conger.Weev\
erfish, false cat shark firefish, wallago peladillo handfish orangespine unicorn fish.Longnose chimaera mustard eel \
hairtail; longfin smelt slender snipe eel lake trout glass catfish codl";

	return writeToFile(path, text);
}

int main(int argc, char* argv[]) {

	std::cout << "Input args:\n";

	for (int i = 0; i < argc; i++) {
		std::cout << "arg[" << i << "] : " << argv[i] << "\n";
	}

	std::cout << "\n";
	if (argc <= 1) {
		helpMenu();
	}
	else if (argc == 2) {
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
			writeToFile(argv[2]);
		}
		else {
			std::cout << "Wrong arguments!\n";
			std::cout << "Run 'ReadWrite h' to get help\n";
		}
	}
	else {
		std::cout << "Too many arguments!\n";
		std::cout << "Run 'ReadWrite h' to get help\n";
	}


	return 0;
}