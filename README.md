# Passageway

## SD Card Setup

1. Install NOOBS v 1.4 (or newer)
2. Connect the Pi to a **wired** connection. With a wired connection NOOBS will show more OS choices.
3. Select the OSes that you want to Install
4. On the next boot time, a boot menu will showup

## Troubleshooting

When connected to an older VGA display, the default HDMI mode may not be supported by the display and the screen
will just stay blank or "video out of range" error may show up. To fix this issue, edit `config.txt` under the `/boot` directory and uncomment the settings that will
force HMDI mode to fallback to VGA mode.
