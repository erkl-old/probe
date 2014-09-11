**probe** is a small command-line utility which prints JSON-formatted
information about a video or audio file fed via stdin, with the help of the
excellent ffmpeg/libav framework(s).

It produces the following report for the theatrical trailer for Wes Anderson's
Grand Budapest Hotel:

```json
{
  "streams": [
    {
      "index": 0,
      "type": "video",
      "codec": "h264",
      "start_time": 0.041708,
      "duration": 144.561083,
      "width": 1920,
      "height": 1080,
      "bit_rate": 3996395,
      "frames": 3466,
      "frame_rate": 23.976024,
      "pixel_format": "yuv420p"
    },
    {
      "index": 1,
      "type": "audio",
      "codec": "aac",
      "start_time": 0.000000,
      "duration": 144.590658,
      "channels": 2,
      "channel_layout": "stereo",
      "bit_rate": 189664,
      "sample_rate": 44100,
      "sample_format": "fltp"
    }
  ]
}
```

### Dependencies

You'll be good to go if `pkg-config --print-errors --exists libavcodec libavformat libavutil`
runs without complaining. Otherwise, install either `ffmpeg` or `libav` using
your favourite package manager.
