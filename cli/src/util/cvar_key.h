// Ŭnicode please 
#pragma once

// engine
static const char *CVAR_ENGINE_DISPLAY_FULLSCREEN  = "engine.display.fullscreen";
static const char *CVAR_ENGINE_DISPLAY_WIDTH = "engine.display.width";
static const char *CVAR_ENGINE_DISPLAY_HEIGHT = "engine.display.height";
static const char *CVAR_ENGINE_DISPLAY_SHOW_FPS = "engine.display.showFps";
static const char *CVAR_ENGINE_DISPLAY_HMD = "engine.display.hmd";

// hmd
static const char *CVAR_HMD_H_RESOLUTION = "hmd.hResolution";
static const char *CVAR_HMD_V_RESOLUTION = "hmd.vResolution";
static const char *CVAR_HMD_H_SCREEN_SIZE = "hmd.hScreenSize";
static const char *CVAR_HMD_V_SCREEN_SIZE = "hmd.vScreenSize";
static const char *CVAR_HMD_INTERPUPILLARY_DISTANCE = "hmd.interpupillaryDistance";
static const char *CVAR_HMD_LENS_SEPARATION_DISTANCE = "hmd.lensSeparationDistance";
static const char *CVAR_HMD_EYE_TO_SCREEN_DISTANCE = "hmd.eyeToScreenDistance";
static const char *CVAR_HMD_DISTORTION_K_1 = "hmd.distortionK_1";
static const char *CVAR_HMD_DISTORTION_K_2 = "hmd.distortionK_2";
static const char *CVAR_HMD_DISTORTION_K_3 = "hmd.distortionK_3";
static const char *CVAR_HMD_DISTORTION_K_4 = "hmd.distortionK_4";

// camera
static const char *CVAR_GAME_CAM_MOVE_SPEED = "game.cam.moveSpeed";
static const char *CVAR_GAME_CAM_ROTATE_SPEED = "game.cam.rotateSpeed";

static const char *CVAR_GAME_CHARACTER_POS_X = "game.character.posX";
static const char *CVAR_GAME_CHARACTER_POS_Y = "game.character.posY";
static const char *CVAR_GAME_CHARACTER_POS_Z = "game.character.posZ";

void startUpCVar();
void shutDownCVar();