#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "camera.h"
#include "graphics.h"
#include "cameracontrol.h"

//#define MAIN_TEXTURE_WIDTH 768
//#define MAIN_TEXTURE_HEIGHT 432

#define MAIN_TEXTURE_WIDTH 64
#define MAIN_TEXTURE_HEIGHT 32

#define UNUSED(x) (void)(x)

char tmpbuf[MAIN_TEXTURE_WIDTH * MAIN_TEXTURE_HEIGHT * 4];


int timespec_subtract(struct timespec *result, struct timespec *x, struct timespec *y)
{
    if (x->tv_nsec < y->tv_nsec) {
        int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
        y->tv_nsec -= 1000000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_nsec - y->tv_nsec > 1000000000) {
        int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
        y->tv_nsec += 1000000000 * nsec;
        y->tv_sec -= nsec;
    }
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_nsec = x->tv_nsec - y->tv_nsec;
    return x->tv_sec < y->tv_sec;
}



//entry point
int _main(int argc, const char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    //should the camera convert frame data from yuv to argb automatically?
    bool do_argb_conversion = true;

    //init graphics and the camera
    InitGraphics();
    CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH,
                               MAIN_TEXTURE_HEIGHT,
                               30,
                               1,
                               do_argb_conversion);
    cam->setExposureMode(MMAL_PARAM_EXPOSUREMODE_AUTO);
    cam->setAwbMode(MMAL_PARAM_AWBMODE_AUTO);

    //create 4 textures of decreasing size
    GfxTexture texture;
    texture.Create(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT);

    printf("Running frame loop\n");
    struct timespec t0;
    clock_gettime(CLOCK_REALTIME, &t0);
    for (int i = 0; i < 3000; ++i) {
        if(i == 100) {
            cam->setExposureMode(MMAL_PARAM_EXPOSUREMODE_OFF);
            cam->setAwbMode(MMAL_PARAM_AWBMODE_FLUORESCENT);
            cam->setISO(100);
            cam->setBrightness(50);
            cam->setContrast(0);
            cam->setSharpness(0);
            cam->setSaturation(0);
            cam->setExposureCompensation(0);
            cam->setShutterSpeed(33333);
        }
        //lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
        const void* frame_data;
        int frame_sz;
        if (cam->BeginReadFrame(0, frame_data, frame_sz)) {
            if (do_argb_conversion) {
                //if doing argb conversion the frame data will be exactly the right size so just set directly
                texture.SetPixels(frame_data);
            }
            else {
                //if not converting argb the data will be the wrong size and look weird, put copy it in
                //via a temporary buffer just so we can observe something happening!
                memcpy(tmpbuf, frame_data, frame_sz);
                texture.SetPixels(tmpbuf);
            }
            cam->EndReadFrame(0);
        }

        //begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
        BeginFrame();
        float aspect_ratio = float(MAIN_TEXTURE_WIDTH) / float(MAIN_TEXTURE_HEIGHT);
        float screen_aspect_ratio = 1280.f / 720.f;
        DrawTextureRect(&texture, -aspect_ratio / screen_aspect_ratio, -1.f, aspect_ratio / screen_aspect_ratio, 1.f);
        EndFrame();

        struct timespec t1;
        struct timespec dt;
        clock_gettime(CLOCK_REALTIME, &t1);
        timespec_subtract(&dt, &t1, &t0);
        float ms = (dt.tv_nsec + 1e9f * dt.tv_sec) * 1e-6f;
        printf("%.3f (%.1f fps) %d\n", ms, 1e3 / ms, frame_sz);
        t0.tv_sec = t1.tv_sec;
        t0.tv_nsec = t1.tv_nsec;
    }

    StopCamera();
}
