/**
*
* Solution to course project # 9
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Ilian Zapryanov
* @idnumber 8MI0600405
* @compiler VC
*
* <предназначение на файла>
*
*/

#include <iostream>
#include <fstream>

const int MAX_SIZE_INPUT = 101;

const int MAX_SIZE_USERNAME = 16;
const int MIN_SIZE_USERNAME = 4;

const int MAX_SIZE_PASSWORD = 32;
const int MIN_SIZE_PASSWORD = 8;

const int MAX_SIZE_OF_FILE_NAME = 40;

const int MAX_SIZE_OF_MATRIX_LEVEL_ONE = 5;
const int MAX_SIZE_OF_MATRIX_LEVEL_TWO = 7;
const int MAX_SIZE_OF_MATRIX_LEVEL_THREE_AND_FOUR = 11;
const int MAX_SIZE_OF_MATRIX_LEVEL_FIVE = 15;

void chooseUser(char username[]);
void registerUser(char username[]);
void loginUser(char username[]);

void playNonogram(char username[], char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram, int mistakeCounter, int levelAccess, int currentLevel, int levelVariation);
bool isGameIsWon(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);
void saveProgressNonogramSheet(char username[], char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram, int levelAccess, int currentLevel, int levelVariation);
void continueOrNewGame(char username[]);
void loadSavedGame(char username[]);
void clearCache(char username[]);

