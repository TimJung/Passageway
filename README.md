# Passageway

## SD Card Setup

1. Install NOOBS v 1.4 (or newer)
2. Connect the Pi to a **wired** connection. With a wired connection NOOBS will show more OS choices.
3. Select the OSes that you want to Install (Pidora seems to be the distro
   that works with the RealTek Wifi dongle)
4. On the next boot time, a boot menu will showup

## Troubleshooting

When connected to an older VGA display, the default HDMI mode may not be
supported by the display and the screen will just stay blank or "video out
of range" error may show up. To fix this issue, edit `config.txt` under the
`/boot` directory and uncomment the settings to force HMDI mode to fallback
to VGA mode.

```
hdmi_group=1
hdmi_mode=1
```

## Wifi Setup

USB Wifi dongle

* 802.11n WLAN adapter
* USB idVendor=0bda
* USB idProduct=8176
* driver: rtl8192cu
* RealTek SerialNumber 00e04c000001

## Pidora WPA2 Enterprise Setup to GVSU Wifi


Save the following file to `/etc/sysconfig/network-scripts/ifcfg-GVSU`

```
ESSID="GV-Student"
MODE=Managed
KEY_MGMT=WPA-EAP
TYPE=Wireless
IEEE_8021X_EAP_METHODS=PEAP
IEEE_8021X_IDENTITY=YourUserIdHere
IEEE_8021X_PASSWORD_FLAGS=ask
IEEE_8021X_ANON_IDENTITY=YourAnonymousNameHere
BOOTPROTO=dhcp
DEFROUTE=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
NAME=GVSU
UUID=cc671239-8c6e-49a8-81f2-530c868eaaa5
ONBOOT=yes
HWADDR=40:A5:EF:03:18:3C
IEEE_8021X_INNER_AUTH_METHODS=MSCHAPV2
PEERDNS=yes
PEERROUTES=yes
IPV6_PEERDNS=yes
IPV6_PEERROUTES=yes
```

## Login

On peacanpi (running Pidora)
userid: passage
password: LetMeIntoP13

