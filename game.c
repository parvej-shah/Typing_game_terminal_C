#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

// Game configuration constants
#define MAX_WORDS 1000       // Maximum number of words in the game
#define MAX_WORD_LENGTH 20   // Maximum length of a single word
#define EASY_WORD_COUNT 20   // Number of words for easy mode
#define MEDIUM_WORD_COUNT 40 // Number of words for medium mode
#define HARD_WORD_COUNT 60   // Number of words for hard mode

// Color definitions for better visual feedback
#define COLOR_HEADER 1    // Green text for headers
#define COLOR_ERROR 2     // Red text for errors
#define COLOR_DEFAULT 3   // White text for normal display
#define COLOR_HIGHLIGHT 4 // Cyan text for highlights
#define COLOR_WARNING 5   // Yellow text for warnings
#define COLOR_SUCCESS 6   // Green for success
#define COLOR_INFO 7      // Blue for information

// Word storage arrays categorized by difficulty
char easy_words[100][5];   // Words with 1-4 letters
char medium_words[100][8]; // Words with 5-7 letters
char hard_words[100][21];  // Words with 8+ letters
int easy_words_len = 0;
int medium_words_len = 0;
int hard_words_len = 0;

// Game state variables
char game_mood;          // Current difficulty level
char passage[MAX_WORDS]; // Passage to be typed
time_t start_time;       // Game start timestamp

// Function prototypes
void initializeNcurses(void);
void cleanupNcurses(void);
void load_words(void);
void create_passage(char *passage, char mood);
void show_progress(double elapsed, int words_typed, int correct, int incorrect);
void game_menu(void);
void show_results(int total_chars, int correct_chars, int mistakes);
void main_game(void);

void initializeNcurses(void)
{
    initscr();
    cbreak();             // to enable some key features like esc to exit
    noecho();             // for not showing users typed word automatically
    keypad(stdscr, TRUE); // for special key handling
    start_color();

    // Initialize color pairs for different UI elements
    init_pair(COLOR_HEADER, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_HIGHLIGHT, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WARNING, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_INFO, COLOR_BLUE, COLOR_BLACK);

    // Set cursor visibility
    curs_set(1);
}

void cleanupNcurses(void)
{
    endwin();
}

void load_words(void)
{
    char word[50];
    // Read words from redirected file and categorize by length
    while (scanf("%49s", word) != EOF)
    {
        int len = strlen(word);
        if (len >= 8 && hard_words_len < 100)
        {
            strcpy(hard_words[hard_words_len++], word);
        }
        else if (len >= 5 && len <= 7 && medium_words_len < 100)
        {
            strcpy(medium_words[medium_words_len++], word);
        }
        else if (len < 5 && easy_words_len < 100)
        {
            strcpy(easy_words[easy_words_len++], word);
        }
    }
    fflush(stdin);
    flushinp();
}

void create_passage(char *passage, char mood)
{
    passage[0] = '\0';
    int word_count = (mood == '1') ? EASY_WORD_COUNT : (mood == '2') ? MEDIUM_WORD_COUNT
                                                                     : HARD_WORD_COUNT;

    // Generate random passage based on difficulty
    for (int j = 0; j < word_count; j++)
    {
        char *word;
        int random_index;

        // Select word based on difficulty
        if (mood == '1' && easy_words_len > 0)
        {
            random_index = rand() % easy_words_len;
            word = easy_words[random_index];
        }
        else if (mood == '2' && medium_words_len > 0)
        {
            random_index = rand() % medium_words_len;
            word = medium_words[random_index];
        }
        else if (hard_words_len > 0)
        {
            random_index = rand() % hard_words_len;
            word = hard_words[random_index];
        }
        else
        {
            break;
        }

        strcat(passage, word);
        strcat(passage, " ");
    }

    // Remove trailing space
    int len = strlen(passage);
    if (len > 0)
    {
        passage[len - 1] = '\0';
    }
}

