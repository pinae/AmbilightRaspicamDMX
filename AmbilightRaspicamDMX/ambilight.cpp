#include "ambilight.h"
#include "camera.h"
#include "graphics.h"

#define MAIN_TEXTURE_WIDTH 64
#define MAIN_TEXTURE_HEIGHT 32

Ambilight::Ambilight()
{
  //should the camera convert frame data from yuv to argb automatically?
  bool do_argb_conversion = true;

  //init graphics and the camera
  InitGraphics();
  cam = StartCamera(MAIN_TEXTURE_WIDTH,
                    MAIN_TEXTURE_HEIGHT,
                    30,
                    1,
                    do_argb_conversion);

  //run the loop
  run = true;
  loop();
}

Ambilight::~Ambilight()
{
  run = false;
  StopCamera();
}

void Ambilight::loop()
{
  while(run) {
    const void* frame_data;
    int frame_sz;
    if (cam->BeginReadFrame(0, frame_data, frame_sz)) {
      //if doing argb conversion the frame data will be exactly the right size so just set directly
      //texture.SetPixels(frame_data);
    }
  }
}
