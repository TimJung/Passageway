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

## Pidora Setup to connect to GVSU-Guest


Add the following lines to `/etc/network/interfaces`

_This section requires an update we have the new Wifi dongle_
```
auto wlan0
iface wlan0 inet static
address 148.xxx.xxx.xxx
netmask 255.255.255.0
gateway 148.xxx.xxx.xxx
```

## Login
_this section needs an update_

On peacanpi (running Pidora)
```
userid: passage
password: LetMeIntoP13
```

# Introduction to AllJoyn 

[AllJoyn Tutorials](https://www.youtube.com/playlist?list=PL4IDeLjCA5CP7whTaPrEKBdXzj9Fa1-Fm)

# Building AllJoyn on MASL

The following environment variables should be set (in your `.profile` or `.bash_profile`)

```
export TARGET_CPU=$(uname -m)
export JAVA_HOME=/usr/lib/jvm/default-jdk
export CLASSPATH=/usr/share/java/junit.jar
export ALLJOYN_TOPDIR=$HOME/AllJoyn
export LD_LIBRARY_PATH=$ALLJOYN_TOPDIR/core/alljoyn/build/linux/$TARGET_CPU/debug/dist/core/alljoyn/build/linux/x86_64/debug/cpp/lib:$LD_LIBRARY_PATH

```

`ALLJOYN_TOPDIR` should be set to the parent directory of `core`, `data`, `tools`, and `services`.


More detailed steps are described at [All Seen Alliance site](https://allseenalliance.org).

[This page](http://likall.com/blog/?p=24) has more detailed instructions compared to the "official" source.


1. Use `git` to download the [source files](https://wiki.allseenalliance.org/develop/downloading_the_source)
2. Use `scons` and follow the [Linux build instruction](https://allseenalliance.org/developers/develop/building/linux)

It seems like, the `ia32-libs` package mentioned in the instructions is not required.

```
cd /path/to/the/directory/where/AllJoyn/source/is/installed
# we will call this directory $ALLJOYN_TOPDIR through out this document
cd core/alljoyn
scons BINDINGS=cpp WS=off ICE=off SERVICES="about,notification,controlpanel,config,onboarding,sample_apps"
apt-get install default-jdk
```

## Building the service sample

```
cd $ALLJOYN_TOPDIR/services/base/sample_apps
scons BINDING=cpp WS=off ALL=1
```

## Building the documentations

The documentation files can be found under `$ALLJOYN_TOPDIR/core/alljoyn/alljoyn_core/docs`

_this section has not been tested yet_

```
sudo apt-get install texlive
cd $ALLJOTYN_TOPDIR/core/alljoyn
scons DOCS=pdf
```

