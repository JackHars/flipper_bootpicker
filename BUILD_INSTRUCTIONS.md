# Build Instructions for BootPicker

## Prerequisites

1. **Unleashed Firmware Repository**
   - Clone the Unleashed firmware repository if you haven't already:
     ```bash
     git clone https://github.com/DarkFlippers/unleashed-firmware.git
     cd unleashed-firmware
     ```

2. **Flipper Build Tool (FBT)**
   - The Unleashed firmware includes FBT, which handles all build dependencies
   - No additional toolchain setup required if using FBT

## Building the FAP

### Method 1: Build as External App (Recommended)

1. Copy the entire BootPicker directory to `applications_user/bootpicker/` in your Unleashed firmware directory:
   ```bash
   cp -r /path/to/this/bootpicker /path/to/unleashed-firmware/applications_user/
   ```

2. Navigate to the Unleashed firmware directory:
   ```bash
   cd unleashed-firmware
   ```

3. Build the FAP:
   ```bash
   ./fbt fap_bootpicker
   ```

4. The compiled FAP will be located at:
   ```
   build/f7-firmware-D/.extapps/bootpicker.fap
   ```

5. Copy to your Flipper Zero's SD card:
   ```bash
   # If Flipper is connected via USB storage mode
   cp build/f7-firmware-D/.extapps/bootpicker.fap /path/to/flipper/sdcard/apps/USB/
   
   # Or use qFlipper to transfer the file
   ```

### Method 2: Build All FAPs

To build all external apps including BootPicker:

```bash
cd unleashed-firmware
./fbt faps
```

This will build all FAPs in the `applications_user/` directory.

## Icon Requirement

Before building, ensure you have created the icon file:
```
icons/bootpicker_10px.png
```

See `icons/ICON_README.txt` for details on creating the icon.

If you don't have an icon ready, you can create a temporary blank one:
```bash
cd bootpicker/icons
convert -size 10x10 xc:white bootpicker_10px.png
```

Or comment out the `fap_icon` line in `application.fam` temporarily.

## Testing

### On Hardware

1. Copy the `.fap` file to your Flipper Zero's SD card at:
   ```
   /ext/apps/USB/bootpicker.fap
   ```

2. On your Flipper Zero:
   - Navigate to Applications
   - Go to USB category
   - Launch BootPicker

### Logs and Debugging

To view logs while the app is running:

1. Connect Flipper via USB
2. Use the Flipper CLI:
   ```bash
   ./fbt cli
   ```
3. Launch the app from the Flipper interface
4. Watch logs in the CLI session

Logs are tagged with:
- `BootPicker` - Main app
- `BootPicker:HID` - HID operations
- `BootPicker:Profiles` - Profile management
- `BootPicker:SceneMain` - Main scene
- `BootPicker:SceneEditor` - Profile editor
- `BootPicker:SceneSettings` - Settings

## Troubleshooting Build Issues

### "File not found" errors

Ensure all files are in the correct structure:
```
bootpicker/
├── application.fam
├── bootpicker.c
├── bootpicker_i.h
├── profiles.c
├── profiles.h
├── hid_sender.c
├── hid_sender.h
├── icons/
│   └── bootpicker_10px.png
├── scenes/
│   ├── bootpicker_scene.c
│   ├── bootpicker_scene.h
│   ├── bootpicker_scene_config.h
│   ├── bootpicker_scene_main.c
│   ├── bootpicker_scene_profile_editor.c
│   └── bootpicker_scene_settings.c
└── views/
    ├── bootpicker_main_view.c
    └── bootpicker_main_view.h
```

### API compatibility issues

This app is designed for Unleashed firmware. If you encounter API incompatibilities:

1. Check the Unleashed firmware version
2. Update to the latest Unleashed firmware
3. Check for API changes in the Unleashed documentation

### Missing dependencies

The `application.fam` file specifies required APIs. If build fails with missing symbols:

1. Ensure you're building against Unleashed firmware (not official firmware)
2. Check that the firmware includes USB HID support
3. Verify the firmware version is recent (2024+)

## Clean Build

If you encounter issues, clean the build directory:

```bash
./fbt clean
./fbt fap_bootpicker
```

## Firmware Flashing (Optional)

If you want to flash the entire firmware with BootPicker included:

```bash
./fbt flash_usb_full
```

This will flash the complete firmware including all FAPs.

## Development Tips

### Rapid Development Cycle

For faster iteration during development:

1. Use `./fbt fap_bootpicker` to build only your app
2. Copy only the `.fap` file to the SD card
3. Reload the app on Flipper (no reboot required)

### Code Formatting

Format your code according to Flipper standards:

```bash
./fbt format_sources
```

### Static Analysis

Run static analysis before committing:

```bash
./fbt lint
```

## Additional Resources

- [Flipper Zero Developer Docs](https://developer.flipper.net/)
- [Unleashed Firmware GitHub](https://github.com/DarkFlippers/unleashed-firmware)
- [Flipper Zero Discord](https://flipperzero.one/discord)

