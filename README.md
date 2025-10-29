# BootPicker - Flipper Zero Boot Menu Selector

BootPicker is a Flipper Zero application that automates boot menu selection by spamming F12 (or other configurable boot keys) and then executing a pre-configured HID keyboard sequence to select a specific boot device.

## Features

- **Automated F12 Spamming**: Continuously sends F12 keypresses until you select a boot profile
- **Profile Management**: Create and manage multiple boot profiles with custom arrow sequences
- **Configurable Delays**: Fine-tune timing for different BIOS/UEFI implementations
- **Calibration Mode**: Built-in helper to measure optimal boot menu timing
- **Simple UI**: Easy profile selection and editing interface

## Installation

### Option 1: Pre-built FAP (when available)

1. Download `bootpicker.fap` from releases
2. Copy to your Flipper Zero SD card: `/ext/apps/USB/bootpicker.fap`
3. Launch from Applications → USB → BootPicker

### Option 2: Build from Source

1. Clone the Unleashed firmware repository:
   ```bash
   git clone https://github.com/DarkFlippers/unleashed-firmware.git
   cd unleashed-firmware
   ```

2. Copy this BootPicker directory to `applications_user/`:
   ```bash
   cp -r /path/to/bootpicker applications_user/
   ```

3. Build the FAP:
   ```bash
   ./fbt fap_bootpicker
   ```

4. The compiled FAP will be at:
   ```
   build/f7-firmware-D/.extapps/bootpicker.fap
   ```

5. Copy to your Flipper Zero SD card

## Usage

### Quick Start

1. **Connect Flipper Zero** to your target computer's USB port before powering on the computer
2. **Launch BootPicker** from Applications → USB → BootPicker
3. The app will immediately start spamming F12 keypresses
4. **Select a profile** using Left/Right arrow buttons
5. **Press OK** when ready to execute the selected profile
6. The Flipper will stop spamming F12 and execute the arrow sequence + ENTER

### Creating a Profile

1. From the main screen, **long-press OK** to open the profile editor
2. Adjust settings:
   - **Direction**: UP or DOWN arrow key
   - **Arrow Count**: Number of times to press the arrow key (0-50)
   - **Post Spam Delay**: Wait time after stopping F12 spam (100-2000ms)
   - **Arrow Delay**: Delay between arrow keypresses (30-500ms)
   - **Retry Attempts**: Number of retry cycles if boot fails
3. **Press Save** to store the profile

### Calibration

Calibration helps determine the optimal `Post Spam Delay` for your system:

1. Enter the profile editor (**long-press OK** on main screen)
2. Select **Calibrate**
3. Follow on-screen instructions:
   - Power on your target computer
   - Press OK on Flipper when the boot menu appears
4. The measured time will be saved as the post-spam delay

### Settings

**Long-press Back** from the main screen to access global settings:

- **Spam Interval**: Frequency of F12 keypresses (200-2000ms)
- **Resume After Run**: Whether to resume F12 spam after executing a profile
- **Spam On Start**: Whether to start spamming F12 when the app launches

## Configuration Files

Profiles are now loaded from JSON files. See [PROFILES_GUIDE.md](PROFILES_GUIDE.md) for detailed documentation.

### Profile Locations

The app searches for profiles in this order:
1. User custom: `/ext/apps_data/bootpicker/profiles.json`
2. Bundled default: `/ext/apps/USB/bootpicker/profiles.json`
3. Hardcoded fallback (if no files found)

### Example profiles.json

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

### Button Mapping

- **LEFT** → Profile 0 (index 0)
- **UP** → Profile 1 (index 1)
- **RIGHT** → Profile 2 (index 2)
- **DOWN** → Profile 8 (index 8)
- **OK** → Cycles through all profiles

See [PROFILES_GUIDE.md](PROFILES_GUIDE.md) for complete customization instructions.

## Troubleshooting

### Flipper doesn't enumerate as USB keyboard

**Problem**: Target computer doesn't recognize Flipper as a keyboard.

**Solutions**:
- Increase `spam_interval_ms` to 800-1000ms to give USB more time to enumerate
- Check USB cable quality (data-capable cable required)
- Ensure Flipper is in USB HID mode (app handles this automatically)

### Boot menu doesn't appear

**Problem**: F12 spam isn't triggering the boot menu.

**Solutions**:
- Some systems use different keys (F11, F8, ESC, DEL). Note: BootPicker currently only supports F12
- BIOS may have "Fast Boot" enabled. Disable it in BIOS settings
- Increase `spam_interval_ms` if USB is enumerating too slowly
- Enable "Legacy USB Support" in BIOS if available

### Wrong boot device selected

**Problem**: Arrow sequence selects the wrong device.

**Solutions**:
- Use calibration mode to fine-tune `post_spam_delay_ms`
- Increase `arrow_delay_ms` to give BIOS more time to register keypresses
- Adjust `arrow_count` - boot menu order may have changed
- Some BIOS implementations require longer delays (200-300ms between arrows)

### Sequence executes too fast

**Problem**: Keys are sent before boot menu is ready.

**Solutions**:
- Increase `post_spam_delay_ms` (the delay after F12 spam stops)
- Use calibration mode to measure actual boot menu appearance time
- Start with conservative values (400-500ms) and decrease if needed

## Technical Details

### APIs Used

- **FuriTimer**: Periodic F12 spam loop
- **FuriHalUsbHid**: USB HID keyboard emulation
- **Storage API**: Profile JSON file management
- **GUI/SceneManager**: User interface framework
- **ViewDispatcher**: View management and navigation

### HID Key Codes

- F12: `0x45`
- Down Arrow: `0x51`
- Up Arrow: `0x52`
- Enter: `0x28`

### Profile Execution Sequence

1. Stop F12 spam timer
2. Wait `post_spam_delay_ms` for boot menu to stabilize
3. Send arrow key `arrow_count` times with `arrow_delay_ms` between each
4. Send ENTER key
5. Optionally resume F12 spam if `resume_after_run` is enabled

### Validation

- `arrow_count` is limited to 50 (safety limit)
- `arrow_delay_ms` minimum is 30ms (USB/BIOS compatibility)
- All timing values are in milliseconds

## Safety and Ethics

**⚠️ Important Notice**

This tool automates keyboard input at the pre-boot level. It is designed for **authorized use only** on systems you own or administer.

**Acceptable uses**:
- Automating boot device selection on your own computers
- System administration and deployment automation
- Testing and development environments

**Unacceptable uses**:
- Accessing systems without explicit permission
- Bypassing security controls on systems you don't own
- Any unauthorized access or malicious activity

The developers of BootPicker are not responsible for misuse of this tool. Use responsibly and ethically.

## License

[Specify your license here]

## Contributing

Contributions are welcome! Please submit pull requests or open issues on the project repository.

## Credits

Created for Flipper Zero running Unleashed firmware.

## Version History

- **v1.0** - Initial release
  - F12 spam with configurable intervals
  - Profile management (create/edit/delete)
  - Arrow sequence execution
  - Calibration mode
  - Settings configuration

