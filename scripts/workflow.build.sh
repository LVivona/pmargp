#!/bin/bash

set -e

# Function to build the project
build() {
    local standard=$1
    echo "Building the project with $standard standard..."
    make CFLAGS="-Wall -Wextra -std=$standard -fPIC"
}

# Function to run tests
test() {
    local standard=$1
    echo "Running tests with $standard standard..."
    make test CFLAGS="-Wall -Wextra -std=$standard -fPIC"
}

# Check if a standard is provided
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 {build|test} <c-standard>"
    exit 1
fi

action=$1
standard=$2

# Main execution
case "$action" in
    build)
        build "$standard"
        ;;
    test)
        test "$standard"
        ;;
    *)
        echo "Usage: $0 {build|test} <c-standard>"
        exit 1
        ;;
esac

exit 0