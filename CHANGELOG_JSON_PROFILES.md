# Changelog: JSON Profile Support

## Summary

BootPicker now loads boot profiles from JSON configuration files instead of hardcoded values, making it easy for users to customize their boot menu options without recompiling.

## Changes Made

### 1. Modified Files

#### `profiles.h`
- Changed from hardcoded `const Profile PROFILES[]` to dynamic `Profile PROFILES[]`
- Added `profiles_load_from_file()` function to load profiles from JSON
- Added `profiles_get_count()` to return the number of loaded profiles
- Changed `PROFILE_COUNT` constant to `MAX_PROFILE_COUNT` (9)
- Updated `Profile` struct to use `char name[MAX_PROFILE_NAME_LEN]` instead of `const char*`

#### `profiles.c`
- Completely rewritten to load profiles from JSON files
- Implemented simple JSON parser for the specific profile format
- Added fallback to default profiles if JSON file not found
- Logs detailed information about profile loading for debugging
- Supports reading from Storage API

#### `bootpicker.c`
- Added profile loading during app initialization
- Searches multiple locations for profiles.json (user custom, then bundled)
- Updated OK button cycling to use `profiles_get_count()` instead of hardcoded `PROFILE_COUNT`
- Gracefully handles missing or invalid JSON files

#### `application.fam`
- Added `fap_files` section to bundle `profiles.json` with the app
- Profile file is now included in the compiled FAP

### 2. New Files

#### `profiles.json`
- Default profile configuration bundled with the app
- Contains 9 profiles matching the original hardcoded values
- Simple, easy-to-edit JSON format

#### `example_profiles.json`
- Updated to use the new simplified JSON structure
- Provides example configurations for common use cases
- Serves as a template for users

#### `PROFILES_GUIDE.md`
- Comprehensive guide for creating and customizing profiles
- Explains the JSON format and profile mapping
- Includes troubleshooting tips and example configurations
- Documents profile storage locations

#### `CHANGELOG_JSON_PROFILES.md`
- This file - documents all changes made

### 3. Updated Files

#### `README.md`
- Updated Configuration Files section to reflect new JSON format
- Added button mapping documentation
- References PROFILES_GUIDE.md for detailed instructions
- Simplified example from complex to simple format

## JSON Format

### Before (Hardcoded in C)
```c
const Profile PROFILES[PROFILE_COUNT] = {
    {"Windows", 0},
    {"Ubuntu", 1},
    {"HIVE", 8},
};
```

### After (JSON File)
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

## Profile Loading Priority

1. **User custom profiles**: `/ext/apps_data/bootpicker/profiles.json`
   - Allows users to create their own profiles without modifying the app
   
2. **Bundled profiles**: `/ext/apps/USB/bootpicker/profiles.json`
   - Default profiles included with the FAP file
   
3. **Hardcoded fallback**: In `profiles.c`
   - Used if no JSON files are found
   - Matches original hardcoded values

## Benefits

1. **User Customization**: Users can now customize profiles without recompiling
2. **Easy Updates**: Change boot menu options by editing a text file
3. **Portability**: Share profile configurations between devices
4. **Backward Compatible**: Falls back to defaults if JSON file missing
5. **Error Handling**: Gracefully handles invalid or missing files

## Button Mapping (Unchanged)

- **LEFT** → Profile 0 (Windows)
- **UP** → Profile 1 (Ubuntu)
- **RIGHT** → Profile 2 (Profile 3)
- **DOWN** → Profile 8 (HIVE)
- **OK** → Cycles through all 9 profiles

## Technical Details

### JSON Parser
- Custom lightweight parser (no external dependencies)
- Parses only the fields we need: `name` and `arrow_count`
- Handles whitespace and formatting variations
- Maximum file size: 4KB (safety limit)

### Memory Management
- Profiles stored in static array (no dynamic allocation for profiles)
- Profile names copied to fixed-size buffers
- Temporary buffer allocated only during parsing

### Error Handling
- Logs all errors for debugging
- Falls back to defaults on any error
- Validates profile count and array bounds

## Testing Recommendations

1. **Test with custom profiles.json**:
   - Create `/ext/apps_data/bootpicker/profiles.json`
   - Verify profiles load correctly
   - Check that button mappings work

2. **Test without custom file**:
   - Verify bundled profiles.json is used
   - Check that defaults load if both files missing

3. **Test with invalid JSON**:
   - Create malformed JSON file
   - Verify app falls back to defaults
   - Check error messages in logs

4. **Test profile cycling**:
   - Press OK button multiple times
   - Verify it cycles through all loaded profiles
   - Check it wraps back to profile 0

## Future Enhancements

Possible improvements for future versions:

1. Support for UP arrow navigation (some BIOS menus)
2. More than 9 profiles
3. Custom button mappings in JSON
4. Profile-specific timing delays
5. JSON validation and better error messages
6. GUI-based profile editor

## Migration Guide

### For Developers

If you've forked this project and made changes to the hardcoded profiles:

1. Keep your hardcoded values in `profiles.c` as the fallback
2. Create a `profiles.json` file with your custom profiles
3. Users can now override without modifying code

### For Users

1. Copy `example_profiles.json` to `/ext/apps_data/bootpicker/profiles.json`
2. Edit the file to match your boot menu
3. Count arrow presses needed for each option
4. Restart the app to load new profiles

## Breaking Changes

None. The app maintains full backward compatibility:
- Default profiles match original hardcoded values
- Button mappings unchanged
- No changes to external API or behavior

