#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gst/gst.h>

static guint spect_bands = 8;

#define AUDIOFREQ 20000

/* receive spectral data from element message */
static gboolean
message_handler (GstBus * bus, GstMessage * message, gpointer data)
{
  if (message->type == GST_MESSAGE_ELEMENT) {
    const GstStructure *s = gst_message_get_structure (message);
    const gchar *name = gst_structure_get_name (s);
    GstClockTime endtime;

    if (strcmp (name, "spectrum") == 0) {
      const GValue *magnitudes;
      const GValue *phases;
      const GValue *mag, *phase;
      gdouble freq;
      guint i;

      if (!gst_structure_get_clock_time (s, "endtime", &endtime))
        endtime = GST_CLOCK_TIME_NONE;

      magnitudes = gst_structure_get_value (s, "magnitude");

      for (i = 0; i < spect_bands; ++i) {
        freq = (gdouble) ((AUDIOFREQ / 2) * i + AUDIOFREQ / 4) / spect_bands;
        mag = gst_value_list_get_value (magnitudes, i);

        if (mag != NULL) {
          g_print ("%04.1f ",(g_value_get_float (mag)+80));
        }
      }
      g_print ("\n");
    }
  }
  return TRUE;
}

int
main (int argc, char *argv[])
{
  GstElement *bin;
  GstElement *src, *audioconvert, *spectrum, *sink;
  GstBus *bus;
  GstCaps *caps;
  GMainLoop *loop;

  gst_init (&argc, &argv);

  bin = gst_pipeline_new ("bin");

  src = gst_element_factory_make ("pulsesrc", "src");
  g_object_set (G_OBJECT (src), "wave", 0, "freq", 6000.0, NULL);
  audioconvert = gst_element_factory_make ("audioconvert", NULL);
  g_assert (audioconvert);

  spectrum = gst_element_factory_make ("spectrum", "spectrum");
  g_object_set (G_OBJECT (spectrum), "bands", spect_bands, "threshold", -80,
      "post-messages", TRUE, "message-phase", TRUE, NULL);

  sink = gst_element_factory_make ("fakesink", "sink");
  g_object_set (G_OBJECT (sink), "sync", TRUE, NULL);   
  g_object_set (G_OBJECT (src), "device", "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor", NULL);
  gst_bin_add_many (GST_BIN (bin), src, audioconvert, spectrum, sink, NULL);

  caps = gst_caps_new_simple ("audio/x-raw",
      "rate", G_TYPE_INT, AUDIOFREQ, NULL);

  if (!gst_element_link (src, audioconvert) ||
      !gst_element_link_filtered (audioconvert, spectrum, caps) ||
      !gst_element_link (spectrum, sink)) {
    fprintf (stderr, "can't link elements\n");
    exit (1);
  }
  gst_caps_unref (caps);

  bus = gst_element_get_bus (bin);
  gst_bus_add_watch (bus, message_handler, NULL);
  gst_object_unref (bus);

  gst_element_set_state (bin, GST_STATE_PLAYING);

  /* we need to run a GLib main loop to get the messages */
  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  gst_element_set_state (bin, GST_STATE_NULL);

  gst_object_unref (bin);

  return 0;
}
