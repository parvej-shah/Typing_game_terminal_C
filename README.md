# Typing_game_terminal_C

Welcome to the **Typing Speed Test Game**, a terminal-based typing game built using **C** and **ncurses**. This game helps you test and improve your typing speed and accuracy through various difficulty levels. The game provides real-time feedback on your progress and shows statistics like Words Per Minute (WPM), Accuracy, and Characters Per Second (CPS) upon completion.

## Features

- **Difficulty Levels**: Choose between three difficulty levels: Easy, Medium, and Hard. Each level has a different set of words based on their length.
- **Real-Time Stats**: Track your performance with WPM, Accuracy, and CPS during the game.
- **Interactive UI**: The game provides color-coded feedback for correct typing, errors, and overall progress.
- **Progress Display**: View a live progress bar displaying your time, correct words, mistakes, WPM, and accuracy.
- **Results**: After completing the game, you will receive detailed results, including your accuracy, WPM, and mistakes.

## Requirements

- **C Compiler** (`GCC`)
- **ncurses library** for handling terminal input/output and colors.

To install **ncurses** on Ubuntu/Debian, run:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/parvej-shah/Typing_game_terminal_C.git
   cd Typing_game_terminal_C
   ```

2. Compile the game:
   ```bash
   gcc -o game game.c -lncurses -lm
   ```

3. Run the game:
   ```bash
   ./game <words.txt
   ```

## Gameplay

1. **Start Menu**: Upon starting the game, you will be presented with a menu to select the difficulty level (Easy, Medium, Hard).
2. **Typing**: After selecting a difficulty, a random passage of words will appear on the screen. Your goal is to type the passage correctly as fast as possible.
3. **Progress**: As you type, the game tracks your progress and provides real-time feedback such as words typed, mistakes, and WPM.
4. **Results**: Once the timer runs out or you stop typing, your results will be displayed, including your **WPM**, **Accuracy**, and **CPS**.

## Controls

- Use the keyboard to type the words in the passage.
- Press **Esc** or **Enter** to stop the game and see your results.
- Press **Backspace** to correct any typing mistakes.
- Press **'R'** to restart the game after completing the round.

## Game Configuration

- **Easy Mode**: 1-4 letter words
- **Medium Mode**: 5-7 letter words
- **Hard Mode**: 8+ letter words

## Colors and Feedback

- **Green**: Correct typing
- **Red**: Mistakes
- **Cyan**: Current typing focus
- **Yellow**: Warnings (e.g., time running out)
- **Blue**: Information (e.g., progress updates)
- **White**: Default text

## Developed By 
**` Parvej Shah, Institute of Information And Technology, Dhaka University `**
