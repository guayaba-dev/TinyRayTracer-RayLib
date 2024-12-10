/*******************************************************************************************
 * This game has been created using raylib (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h
 *for details)
 *
 *   Copyright (c) 2021 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include "screens.h"  // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------

GameScreen currentScreen = LOGO;

Font font = {0};

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

static const int screenWidth = 800;
static const int screenHeight = 450;

// Required variables to manage screen transitins (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

static void ChangeToScreen(int screen);
// Change to screen, no transition effect

static void TransitionToScreen(int screen);
// Request transition to next screen

static void UpdateTransition(void);
// Update transition effect

static void DrawTransition(void);
// Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);
// Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------

int main(void) {
  InitWindow(screenWidth, screenHeight, "raylib game template");
  // Initialization

  InitAudioDevice();
  // Initialize audio device

  font = LoadFont("resources/mecha.png");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);

#else
  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  // Main game loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }

#endif

  // De-Initialization
  //--------------------------------------------------------------------------------------
  // Unload current screen data before closing
  switch (currentScreen) {
    case LOGO:
      UnloadLogoScreen();
      break;

    case GAMEPLAY:
      UnloadGameplayScreen();
      break;

    case RAYPATH:
      UnloadRayPathScreen();
      break;

    default:
      break;
  }

  // Unload global data loaded
  UnloadFont(font);

  CloseAudioDevice();  // Close audio context

  CloseWindow();  // Close window and OpenGL context

  //--------------------------------------------------------------------------------------

  return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition

static void ChangeToScreen(int screen) {
  // Unload current screen
  switch (currentScreen) {
    case LOGO:
      UnloadLogoScreen();
      break;

    case GAMEPLAY:
      UnloadGameplayScreen();
      break;

    case RAYPATH:
      UnloadRayPathScreen();
      break;

    default:
      break;
  }

  // Init next screen
  switch (screen) {
    case LOGO:
      InitLogoScreen();
      break;

    case GAMEPLAY:
      InitGameplayScreen();
      break;

    case RAYPATH:
      InitRayPathScreen();
      break;

    default:
      break;
  }

  currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen) {
  onTransition = true;
  transFadeOut = false;
  transFromScreen = currentScreen;
  transToScreen = screen;
  transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void) {
  if (!transFadeOut) {
    transAlpha += 0.05f;

    // NOTE: Due to float internal representation, condition jumps on 1.0f
    // instead of 1.05f For that reason we compare against 1.01f, to avoid
    // last frame loading stop
    if (transAlpha > 1.01f) {
      transAlpha = 1.0f;

      // Unload current screen
      switch (transFromScreen) {
        case LOGO:
          UnloadLogoScreen();
          break;

        case GAMEPLAY:
          UnloadGameplayScreen();
          break;

        default:
          UnloadRayPathScreen();
          break;
      }

      // Load next screen
      switch (transToScreen) {
        case LOGO:
          InitLogoScreen();
          break;

        case GAMEPLAY:
          InitGameplayScreen();
          break;

        case RAYPATH:
          InitRayPathScreen();

        default:
          break;
      }

      currentScreen = transToScreen;

      // Activate fade out effect to next loaded screen
      transFadeOut = true;
    }
  } else  // Transition fade out logic
  {
    transAlpha -= 0.02f;

    if (transAlpha < -0.01f) {
      transAlpha = 0.0f;
      transFadeOut = false;
      onTransition = false;
      transFromScreen = -1;
      transToScreen = UNKNOWN;
    }
  }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
  // Update
  //----------------------------------------------------------------------------------

  // screens
  if (!onTransition) {
    switch (currentScreen) {
      case LOGO: {
        UpdateLogoScreen();
        if (FinishLogoScreen()) TransitionToScreen(GAMEPLAY);
      } break;

      case GAMEPLAY: {
        UpdateGameplayScreen();
        if (FinishGameplayScreen()) TransitionToScreen(RAYPATH);
      } break;

      case RAYPATH: {
        UpdateRayPathScreen();
      } break;

      default:
        break;
    }
  } else
    UpdateTransition();  // Update transition (fade-in, fade-out)
  //----------------------------------------------------------------------------------

  // Draw
  //----------------------------------------------------------------------------------
  BeginDrawing();

  ClearBackground(WHITE);

  switch (currentScreen) {
    case LOGO:
      DrawLogoScreen();
      break;
    case GAMEPLAY:
      DrawGameplayScreen();
      break;
    case RAYPATH:
      DrawRayPathScreen();
    default:
      break;
  }

  // Draw full screen rectangle in front of everything
  if (onTransition) DrawTransition();

  DrawFPS(10, 10);

  EndDrawing();
  //----------------------------------------------------------------------------------
}
