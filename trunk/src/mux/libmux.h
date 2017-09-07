#ifndef LIBMUX_H
#define LIBMUX_H

#if defined (_WIN32) && defined (DLL_EXPORT)
# define LIBMUX_API __declspec(dllexport)
#elif defined (__GNUC__) && (__GNUC__ >= 4)
# define LIBMUX_API __attribute__((visibility("default")))
#else
# define LIBMUX_API
#endif

typedef int (*CheckData)(int bytes, void *user_data);
typedef int (*GetVideo)(char* rgb24, void *user_data);
typedef int (*GetAudio)(char* s16, int frame_size, int chs, void *user_data);

struct MuxHandle
{
    // default: NULL
    char *ouput_filename;

    // default: 320
    int video_width;
    // default: 240
    int video_height;
    // default: 400,000
    int video_bit_rate;
    // default: 96000
    int audio_bit_rate;

    // callbacks api
    CheckData check_video;
    CheckData check_audio;
    GetVideo  get_video;
    GetAudio  get_audio;

    void *private_data_video;
    void *private_data_audio;

    void *user_data;
};

typedef struct MuxHandle MuxHandle;

#endif // LIBMUX_H
