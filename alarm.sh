#!/usr/bin/env bash
# alarm.sh - countdown timer / alarm clock that plays a video when it hits zero
#
# Usage:
#   ./alarm.sh ~/Videos/song.mp4 1h 25m 30s     countdown (h / m / s, any mix)
#   ./alarm.sh ~/Videos/song.mp4 90m
#   ./alarm.sh ~/Videos/song.mp4 1:00PM         alarm at a clock time
#   ./alarm.sh ~/Videos/song.mp4 "7:30 am"      (next 7:30 AM - tomorrow if already past)
#   ./alarm.sh ~/Videos/song.mp4 13:45          24h format works too
#   ./alarm.sh -v ~/Videos/song.mp4 45s         -v is optional if the file exists
#   ALARM_VIDEO=~/Videos/song.mp4 ./alarm.sh 10m

set -u

VIDEO="${ALARM_VIDEO:-}"
TIME_ARGS=()

while [ $# -gt 0 ]; do
    case "$1" in
        -v|--video) VIDEO="${2:-}"; shift 2 ;;
        -h|--help)  sed -n '2,12p' "$0"; exit 0 ;;
        *)
            if [ -f "$1" ]; then VIDEO="$1"      # an existing file = the video
            else TIME_ARGS+=("$1"); fi
            shift ;;
    esac
done

if [ ${#TIME_ARGS[@]} -eq 0 ]; then
    echo "Usage: $0 [-v video_file] <1h 25m 30s | 1:00PM | 13:45>" >&2
    exit 1
fi

input="${TIME_ARGS[*]}"
input="${input,,}"                                   # lowercase
input="${input#"${input%%[![:space:]]*}"}"           # trim leading space
input="${input%"${input##*[![:space:]]}"}"           # trim trailing space

total=0
target_label=""

# ---- clock time? (1:00pm, 1pm, 7:30 am, 13:45) ----
if [[ "$input" =~ ^[0-9]{1,2}(:[0-9]{2}){0,2}\ ?(am|pm)?$ ]] \
   && [[ "$input" == *:* || "$input" == *am || "$input" == *pm ]]; then
    now=$(date +%s)
    target=$(date -d "$input" +%s 2>/dev/null) || {
        echo "Invalid clock time: '${TIME_ARGS[*]}'" >&2; exit 1; }
    if [ "$target" -le "$now" ]; then                # already passed today -> tomorrow
        target=$(date -d "tomorrow $input" +%s)
    fi
    total=$(( target - now ))
    target_label=" (rings at $(date -d "@$target" '+%a %H:%M:%S'))"

# ---- duration (1h 25m 30s) ----
else
    cleaned="${input//[0-9hms ]/}"                   # anything left is invalid
    if [ -n "$cleaned" ] || ! grep -qE '[0-9]+[hms]' <<<"$input"; then
        echo "Invalid time: '${TIME_ARGS[*]}' (try: 1h 25m 30s  or  1:00PM)" >&2
        exit 1
    fi
    while read -r num unit; do
        case "$unit" in
            h) total=$(( total + num * 3600 )) ;;
            m) total=$(( total + num * 60 ))   ;;
            s) total=$(( total + num ))        ;;
        esac
    done < <(grep -oE '[0-9]+[hms]' <<<"$input" | sed -E 's/([0-9]+)([hms])/\1 \2/')
fi

if [ "$total" -le 0 ]; then
    echo "Time must be greater than zero." >&2
    exit 1
fi

# ---- clean exit on Ctrl+C ----
trap 'printf "\n\nAlarm cancelled.\n"; tput cnorm 2>/dev/null; exit 130' INT TERM
tput civis 2>/dev/null   # hide cursor

# ---- countdown (based on real clock so it doesn't drift) ----
end=$(( $(date +%s) + total ))
printf "Alarm set for %02d:%02d:%02d from now%s\n" \
    $((total/3600)) $((total%3600/60)) $((total%60)) "$target_label"

while :; do
    left=$(( end - $(date +%s) ))
    [ "$left" -le 0 ] && break
    printf "\r  ⏳ %02d:%02d:%02d remaining " $((left/3600)) $((left%3600/60)) $((left%60))
    sleep 0.2
done

tput cnorm 2>/dev/null
printf "\r  ⏰ 00:00:00 - time's up!          \n"

# ---- play video ----
if [ -z "$VIDEO" ]; then
    echo "No video given. Beeping instead."
    for _ in 1 2 3 4 5; do printf '\a'; sleep 0.5; done
    exit 0
fi

if [ ! -f "$VIDEO" ]; then
    echo "Video not found: $VIDEO" >&2
    exit 1
fi

if command -v vlc >/dev/null 2>&1; then
    vlc --fullscreen --play-and-exit "$VIDEO" >/dev/null 2>&1
elif command -v mpv >/dev/null 2>&1; then
    mpv --fs "$VIDEO"
elif command -v ffplay >/dev/null 2>&1; then
    ffplay -autoexit -fs "$VIDEO" >/dev/null 2>&1
elif command -v mplayer >/dev/null 2>&1; then
    mplayer -fs "$VIDEO"
else
    echo "No player found (install vlc, mpv, ffplay or mplayer)." >&2
    exit 1
fi
