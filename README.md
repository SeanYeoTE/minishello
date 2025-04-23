# 🐚 minishello

A minimalist Unix-like shell implemented in C, designed to deepen understanding of system-level programming. This project replicates core functionalities of traditional shells like `bash`, providing users with an interactive prompt to execute commands, handle redirection, manage pipes, and more.

## 🚀 Features

- **Interactive Prompt**: Displays a prompt to receive user input.
- **Command Execution**: Runs executables based on the `PATH` variable or specified absolute/relative paths.
- **Built-in Commands**:
  - `cd`: Change directories.
  - `echo`: Display a line of text.
  - `pwd`: Print the current working directory.
  - `export`: Set environment variables.
  - `unset`: Unset environment variables.
  - `env`: List environment variables.
  - `exit`: Exit the shell.
- **Redirections**:
  - `>`: Output redirection.
  - `<`: Input redirection.
  - `>>`: Append output redirection.
  - `<<`: Here-document redirection.
- **Pipes**: Connect commands using `|` to pipe outputs between commands.
- **Signal Handling**:
  - `Ctrl-C`: Interrupt and show a new prompt.
  - `Ctrl-D`: Exit the shell.
  - `Ctrl-\`: Ignore.

## 🛠️ Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/SeanYeoTE/minishello.git
   ```
2. **Navigate to the project directory**:
   ```bash
   cd minishello
   ```
3. **Compile the project**:
   ```bash
   make
   ```

## 🧪 Usage

After successful compilation, start the shell with:
```bash
./minishello
```
Now you can enter commands as you would in a normal shell.

Example:
```bash
$ echo "Hello, World!"
Hello, World!
```

## 📁 Project Structure

```
minishello/
├── src/
│   ├── builtins/        # Built-in command implementations
│   ├── parser/          # Input parsing logic
│   ├── executor/        # Command execution logic
│   └── utils/           # Utility functions
├── includes/            # Header files
├── Makefile             # Build script
└── README.md            # Project documentation
```

## 📚 Lessons Learned

Throughout the development of `minishello`, several key insights and skills were acquired:

- **System Programming**: Gained hands-on experience with system calls like `fork()`, `execve()`, `pipe()`, and `dup2()`, which are fundamental to Unix-like operating systems.
- **Process Management**: Learned how to manage multiple processes, handle process synchronization, and deal with zombie processes.
- **Signal Handling**: Understood how to handle Unix signals gracefully, ensuring the shell responds appropriately to user interrupts.
- **Memory Management**: Emphasized the importance of proper memory allocation and deallocation to prevent leaks and ensure stability.
- **Parsing Techniques**: Developed robust parsing logic to handle complex command inputs, including nested commands and various redirection scenarios.
- **Modular Design**: Structured the codebase into modular components, enhancing readability, maintainability, and scalability.
- **Debugging Skills**: Improved debugging techniques, especially in a low-level programming context, using tools like `gdb` and `valgrind`.
- **Project Planning**: Learned that proper project planning—such as breaking down tasks, setting priorities, and aligning with deadlines—is crucial in reducing development time and avoiding last-minute crunches.

## 🤝 Acknowledgments

Special thanks to the open-source community and various contributors whose resources and discussions provided invaluable guidance.
