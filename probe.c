#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>

int main(int argc, const char *argv[]) {
  char buf[256];
  unsigned int i;
  int r;

  AVFormatContext *format_ctx;
  AVStream *stream;
  AVCodecContext *codec_ctx;
  AVCodec *codec;

  /* Initialize libav. */
  av_register_all();
  av_log_set_level(AV_LOG_QUIET);

  /* Open stdin. */
  format_ctx = avformat_alloc_context();
  if (format_ctx == NULL)
    return 1;

  r = avformat_open_input(&format_ctx, "pipe:0", NULL, NULL);
  if (r < 0)
    return 1;

  r = avformat_find_stream_info(format_ctx, NULL);
  if (r < 0)
    return 1;

  printf("{\n  \"streams\": [");

  /* Dump information for each stream. */
  for (i = 0; i < format_ctx->nb_streams; i++) {
    stream = format_ctx->streams[i];
    codec_ctx = stream->codec;

    codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (codec == NULL)
      return -1;

    r = avcodec_open2(codec_ctx, codec, NULL);
    if (r < 0)
      return -1;

    /* Open the stream's JSON object. */
    printf(i == 0 ? "\n    {" : ",\n    {");
    printf("\n      \"index\": %d", stream->index);

    switch (codec_ctx->codec_type) {
    case AVMEDIA_TYPE_VIDEO:    printf(",\n      \"type\": \"video\"");    break;
    case AVMEDIA_TYPE_AUDIO:    printf(",\n      \"type\": \"audio\"");    break;
    case AVMEDIA_TYPE_SUBTITLE: printf(",\n      \"type\": \"subtitle\""); break;
    default:                    printf(",\n      \"type\": \"unknown\"");
    }

    printf(",\n      \"codec\": \"%s\"", codec->name);
    printf(",\n      \"start_time\": %f", stream->start_time > 0 ? stream->start_time * av_q2d(stream->time_base) : 0.0);
    printf(",\n      \"duration\": %f", stream->duration > 0 ? stream->duration * av_q2d(stream->time_base) : 0.0);

    /* Video-specific fields. */
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
      printf(",\n      \"width\": %d", codec_ctx->width);
      printf(",\n      \"height\": %d", codec_ctx->height);
      printf(",\n      \"bit_rate\": %d", codec_ctx->bit_rate);
      printf(",\n      \"frames\": %lld", stream->nb_frames);
      printf(",\n      \"frame_rate\": %f", stream->nb_frames > 0 ? av_q2d(stream->avg_frame_rate) : 0.0);

      if (codec_ctx->pix_fmt != -1) {
        printf(",\n      \"pixel_format\": \"%s\"", av_get_pix_fmt_name(codec_ctx->pix_fmt));
      } else {
        printf(",\n      \"pixel_format\": \"unknown\"");
      }
    }

    /* Audio-specific fields. */
    if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
      printf(",\n      \"channels\": %d", codec_ctx->channels);

      if (codec_ctx->channel_layout != -1) {
        av_get_channel_layout_string(&buf[0], 256, codec_ctx->channels, codec_ctx->channel_layout);
        printf(",\n      \"channel_layout\": \"%s\"", buf);
      } else {
        printf(",\n      \"channel_layout\": \"unknown\"");
      }

      printf(",\n      \"bit_rate\": %d", codec_ctx->bit_rate);
      printf(",\n      \"sample_rate\": %d", codec_ctx->sample_rate);

      if (codec_ctx->sample_fmt != -1) {
        printf(",\n      \"sample_format\": \"%s\"", av_get_sample_fmt_name(codec_ctx->sample_fmt));
      }
    }

    /* Close the stream's JSON object. */
    printf("\n    }");
  }

  printf("\n  ]\n}\n");

  /* Close the input. */
  avformat_close_input(&format_ctx);

  return 0;
}
