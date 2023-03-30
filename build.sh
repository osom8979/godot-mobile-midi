#!/usr/bin/env bash

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" || exit; pwd)

USAGE="
Usage: ${BASH_SOURCE[0]} [options] [com1] [com2] ... [comN]

Available options are:
  -h, --help     Print this message
  --             Skip handling options
"

ANDROID_SDK_URL="https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip"
ANDROID_SDK_CHECKSUM="$ROOT_DIR/android.checksum"
ANDROID_SDK_DIST="$ROOT_DIR/commandlinetools-linux-9477386_latest.zip"
ANDROID_SDK_ROOT="$ROOT_DIR/android_sdk"
ANDROID_SDK_CMDLINE_TOOLS_DIR="$ANDROID_SDK_ROOT/cmdline-tools"
ANDROID_SDK_CMDLINE_TOOLS_LATEST_DIR="$ANDROID_SDK_ROOT/cmdline-tools/latest"
ANDROID_SDK_LICENSE="$ROOT_DIR/android.license"

GODOT_URL="https://downloads.tuxfamily.org/godotengine/3.2.4/rc5/Godot_v3.2.4-rc5_x11.64.zip"

SDKMANAGER_CMD="$ANDROID_SDK_CMDLINE_TOOLS_LATEST_DIR/bin/sdkmanager"
GODOT_CMD="$ROOT_DIR/godot"

function print_error
{
    # shellcheck disable=SC2145
    echo -e "\033[31m$@\033[0m" 1>&2
}

function print_message
{
    # shellcheck disable=SC2145
    echo -e "\033[32m$@\033[0m"
}

trap 'cancel_black' INT

function cancel_black
{
    print_error "An interrupt signal was detected."
    exit 1
}

function print_usage
{
    echo "$USAGE"
}

function download_android_sdk
{
    if [[ ! -f "$ANDROID_SDK_DIST" ]]; then
        curl -o "$ANDROID_SDK_DIST" "$ANDROID_SDK_URL"
    fi

    if ! sha256sum -c "$ANDROID_SDK_CHECKSUM" &> /dev/null; then
        print_error "Checksum error detected"
        exit 1
    fi

    if [[ ! -d "$ANDROID_SDK_CMDLINE_TOOLS_DIR" ]]; then
        mkdir -vp "$ANDROID_SDK_CMDLINE_TOOLS_DIR"
    fi

    if [[ ! -d "$ANDROID_SDK_CMDLINE_TOOLS_LATEST_DIR" ]]; then
        echo "Unzip '$ANDROID_SDK_DIST' ..."
        unzip -q "$ANDROID_SDK_DIST" -d "$ANDROID_SDK_CMDLINE_TOOLS_DIR"
        mv -v "$ANDROID_SDK_CMDLINE_TOOLS_DIR/cmdline-tools" "$ANDROID_SDK_CMDLINE_TOOLS_LATEST_DIR"
    fi

    if [[ ! -x "$SDKMANAGER_CMD" ]]; then
        print_error "Not found sdkmanager executable"
        exit 1
    fi

    yes | "$SDKMANAGER_CMD" --licenses > "$ANDROID_SDK_LICENSE"
}

function download_godot
{
    curl -O "$GODOT_URL"
    if ! sha512sum -c godot.checksum &> /dev/null; then
        print_error "Checksum error detected"
        exit 1
    fi

    (
        cd "$ROOT_DIR" && unzip Godot_v3.2.4-rc5_x11.64.zip
    )

    mv "$ROOT_DIR/Godot_v3.2.4-rc5_x11.64" "$GODOT_CMD"

    if [[ ! -x "$GODOT_CMD" ]]; then
        print_error "Not found godot executable"
        exit 1
    fi
}

function build_main
{
    while [[ -n $1 ]]; do
        case $1 in
        -h|--help)
            print_usage
            exit 0
            ;;
        --)
            shift
            break
            ;;
        -*)
            print_error "Unknown option: $1"
            exit 1
            ;;
        *)
            print_error "Unknown command: $1"
            exit 1
            ;;
        esac
    done

    if [[ ! -d "$ANDROID_SDK_ROOT" ]]; then
        print_message "Download Android SDK ..."
        download_android_sdk
    fi

    if [[ ! -x "$GODOT_CMD" ]]; then
        print_message "Download Godot ..."
        download_godot
    fi
}

#build_main "$@"
