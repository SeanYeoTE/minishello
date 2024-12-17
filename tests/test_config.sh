#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Program paths
MINISHELL_PATH="./minishell"
BASH_PATH="bash"

# Temp file prefixes (mktemp will append random strings)
TEMP_PREFIX="/tmp/minishell_test"

# Filters for output cleaning
PROMPT_FILTER='minishell> |sean :.*\$|sean :\/.*\$'
PATH_FILTER='sean@sean-VirtualBox:|sean :'

# Additional filter to remove empty lines after prompt removal
EMPTY_LINES_FILTER='/^$/d'
