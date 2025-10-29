# Developer Notes for BootPicker

## Architecture Overview

### Component Structure

```
BootPicker Application
│
├── Main Application (bootpicker.c)
│   ├── App lifecycle management
│   ├── View dispatcher setup
│   ├── Scene manager initialization
│   └── Input event routing
│
├── HID Sender (hid_sender.c/h)
│   ├── USB HID keyboard wrapper
│   ├── Key press/release handling
│   ├── Profile execution engine
│   └── Timing management
│
├── Profile Manager (profiles.c/h)
│   ├── JSON file I/O
│   ├── Profile validation
│   ├── Config management
│   └── Data structures
│
├── Scenes (scenes/)
│   ├── Main Scene - F12 spam and profile selection
│   ├── Profile Editor - Create/edit/calibrate profiles
│   └── Settings - Global configuration
│
└── Views (views/)
    └── Main View - Custom rendering for main screen
```

## Key Design Decisions

### 1. FuriTimer for F12 Spam

**Decision**: Use `FuriTimer` with periodic mode for F12 spamming.

**Rationale**:
- Non-blocking: UI remains responsive
- Precise timing: Hardware timer ensures consistent intervals
- Easy to start/stop: Simple API for control

**Alternative Considered**: Separate FuriThread with delay loop
- More complex state management
- Higher resource usage
- No significant advantages

### 2. Scene-Based Navigation

**Decision**: Use Flipper's SceneManager pattern.

**Rationale**:
- Standard Flipper app pattern
- Clean separation of concerns
- Built-in navigation handling
- Easy to add new screens

### 3. JSON for Profile Storage

**Decision**: Custom JSON parser instead of FlipperFormat.

**Rationale**:
- More human-readable format
- Easier manual editing
- Standard format for config files
- FlipperFormat is key-value only (not ideal for arrays)

**Trade-off**: Custom parser is more code, but more flexible.

### 4. Atomic File Writes

**Decision**: Write to temp file, then rename.

**Rationale**:
- Prevents corruption on power loss
- Ensures data integrity
- Standard practice for critical files

## API Usage Details

### USB HID Implementation

The app uses `furi_hal_usb_hid.h` for keyboard emulation:

```c
// Set USB mode to HID
furi_hal_usb_set_config(&usb_hid, NULL);

// Press key
furi_hal_hid_kb_press(key_code);

// Release key  
furi_hal_hid_kb_release(key_code);
```

**Important Notes**:
- USB mode switch requires ~100ms for re-enumeration
- Key press needs 20ms hold time for reliable detection
- Release all keys between presses to avoid ghosting

### Storage API

Profile file operations:

```c
// Open storage
Storage* storage = furi_record_open(RECORD_STORAGE);

// Create directory
storage_common_mkdir(storage, PROFILES_PATH);

// File operations via Stream API
Stream* stream = file_stream_alloc(storage);
file_stream_open(stream, path, FSAM_READ, FSOM_OPEN_EXISTING);
```

**File Paths**:
- Use `APP_DATA_PATH("bootpicker")` macro
- Resolves to `/ext/apps_data/bootpicker/`
- Automatically created by Storage API

### Timer Management

F12 spam timer implementation:

```c
// Allocate timer
FuriTimer* timer = furi_timer_alloc(callback, FuriTimerTypePeriodic, context);

// Start timer (period in ticks)
furi_timer_start(timer, furi_ms_to_ticks(interval_ms));

// Stop timer
furi_timer_stop(timer);

// Free timer
furi_timer_free(timer);
```

**Important**: Always stop timer before freeing or exiting app.

## Profile Execution Flow

```
User presses OK on profile
        ↓
Stop F12 timer
        ↓
Wait post_spam_delay_ms
    (allows boot menu to stabilize)
        ↓
Send arrow key × arrow_count
    (with arrow_delay_ms between each)
        ↓
Send ENTER key
        ↓
Profile complete
        ↓
Resume F12 spam (if configured)
```

## Calibration Implementation

Calibration measures the time from power-on to boot menu appearance:

1. User initiates calibration from profile editor
2. App displays "Power on target" popup
3. Timer starts (`furi_get_tick()`)
4. User presses OK when boot menu appears
5. Elapsed time = boot menu appearance time
6. This becomes the `post_spam_delay_ms`

**Why This Works**:
- Accounts for BIOS POST time
- Includes USB enumeration delay
- System-specific measurement
- User validates actual boot menu appearance

## Input Handling

### Main Scene Inputs

| Input | Type | Action |
|-------|------|--------|
| Left | Short | Previous profile |
| Right | Short | Next profile |
| OK | Short | Execute profile |
| OK | Long | Edit profile |
| Back | Short | Exit app |
| Back | Long | Open settings |

**Implementation**: Custom input callback in main view intercepts all inputs before scene manager.

## Memory Management

### Allocation Strategy

- App context: Allocated in `bootpicker_app_alloc()`
- Views: Allocated and registered in app context
- Profiles: Stored in fixed-size array (MAX_PROFILES = 16)
- Editing profile: Temporary allocation during edit

### Cleanup