void show_progress(double elapsed, int words_typed, int correct, int incorrect)
{

    // Display progress bar at top of screen
    move(0, 0);
    clrtoeol();

    int total_keystrokes = correct + incorrect;
    double accuracy = (total_keystrokes > 0) ? (correct * 100.0 / total_keystrokes) : 100.0;
    double wpm = (elapsed > 0) ? ceil(words_typed / (elapsed / 60.0)) : 0;

    // Format progress display with colors
    attron(COLOR_PAIR(COLOR_INFO));
    printw("Time: %.0fs | ", elapsed);
    attron(COLOR_PAIR(COLOR_SUCCESS));
    printw("Correct: %d | ", correct);
    attron(COLOR_PAIR(COLOR_ERROR));
    printw("Mistakes: %d | ", incorrect);
    attron(COLOR_PAIR(COLOR_HIGHLIGHT));
    printw("WPM: %.0f | Accuracy: %.1f%%", wpm, accuracy);
    attroff(COLOR_PAIR(COLOR_HIGHLIGHT));

    refresh();
}

void game_menu(void)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw menu border
    box(stdscr, 0, 0);

    // Center menu text
    move(max_y / 2 - 3, max_x / 2 - 15);
    attron(COLOR_PAIR(COLOR_HEADER));
    printw("=== TYPING SPEED TEST ===\n");

    move(max_y / 2 - 1, max_x / 2 - 15);
    attron(COLOR_PAIR(COLOR_INFO));
    printw("Select Difficulty Level:\n");
    move(max_y / 2 + 1, max_x / 2 - 15);
    printw("1. Easy   (1-4 letter words)\n");
    move(max_y / 2 + 2, max_x / 2 - 15);
    printw("2. Medium (5-7 letter words)\n");
    move(max_y / 2 + 3, max_x / 2 - 15);
    printw("3. Hard   (8+ letter words)\n");
    attroff(COLOR_PAIR(COLOR_INFO));

    char mood = getch();
    if (mood >= '1' && mood <= '3')
    {
        game_mood = mood;
        create_passage(passage, mood);
    }
    else
    {
        move(max_y / 2 + 5, max_x / 2 - 15);
        attron(COLOR_PAIR(COLOR_ERROR));
        printw("Invalid choice! Press any key...");
        attroff(COLOR_PAIR(COLOR_ERROR));
        getch();
        clear();
        game_menu();
    }
}

void show_results(int total_chars, int correct_chars, int mistakes)
{
    double elapsed = difftime(time(NULL), start_time);
    if (correct_chars > total_chars)
    {
        correct_chars = total_chars - mistakes;
    }

    // Calculate statistics
    int avg_word_length = (game_mood == '1') ? 4 : (game_mood == '2' ? 6 : 8);
    double words = (double)correct_chars / avg_word_length; // using actual word length based on difficulty instead of avg 5;
    double wpm = ceil((words / elapsed) * 60.0);
    double accuracy = (correct_chars * 100.0) / (correct_chars + mistakes);
    double cps = ceil((correct_chars) / elapsed);

    // Display results in centered box
    int max_y, max_x;
    clear();
    getmaxyx(stdscr, max_y, max_x);

    // Draw results border
    box(stdscr, 0, 0);

    int start_x = (max_x - 40) / 2;
    int start_y = (max_y - 12) / 2;

    // Display header
    attron(COLOR_PAIR(COLOR_HEADER));
    move(start_y, start_x);
    printw("======= TYPING RESULTS =======");
    attroff(COLOR_PAIR(COLOR_HEADER));

    // Display statistics with colors
    attron(COLOR_PAIR(COLOR_SUCCESS));
    move(start_y + 2, start_x);
    printw("Words per Minute: %.0f", wpm);
    move(start_y + 3, start_x);
    printw("Accuracy: %.2f%%", accuracy);
    move(start_y + 4, start_x);
    printw("Characters per Second: %.0f", cps);
    move(start_y + 5, start_x);
    printw("Total Keystrokes: %d", mistakes + correct_chars);
    attroff(COLOR_PAIR(COLOR_SUCCESS));

    attron(COLOR_PAIR(COLOR_ERROR));
    move(start_y + 6, start_x);
    printw("Total Mistakes: %d", mistakes);
    attroff(COLOR_PAIR(COLOR_ERROR));

    attron(COLOR_PAIR(COLOR_INFO));
    move(start_y + 7, start_x);
    printw("Time Elapsed: %.0fs", elapsed);
    attroff(COLOR_PAIR(COLOR_INFO));

    // Display footer
    attron(COLOR_PAIR(COLOR_HIGHLIGHT));
    move(start_y + 9, start_x);
    printw("Press 'R' to restart or any key to exit");
    attroff(COLOR_PAIR(COLOR_HIGHLIGHT));

    refresh();

    // Handle restart
    int ch = getch();
    if (ch == 'r' || ch == 'R')
    {
        clear();
        game_menu();
        main_game();
    }
    else
    {
        clear();
        move(max_y / 2, max_x / 2 - 10);
        attron(COLOR_PAIR(COLOR_HEADER));
        printw("Thanks for playing!");
        move(max_y / 2 + 1, max_x / 2 - 12);
        printw("Press any key to exit...");
        attroff(COLOR_PAIR(COLOR_HEADER));
    }
}

