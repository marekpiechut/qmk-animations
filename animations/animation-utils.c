/**
 * Utilities for QMK oled animations
 *
 * Copyright (c) Marek Piechut
 * MIT License
 */
#pragma once

//-------- CONFIGURATION START --------

#ifndef ANIM_RENDER_WPM
  #define ANIM_RENDER_WPM true
#endif
#ifndef FAST_TYPE_WPM
  #define FAST_TYPE_WPM 45 //Switch to fast animation when over words per minute
#endif
#ifndef ANIM_INVERT
  #define ANIM_INVERT false //Invert animation color and background
#endif
#ifndef ANIM_BG
  #define ANIM_BG 0x00
#endif
#ifndef ANIM_FRAME_TIME
  #define ANIM_FRAME_TIME 250
#endif
//-------- CONFIGURATION END--------

#define ANIM_WPM_WIDTH 22

static void oled_render_wpm(void) {
  static char wpm_str[4];

  sprintf(wpm_str, "%03d", get_current_wpm());
  oled_set_cursor(0, 1);
  oled_write_P(PSTR("WPM"), false);
  oled_set_cursor(0, 2);
  oled_write(wpm_str, false);
}

static void oled_render_anim_frame(const char **fast_frames, const char **slow_frames, uint8_t frames_len) {

  static uint32_t anim_timer = 0;
  static uint8_t current_frame = 0;
  static uint8_t frame_offset = ANIM_RENDER_WPM ? ANIM_WPM_WIDTH : 0;

  const uint8_t speed = get_current_wpm();


  if (timer_elapsed32(anim_timer) > ANIM_FRAME_TIME) {
    oled_set_cursor(0, 0);
    anim_timer = timer_read32();

    const bool is_fast = speed > FAST_TYPE_WPM;
    const char *frame = is_fast ? fast_frames[current_frame] : slow_frames[current_frame];
    const int8_t bg = ANIM_BG;

    uint8_t frame_start_offset = ANIM_RENDER_WPM ? ANIM_WPM_WIDTH : 0;
    current_frame = (current_frame + 1) % frames_len;

    for(int offset = 0; offset <= OLED_MATRIX_SIZE; offset++) {
      uint16_t col = offset % OLED_DISPLAY_WIDTH;

      if(col < frame_start_offset) {
        continue;
      } else if (col < frame_offset || col > frame_offset + ANIM_FRAME_WIDTH) {
        oled_write_raw_byte((ANIM_INVERT ? ~bg : bg), offset);
      } else if (col <= OLED_DISPLAY_WIDTH) {
        uint8_t row = offset / OLED_DISPLAY_WIDTH;
        uint8_t col_data = pgm_read_byte(frame + ((offset - frame_offset) % ANIM_FRAME_WIDTH) + (ANIM_FRAME_WIDTH * row));
        oled_write_raw_byte(ANIM_INVERT ? ~col_data : col_data, offset );
      }
    }

    #ifdef ANIM_SCROLL
    if(is_fast) {
      frame_offset =  frame_offset > OLED_DISPLAY_WIDTH - 8 ? frame_start_offset : (frame_offset + 8);
    } else if (frame_offset > (OLED_DISPLAY_WIDTH - ANIM_FRAME_WIDTH) || frame_offset < ANIM_FRAME_WIDTH) {
      frame_offset = frame_start_offset;
    }
    #endif

    if(ANIM_RENDER_WPM) {
      oled_render_wpm();
    }
  }
}