All resources freed in `bootpicker_app_free()`:
1. Stop and free timer
2. Free HID sender
3. Free profile list and config
4. Free all views
5. Free scene manager
6. Free view dispatcher
7. Close GUI and Storage records

**Important**: Follow this order to avoid use-after-free.

## Error Handling

### File I/O Errors

- Failed load: Create default profile
- Failed save: Log error, continue with in-memory state
- Corrupt JSON: Skip invalid profiles, load what's valid

### USB/HID Errors

- USB locked: Log warning, continue (user must fix)
- USB not connected: Continue anyway (may work on re-enum)
- HID send failure: No retry, log error

**Philosophy**: Graceful degradation - app remains functional even with errors.

## Performance Considerations

### Timer Precision

- F12 spam interval: 600ms default
- Minimum safe interval: 200ms (USB enumeration time)
- Maximum useful interval: 2000ms (slow but reliable)

### UI Responsiveness

- F12 spam runs in timer callback (non-blocking)
- Profile execution runs in scene event handler (blocks UI)
- Spinner animation updates via view model

**Trade-off**: Profile execution blocks UI for simplicity. Could use FuriThread if needed.

## Testing Checklist

### Basic Functionality
- [ ] App launches without errors
- [ ] F12 spam starts automatically
- [ ] Profile selection works (left/right)
- [ ] Profile execution stops spam and sends keys
- [ ] App exits cleanly

### Profile Management
- [ ] Create new profile
- [ ] Edit existing profile
- [ ] Delete profile
- [ ] Save persists across app restarts
- [ ] Invalid values are clamped

### Edge Cases
- [ ] No profiles file (creates default)
- [ ] Empty profiles list
- [ ] Corrupt JSON (graceful failure)
- [ ] USB not connected
- [ ] Very fast/slow spam intervals
- [ ] Maximum arrow count (50)

### Hardware Testing
- [ ] Cold boot (Flipper starts with target)
- [ ] Hot plug (Flipper connected while target on)
- [ ] Different BIOS types (UEFI, Legacy)
- [ ] Different boot menu keys (F12, F11, etc.)

## Known Limitations

1. **F12 Only**: Currently hardcoded to F12 key
   - Future: Make boot key configurable per profile

2. **No Modifier Keys**: Can't send Shift+F12, Ctrl+F12, etc.
   - Future: Add modifier key support

3. **Single Arrow Direction**: Each profile uses one arrow direction
   - Future: Support mixed sequences (e.g., DOWN, DOWN, UP, ENTER)

4. **No Retry UI**: Retry mechanism exists but no UI to trigger it
   - Future: Add retry button after profile execution

5. **Profile Limit**: Maximum 16 profiles
   - Limitation: UI scrolling and memory constraints
   - Could increase if needed

## Future Enhancements

### Planned
- [ ] Configurable boot key (F8, F11, F12, ESC, DEL)
- [ ] Custom key sequences (not just arrows)
- [ ] Profile import/export
- [ ] Success/failure detection
- [ ] Automatic retry on failure

### Possible
- [ ] Multi-boot profiles (chain sequences)
- [ ] Conditional sequences (if X then Y)
- [ ] Network boot support (PXE sequences)
- [ ] Bootloader-specific presets

### Nice-to-Have
- [ ] Profile sharing community
- [ ] BIOS fingerprinting
- [ ] Auto-calibration without user input
- [ ] Visual sequence builder

## Troubleshooting for Developers

### Build Errors

**Error**: `undefined reference to 'furi_hal_hid_kb_press'`
- **Cause**: USB HID API not available
- **Fix**: Ensure building against Unleashed firmware with HID support

**Error**: `application.fam: No such file or directory`
- **Cause**: App not in correct directory structure
- **Fix**: Place in `unleashed-firmware/applications_user/bootpicker/`

### Runtime Errors

**Error**: F12 keys not sending
- **Cause**: USB mode not set to HID
- **Fix**: Check `hid_sender_ensure_usb_mode()` is called

**Error**: Profiles not saving
- **Cause**: SD card not mounted or write-protected
- **Fix**: Check SD card status, ensure `/ext/apps_data/` exists

**Error**: App crashes on exit
- **Cause**: Resource not freed properly
- **Fix**: Check all allocations have matching frees

## Code Style Notes

### Flipper Conventions

- Use `furi_*` prefix for Furi API calls
- Use `FURI_LOG_*` for logging (not printf)
- Tag all logs with module name
- Use `UNUSED()` macro for unused parameters
- Follow Flipper's naming conventions (snake_case)

### This Project's Conventions

- Prefix all global functions with `bootpicker_`
- Use scene/view pattern for UI
- Keep HID operations in hid_sender module
- Keep file I/O in profiles module
- Comment non-obvious timing decisions

## References

- [Flipper Developer Docs](https://developer.flipper.net/)
- [Furi API Reference](https://developer.flipper.net/flipperzero/doxygen/)
- [Unleashed Firmware](https://github.com/DarkFlippers/unleashed-firmware)
- [USB HID Usage Tables](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)

## Contact

For questions or contributions, see the main README.md.

