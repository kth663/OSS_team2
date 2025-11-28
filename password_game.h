#ifndef PASSWORD_GAME_H
#define PASSWORD_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PASSWORD_LENGTH 5
#define MAX_INPUT_LENGTH (PASSWORD_LENGTH + 1)
#define DATA_FILE_NAME "password.txt"

void set_utf8_encoding();
void generateRandomPassword();
void savePasswordToFile();
void clearInputBuffer();

int runGame();

void displayWelcomeMessage();
void displayExitMessage();

int executeGame();

#endif