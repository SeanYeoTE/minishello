g#!/bin/bash

# Source configurations
source "$(dirname "$0")/test_config.sh"
source "$(dirname "$0")/test_cases.sh"

# Initialize counters
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run a single test
run_test() {
    local CMD="$1"
    ((TOTAL_TESTS++))

    # Create temporary files for outputs
    MINISHELL_OUT=$(mktemp ${TEMP_PREFIX}_out.XXXXXX)
    BASH_OUT=$(mktemp ${TEMP_PREFIX}_out.XXXXXX)
    MINISHELL_ERR=$(mktemp ${TEMP_PREFIX}_err.XXXXXX)
    BASH_ERR=$(mktemp ${TEMP_PREFIX}_err.XXXXXX)
    INPUT_PIPE=$(mktemp -u ${TEMP_PREFIX}_pipe.XXXXXX)

    # Run command in bash
    $BASH_PATH -c "$CMD" > "$BASH_OUT" 2> "$BASH_ERR"
    BASH_EXIT=$?

    # Create named pipe for input
    mkfifo "$INPUT_PIPE"

    # Run minishell in background with input from pipe
    $MINISHELL_PATH < "$INPUT_PIPE" > "$MINISHELL_OUT" 2> "$MINISHELL_ERR" &
    MINISHELL_PID=$!

    # Send command and exit to minishell
    {
        echo "$CMD"
        echo "exit"
    } > "$INPUT_PIPE"

    # Wait for minishell to finish
    wait $MINISHELL_PID
    MINISHELL_EXIT=$?

    # Remove the named pipe
    rm "$INPUT_PIPE"

    # Clean up minishell output:
    # 1. Replace prompts attached to output
    sed -i -E "s/sean :\/[^$]*\$//" "$MINISHELL_OUT"
    sed -i -E "s/sean :.*\$//" "$MINISHELL_OUT"
    # 2. Remove standalone prompts and paths
    sed -i -E "/$PROMPT_FILTER/d; /$PATH_FILTER/d" "$MINISHELL_OUT"
    # 3. Remove empty lines
    sed -i -E "$EMPTY_LINES_FILTER" "$MINISHELL_OUT"

    # Clean up bash output:
    # 1. Remove paths
    sed -i -E "/$PATH_FILTER/d" "$BASH_OUT"
    # 2. Remove empty lines
    sed -i -E "$EMPTY_LINES_FILTER" "$BASH_OUT"

    # Function to show outputs
    show_outputs() {
        local file1=$1
        local file2=$2
        local type=$3
        
        if ! diff "$file1" "$file2" >/dev/null; then
            printf "\n${YELLOW}=== %s outputs ===${NC}\n" "$type"
            printf "${BLUE}minishell output:${NC}\n"
            cat "$file1"
            printf "\n${BLUE}bash output:${NC}\n"
            cat "$file2"
        fi
    }

    # Compare outputs
    if diff "$MINISHELL_OUT" "$BASH_OUT" >/dev/null && diff "$MINISHELL_ERR" "$BASH_ERR" >/dev/null && [ $MINISHELL_EXIT -eq $BASH_EXIT ]; then
        printf "${GREEN}✓${NC} %s\n" "$CMD"
        ((PASSED_TESTS++))
    else
        printf "\n${RED}✗ Test failed${NC}\n"
        printf "${YELLOW}Command:${NC} %s\n" "$CMD"
        
        # Show outputs
        show_outputs "$MINISHELL_OUT" "$BASH_OUT" "stdout"
        show_outputs "$MINISHELL_ERR" "$BASH_ERR" "stderr"
        
        if [ $MINISHELL_EXIT -ne $BASH_EXIT ]; then
            printf "\n${YELLOW}=== Exit codes ===${NC}\n"
            printf "Bash: %d\n" $BASH_EXIT
            printf "Minishell: %d\n" $MINISHELL_EXIT
        fi
    fi

    # Cleanup
    rm "$MINISHELL_OUT" "$BASH_OUT" "$MINISHELL_ERR" "$BASH_ERR"
}

# Run all test cases from test_cases.sh
for cmd in "${test_cases[@]}"; do
    run_test "$cmd"
done

# Print final score
printf "\n${YELLOW}Test Summary${NC}\n"
printf "Passed: ${GREEN}%d${NC}/%d tests\n" $PASSED_TESTS $TOTAL_TESTS
if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    printf "${GREEN}All tests passed!${NC}\n"
else
    printf "${RED}Some tests failed.${NC}\n"
fi