void getFileName(char str[], char fileName[]);
void getUsername(char username[], char usernameInput[]);
int getLevelOfUsername(char username[]);
void checkLevelIfNeedsIncrease(char username[], int& levelAccess, int& currentLevel, int sizeOfMatrixNonogram);
int chooseLevel(char username[], int levelAccess);
void getMatrixOfChosenLevel(char username[], int currentLevel, char matrixNonogram[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int& sizeOfMatrixNonogram, int& levelVariation);
void saveLevelStartedInFile(char username[], int currentLevel, int levelVariation);
void saveLevelAccess(char username[], int levelAccess);
void saveIncreasedMistakes(char username[], int mistakesCounter);

void determineMatrixSize(int currentLevel, int& sizeOfMatrixNonogram);
void printNonogram(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);

void getRowHints(char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int rowHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);
void getColHints(char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int rowHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);
int getHintsMaxWidth(int rowHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);
int getHintsMaxHeight(int colHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram);
int getHintsHeightForColumn(int colHints[]);
int getWidthForRowHints(int rowHints[]);

void checkIfRowOrColIsFilled(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int col, int sizeOfMatrixNonogram);
int getXInCol(char matrix[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int col, int sizeOfMatrixNonogram);
int getXInRow(char matrix[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int sizeOfMatrixNonogram);
void matrixNonogramSheetFillCol(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int col, int sizeOfMatrixNonogram);
void matrixNonogramSheetFillRow(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int sizeOfMatrixNonogram);

bool isUsernameValid(char username[]);
bool isPasswordValid(char username[], char password[]);
bool isAlpha(char ch);
bool isDigit(char ch);
bool isMatch(char firstStr[], char secondStr[]);
bool isSymbolAllowed(char ch);
bool isCoordinateValid(int coordinate, int sizeOfMatrixNonogram);

int getSizeOfStr(char str[]);

int main()
{

	char username[MAX_SIZE_INPUT] = "";
	chooseUser(username);

	//if level is lost or won, start from the beginning
	while (true)
	{
		int levelAccess = getLevelOfUsername(username);
		int levelVariation = 0;
		int currentLevel = chooseLevel(username, levelAccess);

		char matrixNonogramAnswer[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};
		char matrixNonogramSheet[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};

		int sizeOfMatrixNonogram = 0;
		getMatrixOfChosenLevel(username, currentLevel, matrixNonogramAnswer, sizeOfMatrixNonogram, levelVariation);

		printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);
		playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, 0, levelAccess, currentLevel, levelVariation);
	}

	return 0;
}


void chooseUser(char username[])
{
	std::cout << "Select option:\n[1] Register \n[2] Login\n\nYour choice:" << std::endl;

	int choice = 0;
	std::cin >> choice;

	// [1] register
	if (choice == 1)
	{
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		registerUser(username);
	}

	// [2] login
	else if (choice == 2)
	{
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		loginUser(username);
	}

	// else invalid
	else
	{

		std::cout << "[ERROR] Invalid option. Please try again." << std::endl;
		std::cout << std::endl;

		std::cin.clear();
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		chooseUser(username);
	}
}


void registerUser(char username[])
{

	std::cout << "Select a Username: ";
	std::cin.getline(username, MAX_SIZE_INPUT);

	//if username is not valid, return to choosing register/login operation
	if (!isUsernameValid(username))
	{
		chooseUser(username);
		return;
	}

	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	//check if user exists
	std::ifstream fileIn(fileName);
	if (fileIn.is_open())
	{
		std::cout << "[ERROR] User already exists." << std::endl;
		std::cout << std::endl;
		chooseUser(username);
	}

	else
	{
		//save data in file

		char password[MAX_SIZE_INPUT] = "";
		std::cout << "Select a Password: ";
		std::cin.getline(password, MAX_SIZE_INPUT);

		if (!isPasswordValid(username, password))
		{
			chooseUser(username);
			return;
		}
		std::cout << std::endl;

		int currentLevel = 1;

		char fileName[MAX_SIZE_OF_FILE_NAME] = "";
		getFileName(username, fileName);

		std::ofstream file;
		file.open(fileName);
		file << username << std::endl << password << std::endl << currentLevel << std::endl;
		file.close();


		std::cout << "Welcome, " << username << "!" << std::endl;

	}

}


void loginUser(char username[])
{
	char usernameInput[MAX_SIZE_INPUT] = "";
	char passwordInput[MAX_SIZE_INPUT] = "";

	std::cout << "Enter Username: ";
	std::cin.getline(usernameInput, MAX_SIZE_INPUT);

	std::cout << "Enter Password: ";
	std::cin.getline(passwordInput, MAX_SIZE_INPUT);
	std::cout << std::endl;

	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(usernameInput, fileName);

	//if user is found, check if password is right
	std::ifstream fileIn(fileName);
	if (fileIn.is_open())
	{
		char usernameData[MAX_SIZE_INPUT] = "";
		char passwordData[MAX_SIZE_INPUT] = "";

		fileIn.getline(usernameData, MAX_SIZE_INPUT);
		fileIn.getline(passwordData, MAX_SIZE_INPUT);

		//if data is matched log in
		if (isMatch(usernameData, usernameInput) && isMatch(passwordData, passwordInput))
		{

			std::cout << "Welcome, " << usernameInput << "!" << std::endl;

			getUsername(username, usernameInput);
			continueOrNewGame(username);
		}

		else
		{
			std::cout << "[ERROR] wrong password." << std::endl;
			std::cout << std::endl;
			chooseUser(username);
		}

	}

	else
	{
		std::cout << "[ERROR] Username does not exist." << std::endl;
		std::cout << std::endl;
		chooseUser(username);
	}
}


void playNonogram(char username[], char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram, int mistakesCounter, int levelAccess, int currentLevel, int levelVariation)
{

	//game lost
	if (mistakesCounter == 3)
	{
		mistakesCounter = 0;
		std::cout << "Game lost. Please select a new level." << std::endl;
		std::cout << std::endl;
		
		clearCache(username);
		return;
	}

	int row = 0, col = 0;
	std::cin >> row >> col;

	//if coords are valid, check the cell
	if (isCoordinateValid(row, sizeOfMatrixNonogram) && isCoordinateValid(col, sizeOfMatrixNonogram))
	{
		//if cell is already filled
		if (matrixNonogramSheet[row][col] == 'X' || matrixNonogramSheet[row][col] == 'O')
		{
			std::cout << "[ERROR] Cell is already filled." << std::endl;
			std::cout << std::endl;
			playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, mistakesCounter, levelAccess, currentLevel, levelVariation);
		}

		//if cell is wrong (empty)
		else if (matrixNonogramAnswer[row][col] != 'X')
		{
			mistakesCounter++;
			saveIncreasedMistakes(username, mistakesCounter);

			matrixNonogramSheet[row][col] = 'O';
			printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);
			std::cout << "[MISTAKE] The cell you have selected is empty. You have " << 3 - mistakesCounter << " lives left." << std::endl;
			std::cout << std::endl;

			saveProgressNonogramSheet(username, matrixNonogramSheet, sizeOfMatrixNonogram, levelAccess, currentLevel, levelVariation);
			playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, mistakesCounter, levelAccess, currentLevel, levelVariation);
		}

		//if cell is right
		else
		{
			matrixNonogramSheet[row][col] = 'X';

			//check if level is won
			if (isGameIsWon(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram))
			{

				printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);

				std::cout << std::endl;
				std::cout << "You have won! Well played!" << std::endl;
				std::cout << std::endl;

				checkLevelIfNeedsIncrease(username, levelAccess, currentLevel, sizeOfMatrixNonogram);
				saveProgressNonogramSheet(username, matrixNonogramSheet, sizeOfMatrixNonogram, levelAccess, currentLevel, levelVariation);
				
				clearCache(username);
				return;
			}

			else
			{

				std::cout << "[ACCURACY] The cell you have selected is right." << std::endl;
				std::cout << std::endl;

				checkIfRowOrColIsFilled(matrixNonogramSheet, matrixNonogramAnswer, row, col, sizeOfMatrixNonogram);
				printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);
				saveProgressNonogramSheet(username, matrixNonogramSheet, sizeOfMatrixNonogram, levelAccess, currentLevel, levelVariation);
				playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, mistakesCounter, levelAccess, currentLevel, levelVariation);

			}

		}

	}

	else
	{
		std::cout << "[ERROR] Invalid coordinates, please select coordinates between [0-" << sizeOfMatrixNonogram - 1 << "]" << std::endl;
		playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, mistakesCounter, levelAccess, currentLevel, levelVariation);
	}
}