void main_game(void)
{
    int correct = 0;
    int incorrect = 0;
    char typed_text[MAX_WORDS] = {0};
    int cursor_pos = 0;
    int max_y, max_x;

    getmaxyx(stdscr, max_y, max_x);
    clear();

    // Display instructions
    attron(COLOR_PAIR(COLOR_HEADER));
    move(1, 2);
    printw("Type the following text:");
    attroff(COLOR_PAIR(COLOR_HEADER));

    // Display passage with word wrap
    move(3, 2);
    int curr_x = 2;
    int curr_y = 3;

    attron(COLOR_PAIR(COLOR_DEFAULT));
    for (size_t j = 0; j < strlen(passage); j++)
    {
        if (curr_x >= max_x - 2)
        {
            curr_y++;   // move to next row
            curr_x = 2; // get back to initial col
            move(curr_y, curr_x);
        }
        addch(passage[j]); // print the passage
        curr_x++;
    }
    attroff(COLOR_PAIR(COLOR_DEFAULT));

    // Set up input area it will wrap the passage for better feel
    int input_row = 3;
    move(input_row, 2);
    refresh();

    // Start timer
    start_time = time(NULL);

    // Main game loop
    while (1)
    {
        double elapsed = difftime(time(NULL), start_time);
        int avg_word_length = (game_mood == '1') ? 4 : (game_mood == '2' ? 6 : 8);
        show_progress(elapsed, cursor_pos / avg_word_length, correct, incorrect);

        // Check time limit
        if (elapsed >= 60.0)
        {
            break;
        }

        int ch = getch();
        if (ch == 27 || ch == '\n')
        { // ESC or Enter to stop typing and see results
            break;
        }

        // Handle backspace
        if ((ch == KEY_BACKSPACE || ch == 127 || ch == '\b') && cursor_pos > 0)
        {
            cursor_pos--;
            int curr_row = input_row + (cursor_pos / (max_x - 4)); // for getting actual row and col
            int curr_col = 2 + (cursor_pos % (max_x - 4));
            move(curr_row, curr_col);
            addch(passage[cursor_pos]); // add actual character to that position to help user;
            move(curr_row, curr_col);

            // Update statistics
            if (typed_text[cursor_pos] == passage[cursor_pos])
            {
                correct--;
            }
            else
            {
                incorrect--;
            }
            refresh();
            continue;
        }

        // Handle regular input
        if (ch >= 32 && ch <= 126 && cursor_pos < strlen(passage))
        {
            int curr_row = input_row + (cursor_pos / (max_x - 4));
            int curr_col = 2 + (cursor_pos % (max_x - 4));
            move(curr_row, curr_col);

            // Display character with appropriate color
            if (passage[cursor_pos] == ch)
            {
                attron(COLOR_PAIR(COLOR_SUCCESS));
                addch(ch);
                attroff(COLOR_PAIR(COLOR_SUCCESS));
                correct++;
            }
            else
            {
                attron(COLOR_PAIR(COLOR_ERROR));
                addch(ch);
                attroff(COLOR_PAIR(COLOR_ERROR));
                incorrect++;
            }

            typed_text[cursor_pos++] = ch;
            refresh();
        }
    }

    typed_text[cursor_pos] = '\0';
    show_results(strlen(passage), correct, incorrect);
}

int main(void)
{
    // Initialize random seed
    srand(time(NULL));

    // Set up ncurses
    initializeNcurses();

    // Load word dictionary
    load_words();
    freopen("/dev/tty", "r", stdin);

    // Display welcome screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    attron(COLOR_PAIR(COLOR_HEADER));
    move(max_y / 2 - 1, max_x / 2 - 15);
    printw("Welcome to Speed Typing Test!");
    move(max_y / 2 + 1, max_x / 2 - 15);
    printw("Press any key to start...");
    attroff(COLOR_PAIR(COLOR_HEADER));

    getch(); // Wait for user input
    clear();
    game_menu();
    clear();
    main_game();
    refresh();

    getch(); // Wait for final key before exiting
    cleanupNcurses();
    return 0;
}