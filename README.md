# Flipper BootPicker

A Flipper Zero application for automated boot menu selection with F12 spam and HID profile execution.

## Features

- **Automated F12 Spam**: Continuously sends F12 (or ESC) keys to trigger boot menus
- **Profile-Based Selection**: Execute predefined profiles with different arrow key sequences
- **HID Profile Execution**: Send DOWN arrows + ENTER based on profile configuration
- **Second Enter Option**: Optional second ENTER key 1 second after the first
- **Settings Menu**: Configure spam interval, boot key, auto-start, and second enter behavior

## Profiles

The app supports up to 9 profiles (0-8) with configurable arrow counts:

- **Profile 0 (Windows)**: 0 arrows + ENTER
- **Profile 1 (Ubuntu)**: 1 DOWN + ENTER  
- **Profile 2 (Profile 3)**: 2 DOWN + ENTER
- **Profile 3 (Profile 4)**: 3 DOWN + ENTER
- **Profile 4 (Profile 5)**: 4 DOWN + ENTER
- **Profile 5 (Profile 6)**: 5 DOWN + ENTER
- **Profile 6 (Profile 7)**: 6 DOWN + ENTER
- **Profile 7 (Profile 8)**: 7 DOWN + ENTER
- **Profile 8 (P3)**: 8 DOWN + ENTER

## Controls

- **LEFT**: Execute Profile 0 (Windows)
- **UP**: Execute Profile 1 (Ubuntu)
- **RIGHT**: Execute Profile 2 (Profile 3)
- **DOWN**: Execute Profile 8 (P3)
- **OK**: Execute currently selected profile (cycles through all)
- **Long OK**: Open settings menu

## Settings

- **Boot Key**: Choose between F12 or ESC
- **Spam Interval**: Set delay between key presses (200ms - 2000ms)
- **Auto-Start Spam**: Enable/disable automatic F12 spam on startup
- **Second Enter**: Send second ENTER 1 second after profile execution

## Installation

1. Compile the FAP using the Unleashed firmware build system:
   ```bash
   ./fbt fap_bootpicker
   ```

2. Copy the generated `.fap` file to your Flipper Zero's SD card in `/ext/apps/`

3. Or install directly via USB:
   ```bash
   ./fbt launch_app APPSRC=bootpicker
   ```

## Configuration

Profiles are defined in `profiles.json`. You can customize:
- Profile names
- Arrow counts (0-8)
- Execution behavior

## Requirements

- Flipper Zero with Unleashed firmware
- USB HID mode enabled
- SD card for profile storage

## License

This project is part of the Flipper Zero Unleashed firmware ecosystem.