bool isGameIsWon(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{
	//check if not filled right cell exists
	for (int row = 0; row < sizeOfMatrixNonogram; row++)
	{
		for (int col = 0; col < sizeOfMatrixNonogram; col++)
		{
			if (matrixNonogramSheet[row][col] != matrixNonogramAnswer[row][col])
			{
				return false;
			}
		}
	}

	return true;
}


void continueOrNewGame(char username[])
{
	std::cout << "Do you want to continue your last game or start a new one? [Y/N]" << std::endl;

	char choice;
	std::cin >> choice;

	//load last level [Y/y] is chosen
	if (choice == 'Y' || choice == 'y')
	{
		loadSavedGame(username);
	}

	//start over if [N/n] is chosen
	else if (choice == 'N' || choice == 'n')
	{
		int levelAccess = getLevelOfUsername(username);
		int currentLevel = chooseLevel(username, levelAccess);

		char matrixNonogramAnswer[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};
		char matrixNonogramSheet[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};

		int sizeOfMatrixNonogram = 0;
		int levelVariation = 0;

		getMatrixOfChosenLevel(username, currentLevel, matrixNonogramAnswer, sizeOfMatrixNonogram, levelVariation);
		clearCache(username);

		printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);
		playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, 0, levelAccess, currentLevel, levelVariation);
	}

	else
	{
		std::cin.clear();
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		std::cout << "[ERROR] Please select a valid option." << std::endl;

		continueOrNewGame(username);
	}
}


void saveProgressNonogramSheet(char username[], char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram, int levelAccess, int currentLevel, int levelVariation)
{
	char fileName[MAX_SIZE_OF_FILE_NAME];
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}
	fileIn.close();

	std::ofstream fileOut(fileName);
	for (int i = 0; i < 4; i++)
	{
		fileOut << lines[i] << std::endl;
	}

	//save last changes to the sheet
	for (int i = 0; i < sizeOfMatrixNonogram; i++)
	{
		for (int j = 0; j < sizeOfMatrixNonogram; j++)
		{
			fileOut << matrixNonogramSheet[i][j];
		}

		fileOut << std::endl;
	}

	fileOut.close();
}


