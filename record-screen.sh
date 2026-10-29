#!/usr/bin/env bash
# record-screen.sh — Screen + webcam mic recorder
# Usage: ./record-screen.sh [output_file]

OUTPUT="${1:-recording_$(date +%Y-%m-%d_%H-%M-%S).mkv}"

MIC_CARD="plughw:1,0"   # card 1 = USB Camera mic (plughw lets ALSA negotiate format)
DISPLAY_VAR=":0.0"
RES="1280x800"
FPS=30
CRF=28                  # quality: lower = better, higher = smaller file
PRESET="ultrafast"      # safest for T4200; try "veryfast" if CPU allows

echo "Recording screen + mic to: $OUTPUT"
echo "Press Ctrl+C to stop."
echo ""

ffmpeg -f x11grab -r "$FPS" -s "$RES" -i "$DISPLAY_VAR" -f alsa -i "$MIC_CARD" -c:v libx264 -preset "$PRESET" -crf "$CRF" -c:a aac -b:a 128k "$OUTPUT"

echo ""
echo "Saved to: $OUTPUT"
