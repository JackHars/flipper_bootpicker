# BootPicker Profiles Guide

## Overview

BootPicker now loads boot profiles from a JSON configuration file, making it easy to customize your boot menu options without recompiling the application.

## Profile Storage Locations

The application will search for profiles in the following order:

1. **User custom profiles**: `/ext/apps_data/bootpicker/profiles.json`
2. **Bundled profiles**: `/ext/apps/USB/bootpicker/profiles.json`
3. **Fallback**: If no file is found, default hardcoded profiles are used

## JSON Format

The `profiles.json` file uses a simple structure:

```json
{
  "profiles": [
    {
      "name": "Windows",
      "arrow_count": 0
    },
    {
      "name": "Ubuntu",
      "arrow_count": 1
    },
    {
      "name": "HIVE",
      "arrow_count": 8
    }
  ]
}
```

### Fields

- **name**: Display name for the boot option (max 31 characters)
- **arrow_count**: Number of DOWN arrow keypresses needed to reach this option in your boot menu (0-8)

## Profile Mapping to Buttons

The first 9 profiles are mapped to the Flipper Zero controls:

- **LEFT button** → Profile 0 (index 0)
- **UP button** → Profile 1 (index 1)
- **RIGHT button** → Profile 2 (index 2)
- **DOWN button** → Profile 8 (index 8)
- **OK button** → Cycles through all profiles (0-8 in sequence)

## Customizing Your Profiles

### Method 1: Edit on Computer

1. Connect your Flipper Zero via USB
2. Navigate to `/ext/apps_data/bootpicker/`
3. Create or edit `profiles.json` with your boot menu options
4. Restart the BootPicker app

### Method 2: Use the Example

1. Copy `example_profiles.json` to `/ext/apps_data/bootpicker/profiles.json`
2. Edit the copy to match your boot menu structure
3. Restart the BootPicker app

## Determining Arrow Counts

To find the correct `arrow_count` for each boot option:

1. Boot your computer and enter the boot menu (usually F12 or ESC)
2. Count how many times you need to press DOWN arrow to reach each option
3. The first/default option is `0`, the second is `1`, etc.

### Example Boot Menu

```
> Windows Boot Manager        ← arrow_count: 0
  Ubuntu                      ← arrow_count: 1
  USB Drive                   ← arrow_count: 2
  Network Boot                ← arrow_count: 3
```

## Limitations

- Maximum of 9 profiles (indices 0-8)
- Profile names limited to 31 characters
- Arrow count range: 0-8
- Only DOWN arrow navigation is supported (some BIOS menus use UP/DOWN)

## Troubleshooting

### App shows default profiles instead of my custom ones

- Check that your `profiles.json` file is in the correct location
- Verify the JSON syntax is correct (use a JSON validator)
- Check the Flipper logs for parsing errors

### Boot menu doesn't select the correct option

- Recount the arrow presses needed in your actual boot menu
- Some BIOS menus have hidden options or separators
- Try adjusting the `arrow_count` values up or down by 1

### App won't start

- Check for JSON syntax errors (missing commas, brackets, quotes)
- Ensure file is saved as UTF-8 text
- Try removing the custom `profiles.json` to use defaults

## Example Configurations

### Dual Boot (Windows + Linux)

```json
{
  "profiles": [
    {"name": "Windows 11", "arrow_count": 0},
    {"name": "Ubuntu 24.04", "arrow_count": 1},
    {"name": "Unused", "arrow_count": 2},
    {"name": "Unused", "arrow_count": 3},
    {"name": "Unused", "arrow_count": 4},
    {"name": "Unused", "arrow_count": 5},
    {"name": "Unused", "arrow_count": 6},
    {"name": "Unused", "arrow_count": 7},
    {"name": "BIOS Setup", "arrow_count": 8}
  ]
}
```

### Multi-OS Setup

```json
{
  "profiles": [
    {"name": "Windows", "arrow_count": 0},
    {"name": "Ubuntu", "arrow_count": 1},
    {"name": "Fedora", "arrow_count": 2},
    {"name": "Arch Linux", "arrow_count": 3},
    {"name": "FreeBSD", "arrow_count": 4},
    {"name": "Rescue USB", "arrow_count": 5},
    {"name": "MemTest86", "arrow_count": 6},
    {"name": "Network Boot", "arrow_count": 7},
    {"name": "HIVE OS", "arrow_count": 8}
  ]
}
```

### Mining Rig Configuration

```json
{
  "profiles": [
    {"name": "HIVE OS", "arrow_count": 0},
    {"name": "Windows Maint", "arrow_count": 1},
    {"name": "USB Recovery", "arrow_count": 2},
    {"name": "Profile 4", "arrow_count": 3},
    {"name": "Profile 5", "arrow_count": 4},
    {"name": "Profile 6", "arrow_count": 5},
    {"name": "Profile 7", "arrow_count": 6},
    {"name": "Profile 8", "arrow_count": 7},
    {"name": "BIOS", "arrow_count": 8}
  ]
}
```