void loadSavedGame(char username[])
{
	//load all data from user's file
	char matrixNonogramSheet[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE];
	char matrixNonogramAnswer[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};
	int sizeOfMatrixNonogram = 0;

	int levelAccess = 0;
	int currentLevel = 0;
	int levelVariation = 0;
	int mistakesCounter = 0;

	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}

	levelAccess = lines[2][0] - '0';

	//check if level is NOT started
	if (lineCount - 1 < 3)
	{
		std::cin.clear();
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		std::cout << "[ERROR] You have not started a level yet." << std::endl;
		std::cout << std::endl;

		return;
	}

	//if level is started
	else
	{
		//get all info 
		currentLevel = lines[3][0] - '0';
		levelVariation = lines[3][1] - '0';
		mistakesCounter = lines[3][2] - '0';

		determineMatrixSize(currentLevel, sizeOfMatrixNonogram);
		std::ifstream fileInMatrix(fileName);
		int lineCountMatrix = 0;
		while (fileInMatrix.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCountMatrix < 3)
		{
			lineCountMatrix++;
		}

		for (int i = 0; i < sizeOfMatrixNonogram; i++)
		{
			for (int j = 0; j < sizeOfMatrixNonogram; j++)
			{
				fileInMatrix >> matrixNonogramSheet[i][j];
			}
		}
		fileInMatrix.close();

		getMatrixOfChosenLevel(username, currentLevel, matrixNonogramAnswer, sizeOfMatrixNonogram, levelVariation);
		printNonogram(matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram);
		playNonogram(username, matrixNonogramSheet, matrixNonogramAnswer, sizeOfMatrixNonogram, mistakesCounter, levelAccess, currentLevel, levelVariation);
	}

}


void clearCache(char username[])
{
	//clear not needed info [matrixNonogramSheet/currentLevel/levelVariation]
	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}
	fileIn.close();

	std::ofstream fileOut(fileName);
	for (int i = 0; i <= 2; i++)
	{
		fileOut << lines[i] << std::endl;
	}

	fileOut.close();
}


void getFileName(char str[], char fileName[])
{
	//concat [username + .txt] 
	char txtStr[5] = ".txt";

	int sizeOfStr = getSizeOfStr(str);
	int sizeOfTxt = getSizeOfStr(txtStr);

	for (int i = 0; i < sizeOfStr; i++)
	{
		fileName[i] = str[i];
	}

	int j = 0;
	while (txtStr[j] != '\0')
	{
		fileName[sizeOfStr + j] = txtStr[j];
		j++;
	}
	fileName[sizeOfStr + j] = '\0';

}


void getUsername(char username[], char usernameInput[])
{
	int size = getSizeOfStr(usernameInput);

	for (int i = 0; i < size; i++)
	{
		username[i] = usernameInput[i];
	}

}


int getLevelOfUsername(char username[])
{
	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	std::ifstream fileIn(fileName);

	char line[MAX_SIZE_INPUT] = "";
	int lineCount = 0;

	while (fileIn.getline(line, MAX_SIZE_INPUT))
	{
		lineCount++;
		if (lineCount == 3)
		{
			return line[0] - '0';
		}
	}

	return -1;
}


