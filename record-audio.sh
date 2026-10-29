#!/usr/bin/env bash
# record-audio.sh — Webcam mic audio-only recorder
# Usage: ./record-audio.sh [output_file]

OUTPUT="${1:-audio_$(date +%Y-%m-%d_%H-%M-%S).flac}"

MIC_CARD="plughw:1,0"   # card 1 = USB Camera mic (plughw lets ALSA negotiate format)

echo "Recording audio to: $OUTPUT"
echo "Press Ctrl+C to stop."
echo ""

ffmpeg -f alsa -i "$MIC_CARD" -c:a flac "$OUTPUT"

echo ""
echo "Saved to: $OUTPUT"
