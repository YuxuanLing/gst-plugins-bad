/* GStreamer
 * Copyright (C) 2020 Nicolas Dufresne <nicolas.dufresne@collabora.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_V4L2_DECODER_H__
#define __GST_V4L2_DECODER_H__

#include <gst/gst.h>
#include <gst/video/video.h>

#include "gstv4l2codecdevice.h"
#include "linux/videodev2.h"

G_BEGIN_DECLS

#define GST_TYPE_V4L2_DECODER gst_v4l2_decoder_get_type ()
G_DECLARE_FINAL_TYPE (GstV4l2Decoder, gst_v4l2_decoder, GST, V4L2_DECODER, GstObject);

typedef struct _GstV4l2Request GstV4l2Request;

GstV4l2Decoder *  gst_v4l2_decoder_new (GstV4l2CodecDevice * device);

gboolean          gst_v4l2_decoder_open (GstV4l2Decoder * decoder);

gboolean          gst_v4l2_decoder_close (GstV4l2Decoder * decoder);

gboolean          gst_v4l2_decoder_streamon (GstV4l2Decoder * self,
                                             GstPadDirection direction);

gboolean          gst_v4l2_decoder_streamoff (GstV4l2Decoder * self,
                                              GstPadDirection direction);

gboolean          gst_v4l2_decoder_flush (GstV4l2Decoder * self);

gboolean          gst_v4l2_decoder_enum_sink_fmt (GstV4l2Decoder * self,
                                                  gint i, guint32 * out_fmt);

gboolean          gst_v4l2_decoder_set_sink_fmt (GstV4l2Decoder * self, guint32 fmt,
                                                 gint width, gint height);

GstCaps *         gst_v4l2_decoder_enum_src_formats (GstV4l2Decoder * self);

gboolean          gst_v4l2_decoder_select_src_format (GstV4l2Decoder * self,
                                                      GstCaps * caps,
                                                      GstVideoInfo * info);

gint              gst_v4l2_decoder_request_buffers (GstV4l2Decoder * self,
                                                    GstPadDirection direction,
                                                    guint num_buffers);

gboolean          gst_v4l2_decoder_export_buffer (GstV4l2Decoder * self,
                                                  GstPadDirection directon,
                                                  gint index,
                                                  gint * fds,
                                                  gsize * sizes,
                                                  gsize * offsets,
                                                  guint *num_fds);

gboolean          gst_v4l2_decoder_queue_sink_mem (GstV4l2Decoder * self,
                                                   GstV4l2Request * request,
                                                   GstMemory * mem,
                                                   guint32 frame_num,
                                                   gsize bytesused);

gboolean          gst_v4l2_decoder_dequeue_sink (GstV4l2Decoder * self);

gboolean          gst_v4l2_decoder_queue_src_buffer (GstV4l2Decoder * self,
                                                     GstBuffer * buffer,
                                                     guint32 frame_num);

gboolean          gst_v4l2_decoder_dequeue_src (GstV4l2Decoder * self,
                                                guint32 *out_frame_num);

gboolean          gst_v4l2_decoder_set_controls (GstV4l2Decoder * self,
                                                 GstV4l2Request * request,
                                                 struct v4l2_ext_control *control,
                                                 guint count);

void              gst_v4l2_decoder_install_properties (GObjectClass * gobject_class,
                                                       gint prop_offset,
                                                       GstV4l2CodecDevice * device);

void              gst_v4l2_decoder_set_property (GObject * object, guint prop_id,
                                                 const GValue * value, GParamSpec * pspec);

void              gst_v4l2_decoder_get_property (GObject * object, guint prop_id,
                                                 GValue * value, GParamSpec * pspec);

void              gst_v4l2_decoder_register (GstPlugin * plugin,
                                             GType dec_type,
                                             GClassInitFunc class_init,
                                             GInstanceInitFunc instance_init,
                                             const gchar *element_name_tmpl,
                                             GstV4l2CodecDevice * device,
                                             guint rank);

GstV4l2Request   *gst_v4l2_decoder_alloc_request (GstV4l2Decoder * self);

void              gst_v4l2_request_free (GstV4l2Request * request);

gboolean          gst_v4l2_request_queue (GstV4l2Request * request);

gint              gst_v4l2_request_poll (GstV4l2Request * request, GstClockTime timeout);

void              gst_v4l2_request_set_done (GstV4l2Request * request);

gboolean          gst_v4l2_request_is_done (GstV4l2Request * request);

G_END_DECLS

#endif /* __GST_V4L2_DECODER_H__ */