void checkLevelIfNeedsIncrease(char username[], int& levelAccess, int& currentLevel, int sizeOfMatrixNonogram)
{
	if (sizeOfMatrixNonogram == MAX_SIZE_OF_MATRIX_LEVEL_ONE && currentLevel == 1)
	{
		levelAccess++;
	}

	else if (sizeOfMatrixNonogram == MAX_SIZE_OF_MATRIX_LEVEL_TWO && currentLevel == 2)
	{
		levelAccess++;
	}

	else if (sizeOfMatrixNonogram == MAX_SIZE_OF_MATRIX_LEVEL_THREE_AND_FOUR && currentLevel == 3)
	{
		levelAccess++;
	}

	else if (sizeOfMatrixNonogram == MAX_SIZE_OF_MATRIX_LEVEL_THREE_AND_FOUR && currentLevel == 4)
	{
		levelAccess++;
	}

	else if (sizeOfMatrixNonogram == MAX_SIZE_OF_MATRIX_LEVEL_FIVE && currentLevel == 5)
	{
		levelAccess++;
	}

	saveLevelAccess(username, levelAccess);
}


void saveLevelAccess(char username[], int levelAccess)
{
	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}
	fileIn.close();

	std::ofstream fileOut(fileName);
	
	//save levelAccess over when it's increased
	for (int i = 0; i < lineCount; i++)
	{
		if (i == 2)
		{
			lines[i][0]++;
			fileOut << lines[i] << std::endl;

			continue;
		}

		fileOut << lines[i] << std::endl;
	}
	fileOut.close();
}


void saveIncreasedMistakes(char username[], int mistakesCounter)
{
	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}
	fileIn.close();

	//save mistakeCounter over when it's increased
	std::ofstream fileOut(fileName);
	for (int i = 0; i < lineCount; i++)
	{
		if (i == 3)
		{
			lines[i][2]++;
			fileOut << lines[i] << std::endl;
			continue;
		}

		fileOut << lines[i] << std::endl;
	}

	fileOut.close();
}


