    #CodeDecodeWithWithoutKey

This program is a C-based implementation of a text encryption and decryption tool using the XOR cipher. It supports encoding text with a key, decoding with a known key, and attempting to decode without a key by analyzing the encrypted text to estimate the key length and probable keys.
Features

Encoding: Encrypts text from Start.txt using a key from Key.txt and saves the result to Coded.txt.
Decoding with Key: Decrypts text from Coded.txt using a known key from Key.txt and saves the result to End.txt.
Decoding without Key: Analyzes Coded.txt to estimate the key length using the index of coincidence method, generates probable keys, and attempts decryption, saving results to Generate_TEXT.txt and the estimated key to Generate_KEY.txt.
Menu Interface: Provides a simple console-based menu to select encoding, decoding with a key, decoding without a key, or exiting the program.
Language Support: Handles both English and Russian alphabets, along with special characters.

    #Prerequisites

Windows Environment: The program uses Windows-specific functions (system("cls"), system("pause"), _CRT_SECURE_NO_WARNINGS) and is designed to compile with the Microsoft C/C++ compiler (cl.exe).
CMake: Required for building the project (version 3.10 or higher recommended).
Microsoft C/C++ Compiler (cl.exe): Typically included with Visual Studio or the Visual Studio Build Tools.
Input Files: The program assumes the following files are present in the same directory as the executable:
Start.txt: Input text file for encoding.
Key.txt: Key file for encoding or decoding.
Coded.txt: Input file for decoding (output of encoding).

    #Usage

Ensure the required input files (Start.txt, Key.txt, etc.) are in the same directory as the executable.
Run the program from the Visual Studio Developer Command Prompt or a command prompt with cl.exe configured:.\CodeDecodeWithWithoutKey.exe


The program displays a menu in Russian:
1) Зашифровать (Encrypt): Reads Start.txt, encrypts with the key from Key.txt, and writes to Coded.txt.
2) Расшифровать с ключом (Decrypt with Key): Reads Coded.txt, decrypts with Key.txt, and writes to End.txt.
3) Расшифровать без ключа (Decrypt without Key): Analyzes Coded.txt to estimate the key length, generates probable keys, and attempts decryption, saving results to Generate_TEXT.txt and the estimated key to Generate_KEY.txt.
4) Выход (Exit): Exits the program.


Select an option by entering the corresponding number (1–4).
Follow the console output for results, such as estimated key length, probable keys, and decryption results.

    #How It Works

Encoding/Decoding: Uses the XOR cipher, where each byte of the input text is XORed with a byte from the key. The key is repeated if it is shorter than the input text.
Key Length Estimation: Uses the index of coincidence method to estimate the key length by analyzing repeating patterns in the ciphertext.
Key Recovery: Attempts to recover the key by XORing ciphertext blocks with a space character and analyzing the frequency of resulting characters to identify likely key bytes.
Output Files:
Coded.txt: Encrypted text.
End.txt: Decrypted text (with known key).
Generate_TEXT.txt: Decrypted text (without known key).
Generate_KEY.txt: Estimated key from the key recovery process.
probably_keys.txt: List of probable keys generated during key recovery.

    #Limitations

The program is Windows-specific due to its reliance on cl.exe and Windows-specific functions. For cross-platform use, modifications are needed (e.g., replace system("cls") with portable alternatives).
The program assumes input files exist and are readable. No error handling is implemented for missing or corrupted files.
Key length estimation relies on a predefined probability threshold (PROBABILITY = 0.046), which may not work well for all texts.
The menu and console output are in Russian, which may require localization for non-Russian users.
The key recovery algorithm assumes the space character is common in the plaintext, which may not hold for all inputs.

    #Example Files

Start.txt (example input text):Hello, World! This is a test.


Key.txt (example key):secret


Coded.txt (output after encoding):<binary data>


End.txt (output after decoding with key):Hello, World! This is a test.


Generate_TEXT.txt (output after decoding without key):<approximated original text>


Generate_KEY.txt (estimated key):secret

    #Notes

The program uses the Russian locale (setlocale(LC_ALL, "ru")) for console output. To support other languages, modify the locale and character sets (ALPHABET_EN, ALPHABET_RU, SPECIAL) as needed.
For better key recovery, adjust PROBABILITY, BLOCK_SIZE, or GENERATION constants based on the input text characteristics.
If you encounter compilation issues, ensure the Visual Studio environment is properly configured and that cl.exe is accessible.

    License

This project is unlicensed. Feel free to use and modify it as needed.