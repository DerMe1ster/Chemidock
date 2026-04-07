#pragma once
#include <EncButton.h>

void display(int time, bool dots);

void disp_begin();

void disp_start_string(int time);

void disp_clear();

void reset();

void end();

bool preparing_to_escape(EncButton &encoder);