int chooseLevel(char username[], int levelAccess)
{

	std::cout << "You have access to:" << std::endl;
	for (int i = 1; i <= levelAccess; i++)
	{
		std::cout << "-Level: " << i << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Please, select the level you want to play by submitting its number:" << std::endl;

	int currentLevel = 0;
	std::cin >> currentLevel;

	//if input level is valid choose it
	if (currentLevel >= 1 && currentLevel <= levelAccess)
	{
		std::cout << "Level [" << currentLevel << "] was successfully chosen." << std::endl;
		return currentLevel;
	}

	//if input level is not valid [no access]
	else
	{
		std::cin.clear();
		std::cin.ignore(MAX_SIZE_INPUT, '\n');
		std::cout << "[ERROR] Please select a valid level between 1 and " << levelAccess << std::endl;
		return chooseLevel(username, levelAccess);
	}
}


void getMatrixOfChosenLevel(char username[], int currentLevel, char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int& sizeOfMatrixNonogram, int& levelVariation)
{
	//get random variation
	levelVariation = rand() % 2 + 1;

	char lines[MAX_SIZE_INPUT] = "";
	determineMatrixSize(currentLevel, sizeOfMatrixNonogram);

	char levelConcat[3] = "";
	levelConcat[0] = currentLevel + '0';
	levelConcat[1] = levelVariation + '0';

	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(levelConcat, fileName);

	std::ifstream fileIn(fileName);

	int i = 0, j = 0;
	while (fileIn.getline(lines, MAX_SIZE_INPUT))
	{
		if (lines[0] == '\0')
		{
			break;
		}

		//get coords of right cell
		i = lines[0] - '0';
		j = lines[2] - '0';

		matrixNonogramAnswer[i][j] = 'X';
	}
	fileIn.close();

	//set rest to wrong (empty) cells
	for (int i = 0; i < sizeOfMatrixNonogram; i++)
	{
		for (int j = 0; j < sizeOfMatrixNonogram; j++)
		{
			if (matrixNonogramAnswer[i][j] != 'X')
			{
				matrixNonogramAnswer[i][j] = 'O';
			}
		}
	}

	saveLevelStartedInFile(username, currentLevel, levelVariation);

}


//get size of nonogram level
void determineMatrixSize(int currentLevel, int& sizeOfMatrixNonogram)
{
	if (currentLevel == 1)
	{
		sizeOfMatrixNonogram = MAX_SIZE_OF_MATRIX_LEVEL_ONE;
	}

	else if (currentLevel == 2)
	{
		sizeOfMatrixNonogram = MAX_SIZE_OF_MATRIX_LEVEL_TWO;
	}

	else if (currentLevel == 3 || currentLevel == 4)
	{
		sizeOfMatrixNonogram = MAX_SIZE_OF_MATRIX_LEVEL_THREE_AND_FOUR;
	}

	else if (currentLevel == 5)
	{
		sizeOfMatrixNonogram = MAX_SIZE_OF_MATRIX_LEVEL_FIVE;
	}
}


void printNonogram(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{
	int rowHints[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};
	int colHints[MAX_SIZE_OF_MATRIX_LEVEL_FIVE][MAX_SIZE_OF_MATRIX_LEVEL_FIVE] = {};

	getRowHints(matrixNonogramAnswer, rowHints, sizeOfMatrixNonogram);
	getColHints(matrixNonogramAnswer, colHints, sizeOfMatrixNonogram);

	int maxRowHintWidth = getHintsMaxWidth(rowHints, sizeOfMatrixNonogram);
	int maxColHintHeight = getHintsMaxHeight(colHints, sizeOfMatrixNonogram);

	//print hints of cols
	for (int hintRow = 0; hintRow < maxColHintHeight; hintRow++)
	{
		for (int space = 0; space < maxRowHintWidth + 3; space++)
		{
			std::cout << " ";
		}

		for (int col = 0; col < sizeOfMatrixNonogram; col++)
		{

			int hintStart = maxColHintHeight - getHintsHeightForColumn(colHints[col]);
			if (hintRow >= hintStart)
			{
				std::cout << colHints[col][hintRow - hintStart] << " ";
			}

			else
			{
				std::cout << "  ";
			}

		}

		std::cout << std::endl;
	}

	for (int line = 0; line < sizeOfMatrixNonogram + maxRowHintWidth; line++)
	{
		std::cout << "--";
	}
	std::cout << std::endl;


	//print hints of rows
	for (int row = 0; row < sizeOfMatrixNonogram; row++)
	{
		int spaceForHints = maxRowHintWidth - getWidthForRowHints(rowHints[row]);
		for (int space = 0; space < spaceForHints; space++)
		{
			std::cout << " ";
		}

		for (int hint = 0; hint < MAX_SIZE_OF_MATRIX_LEVEL_FIVE && rowHints[row][hint] != 0; hint++)
		{
			std::cout << rowHints[row][hint];
			if (rowHints[row][hint + 1] != 0) std::cout << " ";
		}

		std::cout << " |";

		for (int col = 0; col < sizeOfMatrixNonogram; col++)
		{

			if (matrixNonogramSheet[row][col] != 'X' && matrixNonogramSheet[row][col] != 'O')
			{
				std::cout << "  ";
			}

			else
			{
				std::cout << " " << matrixNonogramSheet[row][col];
			}
		}

		std::cout << std::endl;
	}

}


void getRowHints(char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int rowHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{

	for (int row = 0; row < sizeOfMatrixNonogram; row++)
	{
		int count = 0, indexHints = 0;
		for (int col = 0; col < sizeOfMatrixNonogram; col++)
		{

			if (matrixNonogramAnswer[row][col] == 'X')
			{
				count++;
			}

			else if (count > 0) {
				rowHints[row][indexHints] = count;
				indexHints++;
				count = 0;
			}
		}

		if (count > 0)
		{
			rowHints[row][indexHints] = count;
		}

	}

}


void getColHints(char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int colHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{

	for (int col = 0; col < sizeOfMatrixNonogram; col++)
	{
		int count = 0, indexHints = 0;
		for (int row = 0; row < sizeOfMatrixNonogram; row++)
		{
			if (matrixNonogramAnswer[row][col] == 'X')
			{
				count++;
			}

			else if (count > 0)
			{
				colHints[col][indexHints] = count;
				indexHints++;
				count = 0;
			}
		}
		if (count > 0)
		{
			colHints[col][indexHints] = count;
		}
	}
}


//get the needed width to print the hints
int getHintsMaxWidth(int rowHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{
	int maxRowHintWidth = 0;

	for (int row = 0; row < sizeOfMatrixNonogram; row++)
	{
		int rowHintWidth = 0;
		for (int col = 0; col < MAX_SIZE_OF_MATRIX_LEVEL_FIVE && rowHints[row][col] != 0; col++)
		{
			rowHintWidth += 2;
		}

		if (rowHintWidth > 0)
		{
			rowHintWidth -= 1;
		}

		if (rowHintWidth > maxRowHintWidth)
		{
			maxRowHintWidth = rowHintWidth;
		}
	}

	return maxRowHintWidth;
}


//get the needed height to print the hints
int getHintsMaxHeight(int colHints[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int sizeOfMatrixNonogram)
{
	int maxColHintHeight = 0;
	for (int col = 0; col < sizeOfMatrixNonogram; col++)
	{
		int colHintHeight = 0;
		for (int row = 0; row < MAX_SIZE_OF_MATRIX_LEVEL_FIVE && colHints[col][row] != 0; row++)
		{
			colHintHeight++;
		}

		if (colHintHeight > maxColHintHeight)
		{
			maxColHintHeight = colHintHeight;
		}
	}

	return maxColHintHeight;
}


int getHintsHeightForColumn(int colHints[])
{
	int height = 0;
	for (int i = 0; i < MAX_SIZE_OF_MATRIX_LEVEL_FIVE && colHints[i] != 0; i++)
	{
		height++;
	}

	return height;
}


int getWidthForRowHints(int rowHints[])
{

	int width = 0;
	for (int i = 0; i < MAX_SIZE_OF_MATRIX_LEVEL_FIVE && rowHints[i] != 0; i++)
	{
		width += 2;
	}

	if (width > 0)
	{
		width--;
	}

	return width;
}


void checkIfRowOrColIsFilled(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], char matrixNonogramAnswer[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int col, int sizeOfMatrixNonogram)
{

	//if row or col is filled, fill the rest with 'O' symbol [empty]
	int counterXInColAnswer = getXInCol(matrixNonogramAnswer, col, sizeOfMatrixNonogram);
	int counterXInColSheet = getXInCol(matrixNonogramSheet, col, sizeOfMatrixNonogram);

	if (counterXInColAnswer == counterXInColSheet)
	{
		//fill col
		matrixNonogramSheetFillCol(matrixNonogramSheet, col, sizeOfMatrixNonogram);
	}

	int counterXInRowAnswer = getXInRow(matrixNonogramAnswer, row, sizeOfMatrixNonogram);
	int counterXInRowSheet = getXInRow(matrixNonogramSheet, row, sizeOfMatrixNonogram);

	if (counterXInRowAnswer == counterXInRowSheet)
	{
		//fill row
		matrixNonogramSheetFillRow(matrixNonogramSheet, row, sizeOfMatrixNonogram);
	}
}


int getXInCol(char matrix[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int col, int sizeOfMatrixNonogram)
{
	//count Xs in col
	int counterXInCol = 0;
	for (int currentRow = 0; currentRow < sizeOfMatrixNonogram; currentRow++)
	{
		if (matrix[currentRow][col] == 'X')
		{
			counterXInCol++;
		}
	}
	return counterXInCol;
}


int getXInRow(char matrix[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int sizeOfMatrixNonogram)
{
	//count Xs in row
	int counterXInRow = 0;
	for (int currentCol = 0; currentCol < sizeOfMatrixNonogram; currentCol++)
	{
		if (matrix[row][currentCol] == 'X')
		{
			counterXInRow++;
		}
	}
	return counterXInRow;
}


void matrixNonogramSheetFillCol(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int col, int sizeOfMatrixNonogram)
{
	//fill the rest
	for (int currentRow = 0; currentRow < sizeOfMatrixNonogram; currentRow++)
	{
		if (matrixNonogramSheet[currentRow][col] != 'X')
		{
			matrixNonogramSheet[currentRow][col] = 'O';
		}
	}
}


void matrixNonogramSheetFillRow(char matrixNonogramSheet[][MAX_SIZE_OF_MATRIX_LEVEL_FIVE], int row, int sizeOfMatrixNonogram)
{
	//fill the rest
	for (int currentCol = 0; currentCol < sizeOfMatrixNonogram; currentCol++)
	{
		if (matrixNonogramSheet[row][currentCol] != 'X')
		{
			matrixNonogramSheet[row][currentCol] = 'O';
		}
	}
}


bool isUsernameValid(char username[])
{
	//username validation
	int size = getSizeOfStr(username);
	if (size < 4 || size > 16)
	{
		std::cout << "[ERROR] Username should be 4-16 characters long." << std::endl;
		std::cout << std::endl;
		return false;
	}

	for (int i = 0; i < size; i++)
	{
		if (!(isAlpha(username[i]) || isDigit(username[i])))
		{

			std::cout << "[ERROR] Username should contain only alphanumeric characters." << std::endl;
			std::cout << std::endl;
			return false;
		}
	}

	return true;
}


bool isPasswordValid(char username[], char password[])
{
	//password validation
	int size = getSizeOfStr(password);
	if (size < 8 || size > 32)
	{
		std::cout << "[ERROR] Password should be 8-32 characters long." << std::endl;
		std::cout << std::endl;
		return false;
	}

	for (int i = 0; i < size; i++)
	{
		if (!isSymbolAllowed(password[i]))
		{
			std::cout << "[ERROR] Password contains non-allowed characters. \\n\\t \'\";&|/\\:*?" << std::endl;
			std::cout << std::endl;
			return false;
		}
	}

	return true;
}


bool isAlpha(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}


//check if input info matches data info
bool isMatch(char firstStr[], char secondStr[])
{
	int sizeOfUsernameData = getSizeOfStr(firstStr);
	int sizeOfUsernameInput = getSizeOfStr(secondStr);

	if (sizeOfUsernameData != sizeOfUsernameInput)
	{
		return false;
	}

	for (int i = 0; i < sizeOfUsernameData; i++)
	{
		if (firstStr[i] != secondStr[i])
		{
			return false;
		}
	}

	return true;
}


bool isSymbolAllowed(char ch)
{
	if (ch == ' ' || ch == '\n' || ch == '\t')
	{
		return false;
	}

	else if (ch == '\'' || ch == '\"' || ch == ';' || ch == '&' || ch == '|')
	{
		return false;
	}

	else if (ch == '/' || ch == '\\' || ch == ':' || ch == '*' || ch == '?')
	{
		return false;
	}

	return true;
}


bool isCoordinateValid(int coordinate, int sizeOfMatrixNonogram)
{
	for (int i = 0; i < sizeOfMatrixNonogram; i++)
	{
		if (coordinate == i)
		{
			return true;
		}
	}

	return false;
}


int getSizeOfStr(char str[])
{

	int size = 0;
	while (*(str++))
	{
		size++;
	}

	return size;
}


void saveLevelStartedInFile(char username[], int currentLevel, int levelVariation)
{
	char fileName[MAX_SIZE_OF_FILE_NAME] = "";
	getFileName(username, fileName);

	char lines[MAX_SIZE_INPUT][MAX_SIZE_INPUT] = {};
	int lineCount = 0;

	std::ifstream fileIn(fileName);
	while (fileIn.getline(lines[lineCount], MAX_SIZE_INPUT) && lineCount < 100)
	{
		lineCount++;
	}
	fileIn.close();

	std::ofstream fileOut(fileName);
	for (int i = 0; i < lineCount; i++)
	{
		fileOut << lines[i] << std::endl;
	}

	int mistakeCounter = 0;
	fileOut << currentLevel << levelVariation << mistakeCounter << std::endl;
	fileOut.close();
}
