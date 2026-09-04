#!/bin/bash

set -e

if [ -z "$1" ]; then
    echo "Usage:"
    echo "  ./download_background.sh <video-url> [output-name]"
    exit 1
fi

URL="$1"

OUTPUT_DIR="assets/backgrounds"

mkdir -p "$OUTPUT_DIR"

# Optional custom filename
if [ -n "$2" ]; then
    OUTPUT_NAME="$2"
else
    OUTPUT_NAME="background_$(date +%Y%m%d_%H%M%S)"
fi

TEMP_FILE="/tmp/${OUTPUT_NAME}.webm"
OUTPUT_FILE="${OUTPUT_DIR}/${OUTPUT_NAME}.mp4"

echo "Downloading video..."

yt-dlp \
    -f "bestvideo+bestaudio/best" \
    --merge-output-format webm \
    -o "$TEMP_FILE" \
    "$URL"

echo "Converting to 640x360 H.264 @ 24 FPS..."

ffmpeg \
    -y \
    -ss 30 \
    -i "$TEMP_FILE" \
    -vf "scale=640:360" \
    -r 24 \
    -c:v libx264 \
    -preset medium \
    -crf 23 \
    -pix_fmt yuv420p \
    -an \
    "$OUTPUT_FILE"

echo "Cleaning temporary file..."

rm -f "$TEMP_FILE"

echo
echo "Done:"
echo "$OUTPUT_FILE"
