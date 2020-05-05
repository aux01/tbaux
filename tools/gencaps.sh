#!/bin/sh
#/ Usage: gencaps.sh [<url>]
#/ Generates C preprocessor defines for all terminfo capabilities using the
#/ latest ncurses Caps database published to the ThomasDickey/ncurses-snapshots
#/ repository, or a custom URL if provided.
set -eu

# Show usage
if [ $# -gt 0 ] && [ "$1" = "--help" ]; then
    grep <"$0" '^#/' | cut -c4-
    exit
fi

# URL of raw ncurses Caps file
url="${1:-https://raw.githubusercontent.com/ThomasDickey/ncurses-snapshots/master/include/Caps}"

# Fetch file and do some light preformatting
db=$(
    curl -sS "$url" |
    grep -v -e "^#" |
    awk '{o=$3" "$2" "$1; for(i=8;i<=NF;i++){o=o" "$i}; print o}')


# Reformat lines into CPP defines
def='{o="#define tb_"$3"\t"$1"\t//";for(i=5;i<=NF;i++){o=o" "$i};print o}'
format_defines() {
    nl -s ' ' -w 1 -v 0 |
        awk "$def" |
        sed 's/tb_clear/tb_clear_seq/' | # func name collision
        column -s'	' -t
}

# Write out the bools, nums, and strings sections
echo "/* Capabilities generated by $0 at $(date) */"
echo
echo "// Boolean Capabilities"
echo "$db" | grep '^bool ' | format_defines

echo
echo "// Numeric Capabilities"
echo "$db" | grep '^num ' | format_defines

echo
echo "// String Capabilities"
echo "$db" | grep '^str ' | format_defines
