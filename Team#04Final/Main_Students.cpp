// Main.cpp
//
// This program is being used for the 2016_08 CS4953 REverse Engineering Project
// It encrypts or decrypts the input file based on the password
//
// The students are given an encrypted file that has the password embedded. The program, given an input file
// extracts the password and decrypts the file.
//
// The task is to create the code to encrypt an arbitrary file, so that this specific decryption program succeeds.
//

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include ".//Sha-256/SHA-256.h"

int sha256(char *fileName, char *dataBuffer, DWORD dataLength, unsigned char sha256sum[32]);


// Our mystery functions.
// Mystery function 1 is a nibble swap.
char mysteryOne(unsigned char currentLetter) {

	//unsigned char bitString[9];
	
	//for (int i = 0; i < 8; ++i) {
	//	bitString[i] = temp % 2;
	//	temp = temp / 2;
	//}
	//bitString[8] = '\0';

	unsigned char var_1 = currentLetter << 4;
	unsigned char var_2 = (currentLetter >> 4);
	var_2 = var_2 & 0x0F;


	return var_1 + var_2;
}


char mysteryTwo(unsigned char currentLetter) {

	unsigned char var_1D = currentLetter & 0xC0;
	var_1D = var_1D >> 2;

	unsigned char var_29 = currentLetter & 3;
	var_29 = var_29 << 2;

	unsigned char var_5 = currentLetter << 2;
	var_5 = var_5 & 0xC0;

	unsigned char var_11 = currentLetter >> 2;
	var_11 = var_11 & 3;

	var_5 = (((var_5 | var_1D) | var_29) | var_11);

	return var_5;
}

// Reverse of Mystery3. The decrypter shifts to the right 1. We just do the reverse and it's shift left 1.
char mysteryThree(char currentLetter, int temp2) {

	return currentLetter << 1;
}




// this function is actually the answer - when completed!
int encryptFile(FILE *fptr, char *password)
{
	char *buffer;
	BYTE pwdHash[32];

	FILE *fptrOut;
	DWORD passwordLength, filesize, i;
	int resulti, pwdHashIndx;

	filesize = _filelength(_fileno(fptr));
	if(filesize > 0x100000)	// 1 MB, file too large
	{
		fprintf(stderr, "Error - Input file too large.\n\n");
		return -1;
	}

	passwordLength = (size_t) strlen(password);

	if(passwordLength == 0 || passwordLength >= 256)
	{
		fprintf(stderr, "Error - Password is too long!\n\n");
		return -1;
	}


	resulti = sha256(NULL, password, passwordLength, pwdHash);
	if(resulti != 0)
	{
		fprintf(stderr, "Error - Password not hashed correctly.\n\n");
		return -1;
	}


	buffer = (char *) malloc(filesize);
	if(buffer == NULL)
	{
		fprintf(stderr, "Error - Could not allocate %d bytes of memory on the heap.\n\n", filesize);
		return -1;
	}


	fread(buffer, 1, filesize, fptr);	// should read entire file

	////////////////////////////////////////////////////////////////////////////////////
	// INSERT ENCRYPTION CODE HERE

	// This stuff is decrypter.
	/*
	DWORD index = 0;
	while (index < (filesize - (passwordLength + 1))) 
	{														// Password Length never changes
		buffer[index] = mysteryOne(buffer[index]);
		buffer[index] = mysteryTwo(buffer[index]);

		if((buffer[index] & 0x02) != 0) {
			buffer[index] = buffer[index] ^ 0x59;            //GO RIGHT
		} else{
			 buffer[index] = buffer[index] ^ 0x34;           //GO LEFT
		}

		buffer[index] = mysteryThree(buffer[index], 1);
		index++;
	}

	*/
	//Encrypter 

	DWORD index = 0;
	while (index < filesize) {
		// Index: 0
		char current_letter = buffer[index];
		char modifiedLetter = mysteryThree(current_letter, 1);
		
		DWORD branch = index & 0x02;
		if (branch == 2) {
			// GO RIGHT
			modifiedLetter = modifiedLetter ^ 0x59;
		}
		else {
			// GO LEFT
			modifiedLetter = modifiedLetter ^ 0x34; 
		}
		modifiedLetter = mysteryTwo(modifiedLetter);
		modifiedLetter = mysteryOne(modifiedLetter);

		buffer[index] = modifiedLetter;
		index += 1; 
	}


	printf("I WILL NOW PERFORM MYSTERYTHREE WITH x81\n");
	printf("RESULT: %x\n", mysteryThree(0x81,1));
	////////////////////////////////////////////////////////////////////////////////////

	fptrOut = fopen("encrypted.txt", "wb+");
	if(fptrOut == NULL)
	{
		fprintf(stderr, "Error - Could not open output file.\n\n");
		free(buffer);
		return -1;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// INSERT OUTPUT FILE WRITING CODE HERE
	// WRITE TO FILE WITH FWRITE - Can
	// Write to file password first.
	//strcat(buff, "\n");
	fwrite(password, sizeof(char), passwordLength, fptrOut);
	fwrite(buffer, sizeof(char), filesize, fptrOut);
	printf("Hello!\n");

	////////////////////////////////////////////////////////////////////////////////////

	fclose(fptrOut);
	return 0;
} // encryptFile


FILE *openInputFile(char *filename)
{
	FILE *fptr;

	fptr = fopen(filename, "rb");
	if(fptr == NULL)
	{
		fprintf(stderr, "Error - Could not input file %s!\n\n", filename);
		exit(-1);
	}

	return fptr;
} // openInputFile

int main(int argc, char *argv[])
{
	FILE *fptr;

	if(argc < 3)
	{
		fprintf(stderr, "\n\nTo encrypt, you must enter the file to encrypt followed by the password.\n\n");
		fprintf(stderr, "%s filename password\n\n", argv[0]);
		exit(0);
	}

	// For refernece, I think Ortiz will want the fileToEncrypt be an argument. So we shall call our program like so...
	// encrypt.exe ./file-to-encrypt *ARBITRARY PASSWORD*
	//char * fileToEncrypt = "secret.txt";
	char * fileToEncrypt = argv[1];
	char * passwordInput = argv[2];


	
	passwordInput = "myghws423%ksldfik0USkeU\n";
	//passwordInput = "Hello!";
	//size_t len = strlen(passwordInput);
	//char * finalPassword = (char *) malloc(len + 2);
	//strcpy(finalPassword, passwordInput);
	//finalPassword[len] = 0x0A;
	//finalPassword[len + 1] = '\0';
	//passwordInput += '\n';
	printf("MY PASSWORD: %s\n", passwordInput);

	fptr = openInputFile(fileToEncrypt);
	encryptFile(fptr, passwordInput);

	fclose(fptr);
	return 0;
} // main