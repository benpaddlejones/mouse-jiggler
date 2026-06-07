#!/usr/bin/env bash
set -euo pipefail

sudo apt-get update
sudo apt-get install -y --no-install-recommends curl ca-certificates tar

ARCH="$(dpkg --print-architecture)"
case "$ARCH" in
  amd64) CLI_ARCH="64bit" ;;
  arm64) CLI_ARCH="ARM64" ;;
  *)
    echo "Unsupported architecture for Arduino CLI: $ARCH"
    exit 1
    ;;
esac

LATEST_TAG="$(curl -fsSL https://api.github.com/repos/arduino/arduino-cli/releases/latest | sed -n 's/.*\"tag_name\": \"v\([^\"]*\)\".*/\1/p' | head -n1)"
if [[ -z "$LATEST_TAG" ]]; then
  echo "Failed to resolve latest Arduino CLI version"
  exit 1
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

curl -fL -o "$TMP_DIR/arduino-cli.tar.gz" "https://downloads.arduino.cc/arduino-cli/arduino-cli_${LATEST_TAG}_Linux_${CLI_ARCH}.tar.gz"
tar -xzf "$TMP_DIR/arduino-cli.tar.gz" -C "$TMP_DIR"
sudo install -m 0755 "$TMP_DIR/arduino-cli" /usr/local/bin/arduino-cli

cd /workspaces/mouse-jiggler
arduino-cli config init --overwrite
arduino-cli core update-index
arduino-cli core install arduino:mbed_rp2040

echo "Arduino CLI + RP2040 core installed successfully."
