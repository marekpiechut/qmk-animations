# Animations for QMK OLED displays

This repository contains some animations for [QMK](https://github.com/qmk/qmk_firmware)
keyboard firmware.

They are tested on my [Corne Keyboard](https://github.com/foostan/crkbd) build,
but should work just fine on any other with 128x32 OLED display (Lily58, Sofle, etc.).

You can preview all animations on [Demo page](demo.md)

# How to use

To use any of these you need to be building your firmware locally.
Follow steps from here if you don't do it already:
[Setting Up Your QMK Environment](https://beta.docs.qmk.fm/tutorial/newbs_getting_started)

## Enable required functions

You need to have OLED driver enabled in your firmware, usually you also need to enable
WPM counter in QMK so animation knows how fast you are typing. Just put these in your `rules.mk`
file:

```
WPM_ENABLE = yes					# Enable word per minute counter
LTO_ENABLE = yes					# Makes hex file smaller
OLED_DRIVER_ENABLE = yes	# Enable OLEDs
```

`LTO_ENABLE` is not really required, but animations take a lot of space and it makes
firmware smaller. Otherwise you might not be able to flash your controller. Usually you
will need this if you're on ATMega/Pro Micro controllers. On STM chips theres plenty of
free flash memory, so you might just skip it.

## Setup OLED rotation

All animations assume you're using vertically mounted OLED (like in Corne). In the header of the animation
file there will be a suggested rotation option to set in your `keymap.c file`. It should be enough to just
put something like this at the beggining:

```c
#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_master) {
    return OLED_ROTATION_180;
  }
  return rotation;
}
#endif
```

This will play animation only on secondary half of the keyboard. Adjust to your needs.

## Enable animation

Just copy all animation `.c` files to your keymap folder, include selected one at the top and use inside `keymap.c`:

```c
/**
 * Put this somewhere at the beginning of the file --
 * Make sure you import only one of animations at a time
 * They all have same function exported, so it won't compile if you
 * include more than one at a time. You can also configure some options
 * before including the animation. Not all animations support them, but some do :P.
 */
#define ANIM_INVERT false
#define ANIM_RENDER_WPM true
#define FAST_TYPE_WPM 45 //Switch to fast animation when over words per minute

#ifdef OLED_DRIVER_ENABLE
    #include "demon.c"
#endif

// -- Probably some other stuff and then --

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    if (!is_keyboard_master()) {
        oled_render_anim();
    }
}
#endif
```

Again, this assumes you have a split keyboard and want to display animation
on the secondary half. Adjust to your keyboard.

## Build and flash your firmware

For example:

```bash
make crkbd:marek:flash
```

# Troubleshooting

## My firmware is too big and won't flash

Animations take a lot of memory on the microcontroller. What you can do to still
make it work, is to disable backlight LED animations.

If you don't need them at all, just disable everything in `rules.mk`

```
RGBLIGHT_ENABLE = no
```

If you want to keep some simple LED backlight you can also try to disable most of them.
Here's what worked for me:

**Open `config.h` file in your keymap and:**

- Disable LED animations by commenting out `#define RGBLIGHT_ANIMATIONS`
- Remove all animation data from firmware by adding these:

```c
#    define DISABLE_RGB_MATRIX_ALPHAS_MODS
#    define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#    define DISABLE_RGB_MATRIX_BREATHING
#    define DISABLE_RGB_MATRIX_BAND_SAT
#    define DISABLE_RGB_MATRIX_BAND_VAL
#    define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#    define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#    define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#    define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#    define DISABLE_RGB_MATRIX_CYCLE_ALL
#    define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN
#    define DISABLE_RGB_MATRIX_CYCLE_OUT_IN
#    define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#    define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#    define DISABLE_RGB_MATRIX_DUAL_BEACON
#    define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
#    define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
#    define DISABLE_RGB_MATRIX_RAINBOW_BEACON
#    define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#    define DISABLE_RGB_MATRIX_RAINDROPS
#    define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#    define DISABLE_RGB_MATRIX_TYPING_HEATMAP
#    define DISABLE_RGB_MATRIX_DIGITAL_RAIN
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#    define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#    define DISABLE_RGB_MATRIX_SPLASH
#    define DISABLE_RGB_MATRIX_MULTISPLASH
#    define DISABLE_RGB_MATRIX_SOLID_SPLASH
#    define DISABLE_RGB_MATRIX_SOLID_MULTISPLASH
```

This will still leave you few simple LED effects and free quite a lot of memory.

# Support

If you like this, maybe you'll also like some other stuff I'm working on:

- Ergonomic mechanical keyboards at [D1Keyboards](https://d1keyboards.com)
- JS Podcast (polish) ["Po Prostu JS"](https://poprostujs.pl)
- Or just drop me a line or check what's new at [@marekpiechut](https://twitter.com/marekpiechut)
